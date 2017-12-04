/* Клиент игры Танчики.
   Это сетевая игра, клиент только
    устанавливает соединнение
    посылает сигналы ввода пользователя вида "нажата кнопка ← ↑ → ↓ ␣"
    структура данных -
        имя пользователя,
        время нажатия,
        код действия.
    собственно, пользователю доступно только перемещение и стрельба
    принимает номер, кординаты и статус игровых объектов
    ! - уточнить структуру данных, принимаемую
    применяет все пришедшие изменения
    отрисовывает карту, врагов, обоих игроков
    повторяет(ждёт следующего тика)
*/

#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "AbstractScene.h"
#include "PhisicalScene.h"
#include "DrawScene.h"
#include "ControllerScene.h"
#include "PleerController.h"

// вся графическая подсистема
// кусочек для теста, пример как пользоваться

#define DOWN 0
#define RIGHT 1
#define UP 2
#define LEFT 3

using namespace std;

int main()
{

    AbstractScene abstract_scene;
    fstream file("1.txt");
    abstract_scene.load_map(file);
    file.close();

    abstract_scene.add_obj(0, 0 ,"Tank");

    DrawScene draw_scene;
    AIScene ai_scene;
    PleerController pleer_controller;
    PhisicalScene phisical_scene;
    draw_scene.synchronize(&abstract_scene);
    phisical_scene.synchronize(&abstract_scene);
    ai_scene.synchronize(&abstract_scene);

    // все подготовительные действия
    // минимальная длительность игрового цикла
    sf::Time cycle_time = sf::seconds(0.02);//0.02f);
    sf::RenderWindow window(sf::VideoMode(624, 624), "simple rts");

    sf::Clock clock;
    sf::Event event;
    while (window.isOpen()){
        while (window.pollEvent(event)){
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
                pleer_controller.set_doun();
            };
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
                pleer_controller.set_right();
            };
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
                pleer_controller.set_up();
            };
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                pleer_controller.set_left();
            };
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
                pleer_controller.choot(&abstract_scene);
                std::cout << "сделан выстрел" << std::endl;
            };
            if (event.type == sf::Event::Closed)
                window.close();
        }
        //пришлось дать знать рисующей сцене о абстрактной сцене. Это нужно, что бы получать х, y отображения этого объекта.
        ai_scene.handle_tick_all_AI(&abstract_scene, &phisical_scene);
        phisical_scene.handle_tick_all_objects(&abstract_scene);
        abstract_scene.clear_dead();
        draw_scene.synchronize(&abstract_scene);
        phisical_scene.synchronize(&abstract_scene);
        pleer_controller.manage_tank(&abstract_scene, &phisical_scene);
        ai_scene.synchronize(&abstract_scene);
        //выводим все спрайты на экран
        window.clear();
        draw_scene.draw(window, &abstract_scene);
        window.display();
        sf::Time elapsed_time = clock.getElapsedTime() % cycle_time;
        std::cout << "отрисовано за " << elapsed_time.asMilliseconds() << " миллисекунд\n";
        sf::sleep(cycle_time - elapsed_time);
    }
    return 0;
}
