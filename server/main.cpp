#include "inc/webServerSocket.h"
#include <pqxx/pqxx>

#include <string>

int main(){
    try {
    // Connect to the database
        pqxx::connection c("user=postgres password=admin host=localhost port=5432 dbname=practice target_session_attrs=read-write");
        
        pqxx::work w(c);

        asio::io_context ioc;
        tcp::endpoint    endpoint(tcp::v4(), 9002);
        WebSocketServer  server(ioc, endpoint);
        ioc.run();

        w.exec("smth to execute");
        w.commit();


    } 
    catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}