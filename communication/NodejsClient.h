#ifndef __NODEJSCLIENT
#define __NODEJSCLIENT

#define MAX_BUFFER_SIZE 1024
#define LOOP_TIME 0.01
#define NODEJSCLIENT_DEBUG true

#include <boost/asio.hpp> 
#include <boost/bind.hpp>
#include <boost/array.hpp> 
#include <boost/thread.hpp> 
#include <deque>
#include <iostream> 
#include <string> 

typedef void (*handlerType)(char*);
typedef std::deque<std::string> message_queue;

class NodejsClient {
    private:
    boost::asio::io_service io_service; 
    boost::asio::ip::tcp::resolver resolver; 
    boost::asio::ip::tcp::socket sock_in, sock_out; 
    boost::array<char, MAX_BUFFER_SIZE> msg_buffer; 
    
    boost::thread *th;
    
    int counter;

    handlerType messageHandler;

    int readinchild(int fd);

    public:
    int childReaderSock;
    void read_handler(const boost::system::error_code &ec, std::size_t bytes_transferred);
    
    void resolve_handler_in(const boost::system::error_code &ec, boost::asio::ip::tcp::resolver::iterator it);
    void resolve_handler_out(const boost::system::error_code &ec, boost::asio::ip::tcp::resolver::iterator it);

    void connect_handler_in(const boost::system::error_code &ec); 
    void connect_handler_out(const boost::system::error_code &ec); 

    
    NodejsClient(std::string Server_Name, std::string Server_Port_Out, std::string Server_Port_In );
    ~NodejsClient();

    void run();
    void closeClient();
    bool getSocketState();
    void sendMessage(std::string msg);
    void registerMessageHandler(handlerType func);
};

#endif 
