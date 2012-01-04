#ifndef __NODEJSCLIENT
#define __NODEJSCLIENT

#define MAX_BUFFER_SIZE 1024
#define LOOP_TIME 0.01
#define NODEJSCLIENT_DEBUG true

#include <boost/asio.hpp> 
#include <boost/bind.hpp>
#include <boost/array.hpp> 
#include <boost/thread.hpp> 
#include <iostream> 
#include <string> 

typedef void (*handlerType)(char*, int);

class NodejsClient {
    private:
    boost::asio::io_service io_service; 
    boost::asio::ip::tcp::resolver resolver; 
    boost::asio::ip::tcp::socket sock; 
    boost::array<char, MAX_BUFFER_SIZE> msg_buffer; 
    
    boost::thread *th;

    handlerType messageHandler;

    int readinchild(int fd);

    public:
    int childReaderSock;
    void read_handler(const boost::system::error_code &ec, std::size_t bytes_transferred);
    void resolve_handler(const boost::system::error_code &ec, boost::asio::ip::tcp::resolver::iterator it);
    void connect_handler(const boost::system::error_code &ec); 

    NodejsClient(std::string Server_IP, std::string Server_Port );
    ~NodejsClient();

    void run();
    void closeClient();
    bool getSocketState();
    void sendMessage(std::string msg);
    void registerMessageHandler(handlerType func);
};

#endif 
