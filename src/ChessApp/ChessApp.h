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

    slint::SharedString pieceToImagePath(char p)
    {
        switch(p)
        {
            case 'r': return "assets/b_rook.svg";
            case 'n': return "assets/b_knight.svg";
            case 'b': return "assets/b_bishop.svg";
            case 'q': return "assets/b_queen.svg";
            case 'k': return "assets/b_king.svg";
            case 'p': return "assets/b_pawn.svg";
            case 'R': return "assets/w_rook.svg";
            case 'N': return "assets/w_knight.svg";
            case 'B': return "assets/w_bishop.svg";
            case 'Q': return "assets/w_queen.svg";
            case 'K': return "assets/w_king.svg";
            case 'P': return "assets/w_pawn.svg";
            default: return "";
        }
    }
};