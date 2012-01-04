#include <iostream>
#include "NodejsClient.h"

using namespace std;

NodejsClient *client; //, *client2;

void handleMessage(char* a, int size) {
    cout << "got a message (" << size << ") " << a << endl;
};

int main( int argc, char * argv [] )
{
    client = new NodejsClient("127.0.0.1", "1337");
    client->registerMessageHandler(&handleMessage);
    client->run();

    string msg;
    
    while(client->getSocketState()) {
        cin >> msg;
        client->sendMessage(msg);
    }
   
    client->closeClient();
    return 0;
}

