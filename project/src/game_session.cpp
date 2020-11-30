#include "game.h"
#include "cam.h"
#include "game_map.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>
#include "game_session.hpp"
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

    if(m_is_multiplayer)
    {

        m_game_client.connectToServer(server_ip, server_port);


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

void GameSession::RunGame()
{
    Player player(playerTankImage, 200, 200, 1, 2, 13, 13, 100, 0.1);
    std::vector<Bullet*> vectorBullet;
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
            if (player.getShot()) {
                player.setShot(false);
                vectorBullet.push_back(new Bullet(bulletImage, player.getX(), player.getY(), 0, 0, 15, 15, 0.5, player.getDir()));
            }
        }

        player.makeAction(time);
        for (auto i: vectorBullet) {
            i->move(time);
        }
        m_cam.changeViewCoords(player.getX(), player.getY());
        m_cam.changeView();

        m_window.setView(m_cam.view);//"оживляем" камеру в окне sfml
        m_window.clear();

        m_map.drawMap(m_window);
        for (auto i: vectorBullet) {
            m_window.draw(i->getSprite());
        }
        m_window.draw(player.getSprite());
        m_window.display();
    }
}