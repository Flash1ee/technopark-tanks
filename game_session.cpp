#include "game_session.hpp"

GameSession::GameSession()
{

}

void GameSession::RunGame()
{
    while(true)
    {
        make_actions();
        if(m_is_multiplayer)
        {
            send_data_to_server();
            recieve_data_from_server();
        }
        apply_changes();
    }
}