#pragma once

#include <SFML/Network.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <vector>
#include <variant>

enum PlayerActionType
{
    NewPlayer = 0,
    UpdatePlayer,
    NewBullet,
    UpdateBullet,
    DeleteBullet
};

struct PlayerAction
{
    int player_id;
    sf::Vector2f position {0, 0};
    moveAction direction;
    PlayerActionType msg_type;
};

sf::Packet& operator << (sf::Packet& packet, const sf::Vector2f& position);
sf::Packet& operator >> (sf::Packet& packet, sf::Vector2f& position);

sf::Packet& operator << (sf::Packet& packet, const PlayerActionType& msg_type);
sf::Packet& operator >> (sf::Packet& packet, PlayerActionType& msg_type);

sf::Packet& operator << (sf::Packet& packet, const PlayerAction& message);
sf::Packet& operator >> (sf::Packet& packet, PlayerAction& message);


enum GameActionType
{
    WaitingForPlayers = 0,
    GameBegin,
    GameEnd
};


sf::Packet& operator << (sf::Packet& packet, const GameActionType& msg_type);
sf::Packet& operator >> (sf::Packet& packet, GameActionType& msg_type);

struct PlayerActionVector
{
    int size;
    std::vector<PlayerAction> actions;
};

sf::Packet& operator << (sf::Packet& packet, const PlayerActionVector& msg_vec);
sf::Packet& operator >> (sf::Packet& packet, PlayerActionVector& msg_vec);