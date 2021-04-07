/*
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../DS/lib
export LD_LIBRARY_PATH
gcc -ansi -pedantic ServerNet.c TestServerNet.c -o TestServerNet.out -L ../DS/lib -lDS -lm
./TestServerNet.out
*/

#include "../libH/ServerNet.h"
#include "../libH/list_functions.h" /*for list functions*/
#include "../libH/list.h"
#include "../libH/list_itr.h"

#include <sys/socket.h> /*for socket*/
#include <netinet/in.h> /*for types*/
#include <arpa/inet.h>
#include <unistd.h>     /*close*/
#include <errno.h>      /*for errno*/
#include <sys/select.h> /*for select*/
#include <stdlib.h>     /*for null malloc and free*/
#include <string.h>     /*memset*/
#include <stdio.h>      /*perror*/

#define ERROR -1
#define OK 0
#define MAX 1024
#define MAXCLIENTS 1000

typedef enum Flag
{
    RUN,
    STOP
} Flag;

struct ServerNet
{
    NewClient     m_newClinet; /*function to do if we accept*/
    CloseClient   m_closeFunc;   /*function to do if we close*/
    GotMsg m_gotMsg;         /*function to do if we recv*/
    Fail     m_failFunc;           /*function to do if we fail*/
    Flag m_flag;     /*checks the status of the loop*/
    fd_set m_master;           /*the master with all activated sockets*/
    size_t m_maxBuffer;        /* maximum messeage size*/
    void* m_appContext;        /*context that you can provide*/
    int m_serverSock;          /*the sock of the server*/
    int m_count;               /*how many clients we have*/ 
    List *m_clientList;        /*list of the clients*/
};

static int CheckParameterCreate(uint16_t _port, int _backLog, size_t _maxBuffer, GotMsg _gm, CloseClient _close, NewClient _newClinet);
static ServerNetErr CheckCurClients(ServerNet *_serverNet, fd_set *_changer, int *_numOfSet, void *_returned);
static void FailFunc(ServerNetErr _status, ServerNet *_serverNet, int *numOfSet);
static ServerNetErr CheckNewClients(ServerNet *_serverNet, int *_numOfSet);
static int InitSocket(uint16_t _port, int backLog);
static void DestroyInts(void *_int);

ServerNet *ServerNetCreate(int _backLog ,uint16_t _port, size_t _maxBuffer, GotMsg _gotMsg, Fail _fail, CloseClient _close, NewClient _newClinet, void* _appContext)
{
    ServerNet *serverNet;

    if (ERROR == CheckParameterCreate(_port, _backLog, _maxBuffer, _gotMsg, _close, _newClinet))
    {
        return NULL;
    }

    serverNet = (ServerNet*)malloc(sizeof(ServerNet));
    if (serverNet == NULL)
    {
        return NULL;
    }
    /*no need for higher than 1024*/
    if (_backLog > 1024)
    {
        _backLog = 1024;
    }
    if (NULL == (serverNet->m_clientList = ListCreate()))
    {
        free(serverNet);
        return NULL;
    }
    serverNet->m_serverSock = InitSocket(_port, _backLog);
    if (serverNet->m_serverSock == ERROR)
    {
        ListDestroy(&(serverNet->m_clientList), NULL);
        free(serverNet);
        return NULL;
    }
    /*initialize what's left*/
    serverNet->m_count = 0;
    serverNet->m_appContext = _appContext;
    serverNet->m_flag = STOP;
    serverNet->m_maxBuffer = _maxBuffer;
    serverNet->m_failFunc = _fail;
    serverNet->m_gotMsg = _gotMsg;
    serverNet->m_newClinet = _newClinet;
    serverNet->m_closeFunc = _close;
    FD_ZERO(&(serverNet->m_master));
    FD_SET(serverNet->m_serverSock, &(serverNet->m_master));

    return serverNet;
}

void ServerNetDestroy(ServerNet **_serverNet)
{
    if (NULL == _serverNet || NULL == *_serverNet || RUN == (*_serverNet)->m_flag)
    {
        return;
    }
    close((*_serverNet)->m_serverSock);
    ListDestroy(&((*_serverNet)->m_clientList), DestroyInts);
    free(*_serverNet);
    *_serverNet = NULL;
}

ServerNetErr ServerNetRun(ServerNet *_serverNet)
{
    fd_set changer;
    int numOfSet;
    ServerNetErr status;
    void *returned;

    if (NULL == _serverNet)
    {
        return SERVER_NET_NOT_INITIALIZED;
    }
    /*creates the place that recieved data will be stored*/
    if (NULL == (returned = malloc(_serverNet->m_maxBuffer* sizeof(char))))
    {
        return SERVER_NET_ALOCATION_FAILED;
    }
    _serverNet->m_flag = RUN;
    while (_serverNet->m_flag == RUN)
    {
        changer = _serverNet->m_master;
        numOfSet = select(MAX, &changer, NULL, NULL, NULL);
        if (numOfSet < 0 && errno != EINTR)
        {
            FailFunc(SERVER_NET_SELECT_ERROR, _serverNet, &numOfSet);
            status = SERVER_NET_SELECT_ERROR;
        }
        if (FD_ISSET(_serverNet->m_serverSock, &changer) && numOfSet > 0)
        {
            status = CheckNewClients(_serverNet, &numOfSet);
            --numOfSet;
        }
        if (numOfSet > 0)
        {
            status = CheckCurClients(_serverNet, &changer, &numOfSet, returned);
        }
    }
    free(returned);

    return status;
}

ServerNetErr ServerNetSend(int _clientSock, void *_message, size_t _size)
{
    int sentBytes;

    if (_clientSock < 0 || _message == NULL || _size == 0)
    {
        return SERVER_NET_SEND_PARAMETERS_ERROR;
    }
    /*sends to the client with the message and the size*/
    sentBytes = (int)send(_clientSock, _message, _size, 0);
    if (sentBytes < 0)
    {
        return SERVER_NET_SEND_ERROR;
    }

    return SERVER_NET_SUCCESS;
}
/***********************help func*********************************************/
static ServerNetErr CheckNewClients(ServerNet *_serverNet, int *_numOfSet)
{
    int *clientSock = (int*)malloc(sizeof(int));
    struct sockaddr_in clientSin;
    int addr_len = sizeof(clientSin);
    ListItr end;

    if (clientSock == NULL)
    {
        FailFunc(SERVER_NET_ALOCATION_FAILED, _serverNet, _numOfSet);
        return SERVER_NET_ALOCATION_FAILED;
    }
    /*accepts the client*/
    *clientSock = accept(_serverNet->m_serverSock, (struct sockaddr *)&clientSin, (socklen_t *)&addr_len);
    if (*clientSock < 0)
    {
        free(clientSock);
        FailFunc(SERVER_NET_ACCEPT_FAILED, _serverNet, _numOfSet);
        return SERVER_NET_ACCEPT_FAILED;
    }
    if (_serverNet->m_count >= MAXCLIENTS - 1)
    {
        close(*clientSock);
        free(clientSock);
        return SERVER_NET_SUCCESS;
    }
    if (_serverNet->m_newClinet != NULL)
    {
        _serverNet->m_newClinet(*clientSock, _serverNet->m_appContext);
    }
    /*insert to list*/
    end = ListItrEnd(_serverNet->m_clientList);
    if (NULL == ListItrInsertBefore(end, clientSock))
    {
        free(clientSock);
        FailFunc(SERVER_NET_ALOCATION_FAILED, _serverNet, _numOfSet);
        return SERVER_NET_ALOCATION_FAILED;
    }
    ++(_serverNet->m_count);
    FD_SET(*clientSock, &(_serverNet->m_master));

    return SERVER_NET_SUCCESS;
}

static void FailFunc(ServerNetErr _status, ServerNet *_serverNet, int *numOfSet)
{
    ServerNetErr check; 
    if (_serverNet->m_failFunc != NULL)
    {
        /*if exists and returns stop stops the function*/
        if ((check = _serverNet->m_failFunc(_status, errno, _serverNet->m_appContext)) == SERVER_NET_STOP)
        {
            _serverNet->m_flag = STOP;
            *numOfSet = 0;
            return;
        }
    }
    if(_status == SERVER_NET_STOP)
    {
        _serverNet->m_flag = STOP;
        *numOfSet = 0;
    }
}

static ServerNetErr CheckCurClients(ServerNet *_serverNet, fd_set *_changer, int *_numOfSet, void *_returned)
{
    ListItr begin, end, next;
    int *element;
    int readBytes;
    ServerNetErr check = SERVER_NET_SUCCESS;

    next = ListItrBegin(_serverNet->m_clientList);
    end = ListItrEnd(_serverNet->m_clientList);
    /*go thrugh all the list until you did all the numb of set*/
    while (next != end && (*_numOfSet) > 0)
    {
        begin = next;
        element = (int*)ListItrGet(begin);
        next = ListItrNext(begin);
        if (!FD_ISSET(*element, _changer))
        {
            continue;
        }
        --(*_numOfSet);
        readBytes = (int)recv(*element, _returned, _serverNet->m_maxBuffer, 0);
        if (readBytes <= 0)
        {
            /*clear and release*/
            FD_CLR(*element, &(_serverNet->m_master));
            if(NULL != _serverNet->m_closeFunc)
            {
                _serverNet->m_closeFunc(*element, _serverNet->m_appContext);
            }
            close(*element);
            ListItrRemove(begin);
            free(element);
            --(_serverNet->m_count);
        }
        else
        {
            /*handles the message*/
            if (SERVER_NET_SUCCESS != (check = _serverNet->m_gotMsg(_returned, (size_t)readBytes, *element, _serverNet->m_appContext)))
            {
                FailFunc(check, _serverNet, _numOfSet);
                return check;
            }
        }
        if (_numOfSet == 0)
        {
            break;
        }
    }

    return check;
}

static void DestroyInts(void *_int)
{
    close(*(int *)_int);
    free(_int);
}

static int CheckParameterCreate(uint16_t _port, int _backLog, size_t _maxBuffer, GotMsg _gm, CloseClient _close, NewClient _newClinet)
{
    if (0 >= _backLog || _port < 1024 || 0 == _maxBuffer || _port > 65000)
    {
        return ERROR;
    }
    if (NULL == _gm)
    {
        return ERROR;
    }
    if ((_newClinet == NULL && _close != NULL) || (_newClinet != NULL && _close == NULL))
    {
        return ERROR;
    }

    return OK;
}

static int InitSocket(uint16_t _port, int backLog)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    int optval = 1;
    struct sockaddr_in sin;

    if (sock < 0)
    {
        return ERROR;
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(_port);
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        close(sock);
        return ERROR;
    }
    if (bind(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        close(sock);
        return ERROR;
    }
    if (listen(sock, backLog) < 0)
    {
        close(sock);
        return ERROR;
    }

    return sock;
}
