// Сцена, запускающая звук
//
//
//

#ifndef SIMPLE_RTS_SOUNDSCENE_H
#define SIMPLE_RTS_SOUNDSCENE_H
#include "InterfaseScene.h"
#include "AbstractScene.h"
#include <SFML/Audio.hpp>
#include <unordered_map>

class SoundObject: public InterfaseObject{
public:
    SoundObject(int id, sf::Sound *on_creating, sf::Sound *on_disappearing):
        InterfaseObject(id), on_creating(on_creating), on_disappearing(on_disappearing){
        while_creating();
    }
protected:
    virtual void while_creating(){
        on_creating->play();
    };
    virtual void while_disappearing(){
        on_disappearing->play();
    };
    virtual ~SoundObject(){
        while_disappearing();
    };
    sf::Sound *on_creating;
    sf::Sound *on_disappearing;
};

// сцена звука.
// В архитектуру не были заложены события.
// есть события в виде конструкторов/деструкторов объектов
// пусть они и запускают воспроизведение дорожек.
class SoundScene: public InterfaseScene{
private:
    std::unordered_map <int, SoundObject*> object_list;
    //загрузка звуков в игру
    sf::SoundBuffer background;
    background.loadFromFile("sounds/background.ogg");
    sf::SoundBuffer bonus;
    bonus.loadFromFile("sounds/bonus.ogg");
    sf::SoundBuffer brick;
    brick.loadFromFile("sounds/brick.ogg");
    sf::SoundBuffer explosion;
    explosion.loadFromFile("sounds/explosion.ogg");
    sf::SoundBuffer fire;
    fire.loadFromFile("sounds/fire.ogg");
    sf::SoundBuffer gameover;
    gameover.loadFromFile("sounds/gameover.ogg");
    sf::SoundBuffer gamestart;
    gamestart.loadFromFile("sounds/gamestart.ogg");
    sf::SoundBuffer score;
    score.loadFromFile("sounds/score.ogg");
    sf::SoundBuffer steel;
    steel.loadFromFile("sounds/steel.ogg");

public:
        DrawScene(){

           // "gamestart.ogg"
        };

        void add_obj(const int id, const std::string& type){
            // собираем объекты в зависимости от их типа
            if(type == "Bullet"){
                fire;
            } else if (type == "PleerTank"){

            } else if (type == "HeadquartersBlock") {
                              gameover;
            } else if (type == "Tank") {

            } else if (type == "PleerTank") {

            } else if (type == "Explosion") {
                              explosion;
            } else if (type == "DistrBlock") {
                               brick;
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

        ~SoundScene(){
            //всё статическое, и удалится само.
        };
    };


#endif //SIMPLE_RTS_SOUNDSCENE_H
