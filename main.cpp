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

int main()
{
    // все подготовительные действия
    // минимальная длительность игрового цикла
    sf::Time cycle_time = sf::seconds(1);//0.02f);
    sf::RenderWindow window(sf::VideoMode(480, 416), "simple rts");

    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { };
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){ };
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){ };
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){ };
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.display();
        sf::Time elapsed_time = clock.getElapsedTime() % cycle_time;
        std::cout << elapsed_time.asMicroseconds() << std::endl;
        sf::sleep(cycle_time - elapsed_time);
    }

    return 0;
}
