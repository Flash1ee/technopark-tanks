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
// #define KILL_SOUND "../sounds/explosion.ogg"
#define KILL_SOUND "../sounds/kill.ogg"
#define FINISH_SOUND "../sounds/finish.ogg"
#define BLOOD_SOUND "../sounds/first_blood.ogg"
#define VISABILITY_SOUND "../sounds/visibility.ogg"
#define RICOSCHET_SOUND "../sounds/ricochet.ogg"
#define BOSS_SOUND "../sounds/boss.ogg"



enum class SoundType {
    BULLET,
    COUNT,
    ERROR
};

class Wall;
class Object;
class Brick;
class BasePlayer;
class BaseEnemy;

struct DestructibleWalls {
    std::vector<std::shared_ptr<Wall>> walls;
    std::vector<std::shared_ptr<Object>> grass;
    std::vector<std::shared_ptr<Brick>> bricks;
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
    FINISH,
    BLOOD,
    VISABILITY,
    RICOCHET,
    BOSS,
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

    sf::SoundBuffer finish;
    sf::Sound finish_sound;

    sf::SoundBuffer blood;
    sf::Sound blood_sound;

    sf::SoundBuffer visability;
    sf::Sound visability_sound;

    sf::SoundBuffer ricochet;
    sf::Sound ricochet_sound;

    sf::SoundBuffer tank_kill;
    sf::Sound tank_kill_sound;

    sf::SoundBuffer boss;
    sf::Sound boss_sound;

    public:
    void play(sound_action action);
    // void play();
    //   Sound(std::string path);
    void stop_all();
    Sound();
    bool MainSoundStopped();
};

enum class Direction { UP = 0, DOWN, RIGHT, LEFT, COUNT, ERROR };
class Bots;
class Player;
class BotBoss;

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
    bool comparisonPos(Player &p, std::vector<BotBoss*> boss);


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
    int getBulletsToDeath();

};

class BaseEnemy: public Object {
private:
    int m_hp;
public:
    BaseEnemy(Level& mapObj, sf::String textureFile, sf::IntRect rect,
    sf::Vector2f pos, float speed, int hp, Direction dir)
    : Object(textureFile, rect, pos, speed, dir), m_hp(hp) {
        m_objects = mapObj.GetAllObjects("enemy_base");
    }
    int getHp() const;
    void setHp(int hp);
    int getBulletsToDeath();


};

class Wall: public Object {
    private:
     int m_hp;
     std::string m_type;
     bool m_crash_enemy;
    public:
     sf::IntRect rect_texture;
     sf::Vector2f bef_coords;
     Wall(Level& mapObj, sf::String textureFile, sf::IntRect rect,
           sf::Vector2f pos, float speed, int hp, Direction dir, std::string name)
        : Object(textureFile, rect, pos, speed, dir) {
            m_objects = mapObj.GetAllObjects("wall");
            for (auto &i : mapObj.GetAllObjects("wall_player")) {
                m_objects.push_back(i);
            }
            for (auto &i : mapObj.GetAllObjects("wall_enemy")) {
                m_objects.push_back(i);
            }
            m_crash_enemy = false;
            m_type = name;
            m_hp = hp;
            rect_texture = rect;
            bef_coords = sf::Vector2f(pos.x, pos.y);
            
        }
    std::string getName();
    int getHp() const;
    void setHp(int hp);
          
};

class Brick : public Object {
    private:
     int m_hp;
    public:
     sf::IntRect rect_texture;
     sf::Vector2f bef_coords;
     Brick(Level& mapObj, sf::String textureFile, sf::IntRect rect,
           sf::Vector2f pos, float speed, int hp, Direction dir)
        : Object(textureFile, rect, pos, speed, dir) {
            m_objects = mapObj.GetAllObjects("brick");
            m_hp = hp;
            rect_texture = rect;
            bef_coords = sf::Vector2f(pos.x, pos.y);
            
        }
    int getHp() const;
    void setHp(int hp);
};

class Bullet : public Object {
public:
    Bullet(Level& mapObj, sf::String textureFile, sf::String soundFile, sf::IntRect rect, sf::Vector2f pos,
           float speed, Direction dir, int life, bool bot, int number)
        : Object(textureFile, rect, pos, speed, dir), m_life(life), m_is_bot(bot), bots_number(number) {
        m_objects = mapObj.GetAllObjects("solid");

        ricochet.loadFromFile(RICOSCHET_SOUND);
        ricochet_sound.setBuffer(ricochet);
        };
    int m_id;
    void move(float time, Player& p, std::vector<Bots*> b, DestructibleWalls* walls, std::vector<BotBoss*> boss);
    void move(float time, Player& p, std::vector<Bots*> b, DestructibleWalls* walls, std::map<int, std::shared_ptr<Player>> other);
    void moveBots(float time, Player& p, DestructibleWalls* walls, std::vector<BotBoss*> boss, std::vector<Bots*> b);
    void checkCollisionsObject(float time, Player &p, std::vector<Bots*> b, DestructibleWalls* walls, std::vector<BotBoss*> boss);
    void checkCollisionsObjectBots(Player& p, DestructibleWalls* walls, std::vector<BotBoss*> boss, std::vector<Bots*> b);
    void checkCollisionsObject(DestructibleWalls* walls, Player& p);
    void checkCollisionsObject(float time, Player &p, std::vector<Bots*> b, DestructibleWalls* walls);
    void checkCollisionsObject(Player& p, DestructibleWalls* walls, std::map<int, std::shared_ptr<Player>> other);
    void checkCollisionsPlayers(std::map<int, std::shared_ptr<Player>> other);
    void moveBots(float time, Player& p, DestructibleWalls* walls, std::map<int, std::shared_ptr<Player>> other);
    void play();
    // void sound();
    int getLife() const;
    Direction getDir() const;
    int getNumber() const;
private:
    int bots_number;
    int m_life;
    int m_is_bot;
    sf::SoundBuffer ricochet;
    sf::Sound ricochet_sound;
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
   private:
    int m_count_for_kills;
    bool m_visability;
    sf::SoundBuffer visability;
    sf::Sound visability_sound;
   public:
    Player(Level& mapObj, sf::String textureFile, sf::IntRect rect,
           sf::Vector2f pos, float speed, int hp, Direction dir)
        : Tank(mapObj, textureFile, rect, pos, speed, hp, dir), m_count_for_kills(4), m_visability(true)  {
            visability.loadFromFile(VISABILITY_SOUND);
            visability_sound.setBuffer(visability);
        };
    void checkCollisionsBots(std::vector<Bots*> b, std::vector<BotBoss*> boss);
    void checkCollisionsPlayers(std::map<int, std::shared_ptr<Player>> other);
    int getCount() const;
    void setCount(int count);
    void play_visability();
    void set_visability(bool action);
    bool get_visability();
    void stop_visability();



};

class Bots : public Tank {//класс игрока
private:
    float shoot_time;
public:
    void SetShootTime(float time);
    float GetShootTime();
    Bots(Level& mapObj, sf::String textureFile, sf::IntRect rect,
           sf::Vector2f pos, float speed, int hp, Direction dir)
            : Tank(mapObj, textureFile, rect, pos, speed, hp, dir) {
                m_objects = mapObj.GetAllObjects();
                shoot_time = 0;
            }
    void checkCollisionsObjects(float x_old, float y_old, float x, float y, Player &p,
                                std::vector<Bots*> b, std::vector<BotBoss*> boss);
    void move(float time, Player &p, std::vector<Bots*> b, DestructibleWalls* walls,
              std::vector<std::shared_ptr<Bullet>> all_bullets, std::vector<BotBoss*> boss);
    void checkCollisionsBullet(float x_old, float y_old, float x, float y,
                               std::vector<std::shared_ptr<Bullet>> bullet, Player& p);
    void checkCollisionsWalls(float x_old, float y_old, float x, float y,
                              DestructibleWalls* walls);
    int checkCollisionsBase(std::vector<Bots*> b, DestructibleWalls* walls);

};

class BotBoss : public Tank {  //класс игрока
public:
    BotBoss(Level& mapObj, sf::String textureFile, sf::IntRect rect,
         sf::Vector2f pos, float speed, int hp, Direction dir)
            : Tank(mapObj, textureFile, rect, pos, speed, hp, dir) {
        m_objects = mapObj.GetAllObjects();
    }
    void checkCollisionsObjects(float x_old, float y_old, float x, float y, Player &p,
                                std::vector<Bots*> b);
    void move(float time, Player &p, std::vector<Bots*> b, DestructibleWalls* walls,
              std::vector<std::shared_ptr<Bullet>> all_bullets, std::vector<BotBoss*> boss);
    void checkCollisionsBullet(float x_old, float y_old, float x, float y,
                               std::vector<std::shared_ptr<Bullet>> bullet, Player& p);
    void checkCollisionsWalls(float x_old, float y_old, float x, float y,
                              DestructibleWalls* walls);
    bool checkCollisionsBase(std::vector<BotBoss*> boss, DestructibleWalls* walls);

};
