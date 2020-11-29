#include "objects.h"

Object::Object(std::string filename, int x, int y, float left, float top, float width, float height)
    : m_x(x), m_y(y), m_left(left), m_top(top), m_height(height), m_width(width) {
    this->image.loadFromFile(filename);
    this->image.createMaskFromColor(sf::Color(255, 255, 255));

    this->texture.loadFromImage(this->image);

    this->sprite.setTexture(this->texture);
    this->sprite.setTextureRect(sf::IntRect(this->m_left, this->m_top, this->m_width, this->m_height));
    this->sprite.scale(9, 9);
    this->sprite.setPosition(this->m_x,this->m_x);
}

sf::Sprite Object::getSprite() const {
    return this->sprite;
}

PlayerTank::PlayerTank(std::string filename, int x, int y, float left, float top, float width, float height)
    :Object(filename, x, y, left, top, width, height) {
    this->sprite.setScale(3, 3);
}

void PlayerTank::actions() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        sprite.setTextureRect(sf::IntRect(34, 2, 29, 29));
        sprite.move(-0.1,0);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        sprite.setTextureRect(sf::IntRect(99, 1, 29, 29));
        sprite.move(0,-0.1);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        sprite.setTextureRect(sf::IntRect(2, 2, 29, 29));
        sprite.move(0.1,0);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        sprite.setTextureRect(sf::IntRect(66, 1, 29, 29));
        sprite.move(0,0.1);
    }
}



