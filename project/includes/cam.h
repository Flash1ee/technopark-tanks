#include <SFML/Graphics.hpp>

class Cam {
public:
    sf::View view;//объявили sfml объект "вид", который и является камерой
	Cam();
    void changeViewCoords(float x, float y);
	// void moveMap(float time, float speed);
	void changeView();
};