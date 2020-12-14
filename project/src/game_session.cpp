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

GameSession::GameSession(std::string window_title, std::string& map_path,
                         std::string& player_skin, bool is_multiplayer,
                         std::string server_ip, int server_port)
    :
      m_window(sf::VideoMode(1024, 760), window_title),
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
}

GameSession::~GameSession() {}

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

void GameSession::Run() {
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

    std::shared_ptr<Player> this_player = std::make_shared<Player>(
        m_level, OBJECT_IMAGE, sf::IntRect(1, 2, 13, 13), m_player_pos, 0.07,
        100, Direction::UP);

    std::vector<Bots*> all_bots;
    std::vector<MapObject> spawn;
    spawn.push_back(m_level.GetFirstObject("spawn1"));
    spawn.push_back(m_level.GetFirstObject("spawn2"));

    for (int i = 0; i < 2; i++) {
        size_t ind = 0;
        if (i % 2) {
            ind = 1;
        }
        sf::Vector2f m_bot_pos = {spawn[ind].rect.left + spawn[ind].rect.width / 2,
        spawn[ind].rect.top - spawn[ind].rect.width / 2 };

        // sf::Vector2f m_bot_pos = {static_cast<float>(50 * (i + 1)), static_cast<float>(50 * (i + 1))};
        all_bots.push_back(new Bots(m_level, OBJECT_IMAGE, sf::IntRect(128, 129, 13, 13), m_bot_pos, 0.07,
                                    100, Direction::UP));
    }

    sf::Vector2f old_pos = this_player->getPos();
    sf::Vector2f new_pos = old_pos;

    std::map<int, std::shared_ptr<Player>> players;

    std::vector<std::shared_ptr<Bullet>> new_bullets;
    std::vector<std::shared_ptr<Bullet>> all_bullets;
    sf::Clock clock, timer_bots;
    bool is_new_user = true;

    Sound sounds [static_cast<int>(SoundType::COUNT)] {
        Sound(BULLET_SOUND)
    };

    int stop = 0;
    while (m_window.isOpen()) {
        sf::Time times = timer_bots.getElapsedTime();

        float time =clock.getElapsedTime().asMicroseconds();  //дать прошедшее время в микросекундах
        clock.restart();  //перезагружает время
        time /= 800;      //скорость игры
        if (stop == 2) {
            timer_bots.restart();
        }
        std::cout << times.asSeconds() << std::endl;

        if ((times.asSeconds() > 3) && (stop == 0)) {
            for (int i = 0; i < 2; i++) {
                size_t ind = 0;
                if (i % 2) {
                    ind = 1;
                }
                sf::Vector2f m_bot_pos = {spawn[ind].rect.left + spawn[ind].rect.width / 2,
                                          spawn[ind].rect.top - spawn[ind].rect.width / 2};

                // sf::Vector2f m_bot_pos = {static_cast<float>(50 * (i + 1)), static_cast<float>(50 * (i + 1))};
                all_bots.push_back(new Bots(m_level, OBJECT_IMAGE, sf::IntRect(128, 129, 13, 13), m_bot_pos, 0.07,
                                            100, Direction::UP));
                stop += 1;
            }

        }

        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
                exit(0);
            }
            
            if (this_player->getShot()) {
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
                    OBJECT_IMAGE, BULLET_SOUND,sf::IntRect(323, 102, 4, 4), bullet_pos, 0.1,
                    bullet_dir, 1);

                // sf::Packet packet;
                // PlayerAction new_bullet_action = {-1, bullet_pos, bullet_dir,
                // PlayerActionType::NewBullet};
                // action_vector.actions.push_back(new_bullet_action);

                all_bullets.push_back(new_b);  // Copying is too expensive
                new_bullets.push_back(new_b);
                sounds[static_cast<int>(SoundType::BULLET)].play();
            }
        }

        for (auto &i : all_bots) {
            i->move(time, *this_player, all_bots);
        }

        this_player->makeAction(time);
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
                curr_bullet->move(time);
            }

            m_cam.changeViewCoords(new_pos);
            m_cam.changeView();

            m_window.setView(m_cam.view);  //"оживляем" камеру в окне sfml
            m_window.clear();

            m_level.Draw(m_window);

            for (int i = 0; i < all_bullets.size(); i++) {
                if (all_bullets[i]->getLife()) {
                    m_window.draw(all_bullets[i]->getSprite());
                } else {
                    all_bullets.erase(all_bullets.begin() + i);
                }
            }

            m_window.draw(this_player->getSprite());
            for (auto &i : all_bots) {
                m_window.draw(i->getSprite());
            }
            m_window.display();

        }
    }
}