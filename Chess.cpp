#include<bits/stdc++.h>
using namespace std;

// defines:
#define ll long long
#define pb push_back
#define f first
#define s second
#define A_ 1
#define B_ 2
#define C_ 3
#define D_ 4
#define E_ 5
#define F_ 6
#define G_ 7
#define H_ 8

//global variables:
bool whCast = 1;
bool blCast = 1;
bool blCastA = 1;
bool whCastA = 1;
bool blCastH = 1;
bool whCastH = 1;
char spMove;
char moveSide = 'w';
pair<int ,int> enCtrl = {-1, -1}; // enpassant ctrl
pair<int, int> wKC = {E_, 1}; // white King Coordinate
pair<int, int> bKC = {E_, 8}; // black King Coordinate

pair<pair<int, int>, pair<int, int>> fPosMove = {{0, 0}, {0, 0}};

//boards:

/*
array<array<pair<char, char>, 9>, 9> brdRef = {{
    { { {'!', '!'}, {'!', '!'}, {'!', '!'}, {'!', '!'}, {'!', '!'}, {'!', '!'}, {'!', '!'}, {'!', '!'}, {'!', '!'} } },
    { { {'!', '!'}, {'R', 'w'}, {'P', 'w'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {'P', 'b'}, {'R', 'b'} } },
    { { {'!', '!'}, {'N', 'w'}, {'P', 'w'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {'P', 'b'}, {'N', 'b'} } },
    { { {'!', '!'}, {'B', 'w'}, {'P', 'w'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {'P', 'b'}, {'B', 'b'} } },
    { { {'!', '!'}, {'Q', 'w'}, {'P', 'w'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {'P', 'b'}, {'Q', 'b'} } },
    { { {'!', '!'}, {'K', 'w'}, {'P', 'w'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {'P', 'b'}, {'K', 'b'} } },
    { { {'!', '!'}, {'B', 'w'}, {'P', 'w'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {'P', 'b'}, {'B', 'b'} } },
    { { {'!', '!'}, {'N', 'w'}, {'P', 'w'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {'P', 'b'}, {'N', 'b'} } },
    { { {'!', '!'}, {'R', 'w'}, {'P', 'w'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {'P', 'b'}, {'R', 'b'} } }
}};
*/
array<array<pair<char, char>, 9>, 9> brdRef = {{
    { { {'!', '!'}, {'!', '!'}, {'!', '!'}, {'!', '!'}, {'!', '!'}, {'!', '!'}, {'!', '!'}, {'!', '!'}, {'!', '!'} } },
    { { {'!', '!'}, {' ', '.'}, {'Q', 'w'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'} } },
    { { {'!', '!'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'} } },
    { { {'!', '!'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'} } },
    { { {'!', '!'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'} } },
    { { {'!', '!'}, {'K', 'w'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'} } },
    { { {'!', '!'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'} } },
    { { {'!', '!'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'} } },
    { { {'!', '!'}, {'K', 'b'}, {' ', '.'}, {' ', '.'}, {'P', 'w'}, {' ', '.'}, {'P', 'b'}, {' ', '.'}, {' ', '.'} } }
}};
/*
array<array<pair<char, char>, 9>, 9> brdRef = {{
    { { {'!', '!'}, {'!', '!'}, {'!', '!'}, {'!', '!'}, {'!', '!'}, {'!', '!'}, {'!', '!'}, {'!', '!'}, {'!', '!'} } },
    { { {'!', '!'}, {'R', 'w'}, {'P', 'w'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {'P', 'b'}, {'R', 'b'} } },
    { { {'!', '!'}, {'N', 'w'}, {'P', 'w'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {'P', 'b'}, {'N', 'b'} } },
    { { {'!', '!'}, {'B', 'w'}, {'P', 'w'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {'P', 'b'}, {'B', 'b'} } },
    { { {'!', '!'}, {'Q', 'w'}, {'P', 'w'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {'P', 'b'}, {'Q', 'b'} } },
    { { {'!', '!'}, {'K', 'w'}, {'P', 'w'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {'P', 'b'}, {'K', 'b'} } },
    { { {'!', '!'}, {'B', 'w'}, {'P', 'w'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {'R', 'w'}, {' ', '.'} } },
    { { {'!', '!'}, {'N', 'w'}, {'P', 'w'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {'P', 'b'}, {' ', '.'} } },
    { { {'!', '!'}, {'R', 'w'}, {'P', 'w'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {' ', '.'}, {'P', 'b'}, {'R', 'b'} } }
}};
*/

array<array<pair<char, char>, 9>, 9> brd;

// functions:
void menu(void);// complated && tested
void resetVar(void);// complated && tested
void mMode(void);// 
void bMode(void);
bool brdReset(void);// complated && tested
bool brdPrint(char);// complated && tested
int inpTrns(char);// complated && tested

bool afterMoveAtcCtrl(pair<int, int>, pair<int, int>, pair<int, int>, char);//belirtilen hamle yapıldıktan sonra belirtilen yere belirtilen renkte bir taş bakıyorsa 1 yoksa 0; not : belirtilen hamlenin kurallara uyduğu varsayılır
bool isAtk(pair<int, int>, char);//belirtilen noktaya, belirtilen renkte bir tas bakiyorsa 1 dodurur yoksa 0
bool pawnEndCtrl(void);// complated && tested
bool isCheckMate(void);// ):
bool isStealMate(void);// 
bool isThreefoldRep(void);// 3 hamle tekrari
bool isFiftyMove(void);// 50 hamle
bool isInsufMateMaterial(void);// Yetersiz mataryel

bool move(pair<int ,int>, pair<int, int>);// complated && tested
int pieceVal(char);// tasin degerini soyler
int compPV(char);// belirtilen renkteki taslarin puanlari toplamini dondurur

bool moveCtrl(pair<int, int>, pair<int, int>);// bir tasin a noktasindan b noktasina "rengine bakmadan" gidebilirmi diye bakar

// moveCtrl nin alt fonksiyonları:
bool bCtrl(pair<int ,int>, pair<int ,int>, int, int);
bool rCtrl(pair<int ,int>, pair<int ,int>, int, int);
bool qCtrl(pair<int ,int>, pair<int ,int>, int, int);
int kCtrl(pair<int, int>, pair<int, int>, int, int);
int pCtrl(pair<int, int>, pair<int, int>, int, int);
bool nCtrl(int ,int);

int main(void)// complated
{
    menu();
    return 0;
}

bool isStealMate(void)
{
    return 0;
}

bool isThreefoldRep(void)
{
    return 0;
}

bool isFiftyMove(void)
{
    return 0;
}

bool isInsufMateMaterial(void)
{
    return 0;
}

bool pawnEndCtrl(void)
{
    int sLine = (moveSide == 'w') ? 8 : 1;

    for(int i = 1 ; i <= 8 ; ++i)
    {
        if(brd[i][sLine].f == 'P')
        {
            bool succesinp = 0;
            while(succesinp == 0)
            {
                cout << "Your pawn reached the last rank! Choose a piece to promote to (Q, R, B, or N):\n";
                char inp;
                succesinp = 1;
                cin >> inp;
                switch(inp)
                {
                    case 'Q':
                        brd[i][sLine] = {'Q', moveSide};
                        break;
                    
                    case 'R':
                        brd[i][sLine] = {'R', moveSide};
                        break;

                    case 'B':
                        brd[i][sLine] = {'B', moveSide};
                        break;

                    case 'N':
                        brd[i][sLine] = {'N', moveSide};
                        break;

                    default:
                        cout << "invalid input.\n";
                        succesinp = 0;
                        break;
                }
            }
        }
    }

    return 1;
}

bool isCheckMate(void)
{
    return 0;
}

bool isAtk(pair<int, int> atcCor, char atcSide) 
{
    for(int i = 0; i <= 8; ++i)
        for(int j = 0; j <= 8; ++j)
            if(brd[i][j].s == atcSide && moveCtrl({i, j}, atcCor)) return 1;

    return 0;
}

bool afterMoveAtcCtrl(pair<int, int> mBeg, pair<int, int> mEnd, pair<int, int> atcCor, char atcSide)
{
    // save variables & brds:
        array<array<pair<char, char>, 9>, 9> tempBrd = brd;    
        bool whCast_T = whCast;
        bool blCast_T = blCast;
        bool blCastA_T = blCastA;
        bool whCastA_T = whCastA;
        bool blCastH_T = blCastH;
        bool whCastH_T = whCastH;
        char spMove_T = spMove;
        char moveSide_T = moveSide;
        pair<int ,int> enCtrl_T = enCtrl;
        pair<int, int> wKC_T = wKC;
        pair<int, int> bKC_T = bKC;

    //King special ctrl:
        char isKing = '0';
        if(atcCor == wKC) isKing = 'w';
        if(atcCor == bKC) isKing = 'b';

    //Temp move:
        move(mBeg, mEnd);

    //AtcCtrl:
        bool ret = 0;
        if(isKing == '0') ret = isAtk(atcCor, atcSide);
        else if(isKing == 'w') ret = isAtk(wKC, atcSide);
        else if(isKing == 'b') ret = isAtk(bKC, atcSide);

    // reload variables & brds:
        brd = tempBrd;
        whCast = whCast_T;
        blCast = blCast_T;
        blCastA = blCastA_T;
        whCastA = whCastA_T;
        blCastH = blCastH_T;
        whCastH = whCastH_T;
        spMove = spMove_T;
        moveSide = moveSide_T;
        enCtrl = enCtrl_T;
        wKC = wKC_T;
        bKC = bKC_T;

    return ret;
}

void resetVar(void)
{
    whCast = 1;
    blCast = 1;
    blCastA = 1;
    whCastA = 1;
    blCastH = 1;
    whCastH = 1;
    spMove;
    moveSide = 'w';
    enCtrl = {-1, -1};

    for(int i = 1; i <= 8; ++i)
    {
        for(int j = 1; j <= 8; ++j)
        {
            if(brd[i][j].f == 'K')
            {
                if(brd[i][j].s == 'w') wKC = {i, j};
                if(brd[i][j].s == 'b') bKC = {i, j};
            }
        }
    }

    return;
}

void mMode(void)
{
    cout << "Myself mode\n";
    while(1)
    {
        cout << "Play new game : n\ngo to the menu : m\n";
        char input;
        cin >> input;
        if(input == 'm') return menu();
        else if(input == 'n')
        {
            cout << "Resign : R0 00\nAbort the game : A0 00\n";
            brdReset();
            resetVar();
            brdPrint(moveSide);
            while(1)
            {
                
                //cout << fPosMove.f.f << " " << fPosMove.f.s << " " << fPosMove.s.f << " " << fPosMove.s.s << "\n";

                if(isCheckMate())
                {
                    cout << "its Checkmate.\n" << ((moveSide == 'w') ? "Black" : "White") << " Wins!\n";
                    break;
                }

                //input:
                    cout << ((moveSide == 'w') ? "White" : "Black") << " to move : ";
                    pair<char, int> inp1, inp2;
                    cin >> inp1.f >> inp1.s >> inp2.f >> inp2.s;

                //special inputs:
                    if(inp1.f == 'R')
                    {
                        cout << ((moveSide == 'w') ? "White" : "Black") << " resigns\n" << ((moveSide == 'w') ? "Black" : "White") << " wins!\n";
                        break;
                    }
                    else if(inp1.f == 'A')
                    {
                        cout << "Game aborted.\n";
                        break;
                    }

                //Define variables:
                    pair<int ,int> mBeg, mEnd;
                    mBeg = {inpTrns(inp1.f), inp1.s};
                    mEnd = {inpTrns(inp2.f), inp2.s};

                bool move_ctrl = ((
                    moveCtrl(mBeg, mEnd)) && 
                    (brd[mBeg.f][mBeg.s].s == moveSide) && 
                    !((moveSide == 'w') ? afterMoveAtcCtrl(mBeg, mEnd, wKC, 'b') : afterMoveAtcCtrl(mBeg, mEnd, bKC, 'w')));

                if(move_ctrl)
                {
                    move(mBeg, mEnd);//move

                    brdPrint((moveSide == 'w') ? 'b' : 'w');
                    pawnEndCtrl();

                    moveSide = (moveSide == 'w') ? 'b' : 'w';
                }
                else cout << "invalid input.\n";

                // Piece value comp:
                    if(compPV('w') == compPV('b')) cout << "Piece values are equal.\n";
                    else cout << ((compPV('w') > compPV('b')) ? "White" : "Black") << " is " << (abs(compPV('w') - compPV('b'))) << " point(s) ahead.\n";
            }
        }
    }
}

bool move(pair<int ,int> mBeg, pair<int, int> mEnd)
{
    if(spMove != 7) enCtrl = {-1, -1};
    if(spMove == 1)
    {
        if(brd[mBeg.f][mBeg.s].f == 'K')
        {
            if(brd[mBeg.f][mBeg.s].s == 'w')
            {
                wKC = mEnd;
                whCast = 0;
            }
            else if(brd[mBeg.f][mBeg.s].s == 'b')
            {
                bKC = mEnd;
                blCast = 0;
            }
        }
        else if(brd[mBeg.f][mBeg.s].f == 'R')
        {
            if(brd[mBeg.f][mBeg.s].s == 'w')
            {
                if(mBeg.f == A_ && mBeg.s == 1) whCastA = 0;
                if(mBeg.f == H_ && mBeg.s == 1) whCastH = 0;
            }
            else if(brd[mBeg.f][mBeg.s].s == 'b')
            {
                if(mBeg.f == A_ && mBeg.s == 8) blCastA = 0;
                if(mBeg.f == H_ && mBeg.s == 8) blCastH = 0;
            }
        }

        brd[mEnd.f][mEnd.s] = brd[mBeg.f][mBeg.s];
        brd[mBeg.f][mBeg.s] = {' ', '.'};
    }
    else if(spMove == 7)
    {
        enCtrl = {mEnd.f, mEnd.s};

        brd[mEnd.f][mEnd.s] = brd[mBeg.f][mBeg.s];
        brd[mBeg.f][mBeg.s] = {' ', '.'};
    }
    else if(spMove == 2)
    {
        brd[mEnd.f][mEnd.s] = brd[mBeg.f][mBeg.s];
        brd[mBeg.f][mBeg.s] = {' ', '.'};

        brd[F_][mEnd.s] = brd[H_][mBeg.s];
        brd[H_][mBeg.s] = {' ', '.'};

        wKC = mEnd;
        whCast = 0;
    }
    else if(spMove == 3)
    {
        brd[mEnd.f][mEnd.s] = brd[mBeg.f][mBeg.s];
        brd[mBeg.f][mBeg.s] = {' ', '.'};

        brd[D_][mEnd.s] = brd[A_][mBeg.s];
        brd[A_][mBeg.s] = {' ', '.'};

        wKC = mEnd;
        whCast = 0;
    }
    else if(spMove == 4)
    {
        brd[mEnd.f][mEnd.s] = brd[mBeg.f][mBeg.s];
        brd[mBeg.f][mBeg.s] = {' ', '.'};

        brd[F_][mEnd.s] = brd[H_][mBeg.s];
        brd[H_][mBeg.s] = {' ', '.'};

        bKC = mEnd;
        blCast = 0;
    }
    else if(spMove == 5)
    {
        brd[mEnd.f][mEnd.s] = brd[mBeg.f][mBeg.s];
        brd[mBeg.f][mBeg.s] = {' ', '.'};

        brd[D_][mEnd.s] = brd[A_][mBeg.s];
        brd[A_][mBeg.s] = {' ', '.'};

        bKC = mEnd;
        blCast = 0;
    }
    else if(spMove == 6)
    {
        brd[mEnd.f][mEnd.s] = brd[mBeg.f][mBeg.s];
        brd[mBeg.f][mBeg.s] = {' ', '.'};

        brd[mEnd.f][mBeg.s] = {' ', '.'};
    }

    return 1;
}

int pieceVal(char p)
{
    switch(p)
    {
        case 'P':
            return 1;
        case 'N':
            return 3;
        case 'B':
            return 3;
        case 'R':
            return 5;
        case 'Q':
            return 9;
    }

    return 0;
}

int compPV(char side)
{
    int res = 0;
    for(int i = 1 ; i <= 8 ; ++i)
    {
        for(int j = 1; j <= 8; ++j)
        {
            if(brd[i][j].s == side) res += pieceVal(brd[i][j].f);
        }
    }

    return res;
}

//moveCtrl beg
bool moveCtrl(pair<int, int> mBeg, pair<int, int> mEnd)
{
    if(mBeg.f > 8 || mBeg.f < 1 || mBeg.s > 8 || mBeg.s < 1 || mEnd.f > 8 || mEnd.f < 1 || mEnd.s > 8 || mEnd.s < 1) return 0;
    if(brd[mBeg.f][mBeg.s].s == brd[mEnd.f][mEnd.s].s || mEnd == mBeg) return 0;

    int dx = abs(mBeg.f - mEnd.f);
    int dy = abs(mBeg.s - mEnd.s);

    int pieceCtrl = 1;
    switch(brd[mBeg.f][mBeg.s].f)
    {
        case 'N':
            pieceCtrl = nCtrl(dx, dy);
            break;

        case 'B':
            pieceCtrl = bCtrl(mBeg, mEnd, dx, dy);
            break;
        
        case 'R':
            pieceCtrl = rCtrl(mBeg, mEnd, dx, dy);
            break;
        
        case 'Q':
            pieceCtrl = qCtrl(mBeg, mEnd, dx, dy);
            break;
        
        case 'K':
            pieceCtrl = kCtrl(mBeg, mEnd, dx, dy);
            break;

        case 'P':
            pieceCtrl = pCtrl(mBeg, mEnd, dx, dy);
            break;
        
        default:
            return 0;
    }

    if(pieceCtrl == 0) return 0;
    spMove = pieceCtrl;

    return 1;
}

int pCtrl(pair<int, int> mBeg, pair<int, int> mEnd, int dx, int dy)
{
    char target = brd[mEnd.f][mEnd.s].f;

    if(dx == 0 && (dy == 1 || dy == 2) && target == ' ') // normal moves
    {
        if(dy == 1) return 1;
        if(dy == 2 && (mBeg.s == 2 || mBeg.s == 7)) return 7;
    }
    if(dx == 1 && dy == 1) // eating moves
    {
        if(target != ' ') return 1;// capraz eating
        else if(target == ' ' && enCtrl.f == mEnd.f && enCtrl.s == mBeg.s) return 6;// enpassant
    }

    return 0;
}

int kCtrl(pair<int, int> mBeg, pair<int, int> mEnd, int dx, int dy)//castlings : 2 3 4 5
{
    
    if(dx <= 1 && dy <= 1) return 1; // normal moves
    
    if(dx == 2 && dy == 0 && !(moveSide == 'w' ? (isAtk(wKC, 'b')) : (isAtk(bKC, 'w')))) // Castling
    {
        if(moveSide == 'w' && whCast == 1)// white castling
        {
            if(
                mEnd.f == G_ && 
                brd[F_][1].f == ' ' && 
                brd[G_][1].f == ' ' && 
                whCastH == 1 && 
                !afterMoveAtcCtrl(mBeg, {F_, 1}, {F_, 1}, 'b')) 
                return 2;

            else if(
                mEnd.f == C_ && 
                brd[D_][1].f == ' ' && 
                brd[C_][1].f == ' ' && 
                brd[B_][1].f == ' ' && 
                whCastA == 1 && 
                !afterMoveAtcCtrl(mBeg, {D_, 1}, {D_, 1}, 'b') && 
                !afterMoveAtcCtrl(mBeg, {B_, 1}, {B_, 1}, 'b')) 
                return 3;
        }
        else if(moveSide == 'b' && blCast == 1)// black castling
        {
            if(
                mEnd.f == G_ && 
                brd[F_][8].f == ' ' && 
                brd[G_][8].f == ' ' && 
                blCastH == 1 && 
                !afterMoveAtcCtrl(mBeg, {F_, 8}, {F_, 8}, 'w')) 
                return 4;

            else if(
                mEnd.f == C_ && 
                brd[D_][8].f == ' ' && 
                brd[C_][8].f == ' ' && 
                brd[B_][8].f == ' '  && 
                blCastA == 1 && 
                !afterMoveAtcCtrl(mBeg, {D_, 8}, {D_, 8}, 'w') && 
                !afterMoveAtcCtrl(mBeg, {B_, 8}, {B_, 8}, 'w')) 
                return 5;
        }
    }
    
    return 0;
}

bool qCtrl(pair<int ,int> mBeg, pair<int ,int> mEnd, int dx, int dy)
{
    if(dx == dy) return bCtrl(mBeg, mEnd, dx, dy);
    return rCtrl(mBeg, mEnd, dx, dy);
}

bool rCtrl(pair<int, int> mBeg, pair<int, int> mEnd, int dx, int dy)
{
    if (!((dx == 0 && dy != 0) || (dx != 0 && dy == 0))) return false;

    int stepX = 0, stepY = 0;

    if (dx == 0) stepY = (mEnd.s > mBeg.s) ? 1 : -1;
    else stepX = (mEnd.f > mBeg.f) ? 1 : -1;

    int curX = mBeg.f + stepX;
    int curY = mBeg.s + stepY;

    while (curX != mEnd.f || curY != mEnd.s)
    {
        if (brd[curX][curY].f != ' ') return false;
        curX += stepX;
        curY += stepY;
    }

    return 1;
}

bool bCtrl(pair<int ,int> mBeg, pair<int ,int> mEnd, int dx, int dy)
{
    if(dx != dy) return 0;

    int stepX = (mEnd.f > mBeg.f) ? 1 : -1;
    int stepY = (mEnd.s > mBeg.s) ? 1 : -1;

    int curX = stepX + mBeg.f;
    int curY = stepY + mBeg.s;

    while(curX != mEnd.f)
    {
        if(brd[curX][curY].f != ' ') return 0;

        curX += stepX;
        curY += stepY;
    }

    return 1;
}

bool nCtrl(int dx, int dy)
{
    if((dx == 1 && dy == 2) || (dx == 2 && dy == 1)) return 1;
    return 0;
}
//moveCtrl end

void bMode(void)
{
    cout << "Happybot is not available in this version.\n";
    while(1)
    {
        cout << "go to the menu : m\n"; 
        char input;
        cin >> input;
        if(input == 'm') return menu();
        else cout << "invalid input.\n";
        return bMode();
    }
}

void menu(void)
{
    // bot mode / myself mode
    cout << "Welcome to the HappyChess!\n";
    while(1)
    {
        cout << "play bot mode : b\nplay myself mode : m\nexit : e\n";
        char input;
        cin >> input;
        if(input == 'b') return bMode();
        else if(input == 'm') return mMode();
        else if(input == 'e')
        {
            cout << "Bye bye \n";
            return;
        }
        cout << "invalid input.\n";
    }
}

bool brdReset(void)
{
    for(int i = 0; i <= 8; ++i)
    {
        for(int j = 0; j <= 8; ++j)
        {
            brd[i][j].f = brdRef[i][j].f;
            brd[i][j].s = brdRef[i][j].s;
        }
    }

    return 1;
}

bool brdPrint(char t)
{
    if(t == 'w')
    {
        string resetCode = "\033[0m";
        for (int i = 8; i >= 1; --i)
        {
            cout << i << ":";
            for (int j = 1; j <= 8; ++j)
            {
                string bg;
                string fg;
    
                if(brd[j][i].s == 'w')  fg = "\033[38;2;255;255;255m";
                else if(brd[j][i].s == 'b') fg = "\033[38;2;0;0;0m";
                else if(brd[j][i].s == '.') fg = "\033[38;2;0;0;0m";
    
                if((i + j) % 2 == 1) bg = "\033[48;2;0;255;0m";
                else bg = "\033[48;2;0;0;255m";
    
                cout << bg << fg << " " << brd[j][i].f << " " << resetCode;
    
            }
            cout << "\n";
        }
        cout << "   a  b  c  d  e  f  g  h \n";
    }
    else if(t == 'b')
    {
        string resetCode = "\033[0m";
        for (int i = 1; i <= 8; ++i)
        {
            cout << i << ":";
            for (int j = 8; j >= 1; --j)
            {
                string bg;
                string fg;
    
                if(brd[j][i].s == 'w')  fg = "\033[38;2;255;255;255m";
                else if(brd[j][i].s == 'b') fg = "\033[38;2;0;0;0m";
                else if(brd[j][i].s == '.') fg = "\033[38;2;0;0;0m";
    
                if((i + j) % 2 == 1) bg = "\033[48;2;0;255;0m";
                else bg = "\033[48;2;0;0;255m";
    
                cout << bg << fg << " " << brd[j][i].f << " " << resetCode;
    
            }
            cout << "\n";
        }
        cout << "   h  g  f  e  d  c  b  a \n";
    }

    return 1;
}

int inpTrns(char ch)
{
    return (ch - 'a' + 1);
}


