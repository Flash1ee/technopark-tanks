#include "objects.h"

Object::Object(sf::String textureFile, sf::IntRect rect, sf::Vector2f pos, float speed, Direction dir = Direction::UP)
    : coords(pos),
      rect(rect),
      dx(0),
      dy(0),
      speed(speed),
      dir(dir) {
    this->image.loadFromFile(textureFile);
    this->image.createMaskFromColor(sf::Color(0, 0, 1));
    this->image.createMaskFromColor(sf::Color(0, 0, 0));

    this->texture.loadFromImage(this->image);

    this->sprite.setTexture(this->texture);

    this->sprite.setOrigin(rect.width / 2, rect.height / 2);
    this->sprite.setTextureRect(rect);
    setPos();
    // this->sprite.setPosition(coords.x, coords.y);
    // this->sprite.setScale(5,5);
}

void Tank::move(float time) {
    switch (dir) {
        case Direction::RIGHT:
            dx = speed;
            dy = 0;
            break;  //по иксу задаем положительную скорость, по игреку зануляем.
                    //получаем, что персонаж идет только вправо
        case Direction::LEFT:
            dx = -speed;
            dy = 0;
            break;  //по иксу задаем отрицательную скорость, по игреку зануляем.
                    //получается, что персонаж идет только влево
        case Direction::DOWN:
            dx = 0;
            dy = speed;
            break;  //по иксу задаем нулевое значение, по игреку положительное.
                    //получается, что персонаж идет только вниз
        case Direction::UP:
            dx = 0;
            dy = -speed;
            break;  //по иксу задаем нулевое значение, по игреку отрицательное.
                    //получается, что персонаж идет только вверх
    }

// <<<<<<< HEAD
//     coords.x += dx * time;
//     coords.y += dy * time;
//     sprite.setPosition(coords.x, coords.y);  //выводим спрайт в позицию x y , посередине. бесконечно выводим

//     //sprite.setPosition(coords.x + (rect.width / 2), coords.y + (rect.height / 2));  //выводим спрайт в позицию x y , посередине. бесконечно выводим
// =======
    auto x_old = coords.x;
    auto y_old = coords.y;
    coords.x += dx * time;
    coords.y += dy * time;
    checkCollisionsMap(x_old, y_old, dx, dy);


    // checkCollisionsMap(0, dy);
    setPos();
    
    // sprite.setPosition(coords.x + rect.width / 2, coords.y + rect.height / 2);  //выводим спрайт в позицию x y , посередине. бесконечно выводим
// >>>>>>> Dev
                //в этой функции, иначе бы наш спрайт стоял на месте.
}

void Bullet::move(float time) {
    switch (dir) {
        case Direction::RIGHT:
            this->sprite.setRotation(90);
            dx = speed;
            dy = 0;
            break;
        case Direction::LEFT:
            this->sprite.setRotation(-90);
            dx = -speed;
            dy = 0;
            break;
        case Direction::DOWN:
            this->sprite.setRotation(180);
            dx = 0;
            dy = speed;
            break;
        case Direction::UP:
            this->sprite.setRotation(0);
            dx = 0;
            dy = -speed;
            break;
    }

    coords.x += dx * time;
    coords.y += dy * time;
    setPos();
    // sprite.setPosition(coords.x + 7, coords.y + 7);

}

sf::Sprite& Object::getSprite() {
    return this->sprite;
}

Direction Tank::getDir() const {
    return this->dir;
}

int Tank::makeAction(float time) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        this->shot = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        this->sprite.setRotation(0);
        this->dir = Direction::UP;
        this->move(time);
        return 0;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        this->sprite.setRotation(-90);
        this->dir = Direction::LEFT;
        this->move(time);
        return 0;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        this->sprite.setRotation(90);
        this->dir = Direction::RIGHT;
        this->move(time);
        return 0;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        this->sprite.setRotation(180);
        this->dir = Direction::DOWN;
        this->move(time);
        return 0;
    }
    return -1;
}

sf::Vector2f Object::getPos() const {
    return this->coords;
}

void Object::setPos(const sf::Vector2f& new_pos)
{
    this->coords = new_pos;
    this->sprite.setPosition(coords.x + rect.width / 2, coords.y + rect.height / 2);
}
void Object::setPos() {
    setPos(this->coords);
}
void Tank::setShot(bool shot) {
    this->shot = shot;
}

bool Tank::getShot() const {
    return this->shot;
}
void Tank::checkCollisionsMap(float x_old, float y_old, float x, float y) {
    for (auto &i : m_objects) {
        if (getRect().intersects(static_cast<sf::IntRect>(i.rect)))
        {

            if (dy > 0) {
                // this->y =i.rect.top - this->rect.height;
                coords.y = y_old;
                this->dy = 0;
            }
            if (dy < 0) {
                // this->y = i.rect.top + this->rect.height;
                coords.y = y_old;
                this->dy = 0;
            }
            if (dx > 0) {
                // this->x = i.rect.left - this->rect.width;
                coords.x = x_old;
                this->dx = 0;
            }
            if (dx < 0) {
                // this->x = i.rect.left + this->rect.width;
                coords.x = x_old;
                this->dx = 0;
            }
        }
    }
}
sf::IntRect Object::getRect() {
    return sf::IntRect(coords.x, coords.y, rect.width, rect.height);
}

void Bots::checkCollisionsMap(float x_old, float y_old, float dx, float dy) {
    srand(time(0));
    for (auto &i : m_objects) {
        if (getRect().intersects(static_cast<sf::IntRect>(i.rect)))
        {
            if (dy > 0) {
                // this->y =i.rect.top - this->rect.height;
                coords.y = y_old;
                this->dy = 0;
                dir = static_cast<Direction>(rand() % (3 - 0 + 1) + 0);
            }
            if (dy < 0) {
                // this->y = i.rect.top + this->rect.height;
                coords.y = y_old;
                this->dy = 0;
                dir = static_cast<Direction>(rand() % (3 - 0 + 1) + 0);
            }
            if (dx > 0) {
                // this->x = i.rect.left - this->rect.width;
                coords.x = x_old;
                this->dx = 0;
                dir = static_cast<Direction>(rand() % (3 - 0 + 1) + 0);
            }
            if (dx < 0) {
                // this->x = i.rect.left + this->rect.width;
                coords.x = x_old;
                this->dx = 0;
                dir = static_cast<Direction>(rand() % (3 - 0 + 1) + 0);
            }
        }
    }
}

void Bots::move(float time) {
    switch (dir) {
        case Direction::RIGHT:
            this->sprite.setRotation(90);
            dx = speed;
            dy = 0;
            break;
        case Direction::LEFT:
            this->sprite.setRotation(-90);
            dx = -speed;
            dy = 0;
            break;
        case Direction::DOWN:
            this->sprite.setRotation(180);
            dx = 0;
            dy = speed;
            break;
        case Direction::UP:
            this->sprite.setRotation(0);
            dx = 0;
            dy = -speed;
            break;
    }

    auto x_old = coords.x;
    auto y_old = coords.y;
    coords.x += dx * time;
    coords.y += dy * time;
    this->checkCollisionsMap(x_old, y_old, dx, dy);
    setPos();

    // sprite.setPosition(coords.x + rect.width / 2, coords.y + rect.height / 2);
}

