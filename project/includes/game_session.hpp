#pragma once

#include "map.h"
#include "cam.h"
#include "client.h"

class GameSession {

public:
    GameSession() = delete;
    explicit GameSession(std::string window_title, std::string& map_path,
                         std::string& player_skin, bool is_multiplayer,
                         std::string server_ip = "", int server_port = -1);

    ~GameSession();

    void WaitForOtherPlayers();
    void Run();
    // void RunGame();

   private:
    Level m_level;
    Client m_game_client;
    Cam m_cam;
    int m_user_id;

    bool m_is_multiplayer;
    sf::RenderWindow m_window;

private:
    static int constexpr m_bot_count = 4;
};