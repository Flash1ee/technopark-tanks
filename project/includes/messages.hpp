#pragma once

#include <SFML/Network.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <vector>
#include <variant>

enum class PlayerMessageType
{
    NewPlayer = 0,
    UpdatePlayer
};

struct PlayerActionMessage
{
    int player_id;
    sf::Vector2f position;
    PlayerMessageType msg_type;
};

sf::Packet& operator << (sf::Packet& packet, const PlayerMessageType& msg_type);
sf::Packet& operator >> (sf::Packet& packet, PlayerMessageType& msg_type);

sf::Packet& operator << (sf::Packet& packet, const sf::Vector2f& position);
sf::Packet& operator << (sf::Packet& packet, const sf::Vector2f& position);

sf::Packet& operator << (sf::Packet& packet, const PlayerActionMessage& message);
sf::Packet& operator >> (sf::Packet& packet, PlayerActionMessage& message);


enum class GameMessageType
{
    WaitingForPlayers = 0,
    GameBegin,
    GameEnd
};

struct GameActionMessage
{
    GameMessageType msg_type;
};

sf::Packet& operator << (sf::Packet& packet, const GameMessageType& msg_type);
sf::Packet& operator >> (sf::Packet& packet, GameMessageType& msg_type);

sf::Packet& operator << (sf::Packet& packet, const GameActionMessage& message);
sf::Packet& operator >> (sf::Packet& packet, GameActionMessage& message);