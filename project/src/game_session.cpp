#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <map>
#include <messages.hpp>
#include <string>

#include "cam.h"
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
    MapObject player = m_level.GetFirstObject("player");
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
    sf::Vector2f m_bot_pos = {m_player_pos.x + 13, m_player_pos.y + 10};
    Bots bots(m_level, OBJECT_IMAGE, sf::IntRect(128, 129, 13, 13), m_bot_pos, 0.07,
              100, Direction::UP);

    sf::Vector2f old_pos = this_player->getPos();
    sf::Vector2f new_pos = old_pos;

    std::map<int, std::shared_ptr<Player>> players;

    std::vector<std::shared_ptr<Bullet>> new_bullets;
    std::vector<std::shared_ptr<Bullet>> all_bullets;
    sf::Clock clock;
    bool is_new_user = true;

    while (m_window.isOpen()) {
        float time =
            clock.getElapsedTime()
                .asMicroseconds();  //дать прошедшее время в микросекундах
        clock.restart();  //перезагружает время
        time /= 800;      //скорость игры

        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
                exit(0);
            }

            if (this_player->getShot()) {
                this_player->setShot(false);
                // sf::Vector2f coords = this_player.getPos();
                auto bullet_pos = this_player->getPos();
                auto bullet_dir = this_player->getDir();
                auto new_b = std::make_shared<Bullet>(
                    OBJECT_IMAGE, BULLET_SOUND,sf::IntRect(321, 100, 8, 8), bullet_pos, 0.5,
                    bullet_dir);

                // sf::Packet packet;
                // PlayerAction new_bullet_action = {-1, bullet_pos, bullet_dir,
                // PlayerActionType::NewBullet};
                // action_vector.actions.push_back(new_bullet_action);

                all_bullets.push_back(new_b);  // Copying is too expensive
                new_bullets.push_back(new_b);
                new_b->sound();

            }
        }

        bots.move(time, *this_player);
        this_player->makeAction(time);
        this_player->checkCollisionsBots(bots);
        old_pos = new_pos;
        new_pos = this_player->getPos();

        if (m_is_multiplayer) {
            PlayerActionVector action_vector;

            if(is_new_user)
            {
                PlayerAction create_new_player = { m_game_client.m_id, new_pos, this_player->getDir(), PlayerActionType::NewPlayer};
                action_vector.push(create_new_player);
                is_new_user = false;
                std::cout << "I notified server about me" << std::endl;
            }

            {  // Gathering info for sending to server

                Direction dir = this_player->getDir();

                if(new_pos != old_pos)
                {
                    PlayerAction action = { m_game_client.m_id, new_pos, dir, PlayerActionType::UpdatePlayer};

                    action_vector.push(action);
                    old_pos = new_pos;
                }

                if (new_bullets.size() > 0) {
                    PlayerAction action;
                    for(auto& curr_bullet : new_bullets)
                    {
                        action = { -1, new_pos, dir, PlayerActionType::NewBullet }; // bullets have no id
                        action_vector.push(action);
                    }

                    new_bullets.clear();
                }
            }

            if(action_vector.get_size() > 0)
            {
                sf::Packet packet_to_server;
                packet_to_server << action_vector;
                m_game_client.SendToServer(packet_to_server);
                std::cout << "I send all actions. Amount is " << action_vector.get_size() << std::endl;

            }
            // else 
            //     std::cout << "Nothing to send right now" << std::endl;

            {  // getting info from server and applying it to current session
                PlayerActionVector others_actions;

                sf::Packet packet_from_server;

                m_game_client.RecieveFromServer(packet_from_server);

                packet_from_server >> others_actions;
                
                if(others_actions.get_size() > 0)
                {
                    std::cout << "I recieved " << others_actions.get_size() << " actions" << std::endl;
                    while(others_actions.get_size() > 0)
                    {
                        PlayerAction action = others_actions.pop();
                        
                        switch (action.msg_type)
                        {
                        case PlayerActionType::NewPlayer:
                            {
                                std::cout << "New player connected to game" << std::endl;
                                int id = action.player_id;
                                Direction dir = action.direction;
                                sf::Vector2f pos = action.position;
                                auto new_palyer = std::make_shared<Player>(m_level, OBJECT_IMAGE, sf::IntRect(1, 2, 13, 13), pos, 0.1, 100, dir);
                                players.insert(std::make_pair(id, std::move(new_palyer)));
                            }
                            break;

                        case PlayerActionType::UpdatePlayer:
                            {
                                std::cout << "Other player should be updated" << std::endl;
                                int id = action.player_id;
                                sf::Vector2f new_pos = action.position;
                                players[id]->setPos(new_pos);
                            } break;

                            case PlayerActionType::NewBullet: {
                                std::cout << "Other player shooted"
                                          << std::endl;
                                sf::Vector2f pos = action.position;
                                Direction dir = action.direction;
                                std::shared_ptr<Bullet> new_b(new Bullet(
                                    OBJECT_IMAGE, BULLET_SOUND, sf::IntRect(321, 100, 8, 8),
                                    pos, 0.5, dir));
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

                }
                else
                {
                    //std::cout << "Nothing to apply right now" << std::endl;
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
            for (auto i : all_bullets) {
                m_window.draw(i->getSprite());
            }
            m_window.draw(this_player->getSprite());
            m_window.draw(bots.getSprite());
            m_window.display();
        }
    }
}