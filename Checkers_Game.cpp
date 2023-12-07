#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <vector>

using namespace std;

#define BOARD_SIZE 8
#define EMPTY '\0'
#define RED_PIECE 'r'
#define RED_KING 'R'
#define BLUE_PIECE 'b'
#define BLUE_KING 'B'
#define SQUARE_SIZE 60
#define INF 10e6

char checkerBoard[BOARD_SIZE][BOARD_SIZE];
bool redTurn = true;

enum SquareColor
{
    SQUARE_WHITE,
    SQUARE_BROWN
};

struct Move
{
    int fromRow, fromCol, toRow, toCol;
};


vector<char> eattenPieces;
vector<bool> isPromoted;

void initiateBoard();
void printBoard();
Move getMove();
void getMouseClick(int &x, int &y);
bool isValidMove(Move move);
void playVsHuman();
void playVsComputer(); // not finished
void beginnerComputer();
void intermediateComputer(); // not started      // if time not permits use less layer minimax here
void expertComputer();       // not finished
int minimax(int depth, int height, bool isBlue);    //not performing well add optimized utility function and more layer
bool isGameOver(); // not start                     //add alpha beta pruning as it takes too much time to make move
void makeMove(Move move);
void undoMove(Move latestMove);
void promote(int row, int col);
void collectBlueValidMoves(vector<Move> &blueValidMoves);
void collectRedValidMoves(vector<Move> &redValidMoves);
void pushValidMove(Move move);
void printGraphics();
void drawSquare(int x, int y, SquareColor color);
void drawCheckerPiece(int x, int y, char color);
void drawKingPiece(int x, int y, char color);
void countPieces(int &redPieces, int &redKings, int &bluePieces, int &blueKings);

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int option;
    initiateBoard();
    // printGraphics();

    bool doCycle = true;

    do
    {
        cout << "1. Human vs Human" << endl;
        cout << "2. Human vs Computer" << endl;
        cout << "Enter your choice: ";
        cin >> option;
        if (option == 1 || option == 2)
        {
            doCycle = false;
        }
    } while (doCycle);

    if (option == 1)
        playVsHuman();
    else if (option == 2)
        playVsComputer();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return main(); // Call your main function
}

int minimax(int depth, int height, bool isBlue)
{ // AI is blue. So blue is maxplayer.

    int redPieces, redKings, bluePieces, blueKings;

    if (depth == height)
    {
        countPieces(redPieces, redKings, redPieces, redKings);
        return ((bluePieces + 2*blueKings) - (redPieces + 2*redKings));
    }

    if(isBlue)
    {
        vector<Move> blueValidMoves;
        collectBlueValidMoves(blueValidMoves);

        int value = -INF;
        for(int i=0; i<blueValidMoves.size(); i++)
        {
            makeMove(blueValidMoves[i]);
            value = max(value, minimax(depth+1, height, !isBlue));
            undoMove(blueValidMoves[i]);
        }
        return value;
    }
    else if(!isBlue)
    {
        vector<Move> redValidMoves;
        collectRedValidMoves(redValidMoves);

        int value = INF;
        for(int i=0; i<redValidMoves.size(); i++)
        {
            makeMove(redValidMoves[i]);
            value = min(value, minimax(depth+1, height, !isBlue));
            undoMove(redValidMoves[i]);
        }
        return value;
    }

}

void printGraphics()
{

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {

            SquareColor squareColor = (i + j) % 2 == 0 ? SQUARE_WHITE : SQUARE_BROWN;
            drawSquare(j * SQUARE_SIZE, i * SQUARE_SIZE, squareColor);

            if (checkerBoard[i][j] == RED_PIECE)
            {
                drawCheckerPiece(j * SQUARE_SIZE, i * SQUARE_SIZE, RED_PIECE);
            }
            else if (checkerBoard[i][j] == BLUE_PIECE)
            { // Place pieces on odd (i+j) squares
                drawCheckerPiece(j * SQUARE_SIZE, i * SQUARE_SIZE, BLUE_PIECE);
            }
            else if (checkerBoard[i][j] == RED_KING)
            {
                drawKingPiece(j * SQUARE_SIZE, i * SQUARE_SIZE, RED_KING);
            }
            else if (checkerBoard[i][j] == BLUE_KING)
            {
                drawKingPiece(j * SQUARE_SIZE, i * SQUARE_SIZE, BLUE_KING);
            }
        }
    }
}

void drawSquare(int x, int y, SquareColor color)
{

    if (color == SQUARE_WHITE)
    {
        setfillstyle(SOLID_FILL, WHITE);
    }
    else
    {
        setfillstyle(SOLID_FILL, BROWN);
    }
    bar(x, y, x + SQUARE_SIZE, y + SQUARE_SIZE);
}

void drawCheckerPiece(int x, int y, char color)
{

    setcolor(WHITE);
    if (color == RED_PIECE)
        setfillstyle(SOLID_FILL, RED);
    else
        setfillstyle(SOLID_FILL, BLUE);
    circle(x + SQUARE_SIZE / 2, y + SQUARE_SIZE / 2, SQUARE_SIZE / 2 - 5);
    floodfill(x + SQUARE_SIZE / 2, y + SQUARE_SIZE / 2, WHITE);
}

void drawKingPiece(int x, int y, char color)
{
    setcolor(WHITE);
    if (color == RED_KING)
    {
        setfillstyle(SOLID_FILL, RED);
    }
    else
    {
        setfillstyle(SOLID_FILL, BLUE);
    }

    circle(x + SQUARE_SIZE / 2, y + SQUARE_SIZE / 2, SQUARE_SIZE / 2 - 5);
    floodfill(x + SQUARE_SIZE / 2, y + SQUARE_SIZE / 2, WHITE);

    setcolor(WHITE);
    setbkcolor(RED);
    int textWidth = textwidth("K");
    int textHeight = textheight("K");
    outtextxy(x + SQUARE_SIZE / 2 - textWidth / 2, y + SQUARE_SIZE / 2 - textHeight / 2, "K"); // check it please
}

void playVsComputer()
{

    int option;

    while (1)
    {
        cout << "1. Beginner" << endl;
        cout << "2. Intermediate" << endl;
        cout << "3. Expert" << endl;
        cout << "Enter your choice(1-3): ";
        cin >> option;

        if (option == 1)
        {
            beginnerComputer();
            break;
        }
        else if (option == 2)
        {
            intermediateComputer();
            break;
        }
        else if (option == 3)
        {
            expertComputer();
            break;
        }
        else
        {
            continue;
        }
    }
}

void expertComputer()
{


    while (!isGameOver())
    {
        printBoard();
        printGraphics();

        cout << (redTurn ? "Your (red) " : "Computer's (blue) ") << "turn:" << endl;
        Move move;
        if (redTurn)
        {
            while (1)
            {
                while (1)
                {
                    move = getMove();
                    if (isValidMove(move))
                    {
                        break;
                    }
                }
                makeMove(move);
                printBoard();
                printGraphics();
                int undo = 0;
                // cout << "Want to undo??" << endl;
                // cout << "1. YES" << endl;
                // cout << "0. No" << endl;
                // cin >> undo;
                if (undo == 1)
                {
                    undoMove(move);
                    // printBoard();
                    // printGraphics();
                }
                else
                    break;
            }
        }
        else
        {
            vector<Move> blueValidMoves;
            collectBlueValidMoves(blueValidMoves);

            int maxVal = -INF;
            int value;
            Move bestMove;
            for(int i=0; i<blueValidMoves.size(); i++)
            {
                makeMove(blueValidMoves[i]);
                int value = minimax(0, 5, false);   // "false" because after blue's making move its red's turn
                if(value > maxVal)
                {
                    maxVal = value;
                    bestMove = blueValidMoves[i];
                }
                undoMove(blueValidMoves[i]);
            }
            makeMove(bestMove);
            printBoard();
            printGraphics();
        }
    }

    printBoard();
    cout << "Gameover!!" << endl;

}

void intermediateComputer()
{

    /////// not started yet

}

void beginnerComputer()
{

    while (!isGameOver())
    {

        printBoard();
        printGraphics();
        cout << (redTurn ? "Your (red) " : "Computer's (blue) ") << "turn:" << endl;
        Move move;
        if (redTurn)
        {
            while (1)
            {
                while (1)
                {
                    move = getMove();
                    if (isValidMove(move))
                    {
                        break;
                    }
                }
                makeMove(move);
                printBoard();
                printGraphics();
                int undo = 0;
                // cout << "Want to undo??" << endl;
                // cout << "1. YES" << endl;
                // cout << "0. No" << endl;
                // cin >> undo;
                if (undo == 1)
                {
                    undoMove(move);
                    printBoard();
                    printGraphics();
                }
                else
                    break;
            }
        }
        else
        {
            int random;
            vector<Move> blueValidMoves;
            collectBlueValidMoves(blueValidMoves);
            srand(time(0));
            random = (int)(rand() % blueValidMoves.size());
            move = blueValidMoves[random];
            makeMove(move);
            printBoard();
            printGraphics();
        }
    }

    printBoard();
    cout << "Gameover!!" << endl;
}

void collectBlueValidMoves(vector<Move> &blueValidMoves)
{
    Move move;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {

            if (checkerBoard[i][j] == EMPTY || checkerBoard[i][j] == RED_PIECE || checkerBoard[i][j] == RED_KING)
                continue;

            else if (checkerBoard[i][j] == BLUE_PIECE)
            {
                move.fromRow = i;
                move.fromCol = j;
                move.toRow = i + 1;
                move.toCol = j - 1;
                if (isValidMove(move))
                {
                    blueValidMoves.push_back(move);
                }

                move.toCol = j + 1;
                if (isValidMove(move))
                {
                    blueValidMoves.push_back(move);
                }

                move.toRow = i + 2;
                move.toCol = j - 2;
                if (isValidMove(move))
                {
                    blueValidMoves.push_back(move);
                }

                move.toCol = j + 2;
                if (isValidMove(move))
                {
                    blueValidMoves.push_back(move);
                }
            }
            else if (checkerBoard[i][j] == BLUE_KING)
            {

                move.fromRow = i;
                move.fromCol = j;
                move.toRow = i - 1;
                move.toCol = j - 1;
                if (isValidMove(move))
                {
                    blueValidMoves.push_back(move);
                }

                move.toCol = j + 1;
                if (isValidMove(move))
                {
                    blueValidMoves.push_back(move);
                }

                move.toRow = i + 1;
                if (isValidMove(move))
                {
                    blueValidMoves.push_back(move);
                }

                move.toCol = j - 1;
                if (isValidMove(move))
                {
                    blueValidMoves.push_back(move);
                }

                move.toRow = i - 2;
                move.toCol = j - 2;
                if (isValidMove(move))
                {
                    blueValidMoves.push_back(move);
                }

                move.toCol = j + 2;
                if (isValidMove(move))
                {
                    blueValidMoves.push_back(move);
                }

                move.toRow = i + 2;
                if (isValidMove(move))
                {
                    blueValidMoves.push_back(move);
                }

                move.toCol = j - 2;
                if (isValidMove(move))
                {
                    blueValidMoves.push_back(move);
                }
            }
        }
    }
}

void collectRedValidMoves(vector<Move> &redValidMoves)
{
    Move move;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {

            if (checkerBoard[i][j] == EMPTY || checkerBoard[i][j] == BLUE_PIECE || checkerBoard[i][j] == BLUE_KING)
                continue;

            else if (checkerBoard[i][j] == RED_PIECE)
            {
                move.fromRow = i;
                move.fromCol = j;
                move.toRow = i - 1;
                move.toCol = j - 1;
                if (isValidMove(move))
                {
                    redValidMoves.push_back(move);
                }

                move.toCol = j + 1;
                if (isValidMove(move))
                {
                    redValidMoves.push_back(move);
                }

                move.toRow = i - 2;
                move.toCol = j - 2;
                if (isValidMove(move))
                {
                    redValidMoves.push_back(move);
                }

                move.toCol = j + 2;
                if (isValidMove(move))
                {
                    redValidMoves.push_back(move);
                }
            }
            else if (checkerBoard[i][j] == RED_KING)
            {

                move.fromRow = i;
                move.fromCol = j;
                move.toRow = i - 1;
                move.toCol = j - 1;
                if (isValidMove(move))
                {
                    redValidMoves.push_back(move);
                }

                move.toCol = j + 1;
                if (isValidMove(move))
                {
                    redValidMoves.push_back(move);
                }

                move.toRow = i + 1;
                if (isValidMove(move))
                {
                    redValidMoves.push_back(move);
                }

                move.toCol = j - 1;
                if (isValidMove(move))
                {
                    redValidMoves.push_back(move);
                }

                move.toRow = i - 2;
                move.toCol = j - 2;
                if (isValidMove(move))
                {
                    redValidMoves.push_back(move);
                }

                move.toCol = j + 2;
                if (isValidMove(move))
                {
                    redValidMoves.push_back(move);
                }

                move.toRow = i + 2;
                if (isValidMove(move))
                {
                    redValidMoves.push_back(move);
                }

                move.toCol = j - 2;
                if (isValidMove(move))
                {
                    redValidMoves.push_back(move);
                }
            }
        }
    }
}

void countPieces(int &redPieces, int &redKings, int &bluePieces, int &blueKings)
{
    redPieces = 0, redKings = 0;
    bluePieces = 0, blueKings = 0;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (checkerBoard[i][j] == RED_PIECE)
            {
                redPieces++;
            }
            else if(checkerBoard[i][j] == RED_KING)
            {
                redKings++;
            }
            else if (checkerBoard[i][j] == BLUE_PIECE)
            {
                bluePieces++;
            }
            else if(checkerBoard[i][j] == BLUE_KING)
            {
                blueKings++;
            }
        }
    }
}

void playVsHuman()
{

    while (!isGameOver())
    {
        while (1)
        {
            printBoard();
            printGraphics();
            cout << (redTurn ? "Red's " : "Blue's ") << "turn:" << endl;
            Move move;
            do
            {
                move = getMove();
            } while (!isValidMove(move));
            makeMove(move);
            printBoard();
            printGraphics();

            // int undo;
            // cout << "Want to undo??" << endl;
            // cout << "1. YES" << endl;
            // cout << "(0, 2-9). No" << endl;
            // cin >> undo;
            // if (undo == 1)
            // {
            //     undoMove(move);
            //     printBoard();
            //     printGraphics();
            // }
            // else
            //     break;
        }
    }
    printBoard();
    cout << "Gameover!!" << endl;
}

void undoMove(Move latestMove)
{
    int fromRow = latestMove.fromRow, fromCol = latestMove.fromCol, toRow = latestMove.toRow, toCol = latestMove.toCol;

    if (!isPromoted.back())
    {
        isPromoted.pop_back();
        char piece = checkerBoard[toRow][toCol];
        checkerBoard[toRow][toCol] = EMPTY;
        checkerBoard[fromRow][fromCol] = piece;
    }
    else
    {
        isPromoted.pop_back();
        char piece = checkerBoard[toRow][toCol];
        checkerBoard[toRow][toCol] = EMPTY;
        if (piece == RED_KING)
        {
            checkerBoard[fromRow][fromCol] = RED_PIECE;
        }
        else if (piece == BLUE_KING)
        {
            checkerBoard[fromRow][fromCol] = BLUE_PIECE;
        }
    }

    if (abs(toRow - fromRow) == 2)
    {
        int jumpedRow = (fromRow + toRow) / 2;
        int jumpedCol = (fromCol + toCol) / 2;

        checkerBoard[jumpedRow][jumpedCol] = eattenPieces.back();
        eattenPieces.pop_back();
    }

    redTurn = !redTurn; // make move alter player so we need re-alter player
}

void makeMove(Move move)
{
    int fromRow = move.fromRow, fromCol = move.fromCol, toRow = move.toRow, toCol = move.toCol;

    char piece = checkerBoard[fromRow][fromCol];
    checkerBoard[fromRow][fromCol] = EMPTY;
    checkerBoard[toRow][toCol] = piece;

    if (abs(toRow - fromRow) == 2)
    {
        int jumpedRow = (fromRow + toRow) / 2;
        int jumpedCol = (fromCol + toCol) / 2;

        eattenPieces.push_back(checkerBoard[jumpedRow][jumpedCol]);
        checkerBoard[jumpedRow][jumpedCol] = EMPTY;
    }

    if ((piece == RED_PIECE || piece == BLUE_PIECE) && ((redTurn && toRow == 0) || (!redTurn && toRow == BOARD_SIZE - 1)))
    {
        promote(toRow, toCol);
        isPromoted.push_back(true);
    }
    else
        isPromoted.push_back(false);

    redTurn = !redTurn;
}

void promote(int row, int col)
{
    if (checkerBoard[row][col] == BLUE_PIECE)
        checkerBoard[row][col] = BLUE_KING;
    else if (checkerBoard[row][col] == RED_PIECE)
        checkerBoard[row][col] = RED_KING;
}

bool isGameOver()
{
    return false;
}

bool isValidMove(Move move)
{
    int fromRow = move.fromRow, fromCol = move.fromCol, toRow = move.toRow, toCol = move.toCol;
    if (toRow < 0 || toRow >= BOARD_SIZE || toCol < 0 || toCol >= BOARD_SIZE)
    {
        return false;
    }

    char piece = checkerBoard[fromRow][fromCol];
    if (piece == EMPTY || (redTurn && piece != RED_PIECE && piece != RED_KING) || (!redTurn && piece != BLUE_PIECE && piece != BLUE_KING))
    {
        return false;
    }

    if (checkerBoard[toRow][toCol] != EMPTY)
    {
        return false;
    }

    int rowDiff = abs(toRow - fromRow), colDiff = abs(toCol - fromCol);
    if (rowDiff == 1 && colDiff == 1)
    {
        // prohibiting back move for ordinary piece
        if (piece == RED_PIECE && (toRow - fromRow) == -1)
        {
            return true;
        }
        else if (piece == BLUE_PIECE && (toRow - fromRow) == 1)
        {
            return true;
        }
        else if (piece == RED_KING || piece == BLUE_KING)
        {
            return true;
        }
        else
            return false;
    }
    else if (rowDiff == 2 && colDiff == 2)
    {
        int jumpedRow = (toRow + fromRow) / 2, jumpedCol = (toCol + fromCol) / 2;

        if (piece == RED_PIECE && (toRow - fromRow) == -2)
        {
            if ((redTurn && (checkerBoard[jumpedRow][jumpedCol] == BLUE_PIECE || checkerBoard[jumpedRow][jumpedCol] == BLUE_KING)) || (!redTurn && (checkerBoard[jumpedRow][jumpedCol] == RED_PIECE || checkerBoard[jumpedRow][jumpedCol] == RED_KING)))
            {
                return true;
            }
        }
        else if (piece == BLUE_PIECE && (toRow - fromRow) == 2)
        {
            if ((redTurn && (checkerBoard[jumpedRow][jumpedCol] == BLUE_PIECE || checkerBoard[jumpedRow][jumpedCol] == BLUE_KING)) || (!redTurn && (checkerBoard[jumpedRow][jumpedCol] == RED_PIECE || checkerBoard[jumpedRow][jumpedCol] == RED_KING)))
            {
                return true;
            }
        }
        else if (piece == RED_KING || piece == BLUE_KING)
        {
            if ((redTurn && (checkerBoard[jumpedRow][jumpedCol] == BLUE_PIECE || checkerBoard[jumpedRow][jumpedCol] == BLUE_KING)) || (!redTurn && (checkerBoard[jumpedRow][jumpedCol] == RED_PIECE || checkerBoard[jumpedRow][jumpedCol] == RED_KING)))
            {
                return true;
            }
        }
        else
            return false;
    }

    return false;
}

Move getMove()
{
    Move move;
    int x, y;
    getMouseClick(x, y);
    move.fromRow = y/SQUARE_SIZE;
    move.fromCol = x/SQUARE_SIZE;
    getMouseClick(x, y);
    move.toRow = y/SQUARE_SIZE;
    move.toCol = x/SQUARE_SIZE;

    return move;
}

void getMouseClick(int &x, int &y)
{
    int clickCount = 0;

    while (clickCount < 1)
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN, x, y);
            clearmouseclick(WM_LBUTTONDOWN);
            clickCount++;
        }

        //delay(100);
    }
}

void initiateBoard()
{
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            if ((row + col) % 2 && row < 3)
                checkerBoard[row][col] = BLUE_PIECE;
            else if ((row + col) % 2 && row > 4)
                checkerBoard[row][col] = RED_PIECE;
            else
                checkerBoard[row][col] = EMPTY;
        }
    }
}

void printBoard()
{
    cout << "  0 1 2 3 4 5 6 7" << endl;
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        cout << row << " ";
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            if (!checkerBoard[row][col])
                cout << ". ";
            else
                cout << checkerBoard[row][col] << " ";
        }
        cout << endl;
    }
}
