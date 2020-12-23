#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "cam.h"
#include "client_server_config.h"
#include "game_session.hpp"
#include "game.h"
#include "map.h"

int main(int argc, char* argv[]) {
    // std::string window_title = std::string("Tanks") + std::string(argv[1]);
    std::string window_title = std::string("Tanks");

    std::string player_skin(OBJECT_IMAGE);
    std::string map_skin(MAP_ONE);
    //sf::IpAddress server_ip = "25.63.213.59";
    sf::IpAddress server_ip = sf::IpAddress::getLocalAddress().toString();

    GameSession game_session(window_title, map_skin, player_skin, true, server_ip, PORT);
    //GameSession game_session(window_title, map_skin, player_skin, false);

    game_session.Run();

    return 0;
}
