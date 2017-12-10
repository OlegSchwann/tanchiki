//
// Created by oleg on 29.11.17.
//

#ifndef SIMPLE_RTS_PHISICALSCENE_H
#define SIMPLE_RTS_PHISICALSCENE_H


#include <SFML/System/Time.hpp>
#include <unordered_map>
#include "InterfaseScene.h"
#include "AbstractScene.h"
// как помним, из абстрактной сцены мы вытаскиваем обновления(синхронизируемся),
// кординаты(и кординаты после обработки возвращаем)
// вытаскиваем и устанавливаем направление движения
#include <SFML/Graphics/Rect.hpp> //прямоугольник с возможностью проверки коллизий

//физический объект
class PhisicalObject:public InterfaseObject {
public:
    PhisicalObject(const int id, const int heigth, const int width, int speed=0):
        InterfaseObject(id),
        heigth(heigth),
        width(width),
        speed(speed){
    };
    // рамка, в которой сейчас находится объект
    sf::Rect <int>now_rectangle(AbstractScene *abstract_scene){
        Point point = abstract_scene->obj_list[id]->get_point();
        return sf::Rect <int>(point.x, point.y, width, heigth);
    };
    //sf::Rect future_rect(); - определяется только в движимом объекте

    ~PhisicalObject() {};
    // виртуальный обработчик урона в 1 health
    // (оставим здоровье в абстрактном классе) -
    // вдруг придётся рисовать шкалу здоровья?
    // если не killable - ничего
    // если killable - уменьшаем здоровье на 1
    // потом в обработчике тика в абстрактном классе
    // если здоровье == 0 то удаляем объект из абстрактной сцены
    // остальные две сцены синхронизируются с абстрактной сами
    // Abs   Draw/Phis                 Abs   Draw/Phis
    // (1) --> (0) после синхронизации (1) --> (1)
    // (0) --> (1) после синхронизации (0) --> (0)
    // (1) --> (1) после синхронизации (1) --> (1)
    virtual void make_damage(AbstractScene *abstract_scene) = 0;

    // вся логика перемещения в handle_tick() - прописано поведение каждого объекта
    // если не движимый - return
    // если есть скорость и направление -
    //     вычисляет новую область в форме прямоугольника
    //     future_rect();
    //     с каждым объектом физической сцены
    //         получает его область в данный момент
    //         now_rectangle();
    //         если столкнулся -
    //             если наш объект имеет урон
    //                 вызывает обработчики урона с двух сторон
    //                 прерываем цикл, выходим из функции
    //             если нет урона - выходим из функции - упёрлись в стенку
    //     если не столкнулся ни с одним объектом и вышел из цикла - move();
    virtual void handle_tick(AbstractScene *abstract_scene,  std::unordered_map <int, PhisicalObject*> *object_list) = 0;
    // мы храним скорость по модулю в физической части, a направление в абстрактной
    // потому нужен доступ к атрибутам отображения этого объекта в абстрактной сцене
    void set_speed(int speed){
        this->speed = speed;
    }
    int get_speed(){
        return this->speed;
    }
protected:
    int speed;
    // у блоков = 0 и не используется
    // действительно движущиеся объекты содержат direct в абстрактном классе
    const int width; // по x, горизонтально
    const int heigth; // по y, вертикально(сверху вниз)
    // должно совпадать по размеру с текстурами -
    // не забудь, что они в 3 раза смасштабированы
};

class PhisicalTank: public PhisicalObject{
public:
    PhisicalTank(const int id, const int heigth, const int width, int speed):
        PhisicalObject(id, heigth, width, speed), recently_collided(false){
        //std::cout << "конструктор физического танка"<<std::endl;
    };
    ~PhisicalTank(){};
protected:
    void move(AbstractScene *abstract_scene){
        AbstractObject * abstract_object = abstract_scene->obj_list[id];
        AbstrTank *abstract_tank = dynamic_cast<AbstrTank *>(abstract_object);
        Point point = abstract_tank->get_point();
        int direct = abstract_tank->get_dir();
        //std::cout << "координаты танка " << point.x << ", " << point.y;
        switch (direct){
            case UP:
                point.y -= speed;
                break;
            case DOWN:
                point.y += speed;
                break;
            case LEFT:
                point.x -= speed;
                break;
            case RIGHT:
                point.x += speed;
                break;
        }
        //std::cout << "новые координаты танка " << point.x << ", " << point.y;
        abstract_tank->set_point(point);
    };
    // "недавно столкнулся" требуется в сцене искуственного интеллекта
    bool recently_collided;
public:
    bool did_collided(){
        return recently_collided;
    }

    void make_damage(AbstractScene *abstract_scene){
        AbstractObject * abstract_object = abstract_scene->obj_list[id];
        AbstrTank *abstract_tank = dynamic_cast<AbstrTank *>(abstract_object);
        abstract_tank->set_health(abstract_tank->get_health()-1); // и это ссылка, экземпляр изменился в другой сцене
    };

    // рамка, в которой будет находиться объект, если ему ничего не помешает
    sf::Rect <int>get_future_rectangle(AbstractScene *abstract_scene){
        AbstractObject * abstract_object = abstract_scene->obj_list[id];
        AbstrTank *abstract_tank = dynamic_cast<AbstrTank *>(abstract_object);
        Point point = abstract_tank->get_point();
        int direct = abstract_tank->get_dir();
        switch (direct){
            case UP:
                point.y -= speed;
                break;
            case DOWN:
                point.y += speed;
                break;
            case LEFT:
                point.x -= speed;
                break;
            case RIGHT:
                point.x += speed;
                break;
        }
        return sf::Rect <int>(point.x, point.y, width, heigth);
    };

    void handle_tick(AbstractScene *abstract_scene, std::unordered_map <int, PhisicalObject*> *object_list){
        sf::Rect <int>future_rectangle = get_future_rectangle(abstract_scene);
        for (auto i : *object_list){
            //ROFL! При первых тестах танк не двигался - сталкивался сам с собой
            bool is_intersect = i.second->now_rectangle(abstract_scene).intersects(future_rectangle) && i.first != id;
            if (is_intersect){
                this->recently_collided = true; //Это для AITank.
                //нет урона при наезде танка
                std::cout << "танк столкнулся c " << abstract_scene->accord_list[i.first] << "\n";
                return;
            }
        }
        this->recently_collided = false; //Это для AITank.
        move(abstract_scene);
    };
};

class PhisicalBullet: public PhisicalObject{
public:
    PhisicalBullet(const int id, const int heigth, const int width, const int speed):
        PhisicalObject(id, heigth, width, speed){
        std::cout << "Конструктор PhisicalBullet\n";
    };
    ~PhisicalBullet(){};
protected:
    void move(AbstractScene *abstract_scene){
        AbstractObject * abstract_object = abstract_scene->obj_list[id];
        AbstrBullet *abstract_bullet = dynamic_cast<AbstrBullet *>(abstract_object);
        Point point = abstract_bullet->get_point();
        //std::cout << "Координаты PhisicalBullet " << point.x << ", " << point.y << "\n";
        int direct = abstract_bullet->get_dir();
        switch (direct){
            case UP:
                point.y -= speed;
                break;
            case DOWN:
                point.y += speed;
                break;
            case LEFT:
                point.x -= speed;
                break;
            case RIGHT:
                point.x += speed;
                break;
        }
        abstract_bullet->set_point(point);
        //std::cout << "новые координаты PhisicalBullet " << point.x << ", " << point.y << "\n";
    };
public:
    void make_damage(AbstractScene *abstract_scene){
        AbstractObject * abstract_object = abstract_scene->obj_list[id];
        abstract_object->set_health((abstract_object->get_health())-1);
    };

    // рамка, в которой будет находиться объект, если ему ничего не помешает
    sf::Rect <int>get_future_rectangle(AbstractScene *abstract_scene){
        AbstractObject * abstract_object = abstract_scene->obj_list[id];
        AbstrBullet *abstract_bullet = dynamic_cast<AbstrBullet *>(abstract_object);
        Point point = abstract_bullet->get_point();
        int direct = abstract_bullet->get_dir();
        switch (direct){
            case UP:
                point.y -= speed;
                break;
            case DOWN:
                point.y += speed;
                break;
            case LEFT:
                point.x -= speed;
                break;
            case RIGHT:
                point.x += speed;
                break;
        }
        return sf::Rect <int>(point.x, point.y, width, heigth);
    };

    void handle_tick(AbstractScene *abstract_scene, std::unordered_map <int, PhisicalObject*> *object_list){
        sf::Rect <int>future_rectangle = get_future_rectangle(abstract_scene);
        for (auto i : *object_list){
            bool is_intersect = i.second->now_rectangle(abstract_scene).intersects(future_rectangle) && i.first != id;
            if (is_intersect){
                // пуля нанесла урон в двух
                i.second->make_damage(abstract_scene);
                make_damage(abstract_scene);
                std::cout << "Пуля столкнулась с " << abstract_scene->accord_list[i.first] <<
                        ", здоровье = " << abstract_scene->obj_list[i.first]->get_health() << "\n";
                return;
            }
        }
        move(abstract_scene);
    };
};

//блок не двигается и не получает урон
class UnDistrPhisicalBlock: public PhisicalObject{
public:
    UnDistrPhisicalBlock(const int id, const int heigth, const int width) : PhisicalObject(id, heigth, width){
    };
    ~UnDistrPhisicalBlock(){};
    void make_damage(AbstractScene *abstract_scene){}; //неразрушимая стальная стена
    void handle_tick(AbstractScene *abstract_scene, std::unordered_map <int, PhisicalObject*> *object_list){}; // тупо стоит на месте
};

//блок не двигается и получает урон
class PhisicalBlock: public PhisicalObject{
public:
    PhisicalBlock(const int id, const int heigth, const int width) : PhisicalObject(id, heigth, width){
    };
    ~PhisicalBlock(){};
    void make_damage(AbstractScene *abstract_scene){//разрушимая кирпичная стена
        AbstractObject * abstract_object = abstract_scene->obj_list[id];
        AbstrDistrBlock *abstract_distructable_block = dynamic_cast<AbstrDistrBlock *>(abstract_object);
        abstract_distructable_block->set_health(abstract_distructable_block->get_health()-1);
    };
    void handle_tick(AbstractScene *abstract_scene, std::unordered_map <int, PhisicalObject*> *object_list){}; // тупо стоит на месте
};

class PhisicalScene: public InterfaseScene{
public:
    PhisicalScene(){
        //края карты - фиктивные объекты, неуничтожимые и невидимые, не имеющие абстрактного отобра
        // всё поле 624 * 624
        // косяк - протяжённость сначала по Y, потом по x
        object_list[1] = new UnDistrPhisicalBlock(1, 16, 624); // ▀ верхняя стенка
        object_list[2] = new UnDistrPhisicalBlock(2, 16, 624); // ▄ нижняя стенка
        object_list[3] = new UnDistrPhisicalBlock(3, 624, 16); // ▌ левая стенка
        object_list[4] = new UnDistrPhisicalBlock(4, 624, 16); // ▐ правая стенка
    };
    std::unordered_map <int, PhisicalObject*> object_list;
    ~PhisicalScene(){
        for(auto i : object_list){
            delete i.second;
        }
    };

    void add_obj(const int id, const std::string& type)
    {
        if(type == "DistrBlock") {
            object_list[id] = new PhisicalBlock(id, 8*3, 8*3);
        } else if(type == "UnDistrBlock") {
            object_list[id] = new UnDistrPhisicalBlock(id, 8*3, 8*3);
        } else if(type == "Bullet") {
            object_list[id] = new PhisicalBullet(id, 4*3, 4*3, 10);
        } else if(type == "Tank") {
            object_list[id] = new PhisicalTank(id, 15*3, 15*3, 3);
        } else if(type == "PleerTank") {
            object_list[id] = new PhisicalTank(id, 13*3, 13*3, 8);
        } else if(type == "Board") {
            //нужен край карты
        } else if(type == "WaterBlock") {

        } else if(type == "HeadquartersBlock") {
            //object_list[id] = new PhisicalBlock(id, 16*3, 16*3);
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
        std::vector <int> to_remove;
        for(auto i: object_list){
            if(abstract_scene->obj_list.find(i.first/*id*/) ==
               abstract_scene->obj_list.end()){
                //если нет объекта в абстрактной сцене
                //значит его надо удалить и из нашей сцены
                delete i.second;
                to_remove.push_back(i.first);
            }
        }
        //Нельзя вызывать извлечение элемента из объекта когда итерируемся по нему.
        //это приводит к сигналу 11 - обращение к неверному участку памяти.
        for (auto i : to_remove){
            object_list.erase(i);
        }
    };

    //получается, что вся эта часть с dinamic cast и virtual
    //только ради того, что бы этот цикл был простой
    void handle_tick_all_objects(AbstractScene *abstract_scene){
        for(auto i : object_list){
            i.second->handle_tick(abstract_scene, &(this->object_list));
        }
    }
};

#endif //SIMPLE_RTS_PHISICALSCENE_H
