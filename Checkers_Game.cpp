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
char winner;

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
void graphicsMainMenu();
void graphicsAIMenu();
void scoreBoardGraphics();
Move getMove();
void getMouseClick(int &x, int &y);
bool isValidMove(Move move);
void playVsHuman();
void playVsComputer(); // not finished
void beginnerComputer();
void intermediateComputer();                                          // not started      // if time not permits use less layer minimax here
void expertComputer();                                                // not finished
int minimax(int depth, int height, int alpha, int beta, bool isBlue); // not performing well add optimized utility function and more layer
bool isGameOver();                                                    // not start                     //add alpha beta pruning as it takes too much time to make move
void gameOverGraphics(char winner);
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

        graphicsMainMenu();

        // getting mouse input
        int x, y;
        getMouseClick(x, y);
        if ((x >= 180 && x <= 450) && (y >= 180 && y <= 230))
        {
            option = 1;
        }
        else if ((x >= 180 && x <= 450) && (y >= 250 && y <= 350))
        {
            option = 2;
        }

        if (option == 1 || option == 2)
        {
            doCycle = false;
        }
    } while (doCycle);

    if (option == 1)
    {
        playVsHuman();
    }
    else if (option == 2)
    {
        graphicsAIMenu();
        playVsComputer();
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    return main(); // Call your main function
}

void scoreBoardGraphics()
{
    int redPiece, redKing;
    int bluePiece, blueKing;

    countPieces(redPiece, redKing, bluePiece, blueKing);

    int totalRedScore = 12 - bluePiece - blueKing;
    int totalBlueScore = 12 - redPiece - redKing;

    outtextxy(520, 200, "Turn : ");
    if (redTurn)
    {
        drawCheckerPiece(530, 230, RED_PIECE);
    }
    else
    {
        drawCheckerPiece(530, 230, BLUE_PIECE);
    }

    drawCheckerPiece(490, 360, RED_PIECE);
    char redScore[] = " : ";
    redScore[3] = (char)((totalRedScore / 10) + 48);
    redScore[4] = (char)((totalRedScore % 10) + 48);
    redScore[5] = '\0';
    outtextxy(546, 383, redScore);

    drawCheckerPiece(490, 62, BLUE_PIECE);
    char blueScore[] = " : ";
    blueScore[3] = (char)(totalBlueScore / 10 + 48);
    blueScore[4] = (char)(totalBlueScore % 10 + 48);
    blueScore[5] = '\0';
    outtextxy(546, 82, blueScore);
}

void graphicsMainMenu()
{
    cleardevice();
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);

    outtextxy(50, 50, "Menu:");

    rectangle(180, 180, 450, 230);
    rectangle(180, 250, 450, 300);

    outtextxy(200, 195, "1. Human vs Human");
    outtextxy(200, 265, "2. Human vs AI");
}

void graphicsAIMenu()
{
    cleardevice();
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);

    outtextxy(35, 50, "Back");
    rectangle(25, 40, 100, 80);

    rectangle(180, 150, 450, 200);
    rectangle(180, 220, 450, 270);
    rectangle(180, 290, 450, 340);

    outtextxy(200, 165, "1. Beginner");
    outtextxy(200, 235, "2. Intermediate");
    outtextxy(200, 305, "3. Expert");
}

int evaluateState(int height)
{
    int redPieces, redKings, bluePieces, blueKings;
    countPieces(redPieces, redKings, bluePieces, blueKings);
    if (height % 2)
    {
        return ((bluePieces + 2 * blueKings) - (redPieces + 2 * redKings));
    }
    else
    {
        return ((redPieces + 2 * redKings) - (bluePieces + 2 * blueKings));
    }
}

int minimax(int depth, int height, int alpha, int beta, bool isBlue)
{ // AI is blue. So blue is maxplayer.

    int redPieces, redKings, bluePieces, blueKings;

    if (depth == height)
    {
        // countPieces(redPieces, redKings, redPieces, redKings);
        return evaluateState(height); //((bluePieces + 2 * blueKings) - (redPieces + 2 * redKings));
    }

    if (isBlue)
    {
        vector<Move> blueValidMoves;
        collectBlueValidMoves(blueValidMoves);

        int maxVal = -INF;
        int value;
        for (int i = 0; i < blueValidMoves.size(); i++)
        {
            makeMove(blueValidMoves[i]);
            value = minimax(depth + 1, height, alpha, beta, !isBlue);
            undoMove(blueValidMoves[i]);
            if (value > maxVal)
            {
                maxVal = value;
            }
            if (value > alpha)
            {
                alpha = value;
            }
            if (beta <= alpha)
            {
                break;
            }
        }
        return maxVal;
    }
    else if (!isBlue)
    {
        vector<Move> redValidMoves;
        collectRedValidMoves(redValidMoves);

        int minVal = INF;
        int value;
        for (int i = 0; i < redValidMoves.size(); i++)
        {
            makeMove(redValidMoves[i]);
            value = minimax(depth + 1, height, alpha, beta, !isBlue);
            undoMove(redValidMoves[i]);
            if (value < minVal)
            {
                minVal = value;
            }
            if (value < beta)
            {
                beta = value;
            }
            if (beta <= alpha)
            {
                break;
            }
        }
        return minVal;
    }
}

void printGraphics()
{
    scoreBoardGraphics();

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
    {
        setfillstyle(SOLID_FILL, RED);
    }
    else
    {
        setfillstyle(SOLID_FILL, BLUE);
    }
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

        int x, y;
        getMouseClick(x, y);
        if ((x >= 180 && x <= 450) && (y >= 150 && y <= 200))
        {
            option = 1;
        }
        else if ((x >= 180 && x <= 450) && (y >= 220 && y <= 270))
        {
            option = 2;
        }
        else if ((x >= 180 && x <= 450) && (y >= 290 && y <= 340))
        {
            option = 3;
        }

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

        if (redTurn)
        {
            cout << "Red turn : " << endl;
        }
        else
        {
            cout << "Blue turn: " << endl;
        }

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
            int alpha = -INF;
            int beta = +INF;
            Move bestMove;
            for (int i = 0; i < blueValidMoves.size(); i++)
            {
                makeMove(blueValidMoves[i]);
                int value = minimax(0, 9, alpha, beta, false); // "false" because after blue's making move its red's turn
                undoMove(blueValidMoves[i]);
                if (value > maxVal)
                {
                    maxVal = value;
                    bestMove = blueValidMoves[i];
                }

                if (value > alpha)
                {
                    alpha = value;
                }
                if (beta <= alpha)
                {
                    break;
                }
            }
            makeMove(bestMove);
            printBoard();
            printGraphics();
        }
    }

    printBoard();
    // printGraphics();
    cout << "Gameover!!" << endl;
    gameOverGraphics(winner);
    // delay(10000);
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

        if (redTurn)
        {
            cout << "Red turn : " << endl;
        }
        else
        {
            cout << "Blue turn: " << endl;
        }

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
    // printGraphics();
    cout << "Gameover!!" << endl;
    gameOverGraphics(winner);
    // delay(10000);
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
            else if (checkerBoard[i][j] == RED_KING)
            {
                redKings++;
            }
            else if (checkerBoard[i][j] == BLUE_PIECE)
            {
                bluePieces++;
            }
            else if (checkerBoard[i][j] == BLUE_KING)
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

            if (redTurn)
            {
                cout << "Red turn : " << endl;
            }
            else
            {
                cout << "Blue turn: " << endl;
            }

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
    printGraphics();
    cout << "Gameover!!" << endl;
    gameOverGraphics(winner);
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
    {
        isPromoted.push_back(false);
    }

    redTurn = !redTurn;
}

void promote(int row, int col)
{
    if (checkerBoard[row][col] == BLUE_PIECE)
    {
        checkerBoard[row][col] = BLUE_KING;
    }
    else if (checkerBoard[row][col] == RED_PIECE)
    {
        checkerBoard[row][col] = RED_KING;
    }
}

bool isGameOver()
{
    int redPiece, redKing;
    int bluePiece, blueKing;

    countPieces(redPiece, redKing, bluePiece, blueKing);

    int totalRed = redPiece + redKing;
    int totalBlue = bluePiece + blueKing;

    if (!totalBlue)
    {
        cout << "Red win!!!" << endl;
        winner = RED_PIECE;
        return true;
    }
    else if (!totalRed)
    {
        cout << "Blue win!!!" << endl;
        winner = BLUE_PIECE;
        return true;
    }

    if (!redTurn)
    {
        vector<Move> blueValidMoves;
        collectBlueValidMoves(blueValidMoves);
        if (!blueValidMoves.size())
        {
            cout << "RED WIN!!!" << endl;
            winner = RED_PIECE;
            return true;
        }
        else
        {
            cout << "blueMoves : " << blueValidMoves.size();
        }
    }

    if (redTurn)
    {
        vector<Move> redValidMoves;
        collectRedValidMoves(redValidMoves);
        if (!redValidMoves.size())
        {
            cout << "BLUE WIN!!!" << endl;
            winner = BLUE_PIECE;
            return true;
        }
        else
        {
            cout << "redmoves : " << redValidMoves.size();
        }
    }

    return false;
}

void gameOverGraphics(char winner)
{
    setcolor(YELLOW);
    settextstyle(BOLD_FONT, HORIZ_DIR, 7);

    char gameOver[] = "Game Over";
    int xGameOver = (getmaxx() - textwidth(gameOver)) / 2;
    int yGameOver = (getmaxy() - textheight(gameOver)) / 2 - 50;

    outtextxy(xGameOver, yGameOver, gameOver);

    char youWin[100];
    if (winner == RED_PIECE)
    {
        char msg[] = "Red win!!!";
        for (int i = 0; i < 10; i++)
        {
            youWin[i] = msg[i];
        }
        youWin[10] = '\0';
    }
    else
    {
        char msg[] = "Blue win!!!";
        for (int i = 0; i < 11; i++)
        {
            youWin[i] = msg[i];
        }
        youWin[10] = '\0';
    }

    int xYouWin = (getmaxx() - textwidth(youWin)) / 2;
    int yYouWin = yGameOver + textheight(gameOver);

    setcolor(WHITE);
    settextstyle(BOLD_FONT, HORIZ_DIR, 7);
    outtextxy(xYouWin, yYouWin, youWin);

    delay(200000);
    closegraph();
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
    move.fromRow = y / SQUARE_SIZE;
    move.fromCol = x / SQUARE_SIZE;
    getMouseClick(x, y);
    move.toRow = y / SQUARE_SIZE;
    move.toCol = x / SQUARE_SIZE;

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
    }
}

void initiateBoard()
{
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            if ((row + col) % 2 && row < 3)
            {
                checkerBoard[row][col] = BLUE_PIECE;
            }
            else if ((row + col) % 2 && row > 4)
            {
                checkerBoard[row][col] = RED_PIECE;
            }
            else
            {
                checkerBoard[row][col] = EMPTY;
            }
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
            {
                cout << ". ";
            }
            else
            {
                cout << checkerBoard[row][col] << " ";
            }
        }
        cout << endl;
    }
}
