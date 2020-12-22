#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "messages.hpp"


enum class ClientStatus
{
    Connected = 0,
    ReadyToPlay,
    InGame,
    Disconnected,
};

struct client_data
{
public:

    client_data()
    {
        socket = std::make_shared<sf::TcpSocket>();
    }
    ~client_data() {}

    std::shared_ptr<sf::TcpSocket> socket;
    ClientStatus status;
};

class Server
{
public: 
    Server() = delete;
    Server(int port);
    ~Server();

    bool addNewClient();
    bool recieveFromClient(sf::Packet& packet);
    bool sendToClient();

    bool sendToAll(sf::Packet& packet, int exclude_id);

    bool waitPlayersConnection();
    bool runGame();

private:
    sf::TcpListener m_listener;
    sf::SocketSelector m_selector;
    std::map<int, client_data> m_clients;
    //TODO pack TcpSocket* into shared_ptr
    //TODO change vector to map and add more smart IDs
};


// template <PlayerAction> bool Server::sendToAll(PlayerAction& msg);
// template <GameActionMessage> bool Server::sendToAll(GameActionMessage& msg);
