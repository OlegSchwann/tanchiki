//
// Created by oleg on 03.12.17.
//

#ifndef SIMPLE_RTS_PLEERCONTROLLER_H
#define SIMPLE_RTS_PLEERCONTROLLER_H
#include "InterfaseScene.h" //тут просто макросы направления лежат.
#include "AbstractScene.h"
#include "PhisicalScene.h"

//суть:
// делаем полностью offline клиент.
// имеет доступ к абстрактной сцене для того, что бы подменять направления
// и вызывать создание пули
// время жизни должно совпадать с main()
// при уничтожении танка должен создаться новый танк
// опять таки через абстрактную сцену.
// не нужен synhronize, не нужно вызывать в каждом тике
// Это не сцена

// класс игрока
class PlayerController {
public:
    PlayerController():tank_id(-1), // заведомо некорректное значение
                      pleer_halse(6),
                      direct(UP),
                      since_last_click(30),
                      start_x(0),
                      start_y(0){ // пусть стоит

    };
    void set_startXY(int x, int y){
        start_x = x;
        start_y = y;
    }

    ~PlayerController(){};
    void set_up(){
        direct = UP;
        since_last_click = 0;
    };
    void set_down(){
        direct = DOWN;
        since_last_click = 0;
    };
    void set_left(){
        direct = LEFT;
        since_last_click = 0;
    };
    void set_right(){
        direct = RIGHT;
        since_last_click = 0;
    };

    void choot(AbstractScene *abstract_scene){
        abstract_scene->create_abctract_bullet(tank_id);
    };

    // функция, следящая за абстрактным танком
    void manage_tank(AbstractScene *abstract_scene, PhisicalScene *phisical_scene){
        // функция должна работать только в том случае, если
        // и в абстрактной сцене и в физической сцене существуют экземпляры объектов с нашим id
        if (abstract_scene->obj_list.find(tank_id) != abstract_scene->obj_list.end()){
            if (phisical_scene->object_list.find(tank_id) != phisical_scene->object_list.end()){
                AbstrTank *abstr_tank = dynamic_cast<AbstrTank *>(abstract_scene->obj_list[tank_id]);
                // если в течении последних 10 кадров небыло нажатий, то танк останавливается.
                if (since_last_click < 30) {
                    // теперь устанавливаем направление, такое, как нашей переменной.
                    abstr_tank->set_dir(direct);
                    //запускаем танк
                    phisical_scene->object_list[tank_id]->set_speed(3);
                } else {
                    //останавливаем танк
                    phisical_scene->object_list[tank_id]->set_speed(0);
                }
            }
        } else { //танк был убит!
            pleer_halse--;
            //if(){} TODO: завершение игры тут
            //создаём танк
            std::cout << "Танк игрока был возрождён, осталось жизней: " << pleer_halse << "\n";
            this->tank_id = abstract_scene->add_obj(start_x, start_y, "PleerTank");
            since_last_click = 30; //пусть стоит на старте
        }
        since_last_click ++;
    }

private:
    // знает id танка, которым заведует
    int tank_id;
    // здоровье игрока - количество попыток
    int pleer_halse;
    int direct;
    //счётчик "когда последнее обновление", если > 10 то скорость устанавливается в 0.
    // мне кажется, это не заморочки физической части
    int since_last_click;
    int start_x, start_y;
};

#endif //SIMPLE_RTS_PLEERCONTROLLER_H
