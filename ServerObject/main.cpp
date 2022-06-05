#include "testserver.hpp"

int main() {
    my::Server server("TesterTron");
    server.defineSocket(AF_INET, SOCK_STREAM, 0);
    server.init(8080, 10);
    server.connection(10000);

    my::Server server2("TesterTron2");
    server2.defineSocket(AF_INET, SOCK_STREAM, 0);
    server2.init(4242, 10);
    server2.connection(10000);

    return (0);
}