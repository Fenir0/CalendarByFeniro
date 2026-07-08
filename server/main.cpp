#include "inc/webServerSocket.h"

int main(){
    std::cout << "Web server is running:\n";
    try {
        asio::io_context ioc;
        tcp::endpoint    endpoint(tcp::v4(), 9002);
        WebSocketServer  server(ioc, endpoint);
        ioc.run();
    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}