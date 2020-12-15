#include "objects.h"

#include <random>
#include <iostream>

Object::Object(sf::String textureFile, sf::IntRect rect, sf::Vector2f pos,
               float speed, Direction dir = Direction::UP)
    : coords(pos), rect(rect), dx(0), dy(0), speed(speed), dir(dir) {
    this->image.loadFromFile(textureFile);
    this->image.createMaskFromColor(sf::Color(0, 0, 1));
    this->image.createMaskFromColor(sf::Color(0, 0, 0));

    this->texture.loadFromImage(this->image);
    this->sprite.setTexture(this->texture);

    this->sprite.setOrigin(static_cast<double>(rect.width) / 2, 
                           static_cast<double>(rect.height) / 2);
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
    //     sprite.setPosition(coords.x, coords.y);  //выводим спрайт в позицию x
    //     y , посередине. бесконечно выводим

    //     //sprite.setPosition(coords.x + (rect.width / 2), coords.y +
    //     (rect.height / 2));  //выводим спрайт в позицию x y , посередине.
    //     бесконечно выводим
    // =======
    auto x_old = coords.x;
    auto y_old = coords.y;
    coords.x += dx * time;
    coords.y += dy * time;
    checkCollisionsMap(x_old, y_old, dx, dy);

    // checkCollisionsMap(0, dy);
    setPos();

    // sprite.setPosition(coords.x + rect.width / 2, coords.y + rect.height /
    // 2);  //выводим спрайт в позицию x y , посередине. бесконечно выводим
    // >>>>>>> Dev
    //в этой функции, иначе бы наш спрайт стоял на месте.
}

void Bullet::move(float time, Player& p) {
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

    this->checkCollisionsObject(p);
    if (m_life == 1) {
        coords.x += dx * time;
        coords.y += dy * time;
    }
    setPos();
    // sprite.setPosition(coords.x + 7, coords.y + 7);
}

void Bullet::move(float time, std::vector<Bots*> b) {
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

    this->checkCollisionsObject(b);
    if (m_life == 1) {
        coords.x += dx * time;
        coords.y += dy * time;
    }
    setPos();
    // sprite.setPosition(coords.x + 7, coords.y + 7);
}
// void Bullet::sound() { this->m_sound.play(); }
sf::Sprite &Object::getSprite() { return this->sprite; }

void Object::setDir(Direction dir) {
    this->dir = dir;
}

Direction Tank::getDir() const { return this->dir; }

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

sf::Vector2f Object::getPos() const { return this->coords; }

void Object::setPos(const sf::Vector2f &new_pos) {
    this->coords = new_pos;
    this->sprite.setPosition(coords.x + rect.width / 2,
                             coords.y + rect.height / 2);
}
void Object::setPos() { setPos(this->coords); }
void Tank::setShot(bool shot) { this->shot = shot; }

bool Tank::getShot() const { return this->shot; }
void Tank::checkCollisionsMap(float x_old, float y_old, float x, float y) {
    for (auto &i : m_objects) {
        if (getRect().intersects(static_cast<sf::IntRect>(i.rect))) {
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

void Bullet::checkCollisionsObject(std::vector<Bots*> b) {
    for (auto &i : m_objects) {
            if (getRect().intersects(static_cast<sf::IntRect>(i.rect))) {
                m_life = 0;
            }
        }
    for (auto &it : b) {
        if (getRect().intersects(it->getRect())) {
            it->setHp(it->getHp() - 25);
            m_life = 0;
        }
    }
}

void Bullet::checkCollisionsObject(Player& p) {
    for (auto &i : m_objects) {
        if (getRect().intersects(static_cast<sf::IntRect>(i.rect))) {
            m_life = 0;
        }
    }
    if (getRect().intersects(p.getRect())) {
        p.setHp(p.getHp() - 25);
        m_life = 0;
    }
}

int Tank::getHp() const {
    return this->m_hp;
}

void Tank::setHp(int hp) {
    this->m_hp = hp;
}

int Bullet::getLife() const {
    return this->m_life;
}

void Player::checkCollisionsBots(std::vector<Bots*> b) {
    for (auto &i : m_objects) {
        for (auto &it : b) {
            if (getRect().intersects(static_cast<sf::IntRect>(i.rect)) || getRect().intersects(it->getRect())) {
                if (dy > 0) {
                    // this->y =i.rect.top - this->rect.height;
                    coords.y = it->getPos().y - rect.height;
                    this->dy = 0;
                }
                if (dy < 0) {
                    // this->y = i.rect.top + this->rect.height;
                    coords.y = it->getPos().y + rect.height;
                    this->dy = 0;
                }
                if (dx > 0) {
                    // this->x = i.rect.left - this->rect.width;
                    coords.x = it->getPos().x - rect.width;
                    this->dx = 0;
                }
                if (dx < 0) {
                    // this->x = i.rect.left + this->rect.width;
                    coords.x = it->getPos().x + rect.width;
                    this->dx = 0;
                }
            }
        }
    }
}

sf::IntRect Object::getRect() {
    return sf::IntRect(coords.x, coords.y, rect.width, rect.height);
}

bool Object::comparisonPos(Player &p, std::vector<Bots*> b) {
    for (auto &it : b) {
        for (int i = 0; i < 100; i++) {
            if ((p.coords.x == (it->coords.x + i)) || (p.coords.x == (it->coords.x - i))) {
                return true;
            }
            if ((p.coords.y == (it->coords.y + i)) || (p.coords.y == (it->coords.y - i))) {
                return true;
            } else {
                return false;
            }
        }
    }
}

void Bots::checkCollisionsObjects(float x_old, float y_old, float dx, float dy,
                              Player &p, std::vector<Bots*> b) {
    std::random_device rd;
    std::uniform_int_distribution<int> uid(0, 3);
    for (auto &i : m_objects) {
        for (auto &it : b) {
            if ((getRect().intersects(static_cast<sf::IntRect>(i.rect)) &&
                i.name == "solid") || (getRect().intersects(it->getRect())) && (getRect()) != it->getRect()) {
                if (dy > 0) {
                    // this->y =i.rect.top - this->rect.height;
                    coords.y = y_old;
                    this->dy = 0;
                    dir = static_cast<Direction>(uid(rd));
                }
                if (dy < 0) {
                    // this->y = i.rect.top + this->rect.height;
                    coords.y = y_old;
                    this->dy = 0;
                    dir = static_cast<Direction>(uid(rd));
                }
                if (dx > 0) {
                    // this->x = i.rect.left - this->rect.width;
                    coords.x = x_old;
                    this->dx = 0;
                    dir = static_cast<Direction>(uid(rd));
                }
                if (dx < 0) {
                    // this->x = i.rect.left + this->rect.width;
                    coords.x = x_old;
                    this->dx = 0;
                    dir = static_cast<Direction>(uid(rd));
                }
            } else if (getRect().intersects(static_cast<sf::IntRect>(p.getRect()))) {
                if (dy > 0) {
                    // this->y =i.rect.top - this->rect.height;
                    coords.y = p.getPos().y - rect.height;
                    this->dy = 0;
                    dir = static_cast<Direction>(uid(rd));
                }
                if (dy < 0) {
                    // this->y = i.rect.top + this->rect.height;
                    coords.y = p.getPos().y + rect.height;
                    this->dy = 0;
                    dir = static_cast<Direction>(uid(rd));
                }
                if (dx > 0) {
                    // this->x = i.rect.left - this->rect.width;
                    coords.x = p.getPos().x - rect.width;
                    this->dx = 0;
                    dir = static_cast<Direction>(uid(rd));
                }
                if (dx < 0) {
                    // this->x = i.rect.left + this->rect.width;
                    coords.x = p.getPos().x + rect.width;
                    this->dx = 0;
                    dir = static_cast<Direction>(uid(rd));
                }
            }
        }
    }
}

void Bots::move(float time, Player &p, std::vector<Bots*> b) {
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
    this->checkCollisionsObjects(x_old, y_old, dx, dy, p, b);
    if (true) {
        this->setShot(true);
    }
    setPos();

    // sprite.setPosition(coords.x + rect.width / 2, coords.y + rect.height /
    // 2);
}

// void Sound::play() {
//     this->sound.play();
// }

// Sound::Sound(std::string path) {
//     if (!this->buffer.loadFromFile(path)) {
//         throw std::exception();
//     }
//     this->sound.setBuffer(this->buffer);
// }
Sound::Sound() {
    background.loadFromFile(BACKGROUND_SOUND);
    background_sound.setBuffer(background);

    brick.loadFromFile(BRICK_SOUND);
    brick_sound.setBuffer(brick);

    explosion.loadFromFile(KILL_SOUND);
    explosion_sound.setBuffer(explosion);

    fire.loadFromFile(FIRE_SOUND);
    fire_sound.setBuffer(fire);

    gameover.loadFromFile(GAME_OVER_SOUND);
    gameover_sound.setBuffer(gameover);

    gamestart.loadFromFile(GAME_START_SOUND);
    gamestart_sound.setBuffer(gamestart);

    steel.loadFromFile(SPAWN_SOUND);
    steel_sound.setBuffer(steel);
}
void Sound::play(sound_action action) {
    switch(action) {
        case BACKGROUND:
            if (this->background_sound.getStatus() != sf::Sound::Playing) {
            this->background_sound.play();
            this->background_sound.setVolume(60);
            this->background_sound.setPitch(0.8f);
            }
            break;
        case BRICK:
            if (this->brick_sound.getStatus() != sf::Sound::Playing) {
            this->brick_sound.play();
            this->fire_sound.setVolume(100);
            }
            break;
        case KILL:
            this->explosion_sound.play();
            break;
        case FIRE:
            if (this->fire_sound.getStatus() != sf::Sound::Playing) {
            this->fire_sound.play();
            this->fire_sound.setVolume(60);
            }
            break;
        case GAME_OVER:
            this->gameover_sound.play();
            break;
        case GAME_START:
            this->gamestart_sound.play();
            this->gamestart_sound.setVolume(20);
            break;
        case SPAWN:
            if (this->steel_sound.getStatus() != sf::Sound::Playing) {
                this->steel_sound.play();
            }
            break;
    }
}
bool Sound::MainSoundStopped() {
    if (this->gamestart_sound.getStatus() != sf::Sound::Playing) {
        return true;
    }
    return false;
}
