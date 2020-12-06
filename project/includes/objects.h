#ifndef _OBJECTS_H_
#define _OBJECTS_H_

#include "TmxLevel.h"
#include <SFML/Graphics.hpp>
#include <memory>

enum class Direction {
    UP = 0,
    DOWN,
    RIGHT,
    LEFT,
    COUNT,
    ERROR
};

class Object {

public:
    Object(sf::String textureFile, sf::IntRect rect, sf::Vector2f pos, float speed, Direction dir);

    sf::Vector2f getPos() const;
    void setPos(const sf::Vector2f& new_pos);
    sf::Sprite& getSprite();
    float getX() const;
    float getY() const;
    sf::IntRect getRect();

protected:
    sf::Vector2f coords; //координаты
    float dx, dy; //current delta coordinats
    float speed; //скорость (множитель времени)
    Direction dir;
    sf::IntRect rect; 
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
    std::vector<TmxObject> m_objects;
    // Object();
};

class Bullet : public Object {

public:
    Bullet(sf::String textureFile, sf::IntRect rect, sf::Vector2f pos, float speed, Direction dir)
        : Object(textureFile, rect, pos, speed, dir) {};
    void move(float time);
};

class Tank : public Object { //класс любого танка
protected:
    // sf::String name;
    int hp;
    //Direction dir;
    bool shot;
    // size_t armor;
    // size_t power;
    Tank(TmxLevel &mapObj, sf::String textureFile, sf::IntRect rect, sf::Vector2f pos, float speed, int hp, Direction dir)
        : Object(textureFile, rect, pos, speed, dir), hp(hp), shot(false) {
            m_objects = mapObj.GetAllObjects("solid");
        }

public:
    void checkCollisionsMap(float x_old, float y_old, float x, float y);
    void move(float time);
    int makeAction(float time);
    Direction getDir() const;
    bool getShot() const;
    void setShot(bool shot);
};

class Player : public Tank { //класс игрока
public:
    Player(TmxLevel &mapObj, sf::String textureFile, sf::IntRect rect, sf::Vector2f pos, float speed, int hp, Direction dir)
        : Tank(mapObj, textureFile, rect, pos, speed, hp, dir) {}

};

#endif  // _OBJECTS_H_
