#pragma once

class GameMap; 
class Server;
class Client;


class GameSession
{
public:
    explicit GameSession(){}
    void RunGame();
    

private:
    GameMap m_map;
    Server m_game_server;
    Client m_game_client;
} 