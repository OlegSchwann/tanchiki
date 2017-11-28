//
// Created by oleg on 28.11.17.
//

#ifndef SIMPLE_RTS_INTERFASESCENE_H
#define SIMPLE_RTS_INTERFASESCENE_H


class InterfaseObject{
public:
    InterfaseObject(const int id):id(id){};
    virtual ~InterfaseObject(){};
    const int id;
};

//интерфейсная сцена
class InterfaseScene{
public:
    virtual ~InterfaseScene(){};
};

#endif //SIMPLE_RTS_INTERFASESCENE_H
