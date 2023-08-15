#include<iostream>
#include<math.h>
#define BOARD_SIZE 8
#define EMPTY '\0'
#define RED_PIECE 'r'
#define RED_KING 'R'
#define BLUE_PIECE 'b'
#define BLUE_KING 'B'
using namespace std;

char checkerBoard[BOARD_SIZE][BOARD_SIZE];
bool redTurn;     // identify whose move

struct Move{
    int fromRow, fromCol, toRow, toCol;
};

void initiateBoard();
void printBoard();
Move getMove();
bool isValidMove(Move move);

int main(){
    
    initiateBoard();
    printBoard();
     
}

bool isValidMove(Move move){
    int fromRow = move.fromRow, fromCol = move.fromCol, toRow = move.toRow, toCol = move.toCol;
    if(toRow < 0 || toRow >= BOARD_SIZE || toCol <0 || toCol >= BOARD_SIZE)
        return false;

    char piece = checkerBoard[fromRow][fromCol];
    if(piece == EMPTY || (redTurn && piece != RED_PIECE && piece != RED_KING) 
    || (!redTurn && piece != BLUE_PIECE && piece != BLUE_KING) )
        return false;
        
    if(checkerBoard[toRow][toCol] != EMPTY)
        return false;
    
    int rowDiff = abs(toRow - fromRow), colDiff = abs(toCol - fromCol);
    if(rowDiff == 1 && colDiff == 1)
        return true;
        
    else if(rowDiff == 2 && colDiff == 2){
        int jumpedRow = (toRow + fromRow)/2, jumpedCol = (toCol - fromCol)/2;
        if((redTurn && (checkerBoard[jumpedRow][jumpedCol] == BLUE_PIECE || checkerBoard[jumpedRow][jumpedCol] == BLUE_KING))
            || (!redTurn && (checkerBoard[jumpedRow][jumpedCol] == RED_PIECE || checkerBoard[jumpedRow][jumpedCol] == RED_KING)))
            return true;
    }
    cout<<"hello"<<endl;
    return false;
}

Move getMove(){
    Move move;
    cout << "Enter move (fromRow fromCol toRow toCol): ";
    cin>>move.fromRow>>move.fromCol>>move.toRow>>move.toCol;
    return move;
}

void initiateBoard(){
    for(int row=0; row<BOARD_SIZE; row++){
        for(int col=0; col<BOARD_SIZE; col++){
            if((row + col)%2 && row<3)
                checkerBoard[row][col] = BLUE_PIECE;
            else if((row + col)%2 && row>4)
                checkerBoard[row][col] = RED_PIECE;
            else 
                checkerBoard[row][col] = EMPTY;
        }
    }
}

void printBoard(){
    for(int row=0; row<BOARD_SIZE; row++){
        for(int col=0; col<BOARD_SIZE; col++){
            if(!checkerBoard[row][col])
                cout<<". ";
            else
                cout<<checkerBoard[row][col]<<" ";
        }
        cout<<endl;
    }
}