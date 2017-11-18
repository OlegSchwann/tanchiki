#include <SFML/Graphics.hpp>
#include "player.h"

using namespace sf;

int main()
{
    RenderWindow window(VideoMode(800, 800), "player");
    player player1;
    while(window.isOpen())
    {
        Event event;
        while(window.pollEvent(event))
        {
            if(event.type == Event::Closed)
                window.close();
        }

        if(Keyboard::isKeyPressed(Keyboard::S))
            player1.move(DOWN);
        else if(Keyboard::isKeyPressed(Keyboard::D))
            player1.move(RIGHT);
        else if(Keyboard::isKeyPressed(Keyboard::W))
            player1.move(UP);
        else if(Keyboard::isKeyPressed(Keyboard::A))
            player1.move(LEFT);

        window.clear();
        window.draw(player1.get_texture());
        window.display();
    }
    return 0;
}