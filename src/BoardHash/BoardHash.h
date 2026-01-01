#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>
#include "../Chess/Chess.h"

using Key = uint64_t;

namespace BoardHash
{
    enum Bound { BOUND_EXACT = 0, BOUND_LOWER = 1, BOUND_UPPER = 2 };
    struct Entry 
    {
        int score;
        int depth;
        Bound bound;
    };

    inline std::unordered_map<std::string, Entry> Table;

    /*
    inline std::unordered_map<Key, Entry> Table;
    const int TABLE_SIZE = 1048576; 
    extern Entry* Table;
    
    void init();
    void clear();
    Key generateHash(const Chess::Board& board);
    */
    
    std::string generateID(const Chess::Board& board);
    Chess::Board loadID(const std::string& id);
}