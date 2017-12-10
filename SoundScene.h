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
    virtual ~SoundObject(){
        while_disappearing();
    };
protected:
    virtual void while_creating(){
        if(on_creating != nullptr){
            on_creating->play();
        }
    };
    virtual void while_disappearing(){
        if(on_disappearing != nullptr) {
            on_disappearing->play();
        }
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
    sf::SoundBuffer background;
    sf::Sound background_sound;
    sf::SoundBuffer bonus;
    sf::Sound bonus_sound;
    sf::SoundBuffer brick;
    sf::Sound brick_sound;
    sf::SoundBuffer explosion;
    sf::Sound explosion_sound;
    sf::SoundBuffer fire;
    sf::Sound fire_sound;
    sf::SoundBuffer gameover;
    sf::Sound gameover_sound;
    sf::SoundBuffer gamestart;
    sf::Sound gamestart_sound;
    sf::SoundBuffer score;
    sf::Sound score_sound;
    sf::SoundBuffer steel;
    sf::Sound steel_sound;

public:
    SoundScene(){
        //загрузка звуков в игру
        background.loadFromFile("sounds/background.ogg");
        background_sound.setBuffer(background);

        bonus.loadFromFile("sounds/bonus.ogg");
        bonus_sound.setBuffer(bonus);

        brick.loadFromFile("sounds/brick.ogg");
        brick_sound.setBuffer(brick);

        explosion.loadFromFile("sounds/explosion.ogg");
        explosion_sound.setBuffer(explosion);

        fire.loadFromFile("sounds/fire.ogg");
        fire_sound.setBuffer(fire);

        gameover.loadFromFile("sounds/gameover.ogg");
        gameover_sound.setBuffer(gameover);

        gamestart.loadFromFile("sounds/gamestart.ogg");
        gamestart_sound.setBuffer(gamestart);

        score.loadFromFile("sounds/score.ogg");
        score_sound.setBuffer(score);

        steel.loadFromFile("sounds/steel.ogg");
        steel_sound.setBuffer(steel);

        gamestart_sound.play();//при запуске игры.
    };

    void add_obj(const int id, const std::string& type){
        // собираем объекты в зависимости от их типа
        if (type == "DistrBlock") {

        } else if(type == "Bullet"){
            object_list[id] = new SoundObject(id, &fire_sound, &brick_sound);
        } else if (type == "PleerTank"){
            object_list[id] = new SoundObject(id, &score_sound, nullptr);
        } else if (type == "Tank") {

        } else if (type == "Explosion") {
            object_list[id] = new SoundObject(id, &explosion_sound, nullptr);

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
                delete i.second;
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
