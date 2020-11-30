#include <iostream>
#include <SFML/Network.hpp>
#include <memory>
#include "client_server_config.h"
#include "server.h"
#include <chrono>
#include <thread>


Server::Server(int port)
{
    std::cout << "Server Running" << std::endl;
    m_listener.getLocalPort();
    m_listener.listen(port);
    m_selector.add(m_listener);
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

    if(m_listener.accept(*curr_client.socket) == sf::Socket::Done)
    {
        curr_client.id = m_clients.size();
        curr_client.status = ClientStatus::Connected;

        std::cout << "New connection with id = " << curr_client.id << "accepted" << std::endl;
        
        sf::Packet packet;
        packet << curr_client.id;
        if(curr_client.socket->send(packet) != sf::Socket::Done)
        {
            std::cout << "can't inform user about its id" << std::endl;
            return false;
        }
        
        m_clients.push_back(std::move(curr_client));
        m_selector.add(*m_clients.back().socket);
    }
    else
    {
        std::cout << "can't accept connection" << std::endl;
        return false;
    }

}

bool Server::sendToAll(sf::Packet& packet, const int exclude_id)
{
    for(auto& curr_client : m_clients)
    {
        if(exclude_id != curr_client.id)
        {
            if(curr_client.socket->send(packet) != sf::Socket::Done)
            {
                std::cout << "Error while sending data to all users" << std::endl;
                return false;
            }
        }
    }

    return true;
}

bool Server::recieveFromClient(sf::Packet& packet, int& client_id)
{
    for(auto& curr_client : m_clients)
    {
        if (m_selector.isReady(*curr_client.socket))
        {
            if (curr_client.socket->receive(packet) == sf::Socket::Done)
            {
                std::string text;
                packet >> client_id >> text;
                std::cout << "Recieved data from id = " << client_id << ". Text = " << text << std::endl;
                
            }
        }
    }

    return true;
}


bool Server::runGame()
{
    sf::Packet packet;

    while (true)
    {
        for(auto& curr_client : m_clients)
        {
            if (m_selector.isReady(*curr_client.socket))
            {
                if (curr_client.socket->receive(packet) == sf::Socket::Done)
                {
                    sendToAll(packet, curr_client.id);
                }
                else
                {
                    std::cout << "Cant recieve data from client" << std::endl;
                }
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
        
        if (m_selector.wait())
        {
            if (m_selector.isReady(m_listener))
            {
                addNewClient();
            }
            // else // Server will ignore messages from clients on this step.
            // {
            //     recieveFromClient();
            // }
        }

        if(m_clients.size() == 2)
        {
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