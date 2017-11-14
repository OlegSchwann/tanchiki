#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#define PORT 2000

using namespace sf;

int main()
{   IpAddress ip = IpAddress::getLocalAddress();
    TcpSocket socket;
    Packet packet;
    TcpListener listener;
    listener.listen(PORT);
    listener.accept(socket);
    char data[20];
    size_t received;
    socket.receive(data, sizeof(data), received);
    Font font;
    font.loadFromFile("/home/mrfalcon/CLionProjects/Dandy_Tanks/arial.ttf");
    Text text(data, font, 20);
    RenderWindow window(VideoMode(200, 200), "nettest");
    while(window.isOpen())
    {
        Event event;
        while(window.pollEvent(event))
        {
            if(event.type == Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(text);
        window.display();
    }
    return 0;
}

