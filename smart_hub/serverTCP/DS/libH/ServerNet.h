#ifndef __SERVER_NET_H__
#define __SERVER_NET_H__

#include <stddef.h> /*for size_t*/
#include <stdint.h>

typedef struct ServerNet ServerNet;

typedef enum ServerNetErr
{
    SERVER_NET_SUCCESS,
    SERVER_NET_ACCEPT_FAILED,
    SERVER_NET_ALOCATION_FAILED,
    SERVER_NET_SELECT_ERROR,
    SERVER_NET_RECIEVE_ERROR,
    SERVER_NET_SEND_ERROR,
    SERVER_NET_SEND_PARAMETERS_ERROR,
    SERVER_NET_NOT_INITIALIZED,
    SERVER_NET_STOP,
    SERVER_NET_CLOSE_CLIENT
} ServerNetErr;

/**
 * @brief does something on server failing \n
 * you can stop the server if you return SERVER_STOP \n
 * return SURVER_SUCCES if you dont want to do anything \n
 * if you return anything else the server will not stop
 * @param _status the current status
 * @param _errno the current errno
 * @param context that you can use if you want
 * @return status so the server will know what to do
 */
typedef ServerNetErr (*Fail)(ServerNetErr _status, int _errno, void* _appContext);

/**
 * @brief gets the message and its size and does something \n
 * got msg can return SERVER_STOP and then the server will stop
 * @param _recieved the message
 * @param _size the size of the message
 * @param _sock the _sock that sent it
 * @param context that you can use if you want
 * @return status so the server will know what to do(succese or fail) (if you send in this function please return the send value)
 */
typedef ServerNetErr (*GotMsg)(void *_recieved, size_t _size, int _sock, void* _appContext);

/**
 * @brief does something when accepting a client
 * @param _sock the accepted sock
 * @param context that you can use if you want
 */
typedef void (*NewClient)(int _sock, void* _appContext);

/**
 * @brief does something when accepting a client
 * @param _sock the closed  sock
 * @param context that you can use if you want
 */
typedef void (*CloseClient)(int _sock, void* _appContext);

/** 
 * @brief Create a new server with given parameters.
 * @param _backLog the size of the listening queue ( > 0).
 * @param _port the port (65000 > _port > 1024).
 * @param _maxBuffer the max size you can get from each msg
 * @param _gm what happens when we get a message
 * @param _appContext [optional] context to your app
 * @param _fail [optional] what the function does on fail
 * @param _close [optional] what the function does when client closes (if exists must have accept)
 * @param _accept [optional] what the function does when client get aceepted (if exists must have close)
 * @return newly created server or null on failure
 * @retval if allocation failed if backlog\port is illegal, if gotmsg is null or if only one of close and accept are null or not null
 */
ServerNet *ServerNetCreate(int _backLog ,uint16_t _port, size_t _maxBuffer, GotMsg _gotMsg, Fail _fail, CloseClient _close, NewClient _newClinet, void* _appContext);

/** 
 * @brief deallocates a server
 * @param _server  the server
 */
void ServerNetDestroy(ServerNet **_serverNet);

/** 
 * @brief starts the server
 * @param _server the server
 * @return success or last error gotten before stopping
 */
ServerNetErr ServerNetRun(ServerNet *_serverNet);

/** 
 * @brief send a message to given sock
 * @param _sock the sock you send the message to 
 * @param _message what you want to send
 * @param _size it's size
 * @return success or error
 * @retval if parameters wrong will return error parameters, if couldn't send will return send failed
 */
ServerNetErr ServerNetSend(int _clientSock, void *_message, size_t _size);


#endif /* __SERVER_NET_H__ */