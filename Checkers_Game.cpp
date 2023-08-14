#include<iostream>
#define BOARD_SIZE 8
#define EMPTY '\0'
#define RED_PIECE 'r'
#define RED_KING 'R'
#define BLUE_PIECE 'b'
#define BLUE_KING 'B'
using namespace std;

char checkerBoard[BOARD_SIZE][BOARD_SIZE];

void initiateBoard();
void printBoard();

int main(){
    
    initiateBoard();
    printBoard();
    
}

void initiateBoard(){
    for(int row=0; row<BOARD_SIZE; row++){
        for(int colomn=0; colomn<BOARD_SIZE; colomn++){
            if((row + colomn)%2 && row<3) checkerBoard[row][colomn] = BLUE_PIECE;
            else if((row + colomn)%2 && row>4) checkerBoard[row][colomn] = RED_PIECE;
            else checkerBoard[row][colomn] = EMPTY;
        }
    }
}

void printBoard(){
    for(int row=0; row<BOARD_SIZE; row++){
        for(int colomn=0; colomn<BOARD_SIZE; colomn++){
            if(!checkerBoard[row][colomn]){
                cout<<". ";
            }
            else{
                cout<<checkerBoard[row][colomn]<<" ";
            }
        }
        cout<<endl;
    }
}