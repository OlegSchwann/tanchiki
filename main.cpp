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

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include "InterfaseScene.h"
#include "DrawScene.h"
#include "AbstractScene.h"
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

    DrawScene draw_scene;
    //забирает измемения из абстрактной сцены

    //задача 1 нарисовать стенку
    abstract_scene.add_obj(30, 25, "Tank");
    draw_scene.synchronize(&abstract_scene);

    // все подготовительные действия
    // минимальная длительность игрового цикла
    sf::Time cycle_time = sf::seconds(1);//0.02f);
    sf::RenderWindow window(sf::VideoMode(624, 624), "simple rts");



    sf::Clock clock;
    sf::Event event;
    while (window.isOpen()){
        while (window.pollEvent(event)){
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { };
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){ };
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){ };
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){ };
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        //выводим все спрайты на экран
        //пришлось дать знать рисующей сцене о абстрактной сцене. Это нужно, что бы получать х, y отображения этого объекта.
        draw_scene.draw(window, &abstract_scene);
        window.display();
        sf::Time elapsed_time = clock.getElapsedTime() % cycle_time;
        std::cout << "отрисовано за " << elapsed_time.asMilliseconds() << " миллисекунд\n";
        sf::sleep(cycle_time - elapsed_time);
    }

    return 0;
}
