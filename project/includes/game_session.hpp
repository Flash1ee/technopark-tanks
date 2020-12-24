#pragma once

#include "map.h"
#include "cam.h"
#include "client.h"

class GameSession {
   public:
    GameSession() = delete;
    explicit GameSession(std::string window_title, std::string& map_path,
                         std::string& player_skin, bool is_multiplayer);

    ~GameSession();

    void WaitForOtherPlayers();
    int Run();
    // void RunGame();

   private:
    Level m_level;
    Client m_game_client;
    Cam m_cam;
    int m_user_id;
    sf::Texture dead;
    sf::Sprite m_dead;
    sf::Texture win;
    sf::Sprite m_win;
    sf::Text bots_left;
    sf::Text bots_base_hp;
    sf::Text player_base_hp;
    sf::Text m_left_bots;
    sf::Font font;  
    sf::Music finish;
    bool m_is_multiplayer;
    sf::RenderWindow m_window;
};