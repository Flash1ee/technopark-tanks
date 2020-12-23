#include <SFML/System/Vector2.hpp>
#include <chrono>
#include <ctime>
#include <exception>
#include <messages.hpp>
#include <thread>

#include "client.h"
#include "client_server_config.h"

Client::Client(sf::IpAddress ip_address, int port)
    : m_ip_adress(ip_address), m_port(port) {}

Client::Client(std::string str_ip, int port) {
    Client(sf::IpAddress(str_ip), port);
}

bool Client::connectToServer() {
   m_socket = std::make_shared<sf::TcpSocket>();

    if (m_socket->connect(m_ip_adress, m_port) == sf::Socket::Done)
    {
        std::cout << "connected!" << std::endl;
        m_socket->setBlocking(false);
        // sf::Packet packet;

        // while (m_socket->receive(packet) != sf::Socket::Done) { }
        // if (packet.getDataSize() > 0)
        // {
        //     PlayerAction new_player_msg;
        //     packet >> new_player_msg;
        //     m_user_id = new_player_msg.player_id;

        //     std::cout << "My ID is " << m_id << std::endl;
        // }
        // else
        // {
        //     throw std::runtime_error("Can't get user ID\n");
        // }

    }
    else 
    {
        throw std::runtime_error("Can't connect to server\n");
    }

    return true;

}

Client::~Client() {
//    m_socket->disconnect();
//    if (m_socket != nullptr) {
//        delete m_socket;
//    }
//    std::cout << "Client disconnected" << std::endl;
}

void Client::RunClient() {
    std::this_thread::sleep_for(std::chrono::seconds(5));

    while (true) {
    }
}

bool Client::SendToServer(sf::Packet& packet) {
    sf::Socket::Status status = sf::Socket::Done;
    do
    {
        if(status == sf::Socket::Partial)
        {
            std::cout << "SendToServer: Partial" << std::endl;
        }
        status = m_socket->send(packet);

    } while (status == sf::Socket::Partial);

    if (status == sf::Socket::Done)
        return true;

    else
        return false;
}

bool Client::RecieveFromServer(sf::Packet& packet) {

    while (m_socket->receive(packet) != sf::Socket::Done) {}
}
