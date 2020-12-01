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
            GameActionMessage game_status;
            packet >> game_status;

            if(game_status.msg_type == GameMessageType::GameBegin)
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
    
    Player this_player(playerTankImage, sf::Vector2f{200, 200}, 1, 2, 13, 13, 100, 0.1);
    std::vector<Player> players;
    std::vector<Bullet*> bullets;
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
                sf::Vector2f coords = {this_player.getX(), this_player.getY()};
                bullets.push_back(new Bullet(bulletImage, coords, 0, 0, 15, 15, 0.5, this_player.getDir()));
            }
        }

        for (auto& curr_bullet : bullets) {
            curr_bullet->move(time);
        }


        this_player.makeAction(time);

        m_cam.changeViewCoords(this_player.getX(), this_player.getY());
        m_cam.changeView();

        m_window.setView(m_cam.view);//"оживляем" камеру в окне sfml
        m_window.clear();

        m_map.drawMap(m_window);
        for (auto i : bullets) {
            m_window.draw(i->getSprite());
        }
        m_window.draw(this_player.getSprite());
        m_window.display();
    }
}