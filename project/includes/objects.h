#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

#include "map.h"

enum class Direction { UP = 0, DOWN, RIGHT, LEFT, COUNT, ERROR };
class Bots;
class Object {
   public:
    Object(sf::String textureFile, sf::IntRect rect, sf::Vector2f pos,
           float speed, Direction dir);

    sf::Vector2f getPos() const;
    void setPos(const sf::Vector2f& new_pos);
    void setPos();
    // void setPos(const sf::Vector2f& new_pos);
    sf::Sprite& getSprite();
    float getX() const;
    float getY() const;
    sf::IntRect getRect();

   protected:
    sf::Vector2f coords;  //координаты
    float dx, dy;         // current delta coordinats
    float speed;          //скорость (множитель времени)
    Direction dir;
    sf::IntRect rect;
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
    std::vector<MapObject> m_objects;
    sf::Sound m_sound;
    sf::SoundBuffer m_buffer;
    // Object();
};

class Bullet : public Object {
   public:
    Bullet(sf::String textureFile, sf::String soundFile, sf::IntRect rect, sf::Vector2f pos,
           float speed, Direction dir)
        : Object(textureFile, rect, pos, speed, dir){
            if (!m_buffer.loadFromFile(soundFile)) {
                throw std::exception();
            }
            m_sound.setBuffer(m_buffer);
        };
    void move(float time);
    void sound();
};

class Tank : public Object {  //класс любого танка
   protected:
    // sf::String name;
    int hp;
    // Direction dir;
    bool shot;
    // size_t armor;
    // size_t power;
    Tank(Level& mapObj, sf::String textureFile, sf::IntRect rect,
         sf::Vector2f pos, float speed, int hp, Direction dir)
        : Object(textureFile, rect, pos, speed, dir), hp(hp), shot(false) {
        m_objects = mapObj.GetAllObjects("solid");
    }

   public:
    virtual void checkCollisionsMap(float x_old, float y_old, float x, float y);
    virtual void move(float time);
    int makeAction(float time);
    Direction getDir() const;
    bool getShot() const;
    void setShot(bool shot);
};

class Player : public Tank {  //класс игрока
   public:
    Player(Level& mapObj, sf::String textureFile, sf::IntRect rect,
           sf::Vector2f pos, float speed, int hp, Direction dir)
        : Tank(mapObj, textureFile, rect, pos, speed, hp, dir) {}
    void checkCollisionsBots(Bots &p);
    
    
};

class Bots : public Tank {  //класс игрока
public:
    Bots(Level& mapObj, sf::String textureFile, sf::IntRect rect,
           sf::Vector2f pos, float speed, int hp, Direction dir)
            : Tank(mapObj, textureFile, rect, pos, speed, hp, dir) {
                m_objects = mapObj.GetAllObjects();
            }
    void checkCollisionsMap(float x_old, float y_old, float x, float y, Player &p);
    void move(float time, Player &p);
};
