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
};