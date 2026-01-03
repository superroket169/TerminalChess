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
            case 'r': return "♜";
            case 'n': return "♞";
            case 'b': return "♝";
            case 'q': return "♛";
            case 'k': return "♚";
            case 'p': return "♟";
            case 'R': return "♖";
            case 'N': return "♘";
            case 'B': return "♗";
            case 'Q': return "♕";
            case 'K': return "♔";
            case 'P': return "♙";
            default: return "";
        }
    }
};