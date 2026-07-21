#include "inc/webServerSocket.h"
#include <pqxx/pqxx>

#include <string>

int main(){
    try {
        asio::io_context ioc;
        tcp::endpoint    endpoint(tcp::v4(), 9002);
        WebSocketServer  server(ioc, endpoint);
        ioc.run();
    } 
    catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}