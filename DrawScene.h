//
// Created by oleg on 28.11.17.
//

#ifndef SIMPLE_RTS_DRAWSCENE_H
#define SIMPLE_RTS_DRAWSCENE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>
#include "InterfaseScene.h"
#include "AbstractScene.h"

//рисуемый объект - знает, как нарисовать себя на передаваемом холсте
class DrawObject: public InterfaseObject{
public:
    DrawObject(int id, sf::Sprite * sprite):InterfaseObject(id), sprite(sprite){};
    sf::Sprite * sprite = nullptr; // основной рисуемый объект
    virtual void draw(sf::RenderWindow &window, AbstractScene *abstract_scene) = 0; //сам объект не имеет смысла, это интерфейс.
    ~DrawObject(){
        // все картинки инициализируются и хранятся в сцене, и в ней же уничтожаются в деструкторе, что бы избежать дублирования.
    };
};

//стенка
class DrawBlock: public DrawObject{
public:
    DrawBlock(const int id, sf::Sprite * sprite) : DrawObject(id, sprite){};
    ~DrawBlock(){};
    void draw(sf::RenderWindow &window, AbstractScene *abstract_scene){
        Point point = abstract_scene->Get_point(id);
        sprite->setPosition(point.x, point.y);
        window.draw(*sprite);
    }
};

//штаб
class DrawHeadquarters: public DrawObject{
public:
    DrawHeadquarters(int id, sf::Sprite *living_headquarters_sprite,
        sf::Sprite *dead_headquarters_sprite):
        DrawObject(id, living_headquarters_sprite),
        living_headquarters_sprite(living_headquarters_sprite),
        dead_headquarters_sprite(dead_headquarters_sprite){};
    ~DrawHeadquarters(){};
    void draw(sf::RenderWindow &window, AbstractScene *abstract_scene){
        Point point = abstract_scene->Get_point(id);
        sprite->setPosition(point.x, point.y);
        AbstractObject * abstract_object = abstract_scene->obj_list[id];
        AbstractHeadquarters *abstract_headquarters = dynamic_cast<AbstractHeadquarters *>(abstract_object);
        bool is_alive = abstract_headquarters->is_alive; // Мы с самог начала думали об этом, когда разделили на сцены
        if(is_alive){
            sprite = living_headquarters_sprite;
        } else {
            sprite = dead_headquarters_sprite;
        }
        window.draw(*sprite);
    }

private:
    sf::Sprite *living_headquarters_sprite;
    sf::Sprite *dead_headquarters_sprite;
};
class DrawBullet: public DrawObject{
public:
    DrawBullet(
            const int id,
            sf::Sprite *up_sprite,
            sf::Sprite *down_sprite,
            sf::Sprite *right_sprite,
            sf::Sprite *left_sprite):
            DrawObject(id, up_sprite){
        this->up_sprite = up_sprite;
        this->down_sprite = down_sprite;
        this->right_sprite = right_sprite;
        this->left_sprite = left_sprite;
    };
    void draw(sf::RenderWindow &window, AbstractScene *abstract_scene){
        //получаем направление
        //обращаясь к отображению этого объекта в абстрактной сцене.
        AbstractObject * abstract_object = abstract_scene->obj_list[id];
        AbstrBullet *abstr_bullet = dynamic_cast<AbstrBullet *>(abstract_object);
        int direction = abstr_bullet->get_dir(); // Мы с самог начала думали об этом, когда разделили на сцены
        switch(direction) {
            case DOWN:
                sprite = down_sprite;
                break;
            case RIGHT:
                sprite = right_sprite;
                break;
            case UP:
                sprite = up_sprite;
                break;
            case LEFT:
                sprite = left_sprite;
                break;
        }
        Point point = abstract_scene->Get_point(id);
        sprite->setPosition(point.x, point.y);
        window.draw(*sprite);
    };
private:
    sf::Sprite *up_sprite;
    sf::Sprite *down_sprite;
    sf::Sprite *right_sprite;
    sf::Sprite *left_sprite;
    // sf::Sprite *sprite; - есть в DrawObject, равен одному из четырёх спрайтов
};

//цель - реализовать отрисовку танка
class DrawTank: public DrawObject{
public:
    DrawTank(const int id,
             sf::Sprite *up_sprite,
             sf::Sprite *down_sprite,
             sf::Sprite *right_sprite,
             sf::Sprite *left_sprite):
            DrawObject(id, up_sprite){
        this->up_sprite = up_sprite;
        this->down_sprite = down_sprite;
        this->right_sprite = right_sprite;
        this->left_sprite = left_sprite;
    };
    ~DrawTank(){
        // все картинки инициализируются и хранятся в сцене, и в ней же уничтожаются, что бы избежать дублирования.
    };
    void draw(sf::RenderWindow &window, AbstractScene *abstract_scene){
        //получаем направление
        //обращаясь к отображению этого объекта в абстрактной сцене.
        AbstractObject * abstract_object = abstract_scene->obj_list[id];
        AbstrTank *abstr_tank = dynamic_cast<AbstrTank *>(abstract_object);
        int direction = abstr_tank->get_dir(); // Мы с самог начала думали об этом, когда разделили на сцены

        switch(direction) {
            case DOWN:
                sprite = down_sprite;
                break;
            case RIGHT:
                sprite = right_sprite;
                break;
            case UP:
                sprite = up_sprite;
                break;
            case LEFT:
                sprite = left_sprite;
                break;
        }
        Point point = abstract_scene->Get_point(id);
        sprite->setPosition(point.x, point.y);
        window.draw(*sprite);
    };
private:
    sf::Sprite *up_sprite;
    sf::Sprite *down_sprite;
    sf::Sprite *right_sprite;
    sf::Sprite *left_sprite;
    // sf::Sprite *sprite; - есть в DrawObject, равен одному из четырёх спрайтов
};

//взрыв из трёх кадров. Не имеет физического объекта. самоуничтожается
class DrawExplosion: public DrawObject{
public:
    DrawExplosion(const int id,
                  sf::Sprite *explosion_1,
                  sf::Sprite *explosion_2,
                  sf::Sprite *explosion_3):
            DrawObject(id, explosion_1),
            explosion_1(explosion_1),
            explosion_2(explosion_2),
            explosion_3(explosion_3),
            phase(0){
    };
    void draw(sf::RenderWindow &window, AbstractScene *abstract_scene){
        switch (phase / 20){
            case 0:
                sprite = explosion_1;
            case 1:
                sprite = explosion_2;
            case 2:
                sprite = explosion_3;
        }
        Point point = abstract_scene->Get_point(id);
        sprite->setPosition(point.x, point.y);
        window.draw(*sprite);
        phase++;
        if (phase == 60){
            //надо вызвать удаление этого объекта и его отображения в абстрактном классе.
        }
    };

private:
    sf::Sprite *explosion_1;
    sf::Sprite *explosion_2;
    sf::Sprite *explosion_3;
    int phase; // пусть меняется каждые 20 кадров
};

// напишем рисующую сцену
class DrawScene: public InterfaseScene{
public:
    //вся картинка
    sf::Image   all_image; //создаем объект Image (изображение)
    sf::Texture all_texture;
    sf::Sprite  block_sprite;
    sf::Sprite  indestructible_block_sprite;
    sf::Sprite  water_sprite;
    //штаб
    sf::Sprite  iliving_headquarters_sprite;
    sf::Sprite  dead_headquarters_sprite;
    //танк
    sf::Sprite  tank_up_sprite;
    sf::Sprite  tank_down_sprite;
    sf::Sprite  tank_right_sprite;
    sf::Sprite  tank_left_sprite;
    //пуля
    sf::Sprite  bullet_up_sprite;
    sf::Sprite  bullet_down_sprite;
    sf::Sprite  bullet_right_sprite;
    sf::Sprite  bullet_left_sprite;
    //взрыв
    sf::Sprite  explosion_1;
    sf::Sprite  explosion_2;
    sf::Sprite  explosion_3;

    DrawScene(){
        all_image.loadFromFile("/home/oleg/CLionProject/simple_RTS/60016.png");
        //вырезаем чёрный цвет, делая эту часть прозрачной
        all_image.createMaskFromColor(sf::Color(0, 0, 0));
        all_texture.loadFromImage(all_image);
        block_sprite.setTexture(all_texture);
        block_sprite.setTextureRect(sf::IntRect(256, 0, 8, 8));
        block_sprite.scale(3, 3); //да

        indestructible_block_sprite.setTexture(all_texture);
        indestructible_block_sprite.setTextureRect(sf::IntRect(256, 16, 8, 8));
        indestructible_block_sprite.scale(3, 3); //да

        water_sprite.setTexture(all_texture);
        water_sprite.setTextureRect(sf::IntRect(256, 48, 8, 8));
        water_sprite.scale(3, 3); // да

        iliving_headquarters_sprite.setTexture(all_texture);
        iliving_headquarters_sprite.setTextureRect(sf::IntRect(304, 32, 16, 16));
        iliving_headquarters_sprite.scale(3, 3); // да

        dead_headquarters_sprite.setTexture(all_texture);
        dead_headquarters_sprite.setTextureRect(sf::IntRect(320, 32, 16, 16));
        dead_headquarters_sprite.scale(3, 3); // да

        tank_up_sprite.setTexture(all_texture);
        tank_up_sprite.setTextureRect(sf::IntRect(0, 1, 15, 15));
        tank_up_sprite.scale(3, 3);

        tank_down_sprite.setTexture(all_texture);
        tank_down_sprite.setTextureRect(sf::IntRect(64, 0, 15, 15));
        tank_down_sprite.scale(3, 3);

        tank_right_sprite.setTexture(all_texture);
        tank_right_sprite.setTextureRect(sf::IntRect(97, 0, 15, 15));
        tank_right_sprite.scale(3, 3);

        tank_left_sprite.setTexture(all_texture);
        tank_left_sprite.setTextureRect(sf::IntRect(33, 0, 15, 15));
        tank_left_sprite.scale(3, 3);

        bullet_up_sprite.setTexture(all_texture);
        bullet_up_sprite.setTextureRect(sf::IntRect(323, 102, 3, 4));
        bullet_up_sprite.scale(3, 3);

        bullet_down_sprite.setTexture(all_texture);
        bullet_down_sprite.setTextureRect(sf::IntRect(339, 102, 3, 4));
        bullet_down_sprite.scale(3, 3);

        bullet_right_sprite.setTexture(all_texture);
        bullet_right_sprite.setTextureRect(sf::IntRect(346, 102, 4, 3));
        bullet_right_sprite.scale(3, 3);

        bullet_left_sprite.setTexture(all_texture);
        bullet_left_sprite.setTextureRect(sf::IntRect(330, 102, 4, 3));
        bullet_left_sprite.scale(3, 3);

        explosion_1.setTexture(all_texture);
        explosion_1.setTextureRect(sf::IntRect(256, 128, 16, 16));
        explosion_1.scale(3, 3);

        explosion_2.setTexture(all_texture);
        explosion_2.setTextureRect(sf::IntRect(272, 128, 16, 16));
        explosion_2.scale(3, 3);

        explosion_3.setTexture(all_texture);
        explosion_3.setTextureRect(sf::IntRect(288, 128, 16, 16));
        explosion_3.scale(3, 3);

        //будем добавлять структуры по мере необходимости
    };

    std::unordered_map <int, DrawObject*> object_list;
    void add_obj(const int id, const std::string& type){
        // собираем объекты в зависимости от их типа
               if(type == "DistrBlock"){
            object_list[id] = new DrawBlock(id, &block_sprite);
        } else if (type == "UnDistrBlock"){ //а не надо отдельного класса
            object_list[id] = new DrawBlock(id, &indestructible_block_sprite);
        } else if (type == "WaterBlock") { //а не надо отдельного класса
            object_list[id] = new DrawBlock(id, &water_sprite);
        } else if (type == "HeadquartersBlock") {
            object_list[id] = new DrawBlock(id, &iliving_headquarters_sprite);
        } else if (type == "Tank") {
            object_list[id] = new DrawTank(id, &tank_up_sprite,
                                               &tank_down_sprite,
                                               &tank_right_sprite,
                                               &tank_left_sprite);
        } else if (type == "PleerTank") {
            object_list[id] = new DrawTank(id, &tank_up_sprite, //TODO: другие текстуры игроку
                                               &tank_down_sprite,
                                               &tank_right_sprite,
                                               &tank_left_sprite);
        } else if (type == "Bullet") {
            object_list[id] = new DrawBullet(id, &bullet_up_sprite, &bullet_down_sprite,
                                                 &bullet_right_sprite, &bullet_left_sprite);
        }
    }

    void synchronize(AbstractScene *abstract_scene){
        //забирает измемения из абстрактной сцены и создаёт объекты
        for(auto i: abstract_scene->obj_list){
            if(object_list.find(i.first/*id*/) == object_list.end()){
                //если нет объекта в рисующей схеме
                //значит его надо создать
                add_obj(i.first/*id*/, abstract_scene->accord_list[i.first/*id*/]);
            }
        }
        //сверяет изменения с абстрактной сценой и удаляет объекты
        std::vector<int> to_remove;
        for(auto i: this->object_list) {
            if (abstract_scene->obj_list.find(i.first/*id*/) ==
                abstract_scene->obj_list.end()) {
                //если нет объекта в абстрактной сцене
                //значит его надо удалить и из нашей сцены
                to_remove.push_back(i.first);
            }
        }
        for (auto i : to_remove) {
            object_list.erase(i);
        }
    };
    //глобальная функция рисования
    //проходится по object_list и вызывает для draw каждого
    void draw(sf::RenderWindow &window, AbstractScene *abstract_scene)
    {
        for (auto& i : object_list) {
            //ну никак без этого, мы разделили сцены, хотя логически они связанны общим значением x, y
            i.second->draw(window, abstract_scene);
        }
    }

    ~DrawScene(){
        //всё статическое, и удалится само.
    };
};

#endif //SIMPLE_RTS_DRAWSCENE_H
