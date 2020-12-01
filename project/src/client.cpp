#include "client.h"
#include <exception>
#include <ctime>
#include <chrono>
#include <thread>
#include "client_server_config.h"
#include "messages.hpp"
#include <SFML/System/Vector2.hpp>


Client::Client(sf::IpAddress ip_address, int port) : m_ip_adress(ip_address), m_port(port)
{

}

Client::Client(std::string str_ip, int port)
{
    Client(sf::IpAddress(str_ip), port);
}

sf::Vector2f Client::connectToServer(std::string server_ip, int server_port)
{
    m_ip_adress = server_ip;
    m_port = server_port;

    m_socket = new sf::TcpSocket;
    PlayerActionMessage new_player_msg;


    if(m_socket->connect(m_ip_adress, m_port) == sf::Socket::Done)
    {
        std::cout << "connected!" << std::endl;
        m_socket->setBlocking(false);
        sf::Packet packet;

        while (m_socket->receive(packet) != sf::Socket::Done) {}
        if(packet.getDataSize() > 0)
        {
            packet >> new_player_msg;
            m_id = new_player_msg.player_id;

            std::cout << "My ID is " << m_id << std::endl;
        }
        else
        {
            std::cout << "Cant get my ID" << std::endl;
            throw std::runtime_error("Can't get user ID\n");
        }

    }
    else
    {
        std::cout << "can not connect!" << std::endl;
        throw std::runtime_error("Can't connect to server\n");
    }

    return new_player_msg.position;
}

Client::~Client()
{
    m_socket->disconnect();
    if(m_socket != nullptr)
    {
        delete m_socket;
    }
    std::cout << "Client disconnected" << std::endl;
}

void Client::RunClient()
{
    std::this_thread::sleep_for(std::chrono::seconds(5));
    

    while(true)
    {
        
    }
}


bool Client::SendToServer()
{

    sf::Packet packet;

    std::srand(std::time(nullptr)); // use current time as seed for random generator
    int random_variable = std::rand();

    packet << m_id << random_variable;

    if(m_socket->send(packet) == sf::Socket::Done)
    {
        std::cout << "Sent!" << std::endl;
    }

    else
    {
        std::cout << "Can't send!" << std::endl;
    }

    m_socket->setBlocking(false);

    return true;
}

bool Client::RecieveFromServer(sf::Packet& packet)
{
    while(m_socket->receive(packet) != sf::Socket::Done) {}
        
    return true;
}

// int main()
// {
    // sf::IpAddress ip = sf::IpAddress::getLocalAddress();
    // std::cout << "Current IP" << ip << std::endl;

    // int MyID;

    // sf::TcpSocket *socket = new sf::TcpSocket;
    // sf::SocketSelector selector;
    // sf::Packet packet;
    
    // if(socket->connect(ip, 2003) == sf::Socket::Done)
    // {
    //     std::cout << "connected!" << std::endl;
    //     socket->setBlocking(false);

    //     while (socket->receive(packet) != sf::Socket::Done) {}
    //     if(packet.getDataSize() > 0)
    //     {
    //         packet >> MyID;
    //         std::cout << "My ID is " << MyID << std::endl;
    //     }
    //     else
    //     {
    //         std::cout << "Cant get my ID" << std::endl;
    //         return -1;
    //     }

    //     selector.add(*socket);
        
    // }
    // else
    // {
    //     std::cout << "can not connect!" << std::endl;
    //     return -1;
    // }


    // std::cout << "Now you may send data to server" << std::endl;
    // while(true)
    // {
    //     //if(selector.isReady(*socket))
    //     //{
    //         if(socket->receive(packet) == sf::Socket::Done)
    //         {
    //             int other_ID;
    //             std::string text;
    //             packet >> other_ID >> text;
    //             std::cout << "Message from user with ID " << other_ID << ": " << text << std::endl;
    //         }
    //         // else
    //         // {
    //         //     std::cout << "Can't recieve message!" << std::endl;
    //         // }
    //     //}



    //     std::cout << "Enter text: ";
    //     std::string text;
    //     std::cin >> text;

    //     
    // }

//     Client client(sf::IpAddress::getLocalAddress(), 2000);

//     client.RunClient();

//     return 0;
// }