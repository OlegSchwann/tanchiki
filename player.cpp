//
// Created by mrfalcon on 18.11.17.
//

#include <iostream>
#include "player.h"

using namespace sf;

player::player(const double x, const double y)
    :x(x),
     y(y)
{
    tank_image.loadFromFile("/home/mrfalcon/CLionProjects/resource/Resources/Sprites/60016.png");
    tank_image.createMaskFromColor(Color(0, 0, 0));
    tank_texture.loadFromImage(tank_image);
    tank_sprite.setTexture(tank_texture);
    tank_sprite.setTextureRect(IntRect(64, 0, 15, 15));
    tank_sprite.scale(3, 3);
    tank_sprite.setPosition(x, y);
}
player::~player()
{}
Sprite player::get_texture() const
{
    return tank_sprite;
}
void player::move(const int direction)
{
    switch(direction)
    {
        case DOWN:
            tank_sprite.setTextureRect(IntRect(64, 0, 15, 15));
            tank_sprite.move(0, 0.05);
            y += 0.05;
            break;
        case RIGHT:
            tank_sprite.setTextureRect(IntRect(97, 0, 15, 15));
            tank_sprite.move(0.05, 0);
            x += 0.05;
            break;
        case UP:
            tank_sprite.setTextureRect(IntRect(0, 1, 15, 15));
            tank_sprite.move(0, -0.05);
            y -= 0.05;
            break;
        case LEFT:
            tank_sprite.setTextureRect(IntRect(33, 0, 15, 15));
            tank_sprite.move(-0.05, 0);
            x -= 0.05;
            break;
    }
}