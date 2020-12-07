#pragma once

#include <SFML/Network.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <string>
#include <vector>

class Client {
   public:
    Client() {}
    Client(sf::IpAddress ip, int port);
    Client(std::string str_ip, int port);
    ~Client();

    void RunClient();
    bool SendToServer(sf::Packet& packet);
    bool RecieveFromServer(sf::Packet& packet);
    sf::Vector2f connectToServer(std::string server_ip, int server_port);

    int m_id;

   private:
    sf::TcpSocket* m_socket;
    sf::IpAddress m_ip_adress;
    int m_port;
};