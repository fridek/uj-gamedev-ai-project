#include <iostream>
#include "NodejsClient.h"

using namespace std;

NodejsClient *client; //, *client2;

void handleMessage(char* a) {
    cout << "got a message (" << 0 << ") " << a << endl;
};

int main( int argc, char *argv [] )
{
    client = new NodejsClient("127.0.0.1", "1338", "1337");
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

