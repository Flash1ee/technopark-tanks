#pragma once

#include <SFML/Graphics.hpp>

class Cam {
public:
    sf::View view;//объявили sfml объект "вид", который и является камерой
	Cam();
    void changeViewCoords(const sf::Vector2f& coords);
	// void moveMap(float time, float speed);
	void changeView();
};