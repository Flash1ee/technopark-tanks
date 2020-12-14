#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

struct client_data;

class Server
{
public: 
    Server() = delete;
    Server(int port);
    ~Server();

    bool waitPlayersConnection();
    bool runGame();

private:
    bool add_new_client();
    bool recieve_from_client(sf::Packet& packet);
    bool send_to_all(const sf::Packet& packet, int exclude_id);
    bool send_to_client();
    int get_new_id() const;

private:
    sf::TcpListener m_listener;
    sf::SocketSelector m_selector;
    std::map<int, client_data> m_clients;

    static int constexpr m_players_count = 2;
};
