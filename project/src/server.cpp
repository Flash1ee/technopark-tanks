#include <iostream>
#include <SFML/Network.hpp>
#include <memory>
#include "client_server_config.h"
#include "server.h"
#include <chrono>
#include <thread>
#include <cstring>
#include <messages.hpp>

Server::Server(int port)
{
    std::cout << "Server Running" << std::endl;
    //m_listener.getLocalPort();
    if(m_listener.listen(port) != sf::Socket::Done)
    {
        throw std::runtime_error(std::strerror(errno));
    }
    m_listener.setBlocking(false);
    //m_selector.add(m_listener);
}

Server::~Server()
{
    m_listener.close();
    m_selector.clear();
    std::cout << "Server stop" << std::endl;
}


bool Server::addNewClient()
{
    client_data curr_client;
    //curr_client.socket->setBlocking(false);

    if(m_listener.accept(*curr_client.socket) == sf::Socket::Done)
    {
        std::cout << "Accepted\n";
        int id = m_clients.size();
        curr_client.status = ClientStatus::Connected;

        std::cout << "New connection with id = " << id << "accepted" << std::endl;
        
        PlayerAction msg_to_player;//= {curr_client.id, sf::Vector2f{100.0 * (curr_client.id + 1), (100.0 * curr_client.id + 1)}, PlayerActionType::NewPlayer};
        
        msg_to_player.player_id = id;
        msg_to_player.position = sf::Vector2f{100.0 * (id + 1), (100.0 * (id + 1))};
        msg_to_player.msg_type = PlayerActionType::NewPlayer;

        sf::Packet packet;
        std::cout <<"JOPA" << std::endl;
        packet << msg_to_player;
        std::cout <<"JOPA" << std::endl;


        if(curr_client.socket->send(packet) != sf::Socket::Done)
        {
            std::cout << "can't inform user about its id" << std::endl;
            return false;
        }
        else
        {
            std::cout << "ID sent to user" << std::endl; 
        }
        
        m_selector.add(*curr_client.socket);
        m_clients.insert(std::make_pair(id, curr_client));

    }

    return true;
}

bool Server::sendToAll(sf::Packet& packet, int exclude_id = -1)
{
    for(auto& curr_client : m_clients)
    {
        if(curr_client.first == exclude_id)
            continue;
            
        if(curr_client.second.socket->send(packet) != sf::Socket::Done)
        {
            std::cout << "Error while sending data to all users" << std::endl;
            return false;
        }
    }

    return true;
}

bool Server::recieveFromClient(sf::Packet& packet)
{
    for(auto& curr_client : m_clients)
    {
        if (m_selector.isReady(*curr_client.second.socket))
        {
            if (curr_client.second.socket->receive(packet) == sf::Socket::Done)
            {
                std::string text;
                packet >> text;
                std::cout << "Recieved data. Text = " << text << std::endl;
                
            }
        }
    }

    return true;
}


bool Server::runGame()
{
    GameActionType msg = GameActionType::GameBegin;
    sf::Packet packet;
    packet << msg;

    sendToAll(packet);

    while (true)
    {
        for(auto& curr_client : m_clients)
        {
            if (m_selector.isReady(*curr_client.second.socket))
            {
                while (curr_client.second.socket->receive(packet) == sf::Socket::Done) {}
                
                sendToAll(packet, curr_client.first);
                
            }
        }
    }
}

bool Server::waitPlayersConnection()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "waiting players..." << std::endl;

        addNewClient();
        
        //if (m_selector.wait())
        //{
        //     if (m_selector.isReady(m_listener))
        //     {
        //         std::cout << "ready to accept connection" << std::endl;
        //         addNewClient();
        //     }
        //     else // Server will ignore messages from clients on this step.
        //     {
        //         std::cout << "ready to get data" << std::endl;
        //         sf::Packet packet;
        //         int id;
        //         recieveFromClient(packet, id);
        //     }
        // }
        
        if(m_clients.size() == 2)
        {
            std::cout << "Ready to start game" << std::endl;
            break;
        }
    }

    return true;
}


std::unique_ptr<Server> server_ptr;

int main()
{
    server_ptr.reset(new Server(PORT));

    server_ptr->waitPlayersConnection();
    server_ptr->runGame();

    return 0;
}