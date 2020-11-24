#include <SFML/Graphics.hpp>

int main() {
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Tanks");

    sf::Image tankImage;
    tankImage.loadFromFile("../pltank.bmp");
    tankImage.createMaskFromColor(sf::Color(255, 255, 255));

    sf::Texture tankTexture;
    tankTexture.loadFromImage(tankImage);

    sf::Sprite tankSprite;
    tankSprite.setTexture(tankTexture);
    //set start position (right direction)
    tankSprite.setTextureRect(sf::IntRect(2, 2, 29, 29));
    tankSprite.setPosition(100,100);
    tankSprite.setScale(3,3);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
                window.close();
            }
		}

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            tankSprite.setTextureRect(sf::IntRect(34, 2, 29, 29));
            tankSprite.move(-0.1,0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            tankSprite.setTextureRect(sf::IntRect(99, 1, 29, 29));
            tankSprite.move(0,-0.1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            tankSprite.setTextureRect(sf::IntRect(2, 2, 29, 29));
            tankSprite.move(0.1,0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            tankSprite.setTextureRect(sf::IntRect(66, 1, 29, 29));
            tankSprite.move(0,0.1);
        }

        window.clear();
        window.draw(tankSprite);
        window.display();
	}
}