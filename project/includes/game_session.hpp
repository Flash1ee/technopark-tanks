#pragma once

#include "map.h"
#include "cam.h"
#include "client.h"

class GameSession {
   public:
    int m_user_id;
    GameSession() = delete;
    explicit GameSession(std::string window_title, std::string& map_path,
                         std::string& player_skin, bool is_multiplayer,
                         std::string server_ip = "", int server_port = -1);

    ~GameSession();

    void WaitForOtherPlayers();
    int Run(sf::IntRect pl_rect);
    int single_play(GameSession &game, sf::IntRect pl_rect);
    int multi_play(GameSession &game, sf::IntRect pl_rect);

    // void RunGame();

   private:
    Level m_level;
    Client m_game_client;
    Cam m_cam;
    sf::Vector2f m_player_pos;
    sf::Music finish;
    bool m_is_multiplayer;
    sf::RenderWindow m_window;
};