#pragma once
#include "client.h"
#include "game_map.hpp"


class GameSession
{
public:
    GameSession() = delete;
    explicit GameSession(std::string window_title, 
                         std::string& map_path, 
                         std::string& player_skin, 
                         bool is_multiplayer, 
                         std::string server_ip = "", 
                         int server_port = -1);
                         
    ~GameSession();

    void RunGame();

private:
    Map m_map;
    Client m_game_client;
    Cam m_cam;


    bool m_is_multiplayer;
    sf::RenderWindow m_window;
};