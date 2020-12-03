#include "game.h"
#include "cam.h"
#include "game_map.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>
#include "game_session.hpp"
#include "client_server_config.h"
#include <string>

int main(int argc, char* argv[]) 
{
    std::string window_title = std::string("Tanks");
    std::string player_skin(playerTankImage);
    std::string map_skin(map_one);
    std::string server_ip = sf::IpAddress::getLocalAddress().toString();

    GameSession game_session(window_title, map_skin, player_skin, true, server_ip, PORT);
    //GameSession game_session(window_title, map_skin, player_skin, false);

    
    
    game_session.Run();

    return 0;
}
