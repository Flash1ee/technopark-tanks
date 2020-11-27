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
    listener.listen(2003);
    selector.add(listener);

    while (!done)
    {
        if (selector.wait())
        {
            if (selector.isReady(listener))
            {
                sf::TcpSocket *socket = new sf::TcpSocket;

                int id;

                if(listener.accept(*socket) == sf::Socket::Done)
                {
                    id = clients.size();
                    std::cout << "New connection with id = " << id << "accepted" << std::endl;
                    
                    sf::Packet packet;
                    packet << id;
                    if(socket->send(packet) != sf::Socket::Done)
                    {
                        std::cout << "can't inform user about its id" << std::endl;
                    }
                    clients.push_back(socket);
                    selector.add(*socket);
                }
                else
                {
                        std::cout << "can't accept connection" << std::endl;
                }
                

                // sf::Packet packet;
                // std::string id;
                // std::string status;
                // if (socket->receive(packet) == sf::Socket::Done)
                //     packet >> id>>status;
                // std::cout << id << " has "<<status;

            }
            else
            {
                for (int i = 0; i < clients.size(); ++i)
                {
                    if (selector.isReady(*clients[i]))
                    {
                        sf::Packet packet, sendpacket;
                        if (clients[i]->receive(packet) == sf::Socket::Done)
                        {
                            std::string text;
                            int id;
                            packet >> id >> text;
                            std::cout << "Recieved data from id = " << id << ". Text = " << text << std::endl;

                            for(int j = 0; j < clients.size(); ++j)
                            {
                                if(i != j)
                                {
                                    if(clients[j]->send(packet) != sf::Socket::Done)
                                    {
                                        std::cout << "Error while sending data to all users" << std::endl;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
}

for (std::vector<sf::TcpSocket*>::iterator it = clients.begin();it != clients.end();it++)
    delete *it;
}