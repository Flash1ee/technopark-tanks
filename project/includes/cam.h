#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Cam {
public:
    sf::View view;//объявили sfml объект "вид", который и является камерой
	Cam();
    void changeViewCoords(const sf::Vector2f& coords, double map_width, double map_height);
	// void moveMap(float time, float speed);
	void changeView();
};