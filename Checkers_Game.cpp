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
bool redTurn = true;     // identify whose move

struct Move{
    int fromRow, fromCol, toRow, toCol;
};

Move validMoveCollection[1000];
int numOfMove;

void initiateBoard();
void printBoard();
Move getMove();
bool isValidMove(Move move);  
void playVsHuman();     
bool isGameOver();  // not start
void makeMove(Move move); 
void promote(int row, int col); 
void collectValidMove();    // jumped move is not covered
void playVsComputer();   //not finished
void pushValidMove(Move move);

int main(){

    int option;
    initiateBoard();

    cout<<"Enter your choice: "<<endl;
    cout<<"1. Human vs Human"<<endl;
    cout<<"2. Human vs Computer"<<endl;
    cin>>option;

    if(option == 1)
        playVsHuman();
    else if(option == 2)
        playVsComputer();
     
}

void playVsComputer(){

    while(!isGameOver()){
        printBoard();
        cout<< (redTurn ? "Your " : "Computer's ") <<"turn:"<<endl;
        Move move;
        if(redTurn){
            do{
                move = getMove();
            }while(!isValidMove(move));
            makeMove(move);
        }
        else{
            int random;
            collectValidMove();
            random = (int)(rand()%(numOfMove+1));
            move =  validMoveCollection[random];
            makeMove(move);
        }
    }
    printBoard();
    cout<<"Gameover!!"<<endl;
}

void collectValidMove(){
    Move move;
    numOfMove = -1;

    for(int i=0; i<BOARD_SIZE; i++){
        for(int j=0; j<BOARD_SIZE; j++){
            if(checkerBoard[i][j] == EMPTY || checkerBoard[i][j] == RED_PIECE || checkerBoard[i][j] == RED_KING)
                continue;

            else if(checkerBoard[i][j] == BLUE_PIECE){
                move.fromRow = i;
                move.fromCol = j;
                move.toRow = i+1;
                move.toCol = j-1;
                if(isValidMove(move))
                    pushValidMove(move);

                move.toCol = j+1;
                if(isValidMove(move))
                    pushValidMove(move);

            }
            else{
                move.fromRow = i;
                move.fromCol = j;
                move.toRow = i-1;
                move.toCol = j-1;
                if(isValidMove)
                    pushValidMove(move);

                move.toCol = j+1;
                if(isValidMove)
                    pushValidMove(move);

                move.toRow = i+1;
                if(isValidMove)
                    pushValidMove(move);

                move.toCol = j-1;
                if(isValidMove)
                    pushValidMove(move);
        
            }
        }
    }
}

void pushValidMove(Move move){
    numOfMove++;
    validMoveCollection[numOfMove] = move;
}

void playVsHuman(){

    while(!isGameOver()){
        printBoard();
        cout<< (redTurn ? "Red's " : "Blue's ") <<"turn:"<<endl;
        Move move;
        do{
            move = getMove();
        }while(!isValidMove(move));
        makeMove(move);
    }
    printBoard();
    cout<<"Gameover!!"<<endl;
}

void makeMove(Move move){
    int fromRow = move.fromRow, fromCol = move.fromCol, toRow = move.toRow, toCol = move.toCol;

    char piece = checkerBoard[fromRow][fromCol];
    checkerBoard[fromRow][fromCol] = EMPTY;
    checkerBoard[toRow][toCol] = piece;

    if( abs(toRow - fromRow) == 2){
        int jumpedRow = (fromRow + toRow)/2;
        int jumpedCol = (fromCol + toCol)/2;
        checkerBoard[jumpedRow][jumpedCol] = EMPTY;
    }

    if((redTurn && toRow == 0) || (!redTurn && toRow == BOARD_SIZE-1))
        promote(toRow, toCol);

    redTurn = !redTurn;
}

void promote(int row, int col){
    if(checkerBoard[row][col] == BLUE_PIECE)
        checkerBoard[row][col] = BLUE_KING;
    else if(checkerBoard[row][col] == RED_PIECE)
        checkerBoard[row][col] = RED_KING;
}

bool isGameOver(){ 
    return false;
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
        int jumpedRow = (toRow + fromRow)/2, jumpedCol = (toCol + fromCol)/2;
        if((redTurn && (checkerBoard[jumpedRow][jumpedCol] == BLUE_PIECE || checkerBoard[jumpedRow][jumpedCol] == BLUE_KING))
            || (!redTurn && (checkerBoard[jumpedRow][jumpedCol] == RED_PIECE || checkerBoard[jumpedRow][jumpedCol] == RED_KING)))
            return true;   
    }
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
    cout << "  0 1 2 3 4 5 6 7" << endl;
    for(int row=0; row<BOARD_SIZE; row++){
        cout<< row <<" ";
        for(int col=0; col<BOARD_SIZE; col++){
            if(!checkerBoard[row][col])
                cout<<". ";
            else
                cout<<checkerBoard[row][col]<<" ";
        }
        cout<<endl;
    }
}