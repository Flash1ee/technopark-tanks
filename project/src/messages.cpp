#include <iostream>

#include "messages.hpp"


sf::Packet& operator<<(sf::Packet& packet, const sf::Vector2f& position) {
    return packet << position.x << position.y;
}

sf::Packet& operator>>(sf::Packet& packet, sf::Vector2f& position) {
    return packet >> position.x >> position.y;
}

sf::Packet& operator<<(sf::Packet& packet, const PlayerActionType& msg_type) {
    return packet << msg_type;
}

sf::Packet& operator>>(sf::Packet& packet, PlayerActionType& msg_type) {
    return packet >> msg_type;
}

sf::Packet& operator<<(sf::Packet& packet, const PlayerAction& message) {
    int int_type = message.msg_type;
    return packet << message.player_id << message.position << int_type;
}

sf::Packet& operator>>(sf::Packet& packet, PlayerAction& message) {
    int int_type;
    packet >> message.player_id >> message.position;
    packet >> int_type;
    message.msg_type = static_cast<PlayerActionType>(int_type);
    return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const GameActionType& msg_type) {
    int int_type = msg_type;
    return packet << int_type;
}

sf::Packet& operator>>(sf::Packet& packet, GameActionType& msg_type) {
    int int_type;
    packet >> int_type;
    msg_type = static_cast<GameActionType>(int_type);

    return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const PlayerActionVector& msg_vec) {
    packet << msg_vec.size;

    for (auto& msg : msg_vec.actions) {
        packet << msg;
    }

    return packet;
}

sf::Packet& operator>>(sf::Packet& packet, PlayerActionVector& msg_vec) {
    int size = 0;
    packet >> size;

    msg_vec.actions.resize(size);
    for (int i = 0; i < size; ++i) {
        packet >> msg_vec.actions[i];
    }

    return packet;
}