#include "NodejsClient.h"

using namespace std;

/**
 * @param Server_Name hostname or IP
 * @param Server_Port port number
 */
NodejsClient::NodejsClient(std::string Server_Name, std::string Server_Port_Out, std::string Server_Port_In) : 
    resolver(io_service), sock_in(io_service), sock_out(io_service) {
    counter = 0;
    boost::asio::ip::tcp::resolver::query query_in(Server_Name, Server_Port_In);
    boost::asio::ip::tcp::resolver::query query_out(Server_Name, Server_Port_Out);

    resolver.async_resolve(query_out,
      boost::bind(&NodejsClient::resolve_handler_out, this, 
        boost::asio::placeholders::error, 
        boost::asio::placeholders::iterator
      )
    );
    
    resolver.async_resolve(query_in,
      boost::bind(&NodejsClient::resolve_handler_in, this, 
        boost::asio::placeholders::error, 
        boost::asio::placeholders::iterator
      )
    );    
}

NodejsClient::~NodejsClient() {
    closeClient();
}

/**
 * Called in a loop for each read
 */
void NodejsClient::read_handler(const boost::system::error_code &ec, std::size_t bytes_transferred) {
    if(bytes_transferred) messageHandler(msg_buffer.data());
    
    sock_in.async_read_some(boost::asio::buffer(msg_buffer, MAX_BUFFER_SIZE), 
	boost::bind(&NodejsClient::read_handler, this,
	  boost::asio::placeholders::error, 
	  boost::asio::placeholders::bytes_transferred
	));   
}

/**
 * Called asynchronously when connection to socket is successfull
 */
void NodejsClient::connect_handler_in(const boost::system::error_code &ec) { 
  if (!ec) 
  { 
    sock_in.async_read_some(boost::asio::buffer(msg_buffer, MAX_BUFFER_SIZE), 
	boost::bind(&NodejsClient::read_handler, this,
	  boost::asio::placeholders::error, 
	  boost::asio::placeholders::bytes_transferred
	));   
  } 
}

/**
 * Called asynchronously when connection to socket is successfull
 */
void NodejsClient::connect_handler_out(const boost::system::error_code &ec) { 
  if (!ec) 
  { 
    boost::asio::write(sock_out, boost::asio::buffer("hello mr server"));
  } 
}

/**
 * Called asynchronously when Server_Name is resolved
 */
void NodejsClient::resolve_handler_in(const boost::system::error_code &ec, boost::asio::ip::tcp::resolver::iterator it) { 
  if (!ec) { 
    cout << "socket connected successfully" << endl;    
      boost::asio::async_connect(sock_in, it, boost::bind(&NodejsClient::connect_handler_in, this,
	boost::asio::placeholders::error
      )); 
  } 
} 

/**
 * Called asynchronously when Server_Name is resolved
 */
void NodejsClient::resolve_handler_out(const boost::system::error_code &ec, boost::asio::ip::tcp::resolver::iterator it) { 
  if (!ec) { 
    cout << "socket connected successfully" << endl;    
      boost::asio::async_connect(sock_out, it, boost::bind(&NodejsClient::connect_handler_out, this,
	boost::asio::placeholders::error
      )); 
  } 
} 

/**
 * Create new thread for io_service and run
 */
void NodejsClient::run() {
  th = new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service));
}

/**
 * Close socket and thread
 */
void NodejsClient::closeClient() {
  sock_in.close();
  sock_out.close();
  th->join();
}

/**
 * @param msg Message to send to server
 */
void NodejsClient::sendMessage(string msg) {
  cout << "sending message: " << msg << endl;
  boost::asio::write(sock_out, boost::asio::buffer(msg.c_str(), msg.length())); 
}

/**
 * Sets function to be called on each socket read
 */
void NodejsClient::registerMessageHandler(handlerType func) {
    messageHandler = func;
}

/**
 * @TODO: let server close the program by returning false here
 */
bool NodejsClient::getSocketState() {
  return true;
}

