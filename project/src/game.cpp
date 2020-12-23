#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "cam.h"
#include "menu.h"
#include "client_server_config.h"
#include "game_session.hpp"
#include "game.h"
#include "map.h"

int main(int argc, char* argv[]) {
    int gamePlaying = 1;
    while (gamePlaying) {
    // std::string window_title = std::string("Tanks") + std::string(argv[1]);
        // std::string window_title = std::string("Tanks");

        std::string player_skin(OBJECT_IMAGE);
        std::string map_skin(MAP_ONE);
        // std::string server_ip = sf::IpAddress::getLocalAddress().toString();
        std::string server_ip = "25.63.213.59";
        sf::RenderWindow window(sf::VideoMode(1920, 1080), std::string("Tanks"));

        // GameSession game_session(window_title, map_skin, player_skin, true,
        // server_ip, PORT);
        Menu mainMenu(0, window);
        gamePlaying = mainMenu.menuSwitcher(window, map_skin, player_skin, server_ip);
    }

    return 0;
}
