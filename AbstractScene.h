//
// Created by oleg on 28.11.17.
//

#ifndef SIMPLE_RTS_ABSTRACTSCENE_H
#define SIMPLE_RTS_ABSTRACTSCENE_H

#include <SFML/System/Clock.hpp>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "InterfaseScene.h"
#include <iterator>

struct UpdateData {
    std::string str;
    std::string type;
};

class AbstractObject:public InterfaseObject{
public:
    AbstractObject(const int id, Point point, int health):InterfaseObject(id), point(point), health(health){
        std::cout << "конструктор базового класса\n";
        //конструктор базового класса или неразрушимого блока или стенки или леса, или воды или базы
        //возможность столкнуться существует только в физической сцене
    };
    inline Point get_point(){
        return point;
    };
    virtual UpdateData serialise(){};
    virtual void apply(UpdateData){};
    void set_point(Point point){
        this->point = point;
    };
    //написать get_obj;
    ~AbstractObject(){};
    int health;
    void set_health( int new_healse){
        health = new_healse;
    }
    int get_health(){
        return health;
    }
protected:
    Point point;
};

class Directable
{
public:
    Directable(const int dir = 0):direct(dir){}
    void set_dir(const int dir)
    {
        this->direct = dir;
        return;
    }
    int& get_dir()
    {
        return direct;
    }
    virtual ~Directable(){};
protected:
    int direct;
};

class AbstrTank: public AbstractObject, public Directable{
public:
    AbstrTank(const int id, Point init_point = {0, 0}, const int dir = UP, const int health = 1):
            AbstractObject(id, init_point, health),
            Directable(dir)
    {
        std::cout << "конструктор танка\n";
    };
    ~AbstrTank(){};
    //премя последнего выстрела - используется в "create_abctract_bullet" для проерки допустимости выстрела
    sf::Time last_shoot;
    //написать get_obj
};

class AbstrDistrBlock: public AbstractObject{
public:
    AbstrDistrBlock(const int id, Point point = {0,0}, const int health = 1):AbstractObject(id, point, health){
        std::cout << "конструктор разрушимого блока\n";
    }
    //написать get_obj
};

//штаб, тут только переменная is_alive, которую мы будем устанавливать в обработчике столкновений.
class AbstractHeadquarters: public AbstractObject{
public:
    bool is_alive;
    AbstractHeadquarters(const int id, Point point):AbstractObject(id, point, 1), is_alive(true){
        std::cout << "конструктор штаба\n";
    }
};

class AbstrBullet: public AbstractObject, public Directable{
public:
    AbstrBullet(const int id, Point init_point = {0, 0}, const int dir = 0, const int health = 1):
            AbstractObject(id, init_point, health),
            Directable(dir)
    {
        std::cout << "конструктор пули\n";
        //!!!!!!! нужно время этого выстрела сохранить
    };
    ~AbstrBullet(){};
};

//class InterfaceScene {
//public:
//    virtual ~InterfaceScene(){};
//};

//абстрактная сцена
//общие данные остальных сцен
class AbstractScene: InterfaseScene{
public:
    AbstractScene():count_id(100) { //пусть начинается со 100, 1-4 будет выделено под фиктивные объекты неразрушимых краёв карты
        // всё поле 624 * 624
        accord_list[1] = "UpBorderline";
        obj_list[1] = new AbstractObject(1, Point{0, -16}, 1000); // ▀ верхняя стенка
        accord_list[2] = "DounBorderline";
        obj_list[2] = new AbstractObject(2, Point{0, 625}, 1000); // ▄ нижняя стенка
        accord_list[3] = "LeftBorderline";
        obj_list[3] = new AbstractObject(3, Point{-16, 0}, 1000); // ▌ левая стенка
        accord_list[4] = "RightBorderline";
        obj_list[4] = new AbstractObject(4, Point{625, 0}, 1000); // ▐ правая стенка
    }
    std::unordered_map <int , std::string> accord_list;
    std::unordered_map <int, AbstractObject*> obj_list;
    int add_obj(const int x,const int y, const std::string& type, int dir=UP)
    {
        accord_list[count_id] = type;
        if(type == "DistrBlock") {
            obj_list[count_id] = new AbstrDistrBlock(count_id, Point{x, y}, 1);
        } else if(type == "UnDistrBlock") {
            obj_list[count_id] = new AbstractObject(count_id, Point{x, y}, 1000);
        } else if(type == "Bullet") {
            obj_list[count_id] = new AbstrBullet(count_id, Point{x, y}, dir, 1);
        } else if(type == "Tank") {
            obj_list[count_id] = new AbstrTank(count_id, Point{x, y}, dir, 1);
        } else if(type == "PleerTank") { //отличие только в том, что не создаётся AI_tank
            obj_list[count_id] = new AbstrTank(count_id, Point{x, y}, dir, 1);
        } else if(type == "WaterBlock") {
            obj_list[count_id] = new AbstractObject(count_id, Point{x, y}, 1000);
        } else if(type == "HeadquartersBlock") {
            obj_list[count_id] = new AbstractObject(count_id, Point{x, y}, 1);
        } else if(type == "Spawner") {
            obj_list[count_id] = new AbstractObject(count_id, Point{x, y}, 1000);
        } else if(type == "Explosion") {
            obj_list[count_id] = new AbstractObject(count_id, Point{x, y}, 1000);
        }
        return count_id++;
    }

    //функция, очищающая список объектов от убитых с health <= 0
    void clear_dead(){
        std::vector <int> to_remove;
        for(auto i: obj_list){
            if(i.second->health <= 0){
                to_remove.push_back(i.first);

                //--on-dead-----
                if(accord_list[i.first]=="Tank" or accord_list[i.first]=="PleerTank"){
                    Point point = obj_list[i.first]->get_point();
                    add_obj(point.x, point.y, "Explosion");
                }
                //---циклические зависимости возникают, не получается убрать в объект---
            }
        }
        for(auto i: to_remove) {
            delete obj_list[i];
            obj_list.erase(i);
            accord_list.erase(i);
        }
    }

    // функция, возвращающая point объекта по id
    Point Get_point(int id){
        return obj_list[id]->get_point();
    }

    void load_map(std::fstream& file)
    {
        std::string str;
        int x = 0;
        int y = 0;
        while(std::getline(file, str))
        {
            for (auto& i : str){
                switch(i){
                    case '#':
                        this->add_obj(x, y, "DistrBlock");
                        break;
                    case '@':
                        this->add_obj(x, y, "UnDistrBlock");
                        break;
                    case '.':
                        break;
                    case '!': //база
                        this->add_obj(x, y, "HeadquartersBlock");
                        break;
                    case '~': //вода
                        this->add_obj(x, y, "WaterBlock");
                        break;
                    case 's': //создатель танков
                        this->add_obj(x, y, "Spawner");
                        break;
                }
                x += 24; //3 * 8(px в блоке)
            }
            x = 0;
            y += 24;
        }
    }

    std::vector<std::string>&  get_obj()
    {
        //
    }

    ~AbstractScene(){};
private:
    sf::Clock clock;
public:
    //функция, создающая пулю около этого танка с учётом направления дула танка
    void create_abctract_bullet(int id){
        //напоминание: танк 15*3 на 15*3
        //пуля 4*3 на 4*3
        AbstrTank *abstract_tank = dynamic_cast<AbstrTank *>(this->obj_list[id]);
        Point point = abstract_tank->get_point();
        sf::Time now = clock.getElapsedTime();
        if(now - abstract_tank->last_shoot > sf::seconds(0.5)) {
            abstract_tank->last_shoot = now;

            switch (abstract_tank->get_dir()) {
                case DOWN:  // ⍗
                    add_obj(point.x + 16, point.y + 45, "Bullet", DOWN);
                    break;
                case RIGHT: // ⍈
                    add_obj(point.x + 45, point.y + 16, "Bullet", RIGHT);
                    break;
                case UP:    // ⍐
                    add_obj(point.x + 16, point.y - 12, "Bullet", UP);
                    break;
                case LEFT:  // ⍇
                    add_obj(point.x - 12, point.y + 16, "Bullet", LEFT);
                    break;
            }
            std::cout << "Создали пулю\n" << std::endl;
        }
    }

protected:
    int count_id;
};

#endif //SIMPLE_RTS_ABSTRACTSCENE_H
