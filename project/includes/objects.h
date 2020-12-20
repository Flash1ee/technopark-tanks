#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

#include "map.h"
#include "menu.h"



#define GAME_START_SOUND "../sounds/gamestart.ogg"
#define GAME_OVER_SOUND "../sounds/gameover.ogg"
#define BACKGROUND_SOUND "../sounds/background.ogg"
#define FIRE_SOUND "../sounds/fire.ogg"
#define SPAWN_SOUND "../sounds/bonus.ogg"
#define BRICK_SOUND "../sounds/brick.ogg"
#define KILL_SOUND "../sounds/explosion.ogg"
#define WASTED_SOUND "../sounds/wasted.ogg"


enum class SoundType {
    BULLET,
    COUNT,
    ERROR
};

class Wall;
class BasePlayer;
class BaseEnemy;

struct DestructibleWalls {
    std::vector<std::shared_ptr<Wall>> walls;
    std::vector<std::shared_ptr<BasePlayer>> base_player;
    std::vector<std::shared_ptr<BaseEnemy>> base_enemy;

};
typedef enum {
    BACKGROUND,
    BRICK,
    KILL,
    FIRE,
    GAME_OVER,
    GAME_START,
    SPAWN,
    WASTED_S,
    COUNT
} sound_action;
class Sound {
    private:
    sf::Sound sound;
    sf::SoundBuffer buffer;

    sf::SoundBuffer background;
    sf::Sound background_sound;

    sf::SoundBuffer brick;
    sf::Sound brick_sound;

    sf::SoundBuffer explosion;
    sf::Sound explosion_sound;

    sf::SoundBuffer fire;
    sf::Sound fire_sound;

    sf::SoundBuffer gameover;
    sf::Sound gameover_sound;

    sf::SoundBuffer gamestart;
    sf::Sound gamestart_sound;

    sf::SoundBuffer steel;
    sf::Sound steel_sound;

    sf::SoundBuffer wasted;
    sf::Sound wasted_sound;
    public:
    void play(sound_action action);
    // void play();
    //   Sound(std::string path);
    Sound();
    bool MainSoundStopped();
};

enum class Direction { UP = 0, DOWN, RIGHT, LEFT, COUNT, ERROR };
class Bots;
class Player;

class Object {
   public:
    sf::IntRect rect;
    sf::Vector2f coords;  //координаты


    Object(sf::String textureFile, sf::IntRect rect, sf::Vector2f pos,
           float speed, Direction dir);

    sf::Vector2f getPos() const;
    void setPos(const sf::Vector2f& new_pos);
    void setPos();
    void setDir(Direction dir);


    sf::Sprite& getSprite();
    sf::IntRect getRect();
    int comparisonPos(Player &p, std::vector<Bots*> b);


   protected:
    // sf::Vector2f coords;  //координаты
    float dx, dy;         // current delta coordinats
    float speed;          //скорость (множитель времени)
    Direction dir;
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
    std::vector<MapObject> m_objects;
};

class BasePlayer: public Object {
private:
    int m_hp;
public:
    BasePlayer(Level& mapObj, sf::String textureFile, sf::IntRect rect,
         sf::Vector2f pos, float speed, int hp, Direction dir)
            : Object(textureFile, rect, pos, speed, dir), m_hp(hp) {
        m_objects = mapObj.GetAllObjects("player_base");
    }
    int getHp() const;
    void setHp(int hp);

};

class BaseEnemy: public Object {
private:
    int m_hp;
    int m_count_for_kills;
public:
    BaseEnemy(Level& mapObj, sf::String textureFile, sf::IntRect rect,
    sf::Vector2f pos, float speed, int hp, Direction dir)
    : Object(textureFile, rect, pos, speed, dir), m_hp(hp), m_count_for_kills(4) {
        m_objects = mapObj.GetAllObjects("enemy_base");
    }
    int getHp() const;
    int getCount() const;
    void setCount(int count);
    void setHp(int hp);

};

class Wall: public Object {
    private:
     int m_hp;
     bool crash;
     sf::IntRect rect_crash;
     sf::Vector2f cords_crash;
     sf::Sprite sprite_crash;
    public:
     Wall(Level& mapObj, sf::String textureFile, sf::IntRect rect,
           sf::Vector2f pos, float speed, int hp, Direction dir)
        : Object(textureFile, rect, pos, speed, dir) {
            m_objects = mapObj.GetAllObjects("wall");
            m_hp = hp;
            rect_crash = sf::IntRect(289, 128, 13, 13);
            crash = false;
            sprite_crash.setTexture(this->texture);
            sprite_crash.setOrigin(rect_crash.width / 2, rect_crash.height / 2);
            sprite_crash.setTextureRect(rect_crash);
            
        }
    void setCrash(sf::Vector2f &pos);
    bool getCrash();
    void updateCrash();
    sf::Sprite getCrashSprite();
    int getHp() const;
    void setHp(int hp);
    void setPosWall();
          
};

class Bullet : public Object {
public:
    Bullet(Level& mapObj, sf::String textureFile, sf::String soundFile, sf::IntRect rect, sf::Vector2f pos,
           float speed, Direction dir, int life)
        : Object(textureFile, rect, pos, speed, dir), m_life(life) {
        m_objects = mapObj.GetAllObjects("solid");
        };
    void move(float time, Player& p, std::vector<Bots*> b, DestructibleWalls* walls);
    void moveBots(float time, Player& p, DestructibleWalls* walls);
    void checkCollisionsObject(float time, Player &p, std::vector<Bots*> b, DestructibleWalls* walls);
    void checkCollisionsObject(Player& p, DestructibleWalls* walls);
    void checkCollisionsObject(DestructibleWalls* walls);
    // void sound();
    int getLife() const;
    Direction getDir() const;
private:
    int m_life;
    // int m_damage;
};

class Tank : public Object {  //класс любого танка
   protected:
    // sf::String name;
    int m_hp;
    // Direction dir;
    bool shot;
    // size_t armor;
    // size_t power;
    Tank(Level& mapObj, sf::String textureFile, sf::IntRect rect,
         sf::Vector2f pos, float speed, int hp, Direction dir)
        : Object(textureFile, rect, pos, speed, dir), m_hp(hp), shot(false) {
        m_objects = mapObj.GetAllObjects("solid");
    }

   public:
    virtual void checkCollisionsMap(float x_old, float y_old, float x, float y);
    virtual void checkCollisionsWall(float x_old, float y_old, float x, float y, DestructibleWalls* walls);

    virtual void move(float time, DestructibleWalls* walls);
    int makeAction(float time, DestructibleWalls* walls);
    Direction getDir() const;
    bool getShot() const;
    int getHp() const;
    void setHp(int hp);
    void setShot(bool shot);
};
class Player : public Tank {  //класс игрока
   public:
    Player(Level& mapObj, sf::String textureFile, sf::IntRect rect,
           sf::Vector2f pos, float speed, int hp, Direction dir)
        : Tank(mapObj, textureFile, rect, pos, speed, hp, dir) {}
    void checkCollisionsBots(std::vector<Bots*> b);
    
};

class Bots : public Tank {  //класс игрока
public:
    Bots(Level& mapObj, sf::String textureFile, sf::IntRect rect,
           sf::Vector2f pos, float speed, int hp, Direction dir)
            : Tank(mapObj, textureFile, rect, pos, speed, hp, dir) {
                m_objects = mapObj.GetAllObjects();
            }
    void checkCollisionsObjects(float x_old, float y_old, float x, float y, Player &p,
                                std::vector<Bots*> b);
    void move(float time, Player &p, std::vector<Bots*> b, DestructibleWalls* walls,
              std::vector<std::shared_ptr<Bullet>> all_bullets);
    void checkCollisionsBullet(float x_old, float y_old, float x, float y,
                               std::vector<std::shared_ptr<Bullet>> bullet, Player& p);
    void checkCollisionsWalls(float x_old, float y_old, float x, float y,
                              DestructibleWalls* walls);

};
