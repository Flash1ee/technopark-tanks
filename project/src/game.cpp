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
        sf::RenderWindow menuWindow(sf::VideoMode(1024, 768), std::string("Menu"), sf::Style::None);
    // std::string window_title = std::string("Tanks") + std::string(argv[1]);
        // std::string window_title = std::string("Tanks");

        std::string player_skin(OBJECT_IMAGE);
        std::string map_skin(MAP_ONE);
        std::string server_ip = sf::IpAddress::getLocalAddress().toString();
        sf::RenderWindow window(sf::VideoMode(1024, 768), std::string("Tanks"), sf::Style::Fullscreen);

        // GameSession game_session(window_title, map_skin, player_skin, true,
        // server_ip, PORT);
        // sf::RenderWindow menuWindow(sf::VideoMode(1024, 768), std::string("Menu"), sf::Style::None);
        // std::cout << "GAY"<<'\n';
        Menu mainMenu(0, window);
        gamePlaying = mainMenu.menuSwitcher(window, map_skin, player_skin, server_ip);
        // std::cout << gamePlaying << '\n';
        // switch (result)
        // {
        //     case SINGLE: {
        //         GameSession game_session(window_title, map_skin, player_skin, false);
        //         game_session.Run();
        //         break;
        //     }

        //     case MULTI: {
        //         GameSession game_session(window_title, map_skin, player_skin, true,
        //            server_ip, PORT);
        //         game_session.Run();
        //         break;
        //     }

        //     default: {
        //         throw std::exception();
        //         break;
        //     }
        // }
    }
    // GameSession game_session(window_title, map_skin, player_skin, false);

    // game_session.Run();

    return 0;
}
