#include "objects.h"

#include <random>
#include <iostream>

#define BLOCK_SIZE 16
#define WALL_INIT 100
#define WALL_DAMAGE 25

Object::Object(sf::String textureFile, sf::IntRect rect, sf::Vector2f pos,
               float speed, Direction dir = Direction::UP)
    : coords(pos), rect(rect), dx(0), dy(0), speed(speed), dir(dir){
    this->image.loadFromFile(textureFile);
    this->image.createMaskFromColor(sf::Color(0, 0, 1));
    // this->image.createMaskFromColor(sf::Color(256, 256, 256));
    this->image.createMaskFromColor(sf::Color(255, 255, 255));

    this->texture.loadFromImage(this->image);
    this->sprite.setTexture(this->texture);

    this->sprite.setOrigin(static_cast<double>(rect.width) / 2, 
                           static_cast<double>(rect.height) / 2);
    this->sprite.setTextureRect(rect);
    setPos();
}

void Tank::move(float time, DestructibleWalls* walls) {
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

    auto x_old = coords.x;
    auto y_old = coords.y;
    coords.x += dx * time;
    coords.y += dy * time;
    checkCollisionsMap(x_old, y_old, dx, dy);
    checkCollisionsWall(x_old, y_old, dx, dy, walls);

    setPos();
}

void Bullet::moveBots(float time, Player& p, DestructibleWalls* walls) {
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

    this->checkCollisionsObject(p, walls);
    this->checkCollisionsObject(walls, p);
    if (m_life == 1) {
        coords.x += dx * time;
        coords.y += dy * time;
    }
    setPos();
}

void Bullet::move(float time, Player&p, std::vector<Bots*> b, DestructibleWalls* walls) {
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

    this->checkCollisionsObject(time, p, b, walls);
    this->checkCollisionsObject(walls, p);
    if (m_life == 1) {
        coords.x += dx * time;
        coords.y += dy * time;
    }
    setPos();
}
sf::Sprite &Object::getSprite() { return this->sprite; }

void Object::setDir(Direction dir) {
    this->dir = dir;
}

Direction Tank::getDir() const { return this->dir; }
Direction Bullet::getDir() const {
    return this->dir;
}

int Tank::makeAction(float time, DestructibleWalls* walls) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        this->sprite.setRotation(0);
        this->dir = Direction::UP;
        this->move(time, walls);
        return 0;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        this->sprite.setRotation(-90);
        this->dir = Direction::LEFT;
        this->move(time, walls);
        return 0;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        this->sprite.setRotation(90);
        this->dir = Direction::RIGHT;
        this->move(time, walls);
        return 0;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        this->sprite.setRotation(180);
        this->dir = Direction::DOWN;
        this->move(time, walls);
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
void Tank::setShot(bool shot) {
    this->shot = shot;
}


bool Tank::getShot() const { return this->shot; }
void Tank::checkCollisionsMap(float x_old, float y_old, float x, float y) {
    for (auto &i : m_objects) {
        if (getRect().intersects(static_cast<sf::IntRect>(i.rect))) {
            if (dy > 0) {
                coords.y = y_old;
                this->dy = 0;
            }
            if (dy < 0) {
                coords.y = y_old;
                this->dy = 0;
            }
            if (dx > 0) {
                coords.x = x_old;
                this->dx = 0;
            }
            if (dx < 0) {
                coords.x = x_old;
                this->dx = 0;
            }
        }
    }
}
void Tank::checkCollisionsWall(float x_old, float y_old, float x, float y, DestructibleWalls* walls) {
    for (auto &i : walls->walls) {
        if (getRect().intersects(static_cast<sf::IntRect>(i->getRect()))) {
            if (dy > 0) {
                coords.y = y_old;
                this->dy = 0;
            }
            if (dy < 0) {
                coords.y = y_old;
                this->dy = 0;
            }
            if (dx > 0) {
                coords.x = x_old;
                this->dx = 0;
            }
            if (dx < 0) {
                coords.x = x_old;
                this->dx = 0;
            }
        }
    }
    for (auto &i : walls->bricks) {
        if (getRect().intersects(static_cast<sf::IntRect>(i->getRect()))) {
            if (dy > 0) {
                coords.y = y_old;
                this->dy = 0;
            }
            if (dy < 0) {
                coords.y = y_old;
                this->dy = 0;
            }
            if (dx > 0) {
                coords.x = x_old;
                this->dx = 0;
            }
            if (dx < 0) {
                coords.x = x_old;
                this->dx = 0;
            }
        }
    }
    for (auto &i : walls->base_player) {
        if (getRect().intersects(static_cast<sf::IntRect>(i->getRect()))) {
            if (dy > 0) {
                coords.y = y_old;
                this->dy = 0;
            }
            if (dy < 0) {
                coords.y = y_old;
                this->dy = 0;
            }
            if (dx > 0) {
                coords.x = x_old;
                this->dx = 0;
            }
            if (dx < 0) {
                coords.x = x_old;
                this->dx = 0;
            }
        }
    }
    for (auto &i : walls->base_enemy) {
        if (getRect().intersects(static_cast<sf::IntRect>(i->getRect()))) {
            if (dy > 0) {
                coords.y = y_old;
                this->dy = 0;
            }
            if (dy < 0) {
                coords.y = y_old;
                this->dy = 0;
            }
            if (dx > 0) {
                coords.x = x_old;
                this->dx = 0;
            }
            if (dx < 0) {
                coords.x = x_old;
                this->dx = 0;
            }
        }
    }
}
void Bullet::checkCollisionsObject(float time, Player &p, std::vector<Bots*> b, DestructibleWalls* walls) {
    for (auto &i : m_objects) {
            if (getRect().intersects(static_cast<sf::IntRect>(i.rect))) {
                m_life = 0;
            }
        }
    for (int i = 0; i < b.size(); i++) {
        if (getRect().intersects(b[i]->getRect())) {
            auto probability = rand() % 100;
            if (probability < 50) {
                this->play();
            }
            else {
                b[i]->setHp(b[i]->getHp() - 20);
            }
            m_life = 0;
        }
    }
}

int Player::getCount() const {
    return this->m_count_for_kills;
}
void Player::setCount(int count) {
    this->m_count_for_kills = count;
}
int BasePlayer::getHp() const {
    return this->m_hp;
}
int BaseEnemy::getHp() const {
    return this->m_hp;
}
void BaseEnemy::setHp(int hp) {
    this->m_hp = hp;
}
void BasePlayer::setHp(int hp) {
    this->m_hp = hp;
}

void Bullet::checkCollisionsObject(Player& p, DestructibleWalls* walls) {
    for (auto &i : m_objects) {
        if (getRect().intersects(static_cast<sf::IntRect>(i.rect))) {
            m_life = 0;
        }
    }
    for (auto &i : walls->base_player) {
        if (getRect().intersects(i->getRect())) {
            i->setHp(i->getHp() - WALL_DAMAGE);
            m_life = 0;
        }
    }
    if (getRect().intersects(p.getRect())) {
        p.setHp(p.getHp() - 19);
        m_life = 0;
    }
}

void Bullet::checkCollisionsObject(DestructibleWalls* walls, Player &p) {
    auto damage = WALL_DAMAGE / 2.0;
    auto shift = BLOCK_SIZE * (damage / (double)WALL_INIT);
    for (auto &i : walls->walls) {
        if (getRect().intersects(i->getRect())) {
            if (static_cast<Direction>(getDir()) == Direction::UP && ((m_is_bot && i->getName() == "wall_player"))) {
                i->rect.height -= shift;
                i->getSprite().setTextureRect(i->rect);

                i->setHp(i->getHp() - damage);

            }
            if (static_cast<Direction>(getDir()) == Direction::DOWN && ((m_is_bot && i->getName() == "wall_player"))) {
                i->rect.height -= shift;
                i->coords.y += shift;
                i->getSprite().setPosition(i->coords.x + i->rect_texture.width / 2, i->coords.y + i->rect_texture.height / 2);
                i->getSprite().setTextureRect(i->rect);

                i->setHp(i->getHp() - damage);

            }
            if (static_cast<Direction>(getDir()) == Direction::LEFT && ((m_is_bot && i->getName() == "wall_player"))) {
                i->rect.width -= shift;
                i->getSprite().setTextureRect(i->rect);
                i->getSprite().setPosition(i->coords.x + i->rect_texture.width / 2 , i->coords.y + i->rect_texture.width / 2);

                i->setHp(i->getHp() - damage);

            }
            if (static_cast<Direction>(getDir()) == Direction::RIGHT && ((m_is_bot && i->getName() == "wall_player"))) {
                i->rect.width -= shift;
                i->coords.x += shift;
                i->getSprite().setPosition(i->coords.x + i->rect_texture.width / 2 , i->coords.y + i->rect_texture.width / 2);
                i->getSprite().setTextureRect(i->rect);

                i->setHp(i->getHp() - damage);

            }
            if (static_cast<Direction>(getDir()) == Direction::UP && ((!m_is_bot && i->getName() == "wall_enemy") && (p.getCount() <= 0))) {
                i->rect.height -= shift;
                i->getSprite().setTextureRect(i->rect);
                i->getSprite().setPosition(i->coords.x + i->rect_texture.width / 2 , i->coords.y + i->rect_texture.width / 2);


                i->setHp(i->getHp() - damage);

            }
            if (static_cast<Direction>(getDir()) == Direction::DOWN && ((!m_is_bot && i->getName() == "wall_enemy") && (p.getCount() <= 0))) {
                i->rect.height -= shift;
                i->coords.y += shift;
                i->getSprite().setPosition(i->coords.x + i->rect_texture.width / 2, i->coords.y + i->rect_texture.height / 2);
                i->getSprite().setTextureRect(i->rect);

                i->setHp(i->getHp() - damage);

            }
            if (static_cast<Direction>(getDir()) == Direction::LEFT && ((!m_is_bot && i->getName() == "wall_enemy") && (p.getCount() <= 0))) {
                i->rect.width -= shift;
                i->getSprite().setTextureRect(i->rect);

                i->setHp(i->getHp() - damage);

            }
            if (static_cast<Direction>(getDir()) == Direction::RIGHT && ((!m_is_bot && i->getName() == "wall_enemy") && (p.getCount() <= 0))) {
                i->rect.width -= shift;
                i->coords.x += shift;
                i->getSprite().setPosition(i->coords.x + i->rect_texture.width / 2, i->coords.y + i->rect_texture.width / 2);
                i->getSprite().setTextureRect(i->rect);

                i->setHp(i->getHp() - damage);
            }
            if (static_cast<Direction>(getDir()) == Direction::UP && ((!m_is_bot && i->getName() != "wall_enemy" && i->getName() != "wall_player"))) {
                i->rect.height -= shift;
                i->getSprite().setTextureRect(i->rect);

                i->setHp(i->getHp() - damage);

            }
            if (static_cast<Direction>(getDir()) == Direction::DOWN && ((!m_is_bot && i->getName() != "wall_enemy" && i->getName() != "wall_player"))) {
                i->rect.height -= shift;
                i->coords.y += shift;
                i->getSprite().setPosition(i->coords.x + i->rect_texture.width / 2, i->coords.y + i->rect_texture.height / 2);
                i->getSprite().setTextureRect(i->rect);

                i->setHp(i->getHp() - damage);

            }
            if (static_cast<Direction>(getDir()) == Direction::LEFT && ((!m_is_bot && i->getName() != "wall_enemy" && i->getName() != "wall_player"))) {
                i->rect.width -= shift;
                i->getSprite().setTextureRect(i->rect);

                i->setHp(i->getHp() - damage);

            }
            if (static_cast<Direction>(getDir()) == Direction::RIGHT && ((!m_is_bot && i->getName() != "wall_enemy" && i->getName() != "wall_player"))) {
                i->rect.width -= shift;
                i->coords.x += shift;
                i->getSprite().setPosition(i->coords.x + i->rect_texture.width / 2, i->coords.y + i->rect_texture.width / 2);
                i->getSprite().setTextureRect(i->rect);

                i->setHp(i->getHp() - damage);
            }

            m_life = 0;
        }

    }
    shift = BLOCK_SIZE * (WALL_DAMAGE / (double)WALL_INIT);

    for (auto &i : walls->bricks) {
        if (getRect().intersects(i->getRect())) {
            if (static_cast<Direction>(getDir()) == Direction::UP && !m_is_bot) {
                i->rect.height -= shift;
                i->getSprite().setTextureRect(i->rect);

            }
            if (static_cast<Direction>(getDir()) == Direction::DOWN && !m_is_bot) {
                i->rect.height -= shift;
                i->coords.y += shift;
                i->getSprite().setPosition(i->coords.x + i->rect_texture.width / 2, i->coords.y + i->rect_texture.height / 2 + 1);
                i->getSprite().setTextureRect(i->rect);

            }
            if (static_cast<Direction>(getDir()) == Direction::LEFT && !m_is_bot) {
                i->rect.width -= shift;
                i->getSprite().setTextureRect(i->rect);


            }
            if (static_cast<Direction>(getDir()) == Direction::RIGHT && !m_is_bot) {
                i->rect.width -= shift;
                i->coords.x += shift;
                i->getSprite().setPosition(i->coords.x + i->rect_texture.width / 2 + 1, i->coords.y + i->rect_texture.width / 2);
                i->getSprite().setTextureRect(i->rect);

            }

            i->setHp(i->getHp() - WALL_DAMAGE);
            

            m_life = 0;
        }
    }
    for (auto &i : walls->base_player) {
        if (getRect().intersects(i->getRect())) {
            m_life = 0;
        }
    }
    for (auto &i : walls->base_enemy) {
        if (getRect().intersects(i->getRect())) {
            if (p.getCount() <= 0) {
                i->setHp(i->getHp() - 10);
            }
            m_life = 0;
        }
    }
}

int Tank::getHp() const {
    return this->m_hp;
}

void Tank::setHp(int hp) {
    this->m_hp = hp;
}
void Wall ::setHp(int hp) {
    this->m_hp = hp;
}
void Brick ::setHp(int hp) {
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
                    coords.y = it->getPos().y - rect.height;
                    this->dy = 0;
                }
                if (dy < 0) {
                    coords.y = it->getPos().y + rect.height;
                    this->dy = 0;
                }
                if (dx > 0) {
                    coords.x = it->getPos().x - rect.width;
                    this->dx = 0;
                }
                if (dx < 0) {
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

int Object::comparisonPos(Player &p, std::vector<Bots*> b) {
    for (int i = 0; i < b.size(); i++) {
        if (abs(p.coords.x - b[i]->coords.x) < 5) {
            return i;
        }
        if (abs(p.coords.y - b[i]->coords.y) < 2) {
            return i;
        }
    }
    return b.size();
}

int Bots::checkCollisionsBase(std::vector<Bots *> b, DestructibleWalls *walls) {
    // std::cout << "Base:y" << walls->base_player[0]->coords.y << std::endl;
    for (int i = 0; i < b.size(); i++) {

        if (abs(walls->base_player[0]->coords.y - b[i]->coords.y) < 10) {
            return i;
        }
        if (abs(walls->base_player[0]->coords.x - b[i]->coords.x) < 6) {
            return i;
        }
    }
    return b.size();
}

void Bots::checkCollisionsBullet(float x_old, float y_old, float x, float y,
                                 std::vector<std::shared_ptr<Bullet>> bullet, Player& p) {

    for (auto &it : bullet) {
        if (getRect().intersects(static_cast<sf::IntRect>(it->getRect()))) {
            if (static_cast<Direction>(p.getDir()) == Direction::UP) {
                coords.y = y_old;
                this->dy = 0;
                dir = Direction::DOWN;
            }
            if (static_cast<Direction>(p.getDir()) == Direction::DOWN) {
                coords.y = y_old;
                this->dy = 0;
                dir = Direction::UP;
            }
            if (static_cast<Direction>(p.getDir()) == Direction::LEFT) {
                coords.x = x_old;
                dir = Direction::RIGHT;
                this->dx = 0;
            }
            if (static_cast<Direction>(p.getDir()) == Direction::RIGHT) {
                coords.x = x_old;
                this->dx = 0;
                dir = Direction::LEFT;
            }
        }
    }

}

void Bots::checkCollisionsWalls(float x_old, float y_old, float x, float y, 
                                    DestructibleWalls* walls) {
    std::random_device rd;
    std::uniform_int_distribution<int> uid(0, 3);
        for (auto &it : walls->walls) {
            if (getRect().intersects(static_cast<sf::IntRect>(it->getRect()))) {
                if (dy > 0) {
                    coords.y = y_old;
                    this->dy = 0;
                    dir = static_cast<Direction>(uid(rd));
                }
                if (dy < 0) {
                    coords.y = y_old;
                    this->dy = 0;
                    dir = static_cast<Direction>(uid(rd));
                }
                if (dx > 0) {
                    coords.x = x_old;
                    dir = static_cast<Direction>(uid(rd));
                    this->dx = 0;
                }
                if (dx < 0) {
                    coords.x = x_old;
                    this->dx = 0;
                    dir = static_cast<Direction>(uid(rd));
                }
            }
        }
    for (auto &it : walls->bricks) {
            if (getRect().intersects(static_cast<sf::IntRect>(it->getRect()))) {
                if (dy > 0) {
                    coords.y = y_old;
                    this->dy = 0;
                    dir = static_cast<Direction>(uid(rd));
                }
                if (dy < 0) {
                    coords.y = y_old;
                    this->dy = 0;
                    dir = static_cast<Direction>(uid(rd));
                }
                if (dx > 0) {
                    coords.x = x_old;
                    dir = static_cast<Direction>(uid(rd));
                    this->dx = 0;
                }
                if (dx < 0) {
                    coords.x = x_old;
                    this->dx = 0;
                    dir = static_cast<Direction>(uid(rd));
                }
            }
        }
    for (auto &it : walls->base_enemy) {
        if (getRect().intersects(static_cast<sf::IntRect>(it->getRect()))) {
            if (dy > 0) {
                coords.y = y_old;
                this->dy = 0;
                dir = static_cast<Direction>(uid(rd));
            }
            if (dy < 0) {
                coords.y = y_old;
                this->dy = 0;
                dir = static_cast<Direction>(uid(rd));
            }
            if (dx > 0) {
                coords.x = x_old;
                dir = static_cast<Direction>(uid(rd));
                this->dx = 0;
            }
            if (dx < 0) {
                coords.x = x_old;
                this->dx = 0;
                dir = static_cast<Direction>(uid(rd));
            }
        }
    }
    for (auto &it : walls->base_player) {
        if (getRect().intersects(static_cast<sf::IntRect>(it->getRect()))) {
            if (dy > 0) {
                coords.y = y_old;
                this->dy = 0;
                dir = static_cast<Direction>(uid(rd));
            }
            if (dy < 0) {
                coords.y = y_old;
                this->dy = 0;
                dir = static_cast<Direction>(uid(rd));
            }
            if (dx > 0) {
                coords.x = x_old;
                dir = static_cast<Direction>(uid(rd));
                this->dx = 0;
            }
            if (dx < 0) {
                coords.x = x_old;
                this->dx = 0;
                dir = static_cast<Direction>(uid(rd));
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
                    coords.y = y_old;
                    this->dy = 0;
                    dir = static_cast<Direction>(uid(rd));
                }
                if (dy < 0) {
                    coords.y = y_old;
                    this->dy = 0;
                    dir = static_cast<Direction>(uid(rd));
                }
                if (dx > 0) {
                    coords.x = x_old;
                    this->dx = 0;
                    dir = static_cast<Direction>(uid(rd));
                }
                if (dx < 0) {
                    coords.x = x_old;
                    this->dx = 0;
                    dir = static_cast<Direction>(uid(rd));
                }
            } else if (getRect().intersects(static_cast<sf::IntRect>(p.getRect()))) {
                if (dy > 0) {
                    coords.y = p.getPos().y - rect.height;
                    this->dy = 0;
                    dir = static_cast<Direction>(uid(rd));
                }
                if (dy < 0) {
                    coords.y = p.getPos().y + rect.height;
                    this->dy = 0;
                    dir = static_cast<Direction>(uid(rd));
                }
                if (dx > 0) {
                    coords.x = p.getPos().x - rect.width;
                    this->dx = 0;
                    dir = static_cast<Direction>(uid(rd));
                }
                if (dx < 0) {
                    coords.x = p.getPos().x + rect.width;
                    this->dx = 0;
                    dir = static_cast<Direction>(uid(rd));
                }
            }
        }
    }
}

void Bots::move(float time, Player &p, std::vector<Bots*> b, DestructibleWalls* walls, std::vector<std::shared_ptr<Bullet>> all_bullets) {
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

    for (int i = 0; i < b.size(); i++) {
        if (comparisonPos(p, b) == i) {
            b[i]->setShot(true);
        }
    }
    for (int i = 0; i < b.size(); i++) {
        if (checkCollisionsBase(b, walls) == i) {
            b[i]->setShot(true);
        }
    }

    this->checkCollisionsWalls(x_old, y_old, dx, dy, walls);
    this->checkCollisionsBullet(x_old, y_old, dx, dy, all_bullets, p);
    setPos();

}

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

    finish.loadFromFile(FINISH_SOUND);
    finish_sound.setBuffer(finish);

    blood.loadFromFile(BLOOD_SOUND);
    blood_sound.setBuffer(blood);
    visability.loadFromFile(VISABILITY_SOUND);
    visability_sound.setBuffer(visability);

    ricochet.loadFromFile(RICOSCHET_SOUND);
    ricochet_sound.setBuffer(ricochet);
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
        case FINISH:
            if (this->finish_sound.getStatus() != sf::Sound::Playing) {
                this->finish_sound.play();
            }
            break;
        case BLOOD:
            if (this->blood_sound.getStatus() != sf::Sound::Playing) {
                this->blood_sound.play();
        case VISABILITY:
            if (this->visability_sound.getStatus() != sf::Sound::Playing) {
                this->visability_sound.play();
            }
            break;
        case RICOCHET:
            if (this->ricochet_sound.getStatus() != sf::Sound::Playing) {
                this->ricochet_sound.play();
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
int Wall::getHp() const {
    return this->m_hp;
}
int Brick::getHp() const {
    return this->m_hp;
}
std::string Wall::getName() {
    return m_type;
}
int BasePlayer::getBulletsToDeath() {
    return m_hp / 10;
}
int BaseEnemy::getBulletsToDeath() {
    return m_hp / 10;
}
void Bullet::play() {
    if (this->ricochet_sound.getStatus() != sf::Sound::Playing) {
            this->ricochet_sound.play();
    }
}
void Player::play_visability() {
        if (this->visability_sound.getStatus() != sf::Sound::Playing) {
            this->visability_sound.play();
    }
}

void Player::set_visability(bool action) {
    this->m_visability = action;
}

bool Player::get_visability() {
    return this->m_visability;
}

