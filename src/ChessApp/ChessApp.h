#pragma once
#include <memory>
#include <string>
#include <vector>
#include "appGui.h" 

// --- Forward Declaration ---
namespace Chess { class Board; class BoardCoordinate; }
namespace FlameBoth { class Bot; } 

class ChessApp
{
public:
    ChessApp(); 
    void run();

private:
    slint::ComponentHandle<AppWindow> ui; 
    
    std::shared_ptr<Chess::Board> board;
    std::shared_ptr<FlameBoth::Bot> bot; 

    int selected_square_index; 
    bool is_game_over; 

    void onSquareClicked(int index);
    void onButtonClicked(int id);
    void updateBoardVisuals();
    
    void logToTerminal(std::string message);
    std::string indexToNotation(int index);
    Chess::BoardCoordinate indexToBoardCoord(int index);

    void checkGameState(); 

    slint::SharedString pieceToUnicode(char p)
    {
        switch(p)
        {
            case 'r': return u8"♜";
            case 'n': return u8"♞";
            case 'b': return u8"♝";
            case 'q': return u8"♛";
            case 'k': return u8"♚";
            case 'p': return u8"♟";
            case 'R': return u8"♖";
            case 'N': return u8"♘";
            case 'B': return u8"♗";
            case 'Q': return u8"♕";
            case 'K': return u8"♔";
            case 'P': return u8"♙";
            default: return "";
        }
    }
};