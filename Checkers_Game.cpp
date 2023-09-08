#include<iostream>
#include<math.h>
#include<stdlib.h>
#include<graphics.h>
#include<conio.h>
#include<time.h>

using namespace std;

#define BOARD_SIZE 8
#define EMPTY '\0'
#define RED_PIECE 'r'
#define RED_KING 'R'
#define BLUE_PIECE 'b'
#define BLUE_KING 'B'
#define SQUARE_SIZE 60

char checkerBoard[BOARD_SIZE][BOARD_SIZE];
bool redTurn = true;

enum SquareColor { SQUARE_WHITE, SQUARE_BROWN };

struct Move{
    int fromRow, fromCol, toRow, toCol;
};

Move validMoveCollection[1000];
int numOfMove;

void initiateBoard();
void printBoard();
Move getMove();
bool isValidMove(Move move);    //back move also possible(for red only) but it is not allowed for ordinary piece so fix it
void playVsHuman();
bool isGameOver();  // not start
void makeMove(Move move);
void promote(int row, int col);
void collectValidMove();    // jumped move is covered but not checked
void playVsComputer();   //not finished
void pushValidMove(Move move);
void printGraphics();       //not showing king conversion so add it
void drawSquare(int x, int y, SquareColor color);
void drawCheckerPiece(int x, int y, char color);

int main(){
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");


    int option;
    initiateBoard();
    printGraphics();



    bool doCycle = true;

    do{
        cout<<"1. Human vs Human"<<endl;
        cout<<"2. Human vs Computer"<<endl;
        cout<<"Enter your choice: ";
        cin>>option;
        if(option == 1 || option == 2){
            doCycle = false;
        }
    }while(doCycle);

    if(option == 1)
        playVsHuman();
    else if(option == 2)
        playVsComputer();

}



void printGraphics(){

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            SquareColor squareColor = (i + j) % 2 == 0 ? SQUARE_WHITE : SQUARE_BROWN;
            drawSquare(j * SQUARE_SIZE, i * SQUARE_SIZE, squareColor);

            if (checkerBoard[i][j] == RED_PIECE ) {
               drawCheckerPiece(j * SQUARE_SIZE, i * SQUARE_SIZE, RED_PIECE);
            } else if (checkerBoard[i][j] == BLUE_PIECE) {  // Place pieces on odd (i+j) squares
                drawCheckerPiece(j * SQUARE_SIZE, i * SQUARE_SIZE, BLUE_PIECE);
            }
        }
    }

}

void drawSquare(int x, int y, SquareColor color) {
    if (color == SQUARE_WHITE) {
        setfillstyle(SOLID_FILL, WHITE);
    } else {
        setfillstyle(SOLID_FILL, BROWN);
    }
    bar(x, y, x + SQUARE_SIZE, y + SQUARE_SIZE);
}

void drawCheckerPiece(int x, int y, char color) {
    setcolor(WHITE);
    if(color == RED_PIECE)
        setfillstyle(SOLID_FILL, RED);
    else
        setfillstyle(SOLID_FILL, BLUE);
    circle(x + SQUARE_SIZE / 2, y + SQUARE_SIZE / 2, SQUARE_SIZE / 2 - 5);
    floodfill(x + SQUARE_SIZE / 2, y + SQUARE_SIZE / 2, WHITE);
}

void playVsComputer(){

    while(!isGameOver()){
        printBoard();
        printGraphics();
        cout<< (redTurn ? "Your (red) " : "Computer's (blue) ") <<"turn:"<<endl;
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
            srand(time(0));
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
                pushValidMove(move);

                move.toCol = j+1;
                pushValidMove(move);

                move.toRow = i+2;
                move.toCol = j-2;
                pushValidMove(move);

                move.toCol = j+2;
                pushValidMove(move);

            }
            else{
                move.fromRow = i;
                move.fromCol = j;
                move.toRow = i-1;
                move.toCol = j-1;
                pushValidMove(move);

                move.toCol = j+1;
                pushValidMove(move);

                move.toRow = i+1;
                pushValidMove(move);

                move.toCol = j-1;
                pushValidMove(move);

                move.toRow = i-2;
                move.toCol = j-2;
                pushValidMove(move);

                move.toCol = j+2;
                pushValidMove(move);

                move.toRow = i+2;
                pushValidMove(move);

                move.toCol = j-2;
                pushValidMove(move);

            }
        }
    }
}

void pushValidMove(Move move){
    if(isValidMove(move)){
        numOfMove++;
        validMoveCollection[numOfMove] = move;
    }
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
