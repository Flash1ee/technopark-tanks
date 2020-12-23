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
                         sf::IpAddress server_ip, int server_port)
    :
      m_window(sf::VideoMode(1024, 760), window_title),
      m_is_multiplayer(is_multiplayer),
      m_game_client(server_ip, server_port)
{

    // if(m_is_multiplayer)
    // {
    //     m_game_client = Client(server_ip, server_port);
    // }

    m_level.LoadFromFile("../maps/map1.tmx");

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

    sf::Vector2f this_player_pos;

    if (m_is_multiplayer)
    {
        if(m_game_client.connectToServer())
        {
            sf::Packet packet;
            PlayerAction new_player_msg;
            
            while (m_game_client.m_socket->receive(packet) != sf::Socket::Done) { }

            //if(m_game_client.RecieveFromServer(packet))
            if(packet.getDataSize() > 0)
            {
                packet >> new_player_msg;
                m_user_id = new_player_msg.player_id;
                this_player_pos = new_player_msg.position;

                std::cout << "Client id on server = " << m_user_id << std::endl;
            }
            else
            {
                std::cout << "cannot get client id from server" << std::endl;
                exit(-1);
            }
        }

        WaitForOtherPlayers();

    }
    else
    {
        MapObject player_obj = m_level.GetFirstObject("player");
        sf::FloatRect p_pos = player_obj.rect;
        this_player_pos = {p_pos.left + p_pos.width / 2, p_pos.top - p_pos.width / 2};
    }

    std::shared_ptr<Player> this_player = std::make_shared<Player>(m_level,
                                                                   OBJECT_IMAGE,
                                                                   sf::IntRect(1, 2, 13, 13),
                                                                   this_player_pos, 0.07, 100,
                                                                   Direction::UP);

    std::vector<std::shared_ptr<Bot>> all_bots;

    for (int i = 0; i < m_bot_count; ++i) {
        sf::Vector2f m_bot_pos = {static_cast<float>(50 * (i + 1)), static_cast<float>(50 * (i + 1))};
        all_bots.push_back(std::make_shared<Bot>(m_level, OBJECT_IMAGE, sf::IntRect(128, 129, 13, 13), m_bot_pos, 0.07, 100, Direction::UP));
    }

    std::map<int, std::shared_ptr<Player>> other_players;

    std::vector<std::shared_ptr<Bullet>> new_bullets;
    std::vector<std::shared_ptr<Bullet>> all_bullets;

    sf::Clock clock;
    bool is_new_user = true;

    Sound sounds [static_cast<int>(SoundType::COUNT)] {
        Sound(BULLET_SOUND)
    };

    while (m_window.isOpen()) {
        float time = clock.getElapsedTime().asMicroseconds();  //дать прошедшее время в микросекундах
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
                auto bullet_pos = this_player->getPos();
                auto bullet_dir = this_player->getDir();
                auto new_b = std::make_shared<Bullet>(m_level,
                                                      OBJECT_IMAGE,
                                                      BULLET_SOUND,
                                                      sf::IntRect(321, 100, 8, 8),
                                                      bullet_pos,
                                                      0.5, bullet_dir, 1);

                sounds[static_cast<int>(SoundType::BULLET)].play();

                all_bullets.push_back(new_b);  // Copying is too expensive
                new_bullets.push_back(new_b);  // FIX IT
            }
        }

        for (auto &i : all_bots) {
            i->move(time, *this_player, all_bots); // move all bots
        }

        for (auto& curr_bullet : all_bullets) {
            curr_bullet->move(time); // move all bullets
        }

        this_player->makeAction(time); // move this player
        this_player->checkCollisionsBot(all_bots);

        if (m_is_multiplayer) {

            PlayerActionVector action_vector;
            sf::Vector2f curr_player_pos = this_player->getPos();
            Direction curr_player_dir = this_player->getDir();

            if(is_new_user)
            {
                PlayerAction create_new_player = { m_user_id, curr_player_pos, curr_player_dir, PlayerActionType::NewPlayer};
                action_vector.push(create_new_player);
                is_new_user = false;
                std::cout << "I notified server about me" << std::endl;
            }
            else
            {
                PlayerAction action = { m_user_id, curr_player_pos, curr_player_dir, PlayerActionType::UpdatePlayer};
                action_vector.push(action);
            }
            

            {  // Gathering info for sending to server
                

                if (new_bullets.size() > 0) {
                    PlayerAction bullet_action;
                    for(auto& curr_bullet : new_bullets)
                    {
                        auto bullet_pos = curr_bullet->getPos();
                        auto bullet_dir = curr_bullet->getDir();

                        bullet_action = { -1, bullet_pos, bullet_dir, PlayerActionType::NewBullet }; // bullets have no id
                        action_vector.push(bullet_action);
                    }

                    new_bullets.clear();
                }
            }

            if(action_vector.get_size() > 0)
            {
                sf::Packet packet_to_server;
                packet_to_server << action_vector;
                m_game_client.SendToServer(packet_to_server);
                //std::cout << "I send all actions. Amount is " << action_vector.get_size() << std::endl;
            }

            {  // getting info from server and applying it to current session
                PlayerActionVector others_actions;

                sf::Packet packet_from_server;

                m_game_client.RecieveFromServer(packet_from_server);

                packet_from_server >> others_actions;
                
                if(others_actions.get_size() > 0)
                {
                    //std::cout << "I recieved " << others_actions.get_size() << " actions" << std::endl;
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

                                auto new_palyer = std::make_shared<Player>(m_level,
                                                                           OBJECT_IMAGE,
                                                                           sf::IntRect(1, 2, 13, 13),
                                                                           pos, 0.07, 100, dir);

                                std::cout << "Other player added. Position is " << pos.x << " " << pos.y << std::endl;
                                
                                other_players.insert(std::make_pair(id, new_palyer));
                            }
                            break;

                            case PlayerActionType::UpdatePlayer:
                            {
                                //std::cout << "Other player should be updated" << std::endl;
                                int id = action.player_id;
                                if(id == m_user_id)
                                {
                                    std::cout << "PNX" << std::endl;
                                }
                                auto new_dir = action.direction;
                                auto new_pos = action.position;

                                auto player_iter = other_players.find(id);

                                if(player_iter == other_players.end())
                                {
                                    std::cout << "No player with id " << id << std::endl;
                                    exit(-1);
                                }
                                else
                                {
                                    other_players[id]->setDir(new_dir);
                                    other_players[id]->setPos(new_pos);
                                }

                                std::cout << "Other player new pos is " << new_pos.x << " " << new_pos.y << std::endl;

                            } break;

                            case PlayerActionType::NewBullet: {
                                std::cout << "Other player shooted" << std::endl;
                                sf::Vector2f pos = action.position;
                                Direction dir = action.direction;
                                std::shared_ptr<Bullet> new_b(new Bullet(m_level,
                                                                         OBJECT_IMAGE,
                                                                         BULLET_SOUND,
                                                                         sf::IntRect(321, 100, 8, 8),
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

                }
                else
                {
                    //std::cout << "Nothing to apply right now" << std::endl;
                }
            }
        }

        {  // Drawing is here

            m_cam.changeViewCoords(this_player->getPos());
            m_cam.changeView();

            m_window.setView(m_cam.view);  //"оживляем" камеру в окне sfml
            m_window.clear();

            m_level.Draw(m_window);

            for (int i = 0; i < all_bullets.size(); ++i) {
                if (all_bullets[i]->getLife()) {
                    m_window.draw(all_bullets[i]->getSprite());
                } else {
                    all_bullets.erase(all_bullets.begin() + i);
                }
            }

            m_window.draw(this_player->getSprite());

            for(auto& other_player : other_players)
            {
                m_window.draw(other_player.second->getSprite());
            }

            for (auto &i : all_bots) {
                m_window.draw(i->getSprite());
            }

            m_window.display();
        }
    }
}