#include <iostream>
#include <SFML/Network.hpp>

using namespace sf;

int main()
{
    std::string hostname = "mrfalcon-Lenovo-G505s";
    TcpSocket socket;
    socket.connect(hostname, 2000);
    char data[20];
    std::cin >> data;
    socket.send(data, sizeof(data));
    return 0;
}
