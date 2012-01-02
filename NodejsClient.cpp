#include <errno.h>
#include <iostream>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#include "NodejsClient.h"

using namespace std;

int NodejsClient::readinchild(int fd) {
   char ptr[MAX_BUFFER_SIZE];
   int f, i;
   int nread=1;

   f = fork();
   if( f == 0 ) {
     childReaderSock = fd;
     while(true) {
        cout.flush();
        nread = recv(fd, ptr, MAX_BUFFER_SIZE, 0);
        ptr[nread] = '\0'; // terminate a string
        if(nread == 0) continue;
        messageHandler(ptr);
     }
     close(fd);
     exit(0);
   }
   return f;
}

NodejsClient::NodejsClient(char * Server_IP = "127.0.0.1", int Server_Port = 1337) {
    struct sockaddr_in srvadd;
    closed = false;

    if( ( Sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP) ) < 0 ) {
        if(NODEJSCLIENT_DEBUG) cout << "Can't create socket Sock";
    }
    if(NODEJSCLIENT_DEBUG) cout <<  "Socket created, handle=" << Sock << endl;
    if(NODEJSCLIENT_DEBUG) cout << "Sock will communicate with " << Server_IP << ":" << Server_Port << endl;

    memset(&srvadd, 0, sizeof(struct sockaddr_in) );
    srvadd.sin_family = AF_INET;
    srvadd.sin_addr.s_addr = inet_addr(Server_IP);
    srvadd.sin_port = htons((u_short) Server_Port);

    if( connect( Sock, (struct sockaddr *) &srvadd, sizeof(srvadd) ) < 0 ) {
       if(NODEJSCLIENT_DEBUG) cout << "Can't connect to remote host" << endl;
    } 
}

NodejsClient::~NodejsClient()
{
    closeClient();
}

void NodejsClient::run() {
    reader = readinchild(Sock);
}

void NodejsClient::closeClient() {
    buf[0] = 26;
    buf[1] = 10;
    send(Sock, buf, 2, 0);
    close(childReaderSock);    
    kill( reader, SIGINT );
    close( Sock );
    wait(0);
    if(NODEJSCLIENT_DEBUG) cout << "Sender closing..." << endl;
}

bool NodejsClient::getSocketState() {
    return closed;
}

void NodejsClient::sendMessage(char* msg) {
    cout << "sending message" << msg << " len: " << strlen(msg) << endl;
    send(Sock, msg, strlen(msg), 0 );
}

void NodejsClient::registerMessageHandler(handlerType func) {
    messageHandler = func;
}

