#include<SFML/Network.hpp>
#include<SFML/Graphics.hpp>
#include<iostream>
#include<string>
#include<vector>

class Client
{
public:
    Client(){}
    Client(sf::IpAddress ip, int port);
    Client(std::string str_ip, int port);
    ~Client();

    void RunClient();
    bool SendToServer();
    bool RecieveFromServer();

private:
    sf::TcpSocket *m_socket;
    sf::IpAddress m_ip_adress;
    int m_port;
    int m_id;

};