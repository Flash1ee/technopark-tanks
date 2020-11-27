#include<SFML/Network.hpp>
#include<SFML/Graphics.hpp>
#include<iostream>
#include<string>
#include<vector>

int main()
{
    sf::IpAddress ip = sf::IpAddress::getLocalAddress();
    std::cout << "Current IP" << ip << std::endl;

    int MyID;

    sf::TcpSocket *socket = new sf::TcpSocket;
    sf::SocketSelector selector;
    sf::Packet packet;
    
    if(socket->connect(ip, 2003) == sf::Socket::Done)
    {
        std::cout << "connected!" << std::endl;
        socket->setBlocking(false);

        while (socket->receive(packet) != sf::Socket::Done) {}
        if(packet.getDataSize() > 0)
        {
            packet >> MyID;
            std::cout << "My ID is " << MyID << std::endl;
        }
        else
        {
            std::cout << "Cant get my ID" << std::endl;
            return -1;
        }

        selector.add(*socket);
        
    }
    else
    {
        std::cout << "can not connect!" << std::endl;
        return -1;
    }


    std::cout << "Now you may send data to server" << std::endl;
    while(true)
    {
        //if(selector.isReady(*socket))
        //{
            if(socket->receive(packet) == sf::Socket::Done)
            {
                int other_ID;
                std::string text;
                packet >> other_ID >> text;
                std::cout << "Message from user with ID " << other_ID << ": " << text << std::endl;
            }
            // else
            // {
            //     std::cout << "Can't recieve message!" << std::endl;
            // }
        //}



        std::cout << "Enter text: ";
        std::string text;
        std::cin >> text;

        sf::Packet packet;
        packet << MyID << text;

        if(socket->send(packet) == sf::Socket::Done)
        {
            std::cout << "Sent!" << std::endl;
        }
        else
        {
            std::cout << "Can't send!" << std::endl;
        }

        socket->setBlocking(false);

    }
}