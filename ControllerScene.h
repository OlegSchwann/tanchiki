//
// Created by oleg on 01.12.17.
// Сцена обработки поведения врагов


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


class AITank: public AIObject{
    //AbstractTank содержит id, init_point, dir, health
    //PhisicalTank содержит id, heigth, width, speed
    //TODO: ввести bool recently_collided
public:
    AITank(int id):AIObject(id){

    };
    ~AITank(){};
    void think_about_step(AbstractScene *abstract_scene, PhisicalScene *phisical_scene){
        // Пусть будет как в оригинальной игре -
        // танк едет вперёд
        // с небольшой вероятностью поворачивает в любую сторону
        // с небольшой вероятностью стреляет вперёд
        // если столкнулся - поворачивает в любую сторону
        AbstractObject * abstract_object = abstract_scene->obj_list[id];
        AbstrTank *abstract_tank = dynamic_cast<AbstrTank *>(abstract_object);
        PhisicalObject * phisical_object = phisical_scene->object_list[id];
        PhisicalTank  *phisical_tank = dynamic_cast<PhisicalTank *>(phisical_object);
        bool was_collided = phisical_tank->did_collided();
        int random_num = std::rand();
        bool want_change_direction = random_num < 2147483647*0.05; // RAND_MAX
        // Почему у стандартной библиотеки настолько неудобное использование?
        if (was_collided || want_change_direction){
            int direction = abstract_tank->get_dir();
            if(random_num % 2 == 0){
                // +1 против часовой стрелки
                direction = (direction + 1)%4;
            } else{
                // -1 по часовой стрелке
                direction = (4 + direction - 1)%4;
            }
            abstract_tank->set_dir(direction);
        }
        bool want_shoot = random_num > 2147483647*0.95; // RAND_MAX
        if (want_shoot){
            abstract_scene->create_abctract_bullet(id);
        }
    };
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
