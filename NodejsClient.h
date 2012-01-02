#ifndef __NODEJSCLIENT
#define __NODEJSCLIENT

#define MAX_BUFFER_SIZE 1024
#define LOOP_TIME 0.01
#define NODEJSCLIENT_DEBUG true

using namespace std;

typedef void (*handlerType)(char*);

class NodejsClient {
    private:
    bool closed;
    int Sock;
    char buf [MAX_BUFFER_SIZE];
    int reader;

    handlerType messageHandler;

    int readinchild(int fd);

    public:
    int childReaderSock;
    NodejsClient(char * Server_IP, int Server_Port );
    ~NodejsClient();

    void run();
    void closeClient();
    bool getSocketState();
    void sendMessage(char* msg);
    void registerMessageHandler(handlerType func);
};

#endif 
