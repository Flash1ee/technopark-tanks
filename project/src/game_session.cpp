#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <map>
#include <string>

#include "messages.hpp"
#include "cam.h"
#include "menu.h"
#include "game.h"
#include "game_map.hpp"
#include "game_session.hpp"
#include "statistics.h"

GameSession::GameSession(std::string window_title, std::string& map_path,
                         std::string& player_skin, bool is_multiplayer,
                         std::string server_ip, int server_port)
    :
      m_window(sf::VideoMode(1920, 1080), window_title),
      m_is_multiplayer(is_multiplayer) {
    m_level.LoadFromFile("../maps/map1.tmx");
    MapObject player = m_level.GetFirstObject("player1");
    

    sf::FloatRect p_pos = player.rect;
    m_player_pos = {p_pos.left + p_pos.width / 2, p_pos.top - p_pos.width / 2};
    if (m_is_multiplayer) {
        m_player_pos = m_game_client.connectToServer(server_ip, server_port);

        // sf::Packet packet;

        // if(m_game_client.RecieveFromServer(packet))
        // {
        //     int id;
        //     packet >> id;

        //     std::cout << "Client id on server = " << id << std::endl;
        // }
        // else
        // {
        //     std::cout << "cannot get client id from server" << std::endl;
        //     exit(-1);
        // }
    }
    if (!dead.loadFromFile(WASTED)) {
        throw std::exception();
    };
    m_dead.setTexture(dead);
    m_dead.setTextureRect(sf::IntRect( 3, 150, 390, 104));
    m_dead.setScale(0.5, 0.5);
}

GameSession::~GameSession() {};

void GameSession::WaitForOtherPlayers() {
    // encapsulation error
    sf::Packet packet;

    while (true) {
        if (m_game_client.RecieveFromServer(packet)) {
            GameActionType game_status;
            packet >> game_status;

            if (game_status == GameActionType::GameBegin) {
                std::cout << "ALL USERS ARE CONNECTED. GAME WILL START NOW!"
                          << std::endl;
                return;
            }
        }
    }
}

int GameSession::Run() {
    if (m_is_multiplayer) {
        WaitForOtherPlayers();
        // RunOnlineGame()
    }
    // else
    // {
    //     RunOfflineGame();
    // }

    // TmxObject Player_obj = m_level.GetFirstObject("player"); //TODO: make
    // const name
    Statistic stats(m_window);
    Sound sounds;
    sounds.play(GAME_START);
    std::vector<MapObject> walls_objs = m_level.GetAllObjects("wall");
    std::vector<MapObject> player_obj = m_level.GetAllObjects("player_base");
    std::vector<MapObject> enemy_obj = m_level.GetAllObjects("enemy_base");

    DestructibleWalls walls;

    for (auto i: walls_objs) {
        sf::Vector2f wall_pos = {i.rect.left, i.rect.top - i.rect.width};
        // std::cout << wall_pos.x << " " << wall_pos.y << std::endl;
        std::shared_ptr<Wall> wall = std::make_shared<Wall>(
        m_level, OBJECT_IMAGE, sf::IntRect(256, 16, 16, 16), wall_pos, 0,
        100, Direction::UP);
        walls.walls.push_back(wall);
    }
    for (auto i: player_obj) {
        sf::Vector2f base_player_pos = {i.rect.left, i.rect.top - i.rect.width};
        // std::cout << base_player_pos.x << " " << base_player_pos.y << std::endl;
        std::shared_ptr<BasePlayer> basePlayer = std::make_shared<BasePlayer>(
                m_level, OBJECT_IMAGE, sf::IntRect(304, 32, 16, 16), base_player_pos, 0,
                200, Direction::UP);
        walls.base_player.push_back(basePlayer);
    }
    for (auto i: enemy_obj) {
        sf::Vector2f base_enemy_pos = {i.rect.left, i.rect.top - i.rect.width};
        // std::cout << base_enemy_pos.x << " " << base_enemy_pos.y << std::endl;
        std::shared_ptr<BaseEnemy> baseEnemy = std::make_shared<BaseEnemy>(
                m_level, OBJECT_IMAGE, sf::IntRect(304, 32, 16, 16), base_enemy_pos, 0,
                200, Direction::UP);
        walls.base_enemy.push_back(baseEnemy);
    }
    std::shared_ptr<Player> this_player = std::make_shared<Player>(
        m_level, OBJECT_IMAGE, sf::IntRect(1, 2, 13, 13), m_player_pos, 0.05,
        100, Direction::UP);

    std::vector<Bots*> all_bots;
    std::vector<MapObject> spawn;
    spawn.push_back(m_level.GetFirstObject("spawn1"));
    spawn.push_back(m_level.GetFirstObject("spawn2"));

    // for (int i = 0; i < 2; i++) {
    //     size_t ind = 0;
    //     if (i % 2) {
    //         ind = 1;
    //     }
    //     sf::Vector2f m_bot_pos = {spawn[ind].rect.left + spawn[ind].rect.width / 2,
    //     spawn[ind].rect.top - spawn[ind].rect.width / 2 };

    //     // sf::Vector2f m_bot_pos = {static_cast<float>(50 * (i + 1)), static_cast<float>(50 * (i + 1))};
    //     all_bots.push_back(new Bots(m_level, OBJECT_IMAGE, sf::IntRect(128, 129, 13, 13), m_bot_pos, 0.07,
    //                                 100, Direction::UP));
    // }

    sf::Vector2f old_pos = this_player->getPos();
    sf::Vector2f new_pos = old_pos;

    std::map<int, std::shared_ptr<Player>> players;
    std::map<std::vector<Bots*>, bool> shot;

    std::vector<std::shared_ptr<Bullet>> new_bullets;
    std::vector<std::shared_ptr<Bullet>> all_bullets;
    std::vector<std::shared_ptr<Bullet>> bots_bullets;
    sf::Clock clock, timer_bots, timer_shoots;
    bool is_new_user = true;

    // Sound sounds [static_cast<int>(SoundType::COUNT)] {
    //     Sound(BULLET_SOUND)wawasaawa
    // };
    size_t count_bots = 0;


    int stop = 0;
    int time_before = clock.getElapsedTime().asSeconds();
    timer_bots.restart();

    sf::Clock main_timer;
    sf::Time last_pl_bull;
    sf::Time pause_time;
    sf::Time win_time;
    sf::Time kill_time;
    sf::Time start_pause_time;
    pause_time.Zero;
    last_pl_bull.Zero;
    win_time.Zero;
    kill_time.Zero;

    while (m_window.isOpen()) {
        sf::Time times = timer_bots.getElapsedTime();
        sf::Time timer = timer_shoots.getElapsedTime();

        float time = clock.getElapsedTime().asMicroseconds();  //дать прошедшее время в микросекундах

        clock.restart();  //перезагружает время
        time /= 800;      //скорость игры


        if (times.asSeconds() > 7 && sounds.MainSoundStopped() && count_bots < 4) {
            for (int i = 0; i < 2; i++) {
                size_t ind = 0;
                if (i % 2) {
                    ind = 1;
                }
                sf::Vector2f m_bot_pos = {spawn[ind].rect.left + spawn[ind].rect.width / 2,
                                          spawn[ind].rect.top - spawn[ind].rect.width / 2};

                // sf::Vector2f m_bot_pos = {static_cast<float>(50 * (i + 1)), static_cast<float>(50 * (i + 1))};
                all_bots.push_back(new Bots(m_level, OBJECT_IMAGE, sf::IntRect(128, 129, 13, 13), m_bot_pos, 0.05,
                                            100, Direction::UP));
                stop += 1;
            }
            timer_bots.restart();

        }

        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
                exit(0);
            }

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Escape) {
                    start_pause_time = main_timer.getElapsedTime(); 
                    // sf::RenderWindow menu_window(sf::VideoMode(1024, 768), std::string("Game menu"), 
                    //                             sf::Style::None);

                    Menu gameMenu(1, m_window);
                    m_window.setView(m_window.getDefaultView());  //"оживляем" камеру в окне sfml
                    if (gameMenu.show(m_window) == STOP_RUN) {
                        return STOP_RUN;
                    }
                    pause_time += main_timer.getElapsedTime() - start_pause_time;
                    clock.restart();
                }
                if (event.key.code == sf::Keyboard::Space
                    && (main_timer.getElapsedTime().asSeconds() - last_pl_bull.asSeconds() > 0.2) )
                    {
                    this_player->setShot(true);
                    last_pl_bull = main_timer.getElapsedTime();
                }
            }

            if (this_player->getShot() && this_player->getHp() > 0) {
                this_player->setShot(false);
                // sf::Vector2f coords = this_player.getPos();
                auto bullet_dir = this_player->getDir();
                sf::Vector2f bullet_pos;
                if (bullet_dir == Direction::UP || bullet_dir == Direction::RIGHT) {
                    bullet_pos.x = this_player->getPos().x + 4.5;
                    bullet_pos.y = this_player->getPos().y + 4.5;
                } else {
                    bullet_pos.x = this_player->getPos().x + 3.5;
                    bullet_pos.y = this_player->getPos().y + 3.5;
                }
                auto new_b = std::make_shared<Bullet>(m_level,
                                                      OBJECT_IMAGE, BULLET_SOUND, sf::IntRect(323, 102, 4, 4),
                                                      bullet_pos, 0.3,
                                                      bullet_dir, 1);

                // sf::Packet packet;
                // PlayerAction new_bullet_action = {-1, bullet_pos, bullet_dir,
                // PlayerActionType::NewBullet};
                // action_vector.actions.push_back(new_bullet_action);

                all_bullets.push_back(new_b);  // Copying is too expensive
                new_bullets.push_back(new_b);
                sounds.play(FIRE);
                // sounds[static_cast<int>(SoundType::BULLET)].play();
            }


        }

        // std::cout << timer.asSeconds() << std::endl;
        if ((timer.asSeconds() > 1) && (timer.asSeconds() < 1.07)) {

            for (auto &i : all_bots) {
                if (i->getShot()) {
                    i->setShot(false);
                    // sf::Vector2f coords = this_player.getPos();
                    auto bullet_dir = i->getDir();
                    sf::Vector2f bullet_pos;
                    if (bullet_dir == Direction::UP || bullet_dir == Direction::RIGHT) {
                        bullet_pos.x = i->getPos().x + 4.5;
                        bullet_pos.y = i->getPos().y + 4.5;
                    } else {
                        bullet_pos.x = i->getPos().x + 3.5;
                        bullet_pos.y = i->getPos().y + 3.5;
                    }
                    auto new_b = std::make_shared<Bullet>(m_level,
                                                          OBJECT_IMAGE, BULLET_SOUND, sf::IntRect(323, 102, 4, 4),
                                                          bullet_pos, 0.3,
                                                          bullet_dir, 1);

                    // sf::Packet packet;
                    // PlayerAction new_bullet_action = {-1, bullet_pos, bullet_dir,
                    // PlayerActionType::NewBullet};
                    // action_vector.actions.push_back(new_bullet_action);

                    bots_bullets.push_back(new_b);  // Copying is too expensive
                    new_bullets.push_back(new_b);
                    sounds.play(FIRE);
                    // sounds[static_cast<int>(SoundType::BULLET)].play();
                }
            }
            timer_shoots.restart();
        }

        for (auto &i : all_bots) {
            i->move(time, *this_player, all_bots, &walls, all_bullets);
        }

        if (this_player->getHp() > 0) {
            if (!this_player->makeAction(time, &walls)) {
                sounds.play(BACKGROUND);
            }
        }
        this_player->checkCollisionsBots(all_bots);
        old_pos = new_pos;
        new_pos = this_player->getPos();

        if (m_is_multiplayer) {
            PlayerActionVector action_vector;

            if (is_new_user) {
                sf::Packet new_player_packet;
                PlayerAction create_new_player = {m_game_client.m_id, new_pos,
                                                  this_player->getDir(),
                                                  PlayerActionType::NewPlayer};
                new_player_packet << create_new_player;
                m_game_client.SendToServer(new_player_packet);
                is_new_user = false;
                std::cout << "I notified server about me" << std::endl;
            }

            {  // Gathering info for sending to server

                Direction dir = this_player->getDir();

                if (new_pos != old_pos) {
                    sf::Packet packet;
                    PlayerAction action = {m_game_client.m_id, new_pos, dir,
                                           PlayerActionType::UpdatePlayer};

                    action_vector.actions.push_back(action);
                    old_pos = new_pos;
                }

                if (new_bullets.size() > 0) {
                    PlayerAction action;
                    for (auto& curr_bullet : new_bullets) {
                        action = {
                            -1, new_pos, dir,
                            PlayerActionType::NewBullet};  // bullets have no id
                        action_vector.actions.push_back(action);
                    }

                    new_bullets.clear();
                }
            }

            if (action_vector.actions.size() > 0) {
                sf::Packet packet;
                packet << action_vector;
                m_game_client.SendToServer(packet);
                std::cout << "I send all my action to server" << std::endl;
            } else
                std::cout << "Nothing to send right now" << std::endl;

            {  // getting info from server and applying it to current session
                PlayerActionVector others_actions;

                sf::Packet packet;

                m_game_client.RecieveFromServer(packet);

                if (packet.getDataSize() > 0) {
                    packet >> others_actions;

                    for (auto& action : others_actions.actions) {
                        switch (action.msg_type) {
                            case PlayerActionType::NewPlayer: {
                                std::cout << "New player connected to game"
                                          << std::endl;
                                int id = action.player_id;
                                Direction dir = action.direction;
                                sf::Vector2f pos = action.position;
                                players.insert(std::make_pair(
                                    id, std::make_shared<Player>(
                                            m_level, OBJECT_IMAGE,
                                            sf::IntRect(1, 2, 13, 13), pos, 0.1,
                                            100, dir)));
                            } break;

                            case PlayerActionType::UpdatePlayer: {
                                std::cout << "Other player should be updated"
                                          << std::endl;
                                int id = action.player_id;
                                sf::Vector2f new_pos = action.position;
                                players[id]->setPos(new_pos);
                            } break;

                            case PlayerActionType::NewBullet: {
                                std::cout << "Other player shooted"
                                          << std::endl;
                                sf::Vector2f pos = action.position;
                                Direction dir = action.direction;
                                std::shared_ptr<Bullet> new_b(new Bullet(m_level,
                                    OBJECT_IMAGE, BULLET_SOUND, sf::IntRect(321, 100, 8, 8),
                                    pos, 0.5, dir, 1));
                                all_bullets.push_back(new_b);
                            } break;

                            case PlayerActionType::UpdateBullet: {
                            } break;

                            case PlayerActionType::DeleteBullet: {
                            } break;

                            default:
                                break;
                        }
                    }

                } else {
                    std::cout << "Nothing to apply right now" << std::endl;
                }
            }
        }

        {  // Drawing is here

            for (auto& curr_bullet : all_bullets) {
                curr_bullet->move(time, *this_player, all_bots, &walls);
                // for (auto &wall : walls.walls) {
                //     if (wall->getCrash()) {
                //         std::cout << "CRASHING" << std::endl;
                //         m_window.draw(wall->getCrashSprite());
                //         wall->updateCrash();
                //     }
                // }
            }

            for (auto& curr_bullet : bots_bullets) {
                curr_bullet->moveBots(time, *this_player, &walls);
            }

            m_cam.changeViewCoords(new_pos, m_level.GetTilemapWidth(), m_level.GetTilemapHeight());
            // m_cam.changeView();

            m_window.setView(m_cam.view);  //"оживляем" камеру в окне sfml
            m_window.clear();
            m_level.Draw(m_window);

            for (int i = 0; i < all_bullets.size(); i++) {
                if (all_bullets[i]->getLife()) {
                    m_window.draw(all_bullets[i]->getSprite());
                } else {
                    all_bullets.erase(all_bullets.begin() + i);
                    sounds.play(BRICK);
                }
            }
            for (int i = 0; i < bots_bullets.size(); i++) {
                
                if (bots_bullets[i]->getLife()) {
                    m_window.draw(bots_bullets[i]->getSprite());
                } else {
                    bots_bullets.erase(bots_bullets.begin() + i);
                    sounds.play(BRICK);
                }
            }
            if (all_bots.size() != count_bots) {
                sounds.play(SPAWN);
                count_bots = all_bots.size();
            }
            if (this_player->getHp() > 0) {
                m_window.draw(this_player->getSprite());
            }


            for (int i = 0; i < walls.walls.size(); i++) {
                if (walls.walls[i]->getHp() > 0) {
                        m_window.draw(walls.walls[i]->getSprite());
                    //     if (walls.walls[i]->getCrash()) {
                    
                    //     m_window.draw(walls.walls[i]->getCrashSprite());
                    //     // walls.walls[i]->updateCrash();
                    // }
                }
                if (walls.walls[i]->getHp() <= 0) {
                    walls.walls.erase(walls.walls.begin() + i);
                }
            }
            for (int i = 0; i < walls.base_player.size(); i++) {
                if (walls.base_player[i]->getHp() >= 0) {
                    m_window.draw(walls.base_player[i]->getSprite());
                }
                if (walls.base_player[i]->getHp() <= 0) {
                    walls.base_player.erase(walls.base_player.begin() + i);
                }
            }
            for (int i = 0; i < walls.base_enemy.size(); i++) {
                if (walls.base_enemy[i]->getHp() >= 0) {
                    m_window.draw(walls.base_enemy[i]->getSprite());
                }
                if (walls.base_enemy[i]->getHp() <= 0) {
                    walls.base_enemy.erase(walls.base_enemy.begin() + i);
                    exit(0);
                }
            }
            for (int i = 0; i < all_bots.size(); i++) {
                if (all_bots[i]->getHp() > 0) {
                    m_window.draw(all_bots[i]->getSprite());
                }
                if (all_bots[i]->getHp() == 0) {
                    walls.base_enemy[0]->setCount(walls.base_enemy[0]->getCount() - 1);
                    all_bots.erase(all_bots.begin() + i);
                    sounds.play(KILL);
                    count_bots--;
                }
            }
            if (this_player->getHp() <= 0) {
                if (kill_time == sf::Time::Zero) {
                    kill_time = main_timer.getElapsedTime();
                    sounds.play(WASTED_S);
                    stats.update(m_window, this_player->getHp(), 
                        main_timer.getElapsedTime().asSeconds() - pause_time.asSeconds());
                }
                m_cam.view.zoom(1.0001);
                if (main_timer.getElapsedTime().asSeconds() - 
                    kill_time.asSeconds() > 7) {
                        return STOP_RUN;
                }
                m_dead.setPosition(m_window.getView().getCenter().x - 90, m_window.getView().getCenter().y - 26);
                m_window.draw(m_dead);
            } else {
            stats.update(m_window, this_player->getHp(), 
                        main_timer.getElapsedTime().asSeconds() - pause_time.asSeconds());
            }
            stats.draw(m_window);
            m_window.display();
        }
    }
    return 0;
}