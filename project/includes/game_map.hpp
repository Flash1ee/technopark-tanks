#ifndef _GAME_MAP_H_
#define _GAME_MAP_H_

#include <SFML/Graphics.hpp>
#include <istream>
#include <map>
#include <memory>
#include <vector>

#include "objects.h"

typedef enum { EMPTY, WALL, COUNT_W } entities;

class Map {
   private:
    size_t mCols;
    size_t mRows;
    size_t mBlockSize;
    std::vector<std::vector<std::shared_ptr<Object> > > mField;

    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;

   public:
    explicit Map(std::string map, std::string sprite);
    // ~Map();
    void set_pos(size_t i, size_t j, std::shared_ptr<Object> p);
    sf::Sprite getSprite() const;
    void drawMap(sf::RenderWindow &window);
};

#endif  // _GAME_MAP_H_