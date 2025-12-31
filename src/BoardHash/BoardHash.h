#pragma once
#include <unordered_map>
#include <string>
#include"../Chess/Chess.h"

namespace BoardHash
{
    struct boardInf
    {
        int score;
        int depth;
    };
    /**
     * fikir : bitset ile daha iyi (hızlı) hashleme
     */
    std::string generateID(const Chess::Board& board);

    inline std::unordered_map<std::string, boardInf> boardMap;
}