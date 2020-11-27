#ifndef _GAME_MAP_H_
#define _GAME_MAP_H_

#include <istream>
#include <map>
#include <vector>

typedef enum
{
    EMPTY,
    WALL,
    COUNT
} entities;

std::map<std::string, entities> map_entity = {
    {".", EMPTY},
    {"#", WALL}
};

class Map {
 private:
    size_t m_cols;
    size_t m_rows;
    std::vector<std::vector<size_t>> m_field;

 public:
    explicit Map(std::istream &in);
    void set_pos(size_t i, size_t j, entities type);
    ~Map();
};


#endif // _GAME_MAP_H_