#pragma once

#include <SFML/Network.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <variant>
#include <vector>

#include "objects.h"

enum PlayerActionType {
    NewPlayer = 0,
    UpdatePlayer,
    NewBullet,
    UpdateBullet,
    DeleteBullet
};

struct PlayerAction {
    int player_id;
    sf::Vector2f position{0, 0};
    Direction direction;
    int id;
    PlayerActionType msg_type;
};

sf::Packet& operator<<(sf::Packet& packet, const sf::Vector2f& position);
sf::Packet& operator>>(sf::Packet& packet, sf::Vector2f& position);

sf::Packet& operator<<(sf::Packet& packet, const PlayerActionType& msg_type);
sf::Packet& operator>>(sf::Packet& packet, PlayerActionType& msg_type);

sf::Packet& operator<<(sf::Packet& packet, const PlayerAction& message);
sf::Packet& operator>>(sf::Packet& packet, PlayerAction& message);

enum GameActionType { WaitingForPlayers = 0, GameBegin, GameEnd };

sf::Packet& operator<<(sf::Packet& packet, const GameActionType& msg_type);
sf::Packet& operator>>(sf::Packet& packet, GameActionType& msg_type);

class PlayerActionVector
{
public:

    PlayerActionVector()
    {
    }
    ~PlayerActionVector()
    {
        actions.clear();
    }

    void push(const PlayerAction& action)
    {
        actions.push_back(action);
    }

    PlayerAction pop()
    {
        auto action = actions.back();
        actions.pop_back();
        return action;
    }

    int get_size() const
    {
        return actions.size();
    }

    friend sf::Packet& operator << (sf::Packet& packet, const PlayerActionVector& msg_vec);
    friend sf::Packet& operator >> (sf::Packet& packet, PlayerActionVector& msg_vec);

private:
    std::vector<PlayerAction> actions;
};

