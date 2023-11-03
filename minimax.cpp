#include <iostream>
#include <vector>

using namespace std;

#define BOARD_SIZE 8
#define EMPTY '\0'
#define RED_PIECE 'r'
#define RED_KING 'R'
#define BLUE_PIECE 'b'
#define BLUE_KING 'B'
#define INFINITY 1e9

struct Move
{
    int fromRow, fromCol, toRow, toCol;
};

char checkerBoard[BOARD_SIZE][BOARD_SIZE];

int redPieces = 12;
int bluePieces = 12;

char maxPlayer; // initiate it
char minPlayer; // also initiate it

int blueValidMoves;
int redValidMoves;

bool isValidMove(Move move);

bool terminal()
{
    if (redPieces == 0 || bluePieces == 0)
    {
        return true;
    }

    else if (blueValidMoves == 0 || redValidMoves == 0)
    {
        return true;
    }
}

int value()
{
}

bool player()
{
}

int numOfMove = 0;
Move validMoveCollection[100000];

void pushValidMove(Move move)
{
    if (isValidMove(move))
    {
        numOfMove++;
        validMoveCollection[numOfMove] = move;
    }
}

vector<Move> actions()
{
    Move move;
    numOfMove = -1;

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
                pushValidMove(move);

                move.toCol = j + 1;
                pushValidMove(move);

                move.toRow = i + 2;
                move.toCol = j - 2;
                pushValidMove(move);

                move.toCol = j + 2;
                pushValidMove(move);
            }
            else
            {
                move.fromRow = i;
                move.fromCol = j;
                move.toRow = i - 1;
                move.toCol = j - 1;
                pushValidMove(move);

                move.toCol = j + 1;
                pushValidMove(move);

                move.toRow = i + 1;
                pushValidMove(move);

                move.toCol = j - 1;
                pushValidMove(move);

                move.toRow = i - 2;
                move.toCol = j - 2;
                pushValidMove(move);

                move.toCol = j + 2;
                pushValidMove(move);

                move.toRow = i + 2;
                pushValidMove(move);

                move.toCol = j - 2;
                pushValidMove(move);
            }
        }
    }
}

char result(Move move)
{

    // return  checkerBoard;
}

int minimax(char checkerBoard[BOARD_SIZE][BOARD_SIZE])
{
    if (terminal())
    {
        return value();
    }

    if (player() == maxPlayer)
    {
        int maxEval = -INFINITY;
        vector<Move> possibleMoves = actions();
        for (Move move : possibleMoves)
        {
            int eval = minimax(result(move));
            maxEval = max(maxEval, eval);
        }
        return maxEval;
    }
    else
    {
        int minEval = INFINITY;
        vector<Move> possibleMoves = actions();
        for (Move move : possibleMoves)
        {
            int eval = minimax(result(move));
            minEval = min(minEval, eval);
        }
        return minEval;
    }
}

Move getBestMove()
{
    int bestValue = (player() ? maxPlayer : minPlayer);
    Move bestMove;

    for (int i = 0; i < numOfMove; i++)
    {
        int moveValue = minimax(result(validMoveCollection[i]));
        if ((player() && moveValue > bestValue) || (!player() && moveValue < bestValue))
        {
            bestValue = moveValue;
            bestMove = validMoveCollection[i];
        }
    }

    return bestMove;
}
