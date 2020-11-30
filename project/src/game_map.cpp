#include "game_map.hpp"

#include <fstream>
#include <iostream>

auto blockSize = 16;
std::map<char, entities> mapEntity = {{'.', EMPTY}, {'#', WALL}};

std::map<entities, std::vector<float>> coords = {
    {WALL,
     {256, 0, static_cast<float>(blockSize), static_cast<float>(blockSize)}},
    {EMPTY,
     {272, 32, static_cast<float>(blockSize), static_cast<float>(blockSize)}}};

void Map::set_pos(size_t i, size_t j, std::shared_ptr<Object> p) {
    if (i >= mRows || j >= mCols) {
        throw std::invalid_argument("Bad position");
    }
    mField[i][j] = p;
    return;
}
sf::Sprite Map::getSprite() const { return this->sprite; }

Map::Map(std::string map, std::string sprite) : mBlockSize(blockSize) {
    if (map.empty() || sprite.empty()) {
        throw std::invalid_argument("Bad stream");
    }
    std::ifstream in(map);
    if (!in.is_open()) {
        throw std::runtime_error("Can't open map file");
    }
    in >> mRows >> mCols;
    if (!mRows || !mCols) {
        throw "Empty play field";
    }

    mField.resize(mRows);
    for (auto& row : mField) {
        row.resize(mCols);
    }

    std::string line;
    for (size_t i = 0; i < mRows; i++) {
        in >> line;
        for (size_t j = 0; j < mCols; j++) {
            char cur = line[j];
            if (!mapEntity.count(cur)) {
                throw std::invalid_argument("Unknown map entity");
            }
            auto flag = mapEntity[cur];
            std::vector<float> cord = coords[flag];
            auto p = std::make_shared<Object>(sprite, i, j, cord[0], cord[1],
                                              cord[2], cord[3], 0);
            set_pos(i, j, p);
        }
    }
    if (!in.eof()) {
        throw "read error, not found eof";
    }
    in.close();
}
void Map::drawMap(sf::RenderWindow& window) {
    for (auto row = 0; row < mRows; row++) {
        for (auto col = 0; col < mCols; col++) {
            sf::Sprite temp = mField[row][col]->getSprite();
            temp.setPosition(row * blockSize, col * blockSize);
            window.draw(temp);
        }
    }
}