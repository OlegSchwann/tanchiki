//
// Created by oleg on 01.12.17.
// Сцена обработки поведения танков
// задача на эту ночь - замкнуть игру на себя
//create_player();
//string& get_nickname();
//void add_kill();
//erase_player();
//add_player(cosnt string& nickname);
//erase_player(const string& nickname);
//int resolve(const string& nickname);
//void add_kill();

#ifndef SIMPLE_RTS_AISCENE_H
#define SIMPLE_RTS_AISCENE_H

#include "InterfaseScene.h"
#include "AbstractScene.h"
#include "PhisicalScene.h"
#include <random>

class AIObject: public InterfaseObject{
public:
    AIObject(int id):InterfaseObject(id){

    };
    // вытаскивает данные из объекта и меняет его поведение(его переменные)
    // вызывается до физического обработчика(и до рисующей сцены соответственно)
    virtual void think_about_step/*it*/(AbstractScene *abstract_scene, PhisicalScene *phisical_scene){

    };
};

// хотим реализовать создание танков в заданных символом 't' местах карты
// функция создать танк add_obj() в абстрактной сцене
// экземпляр объекта в абстрактной сцене,
// экземпляр объекта в ai сцене, каждые 10 секунд создающий новый объект для каждого места появления,
// обращаясь к абстрактной сцене
class AISpawner: public AIObject{
public:
    AISpawner(int id):AIObject(id), create_counter(100){
        std::cout << "Конструктор Spawner\n";
    };
    // вытаскивает данные из объекта и меняет его поведение(его переменные)
    // вызывается до физического обработчика(и до рисующей сцены соответственно)
    void think_about_step/*it*/(AbstractScene *abstract_scene, PhisicalScene *phisical_scene){
        if(create_counter > 1000){
            create_counter = 0;
            Point point = abstract_scene->obj_list[id]->get_point();
            abstract_scene->add_obj(point.x, point.y, "Tank");
            std::cout << "создан танк враг\n";
        }
        create_counter++;
    };
private:
    int create_counter;
};

class AITank: public AIObject{
    //AbstractTank содержит id, init_point, dir, health
    //PhisicalTank содержит id, heigth, width, speed
public:
    AITank(int id):AIObject(id){

    };
    ~AITank(){};

    //код Гриши
    void think_about_step(AbstractScene *abstract_scene, PhisicalScene *phisical_scene){
        srand(time(NULL));
        AbstractObject *abstract_object = abstract_scene->obj_list[id];
        AbstrTank *abstract_tank = dynamic_cast<AbstrTank *>(abstract_object);
        PhisicalObject *phisical_object = phisical_scene->object_list[id];
        PhisicalTank *phisical_tank = dynamic_cast<PhisicalTank *>(phisical_object);
        bool was_collided = phisical_tank->did_collided();
        Point point = abstract_object->get_point();
        int target = -1;
        int target_id;
        double min_range = 10000;
        int want_turn = rand() % 8;
        int want_shoot = rand() % 4;
        for (auto i : abstract_scene->obj_list) {
            if (i.first != id) {
                Point p = i.second->get_point();
                if ((p.x <= point.x + 3) && (p.x >= point.x - 3)) {
                    if (abstract_scene->accord_list[i.first] == "PleerTank") {
                        if (p.y < point.y)
                            target = UP;
                        else
                            target = DOWN;
                    }
                    if (abstract_scene->accord_list[i.first] == "UnDistrBlock") {
                        if (((p.y < point.y) && (target == UP)) || ((p.y > point.y) && (target == DOWN)))
                            target = -1;
                    }
                }
                if ((p.y <= point.y + 3) && (p.y >= point.y - 3)) {
                    if (abstract_scene->accord_list[i.first] == "PleerTank") {
                        if (p.x < point.x)
                            target = LEFT;
                        else
                            target = RIGHT;
                    }
                    if (abstract_scene->accord_list[i.first] == "UnDistrBlock") {
                        if (((p.x < point.x) && (target == LEFT)) || ((p.x > point.x) && (target == RIGHT)))
                            target = -1;
                    }
                }
                if (abstract_scene->accord_list[i.first] == "PleerTank") {
                    double range = sqrt(pow(p.x - point.x, 2) + pow(p.y - point.y, 2));
                    if (range < min_range)
                        min_range = range;
                    target_id = i.first;
                }
            }
        }
        int direction = abstract_tank->get_dir();
        if ((target == direction) || (want_shoot == 0)) {
            abstract_scene->create_abctract_bullet(id);
            return;
        }
        else if (target != -1) {
            abstract_tank->set_dir(target);
            return;
        }
        Point p = abstract_scene->obj_list[target_id]->get_point();
        if (was_collided)
        {
            if ((direction != RIGHT) && (direction != LEFT))
            {
                if (p.x > point.x) {
                    abstract_tank->set_dir(RIGHT);
                    return;
                }
                if(p.x < point.x) {
                    abstract_tank->set_dir(LEFT);
                    return;
                }
                abstract_tank->set_dir(rand() % 2 + 2);
                return;
            }

            if ((direction != DOWN) && (direction != UP)) {
                if(p.y > point.y) {
                    abstract_tank->set_dir(DOWN);
                    return;
                }
                if(p.y < point.y) {
                    abstract_tank->set_dir(UP);
                    return;
                }
                abstract_tank->set_dir(rand() % 2);
                return;
            }
        }
        if(want_turn == 0) {
            if (p.x > point.x) {
                abstract_tank->set_dir(RIGHT);
                return;
            }
            if (p.x < point.x) {
                abstract_tank->set_dir(LEFT);
                return;
            }
            if (p.y > point.y) {
                abstract_tank->set_dir(DOWN);
                return;
            }
            if (p.y < point.y) {
                abstract_tank->set_dir(UP);
                return;
            }
        }
    }
};

class AIScene: public InterfaseScene{
public:
    AIScene(){};
    std::unordered_map <int, AIObject*> object_list;
    ~AIScene(){
        for(auto i : object_list){
            delete i.second;
        }
    };

    void add_object(const int id, const std::string &type)
    {
        if(type == "Tank") {
            object_list[id] = new AITank(id);
        }else if(type == "Spawner") {
            object_list[id] = new AISpawner(id);
        }
    }


    void synchronize(AbstractScene *abstract_scene){
        //забирает измемения из абстрактной сцены и создаёт объекты
        for(auto i: abstract_scene->obj_list){
            if(object_list.find(i.first/*id*/) == object_list.end()){
                //если нет объекта в рисующей схеме
                //значит его надо создать
                add_object(i.first/*id*/, abstract_scene->accord_list[i.first/*id*/]);
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

    void handle_tick_all_AI(AbstractScene *abstract_scene,
                            PhisicalScene *phisical_scene){
        for(auto i : object_list){
            i.second->think_about_step(abstract_scene, phisical_scene);
        }
    }
};

#endif //SIMPLE_RTS_AISCENE_H
