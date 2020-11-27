#pragma once
#include <SFML/Graphics.hpp>

class Object {
public:
    double x, y; //координаты
    int dir; //направление движения
    //double speed; //скорость (множитель времени)
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
    virtual void move(double x, double y) = 0;
};

// class Bullet : public Object {
// };

class Tank : protected Object { //класс любого танка
protected:
    sf::String name;
    int hp;
    size_t armor;
    size_t power;
public:
    sf::Sprite& getSprite();
    void setTexture(int px, int py, int width, int height);
    Tank(sf::String textureFile, double x, double y);
    void move(double x, double y) override;
};

class Player : public Tank { //класс игрока
public:
    Player(sf::String textureFile, double x, double y)
        : Tank(textureFile, x, y) {};
    Player();
};