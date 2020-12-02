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
    float speed; //скорость (множитель времени)
    sf::IntRect::Rect rect; 
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
    Object();
public:
    float getX() const;
    float getY() const;
    sf::Sprite& getSprite();
    Object(sf::String textureFile, sf::IntRect::Rect rect, float x, float y, float speed);
};

class Bullet : public Object {
private:
    int dir; //направление движения
public:
    Bullet(sf::String textureFile, sf::IntRect::Rect rect, float x, float y, float speed, int dir)
        : Object(textureFile, rect, x, y, speed), dir(dir) {};
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
    Tank(sf::String textureFile, sf::IntRect::Rect rect, float x, float y, float speed, int hp)
        : Object(textureFile, rect, x, y, speed), hp(hp), shot(false) {};
public:
    void move(float time);
    int makeAction(float time);
    int getDir() const;
    bool getShot() const;
    void setShot(bool shot);
};

class Player : public Tank { //класс игрока
public:
    Player(sf::String textureFile, sf::IntRect::Rect rect, float x, float y, float speed, int hp)
        : Tank(textureFile, rect, x, y, speed, hp),
          Object(textureFile, rect, x, y, speed) {};
};
#endif  // _OBJECTS_H_
