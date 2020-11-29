#include "objects.h"

Object::Object(sf::String textureFile, float x, float y, float left, float top, float width, float height) 
    : x(x), y(y), left(left), top(top), width(width), height(height), dx(0), dy(0), speed(0.5) {
    this->image.loadFromFile(textureFile);
    // this->image.createMaskFromColor(sf::Color(255, 255, 255));

    this->texture.loadFromImage(this->image);

    this->sprite.setTexture(this->texture);
    this->sprite.setTextureRect(sf::IntRect(this->m_left, this->m_top, this->m_width, this->m_height));
    this->sprite.setPosition(this->m_x * height,this->m_y * width);
}

sf::Sprite Object::getSprite() const {
    return this->sprite;
    switch (dir) {
            case RIGHT: dx = speed; dy = 0; break;//по иксу задаем положительную скорость, по игреку зануляем. получаем, что персонаж идет только вправо
            case LEFT: dx = -speed; dy = 0; break;//по иксу задаем отрицательную скорость, по игреку зануляем. получается, что персонаж идет только влево
            case DOWN: dx = 0; dy = speed; break;//по иксу задаем нулевое значение, по игреку положительное. получается, что персонаж идет только вниз
            case UP: dx = 0; dy = -speed; break;//по иксу задаем нулевое значение, по игреку отрицательное. получается, что персонаж идет только вверх
    }

    x += dx*time;
    y += dy*time;
    // speed = 0;
    sprite.setPosition(x,y); //выводим спрайт в позицию x y , посередине. бесконечно выводим в этой функции, иначе бы наш спрайт стоял на месте.
}

sf::Sprite& Tank::getSprite() {
    return this->sprite;
}

void Tank::setDir(moveAction dir) {
    this->dir = dir;
}


void Tank::setTexture(int px, int py, int width, int height) {
    this->sprite.setTextureRect(sf::IntRect(px, py, width, height));
}

int Tank::makeAction(float time) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        this->sprite.setRotation(0);
        this->dir = moveAction::UP;
        this->move(time);
        return 0;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        this->sprite.setRotation(-90);
        this->dir = moveAction::LEFT;
        this->move(time);
        return 0;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        this->sprite.setRotation(90);
        this->dir = moveAction::RIGHT;
        this->move(time);
        return 0;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        this->sprite.setRotation(180);
        this->dir = moveAction::DOWN;
        this->move(time);
        return 0;
    }
    return moveAction::ERROR;
}
