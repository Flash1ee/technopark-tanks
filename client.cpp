#include<SFML/Network.hpp>
#include<SFML/Graphics.hpp>
#include<iostream>
#include<string>
#include<vector>

int main()
{
    sf::IpAddress ip = sf::IpAddress::getLocalAddress();
    std::cout << "Current IP" << ip << std::endl;

    sf::TcpSocket socket;
    if(socket.connect(ip, 2000) == sf::Socket::Done)
    {
        std::cout << "connected!" << std::endl;
    }
    else
    {
        std::cout << "can not connect!" << std::endl;
        return -1;
    }


    std::cout << "Now you may send data to server" << std::endl;
    while(true)
    {
        std::cout << "Enter number id: ";
        std::string id;
        std::cin >> id;

        sf::Packet packet;
        packet << id;

        if(socket.send(packet) == sf::Socket::Done)
        {
            std::cout << "Sent!" << std::endl;
        }
        else
        {
            std::cout << "Can't send!" << std::endl;
        }
        
        socket.setBlocking(false);

    }
}