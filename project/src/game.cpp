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
        std::string player_skin(OBJECT_IMAGE);
        std::string server_ip = sf::IpAddress::getLocalAddress().toString();
        sf::RenderWindow window(sf::VideoMode(1920, 1080), std::string("Tanks"), sf::Style::Fullscreen);
    
        Menu mainMenu(0, window);
        gamePlaying = mainMenu.menuSwitcher(window, player_skin, server_ip);
    }

    return 0;
}
