#include "game.h"
#include "cam.h"
#include "game_map.hpp"
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "game_session.hpp"
#include <messages.hpp>
#include <string>
#include <iostream>
#include <map>

GameSession::GameSession(std::string window_title, 
                         std::string& map_path,
                         std::string& player_skin, 
                         bool is_multiplayer, 
                         std::string server_ip, 
                         int server_port) : 
                            
                            m_window(sf::VideoMode(640, 480), window_title),
                            m_map(map_path, player_skin),
                            m_is_multiplayer(is_multiplayer)
{

    m_player_pos = {200, 200};

    if(m_is_multiplayer)
    {

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

GameSession::~GameSession()
{

}

void GameSession::WaitForOtherPlayers()
{
    //encapsulation error
    sf::Packet packet;

    while(true)
    {
        if(m_game_client.RecieveFromServer(packet))
        {
            GameActionType game_status;
            packet >> game_status;

            if(game_status == GameActionType::GameBegin)
            {
                std::cout << "ALL USERS ARE CONNECTED. GAME WILL START NOW!" << std::endl;
                break;
            }
        }
    }


}

void GameSession::Run()
{
    if(m_is_multiplayer)
    {

        WaitForOtherPlayers();
        //RunOnlineGame()
    }
    // else
    // {
    //     RunOfflineGame();
    // }
    
    Player this_player(playerTankImage, m_player_pos, 1, 2, 13, 13, 100, 0.1);
    
    sf::Vector2f old_pos = this_player.getPos();
    sf::Vector2f new_pos = old_pos;

    std::map<int, Player> players;

    std::vector<std::shared_ptr<Bullet>> new_bullets;
    std::vector<std::shared_ptr<Bullet>> all_bullets;
    sf::Clock clock;

    while (m_window.isOpen()) {
        float time = clock.getElapsedTime().asMicroseconds();  //дать прошедшее время в микросекундах
        clock.restart();    //перезагружает время
        time = time / 800;  //скорость игры

        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
            }

            if (this_player.getShot()) {
                this_player.setShot(false);
                //sf::Vector2f coords = this_player.getPos();
                auto new_b = std::make_shared<Bullet>(bulletImage, old_pos, 0, 0, 15, 15, 0.5, this_player.getDir());
                
                all_bullets.push_back(new_b); // Copying is too expensive
                new_bullets.push_back(new_b);
            }
        }

       
        this_player.makeAction(time);
        sf::Vector2f new_pos = this_player.getPos();
        
        {// Gathering info for sending to server
            PlayerActionVector action_vector;

            moveAction dir = static_cast<moveAction>(this_player.getDir());

            if(new_pos != old_pos)
            {
                sf::Packet packet;
                PlayerAction action = { m_game_client.m_id, new_pos, dir, PlayerActionType::UpdatePlayer};

                action_vector.actions.push_back(action);
                old_pos = new_pos;
            }

            if(new_bullets.size() > 0)
            {
                PlayerAction action;
                for(auto& curr_bullet : new_bullets)
                {
                    action = { -1, new_pos, dir, PlayerActionType::UpdatePlayer }; // bullets have no id
                    action_vector.actions.push_back(action);
                }

                new_bullets.clear();
            }

            sf::Packet packet;
            packet << action_vector;

            m_game_client.SendToServer(packet);
        }
        
        { // getting info from server and applying it to current session
            PlayerActionVector others_actions;
            sf::Packet packet;

            m_game_client.RecieveFromServer(packet);

            packet >> others_actions;


            for(auto& action : others_actions.actions)
            {
                switch (action.msg_type)
                {
                case PlayerActionType::NewPlayer:
                    {
                        int id = action.player_id;
                        moveAction dir = action.direction;
                        sf::Vector2f pos = action.position;
                        players.insert(std::make_pair(id, Player(playerTankImage, pos, 1, 2, 13, 13, 100, 0.1, dir)));
                    }
                    break;

                case PlayerActionType::UpdatePlayer:
                    {
                        int id = action.player_id;
                        sf::Vector2f new_pos = action.position;
                        players[id].SetPos(new_pos);
                    }
                    break;

                case PlayerActionType::NewBullet:
                    {
                        sf::Vector2f pos = action.position;
                        moveAction dir = action.direction;
                        std::shared_ptr<Bullet> new_b(new Bullet(bulletImage, pos, 0, 0, 15, 15, 0.5, dir));
                        all_bullets.push_back(new_b);
                    }
                    break;

                case PlayerActionType::UpdateBullet:
                    {
                        
                    }
                    break;

                case PlayerActionType::DeleteBullet:
                    {
                        
                    }
                    break;
                
                default:
                    break;
                }
            }
            
        }

        { // Drawing is here

            for (auto& curr_bullet : all_bullets) {
                curr_bullet->move(time);
            }

            m_cam.changeViewCoords(new_pos);
            m_cam.changeView();

            m_window.setView(m_cam.view);//"оживляем" камеру в окне sfml
            m_window.clear();

            m_map.drawMap(m_window);
            for (auto i : all_bullets) {
                m_window.draw(i->getSprite());
            }
            m_window.draw(this_player.getSprite());
            m_window.display();
        }
    }
}