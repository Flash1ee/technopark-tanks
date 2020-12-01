#include <messages.hpp>
#include <iostream>

sf::Packet& operator << (sf::Packet& packet, const sf::Vector2f& position)
{
    return  packet << position.x << position.y;
}

sf::Packet& operator >> (sf::Packet& packet, sf::Vector2f& position)
{
    return packet >> position.x >> position.y;
}

sf::Packet& operator << (sf::Packet& packet, const PlayerMessageType& msg_type)
{
    return packet << msg_type;
}

sf::Packet& operator >> (sf::Packet& packet, PlayerMessageType& msg_type)
{
    return packet >> msg_type;
}

sf::Packet& operator << (sf::Packet& packet, const PlayerActionMessage& message)
{
    return packet << message.player_id << message.position << message.msg_type;
}

sf::Packet& operator >> (sf::Packet& packet, PlayerActionMessage& message)
{
    return packet >> message.player_id >>  message.position >> message.msg_type;
}


sf::Packet& operator << (sf::Packet& packet, const GameMessageType& msg_type)
{
    return packet << msg_type;
}

sf::Packet& operator >> (sf::Packet& packet, GameMessageType& msg_type)
{
    return packet >> msg_type;
}

sf::Packet& operator << (sf::Packet& packet, const GameActionMessage& message)
{
    return packet << message.msg_type;
}

sf::Packet& operator >> (sf::Packet& packet, GameActionMessage& message)
{
    return packet >> message.msg_type;
}