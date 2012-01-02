#include <iostream>
#include "NodejsClient.h"

using namespace std;

NodejsClient *client, *client2;

void handleMessage(char* a) {
    cout << a << endl;
};

int main( int argc, char * argv [] )
{
    client = new NodejsClient("127.0.0.1", 1337);
    client->registerMessageHandler(&handleMessage);
    client->run();

    client2 = new NodejsClient("127.0.0.1", 1337);
    client2->registerMessageHandler(&handleMessage);
    client2->run();
    
   while(!client->getSocketState()) {
        char* msg = new char[MAX_BUFFER_SIZE];
        cin >> msg;        
        client->sendMessage(msg);
        client2->closeClient();
   }
   
   client->closeClient();
   return 0;
}

