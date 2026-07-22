#include "inc/webServerSocket.h"
#include <pqxx/pqxx>

#include <string>

int main(){
    try {
        asio::io_context ioc;
        
        tcp::endpoint endpoint_v6(tcp::v6(), 9002);
        WebSocketServer server_v6(ioc, endpoint_v6);
        ioc.run();
    } 
    catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}