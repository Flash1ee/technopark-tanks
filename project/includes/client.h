#include<SFML/Network.hpp>
#include<iostream>
#include<string>
#include<vector>
#include <SFML/System/Vector2.hpp>

class Client
{
public:
    Client(){}
    Client(sf::IpAddress ip, int port);
    Client(std::string str_ip, int port);
    ~Client();

    void RunClient();
    bool SendToServer();
    bool RecieveFromServer(sf::Packet& packet);
    sf::Vector2f connectToServer(std::string server_ip, int server_port);

private:
    sf::TcpSocket *m_socket;
    sf::IpAddress m_ip_adress;
    int m_port;
    int m_id;

};