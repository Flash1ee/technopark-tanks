#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

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
    int id;
    ClientStatus status;
};

class Server
{
public: 
    Server() = delete;
    Server(int port);
    ~Server();

    bool addNewClient();
    bool recieveFromClient(sf::Packet& packet, int& client_id);
    bool sendToClient();

    template <class T>
    bool sendToAll(T& msg);

    bool waitPlayersConnection();
    bool runGame();

private:
    sf::TcpListener m_listener;
    sf::SocketSelector m_selector;
    std::vector<client_data> m_clients;
    //TODO pack TcpSocket* into shared_ptr
    //TODO change vector to map and add more smart IDs
};


template <> bool Server::sendToAll(PlayerActionMessage& msg);
template <> bool Server::sendToAll(GameActionMessage& msg);
