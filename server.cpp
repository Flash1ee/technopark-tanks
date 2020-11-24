#include <iostream>
#include <SFML/Network.hpp>

int main()
{
    std::cout << "Server Running" << std::endl;
    sf::TcpListener listener;
    sf::SocketSelector selector;
    bool done = false;
    std::vector<sf::TcpSocket*>clients;
    listener.getLocalPort();
    listener.listen(2000);
    selector.add(listener);

    while (!done)
    {
        if (selector.wait())
        {
            if (selector.isReady(listener))
            {
                sf::TcpSocket *socket = new sf::TcpSocket;

                if(listener.accept(*socket) == sf::Socket::Done)
                {
                    std::cout << "New connection accepted" << std::endl;
                }

                // sf::Packet packet;
                // std::string id;
                // std::string status;
                // if (socket->receive(packet) == sf::Socket::Done)
                //     packet >> id>>status;
                // std::cout << id << " has "<<status;
                clients.push_back(socket);
                selector.add(*socket);
            }
            else
            {
                for (int i = 0;i < clients.size();i++)
                {
                    if (selector.isReady(*clients[i]))
                    {
                        sf::Packet packet, sendpacket;
                        if (clients[i]->receive(packet) == sf::Socket::Done)
                        {
                            std::string text;
                            packet >> text;
                            std::cout << "Recieved data: " << text << std::endl;
                        }
                    }
                }
            }
        }
}

for (std::vector<sf::TcpSocket*>::iterator it = clients.begin();it != clients.end();it++)
    delete *it;
}