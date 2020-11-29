#pragma once
#include <SFML/Graphics.hpp>

class Object {
 protected:
    int m_x, m_y;
    float m_left, m_top;
    float m_height, m_width;
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
 public:
    Object(std::string, int, int, float, float, float, float);
    sf::Sprite getSprite() const;
};

class PlayerTank: public Object {
 public:
    PlayerTank(std::string, int, int, float, float, float, float);
    void actions();
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
    int dir; //направление движения
    float speed; //скорость (множитель времени)
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
    Object();
    Object(sf::String textureFile, float x, float y, float left, float top, float width, float height);
};

// class Bullet : public Object {
// };

class Tank : virtual public Object { //класс любого танка
protected:
    // sf::String name;
    int hp;
    // size_t armor;
    // size_t power;
    Tank(sf::String textureFile, float x, float y, float left, float top, float width, float height, int hp)
        : Object(textureFile, x, y, left, top, width, height), hp(hp) {};
public:
    sf::Sprite& getSprite();
    void setDir(moveAction dir);
    void setTexture(int px, int py, int width, int height);
    void move(float time);
    int makeAction(float time);
    // virtual void shoot(double x, double y) = 0;
};

class Player : public Tank { //класс игрока
public:
    Player(sf::String textureFile, float x, float y, float left, float top, float width, float height, int hp)
        : Tank(textureFile, x, y, left, top, width, height, hp),
        Object(textureFile, x, y, left, top, width, height) {};
};
