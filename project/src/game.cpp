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
    // std::string window_title = std::string("Tanks") + std::string(argv[1]);
    std::string window_title = std::string("Tanks");

    std::string player_skin(OBJECT_IMAGE);
    std::string map_skin(MAP_ONE);
    std::string server_ip = sf::IpAddress::getLocalAddress().toString();

    // GameSession game_session(window_title, map_skin, player_skin, true,
    // server_ip, PORT);
    sf::RenderWindow menuWindow(sf::VideoMode(1024, 760), window_title, sf::Style::None);
    Menu mainMenu(MenuSelector::MAIN, menuWindow);
    int result = mainMenu.show(menuWindow);
    std::cout << result << '\n';
    switch (result)
    {
        case SINGLE: {
            GameSession game_session(window_title, map_skin, player_skin, false);
            game_session.Run();
            break;
        }

        case MULTI: {
            GameSession game_session(window_title, map_skin, player_skin, true,
               server_ip, PORT);
            game_session.Run();
            break;
        }
    
        default: {
            throw std::exception();
            break;
        }
    }
    
    // GameSession game_session(window_title, map_skin, player_skin, false);

    // game_session.Run();

    return 0;
}
