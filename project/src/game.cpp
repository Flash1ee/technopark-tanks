#include <SFML/Graphics.hpp>

//Перенести потом в метод класса танка
void motion(int& dir, sf::Sprite& tankSprite) {
    if (dir == 1) {
        tankSprite.move(-0.1,0);
    }
    if (dir == 2) {
        tankSprite.move(0,-0.1);
    }
    if (dir == 3) {
        tankSprite.move(0.1,0);
    }
    if (dir == 4) {
        tankSprite.move(0,0.1);
    }

    dir = 0;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Tanks");

    sf::Image tankImage;
    tankImage.loadFromFile("../textures/pltank.bmp");
    tankImage.createMaskFromColor(sf::Color(255, 255, 255));

    sf::Texture tankTexture;
    tankTexture.loadFromImage(tankImage);

    sf::Sprite tankSprite;
    tankSprite.setTexture(tankTexture);
    //set start position (right direction)
    tankSprite.setTextureRect(sf::IntRect(2, 2, 29, 29));
    tankSprite.setPosition(100,100);
    tankSprite.setScale(3,3);

    int dir = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            dir = 1;
            tankSprite.setTextureRect(sf::IntRect(34, 2, 29, 29));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            dir = 2;
            tankSprite.setTextureRect(sf::IntRect(99, 1, 29, 29));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            dir = 3;
            tankSprite.setTextureRect(sf::IntRect(2, 2, 29, 29));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            dir = 4;
            tankSprite.setTextureRect(sf::IntRect(66, 1, 29, 29));
        }

        motion(dir, tankSprite);

        window.clear();
        window.draw(tankSprite);
        window.display();
    }
}
