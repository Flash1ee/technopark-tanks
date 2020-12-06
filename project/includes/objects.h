#ifndef _OBJECTS_H_
#define _OBJECTS_H_

#include <SFML/Graphics.hpp>

enum class moveAction {
    UP = 0,
    DOWN,
    RIGHT,
    LEFT,
    COUNT,
    ERROR
};

class Object {
protected:
    sf::Vector2f coords; //координаты
    float dx, dy; //current delta coordinats
    float left, top;
    float width, height;
    float speed; //скорость (множитель времени)
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
    Object();
public:
    sf::Vector2f getPos() const;
    sf::Sprite& getSprite();
    Object(sf::String textureFile, sf::Vector2f pos, float left, float top,
           float width, float height, float speed);
};

class Bullet : public Object {
private:
    int dir; //направление движения
public:
    Bullet(sf::String textureFile, sf::Vector2f pos, float left, float top,
           float width, float height,float speed, int dir)
        : Object(textureFile, pos, left, top, width, height, speed), dir(dir) {};
    void move(float time);
};

class Tank : virtual public Object { //класс любого танка
protected:
    // sf::String name;
    int hp;
    int dir;
    bool shot;
    // size_t armor;
    // size_t power;
    Tank(sf::String textureFile, sf::Vector2f pos, float left, float top, float width,
         float height, int hp, float speed)
        : Object(textureFile, pos, left, top, width, height, speed), hp(hp), shot(false) {};
public:
    void move(float time);
    int makeAction(float time);
    int getDir() const;
    bool getShot() const;
    void setShot(bool shot);
};

class Player : public Tank { //класс игрока
public:
    Player(sf::String textureFile, sf::Vector2f pos, float left, float top, float width,
           float height, int hp, float speed)
        : Tank(textureFile, pos, left, top, width, height, hp, speed),
          Object(textureFile, pos, left, top, width, height, speed) {};
};

#endif  // _OBJECTS_H_
