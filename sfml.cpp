#include <SFML/Graphics.hpp>

int main() {
	sf::RenderWindow Window(sf::VideoMode(1920, 1080), "Tanks");

    sf::Image tankimage;
    tankimage.loadFromFile("Tanks.png");
    tankimage.createMaskFromColor(sf::Color(255, 154, 38));

    sf::Texture tanktext;
    tanktext.loadFromImage(tankimage);

    sf::Sprite tanksp;
    tanksp.setTexture(tanktext);
    tanksp.setPosition(100,100);

	while (Window.isOpen()) {
		sf::Event event;
		while (Window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
                Window.close();
            }
		}

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            tanksp.move(-1,0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            tanksp.move(0,-1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            tanksp.move(1,0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            tanksp.move(0,1);
        }

        Window.clear();
        Window.draw(tanksp);
        Window.display();
	}
}