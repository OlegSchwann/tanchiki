//
// Created by mrfalcon on 17.11.17.
//

#ifndef RESOURCE_PLAYER_H
#define RESOURCE_PLAYER_H
#include <iostream>
#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>

#define DOWN 0
#define RIGHT 1
#define UP 2
#define LEFT 3

using namespace sf;

class player
{
public:
    player(const double x = 0, const double y = 0);
    ~player();
    Sprite get_texture() const;
    void move(const int direction);
    /*void shoot(const int direction);
    void set_ip(const std::string ip);
    void get_ip() const;*/
private:
    const std::string ip;
    double x;
    double y;
    Image tank_image;
    Texture tank_texture;
    Sprite tank_sprite;
    //audio
};

#endif //RESOURCE_PLAYER_H
