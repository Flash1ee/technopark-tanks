#include "objects.h"

Tank::Tank(sf::String textureFile, double x, double y) {
    this->x = x;
    this->y = y;
    this->image.loadFromFile(textureFile);
    this->image.createMaskFromColor(sf::Color(255, 255, 255));

    this->texture.loadFromImage(this->image);

    this->sprite.setTexture(this->texture);

    this->sprite.setTextureRect(sf::IntRect(2, 2, 29, 29));
    this->sprite.setPosition(this->x, this->y);
    this->sprite.setScale(3,3);
}

void Tank::move(double x, double y) {
    this->sprite.move(x, y);
}

sf::Sprite& Tank::getSprite() {
    return this->sprite;
}

void Tank::setTexture(int px, int py, int width, int height) {
    this->sprite.setTextureRect(sf::IntRect(px, py, width, height));
}