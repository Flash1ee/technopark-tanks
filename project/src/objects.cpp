#include "objects.h"

Object::Object(sf::String textureFile, float x, float y, float left, float top,
               float width, float height, float speed)
    : x(x),
      y(y),
      left(left),
      top(top),
      width(width),
      height(height),
      dx(0),
      dy(0),
      speed(speed) {
    this->image.loadFromFile(textureFile);
    this->image.createMaskFromColor(sf::Color(0, 0, 0));

    this->texture.loadFromImage(this->image);

    this->sprite.setTexture(this->texture);
    this->sprite.setOrigin(this->width / 2, this->height / 2);
    this->sprite.setTextureRect(
        sf::IntRect(this->left, this->top, this->width, this->height));
    this->sprite.setPosition(this->x, this->y);
    // this->sprite.setScale(5,5);
}

void Tank::move(float time) {
    switch (dir) {
        case RIGHT:
            dx = speed;
            dy = 0;
            break;  //по иксу задаем положительную скорость, по игреку зануляем.
                    //получаем, что персонаж идет только вправо
        case LEFT:
            dx = -speed;
            dy = 0;
            break;  //по иксу задаем отрицательную скорость, по игреку зануляем.
                    //получается, что персонаж идет только влево
        case DOWN:
            dx = 0;
            dy = speed;
            break;  //по иксу задаем нулевое значение, по игреку положительное.
                    //получается, что персонаж идет только вниз
        case UP:
            dx = 0;
            dy = -speed;
            break;  //по иксу задаем нулевое значение, по игреку отрицательное.
                    //получается, что персонаж идет только вверх
    }

    x += dx * time;
    y += dy * time;
    sprite.setPosition(x, y);  //выводим спрайт в позицию x y , посередине. бесконечно выводим
                //в этой функции, иначе бы наш спрайт стоял на месте.
}

void Bullet::move(float time) {
    switch (dir) {
        case RIGHT:
            dx = speed;
            dy = 0;
            break;
        case LEFT:
            dx = -speed;
            dy = 0;
            break;
        case DOWN:
            dx = 0;
            dy = speed;
            break;
        case UP:
            dx = 0;
            dy = -speed;
            break;
    }

    x += dx * time;
    y += dy * time;
    sprite.setPosition(x, y);
}

sf::Sprite& Object::getSprite() {
    return this->sprite;
}

int Tank::getDir() const {
    return this->dir;
}

int Tank::makeAction(float time) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        this->shot = true;
    }
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

float Object::getX() const {
    return this->x;
}

float Object::getY() const {
    return this->y;
}

void Tank::setShot(bool shot) {
    this->shot = shot;
}

bool Tank::getShot() const {
    return this->shot;
}