#pragma once

#include <cstdint>
#include <unordered_map>
#include "../Chess/Chess.h"

using Key = uint64_t;

namespace BoardHash
{
    struct Entry 
    {
        Key key;
        int score;
        int depth;
        // int flag;    // : Exact, Alpha, Beta (olacak)
        // Move bestMove; // : En iyi hamle (olacak)
    };

    // inline std::unordered_map<Key, Entry> Table;
    
    const int TABLE_SIZE = 1048576; 
    
    extern Entry* Table;
    
    void init();
    void clear();
    Key generateHash(const Chess::Board& board);

    // std::string generateID(const Chess::Board& board);
}