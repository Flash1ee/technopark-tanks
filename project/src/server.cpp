#include <SFML/Network.hpp>
#include <chrono>
#include <cstring>
#include <iostream>
#include <memory>
#include <messages.hpp>
#include <thread>

#include "client_server_config.h"
#include "server.h"
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


Server::Server(int port) {
    if (m_listener.listen(port) != sf::Socket::Done) {
        throw std::runtime_error(std::strerror(errno));
    }
    m_listener.setBlocking(false);

    std::cout << "Server Running" << std::endl;
}

Server::~Server() {
    m_listener.close();
    m_selector.clear();

    std::cout << "Server stop" << std::endl;
}

bool Server::add_new_client() {
    client_data curr_client;

    if (m_listener.accept(*curr_client.socket) == sf::Socket::Done) {
        std::cout << "Accepted\n";
        int id = this->get_new_id();
        curr_client.status = ClientStatus::Connected;

        std::cout << "New connection with id = " << id << "accepted" << std::endl;

        PlayerAction msg_to_player;

        msg_to_player.player_id = id;
        msg_to_player.position = sf::Vector2f {50.0 * id , 50.0 * id};
        msg_to_player.msg_type = PlayerActionType::NewPlayer;

        sf::Packet packet;
        packet << msg_to_player;

        if (curr_client.socket->send(packet) != sf::Socket::Done) {
            std::cout << "can't inform user about its id" << std::endl;
            return false;
        } else {
            std::cout << "ID sent to user" << std::endl;
        }
        
        curr_client.socket->setBlocking(false);
        m_selector.add(*curr_client.socket);
        m_clients.insert(std::make_pair(id, curr_client));
    }

    return true;
}

bool Server::send_to_all(const sf::Packet& packet, int exclude_id = -1)
{
    for(auto& curr_client : m_clients)
    {
        if(curr_client.first == exclude_id)
        {
            std::cout << "Dont send message to its owner" << std::endl;
            continue;
        }
        sf::Packet tmp_packet = packet;
        while(curr_client.second.socket->send(tmp_packet) == sf::Socket::Partial) {}
    }

    return true;
}

bool Server::recieve_from_client(sf::Packet& packet) { }

bool Server::runGame() {

    GameActionType msg = GameActionType::GameBegin;
    sf::Packet packet;
    packet << msg;

    send_to_all(packet);

    while (true)
    {
        if(m_selector.wait())
        {
            for(auto& curr_client : m_clients)
            {
                if (m_selector.isReady(*curr_client.second.socket))
                {
                    while (curr_client.second.socket->receive(packet) != sf::Socket::Done) {}

                    send_to_all(packet, curr_client.first);
                    std::cout << "SEND ACTION TO ALL PLAYERS" << std::endl;

                }
            }
        }
        
    }
}

bool Server::waitPlayersConnection() {
    while (true) {

        //std::this_thread::sleep_for(std::chrono::seconds(3));
        //std::cout << "waiting players..." << std::endl;

        add_new_client();

        if (m_clients.size() == m_players_count) {
            std::cout << "Ready to start game" << std::endl;
            return true;
        }
    }

}

int Server::get_new_id() const
{
    int max_id;
    if(m_clients.size() == 0)
    {
        max_id = 0;
    }
    else
    {
        max_id = m_clients.rbegin()->first;
    }
    
    return max_id + 1;
}

int main() {
    std::unique_ptr<Server> server_ptr(new Server(PORT));

    server_ptr->waitPlayersConnection();
    server_ptr->runGame();

    return 0;
}
