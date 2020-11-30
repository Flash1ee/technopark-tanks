#ifndef _OBJECTS_H_
#define _OBJECTS_H_

#include <SFML/Graphics.hpp>

enum moveAction {
    UP,
    DOWN,
    RIGHT,
    LEFT,
    COUNT,
    ERROR
};

class Object {
protected:
    float x, y; //координаты
    float dx, dy; //current delta coordinats
    float left, top;
    float width, height;
    float speed; //скорость (множитель времени)
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
    Object();
public:
    float getX() const;
    float getY() const;
    void setX(float x);
    void setY(float y);
    sf::Sprite& getSprite();
    Object(sf::String textureFile, float x, float y, float left, float top,
           float width, float height, float speed);
};

class Bullet : public Object {
private:
    int dir; //направление движения
public:
    Bullet(sf::String textureFile, float x, float y, float left, float top,
           float width, float height,float speed, int dir)
        : Object(textureFile, x, y, left, top, width, height, speed), dir(dir) {};
    void move(float time);
    void setDir(int dir);
};

class Tank : virtual public Object { //класс любого танка
protected:
    // sf::String name;
    int hp;
    int dir;
    bool shot;
    // size_t armor;
    // size_t power;
    Tank(sf::String textureFile, float x, float y, float left, float top, float width,
         float height, int hp, float speed)
        : Object(textureFile, x, y, left, top, width, height, speed), hp(hp), shot(false) {};
public:
    void move(float time);
    int makeAction(float time);
    int getDir() const;
    bool getShot() const;
    void setShot(bool shot);
};

class Player : public Tank { //класс игрока
public:
    Player(sf::String textureFile, float x, float y, float left, float top, float width,
           float height, int hp, float speed)
        : Tank(textureFile, x, y, left, top, width, height, hp, speed),
          Object(textureFile, x, y, left, top, width, height, speed) {};
};
#endif  // _OBJECTS_H_
