CC = gcc
INFRA_DIR = ../../../serverTCP/DS/libC/

libserver:
	$(CC) -ansi -pedantic -Wall -g -fPIC  -I../libH/ -c $(INFRA_DIR)ServerNet.c $(INFRA_DIR)list_itr.c $(INFRA_DIR)list_functions.c $(INFRA_DIR)list.c -I inc/
	$(CC) -shared -o libserver.so *.o


