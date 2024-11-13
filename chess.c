/*
Aidan Cotsakis
2022-10-29
A chess A.I. using the minimax algorithm 
*/ 

// function prototypes
double evaluate(int board[8][8][3]);
int inCheck(int board[8][8][3], int x, int y, int player);
void movePiece(int board[8][8][3], int x1, int y1, int x2, int y2, int turnNum);
void copyBoard(int board[8][8][3], int boardCopy[8][8][3]);
int isValid(int board[8][8][3], int x2, int y2, int player);
void generateChildren(int board[8][8][3], int possibleMoves[][4], int player, int turnNum);
double minimax(int board[8][8][3], int depth, int alpha, int beta, int player, int turnNum);
void printBoard(int board[8][8][3]);

// includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include<windows.h>

// global vairables
int globalBoard[8][8][3] = {
    {{2,4,0},{2,2,0},{2,3,0},{2,5,0},{2,6,0},{2,3,0},{2,2,0},{2,4,0}},
    {{2,1,0},{2,1,0},{2,1,0},{2,1,0},{2,1,0},{2,1,0},{2,1,0},{2,1,0}},
    {{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}},
    {{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}},
    {{1,1,0},{1,1,0},{1,1,0},{1,1,0},{1,1,0},{1,1,0},{1,1,0},{1,1,0}},
    {{1,4,0},{1,2,0},{1,3,0},{1,5,0},{1,6,0},{1,3,0},{1,2,0},{1,4,0}}};

int globalTurn = 1;

int kingMoves[][2] = {{1,1},{1,0},{1,-1},{0,1},{0,-1},{-1,1},{-1,0},{-1,-1}};
int queenMoves[][2] = {{1,1},{1,0},{1,-1},{0,1},{0,-1},{-1,1},{-1,0},{-1,-1}};
int rookMoves[][2] = {{1,0},{-1,0},{0,1},{0,-1}};
int bishopMoves[][2] = {{1,1},{-1,-1},{-1,1},{1,-1}};
int knightMoves[][2] = {{2,1},{2,-1},{-2,1},{-2,-1},{1,2},{-1,2},{1,-2},{-1,-2}};

int seenPositions[500];

int distances[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,0},
    {0,1,4,4,4,4,1,0},
    {0,1,4,9,9,4,1,0},
    {0,1,4,9,9,4,1,0},
    {0,1,4,4,4,4,1,0},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0}};

int pawnDistances[8][8] = {
    {9,9,9,9,9,9,9,9},
    {0,1,1,1,1,1,1,0},
    {1,1,4,4,4,4,1,1},
    {0,1,4,9,9,4,1,0},
    {0,1,4,9,9,4,1,0},
    {1,1,4,4,4,4,1,1},
    {0,1,1,1,1,1,1,0},
    {9,9,9,9,9,9,9,9}};

int knightDistances[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,0},
    {0,1,7,4,4,7,1,0},
    {0,1,7,6,6,7,1,0},
    {0,1,7,6,6,7,1,0},
    {0,1,7,4,4,7,1,0},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0}};

int bishopDistances[8][8] = {
    {1,1,1,1,1,1,1,1},
    {1,7,1,1,1,1,7,1},
    {1,1,1,5,5,1,1,1},
    {1,5,3,3,3,3,5,1},
    {1,5,3,3,3,3,5,1},
    {1,1,1,5,5,1,1,1},
    {1,7,1,1,1,1,7,1},
    {1,1,1,1,1,1,1,1}};

int rookDistances[8][8] = {
    {6,6,7,6,6,7,6,6},
    {1,1,4,1,1,4,1,1},
    {1,1,4,1,1,4,1,1},
    {1,1,4,1,1,4,1,1},
    {1,1,4,1,1,4,1,1},
    {1,1,4,1,1,4,1,1},
    {1,1,4,1,1,4,1,1},
    {6,6,7,6,6,7,6,6}};

int queenDistances[8][8] = {
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,4,4,4,4,1,1},
    {1,1,4,4,4,4,1,1},
    {1,1,4,4,4,4,1,1},
    {1,1,4,4,4,4,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1}};

int kingDistances[8][8] = {
    {2,2,2,2,2,2,2,2},
    {0,0,0,0,0,0,0,0},
    {2,2,2,2,2,2,2,2},
    {4,4,4,4,4,4,4,4},
    {4,4,4,4,4,4,4,4},
    {2,2,2,2,2,2,2,2},
    {0,0,0,0,0,0,0,0},
    {2,2,2,2,2,2,2,2}};

char labels[] = {'a','b','c','d','e','f','g','h'};

double checkAdvantage = 0.5;

int count;

// generate a random floating point number from min to max
double random() {
    return ((double)rand()/RAND_MAX*2.0-1.0)/10000;//float in range -1 to 1
}

int hashBoard(int board[8][8][3]) {
    int playerPrimes[2] = {2,3};
    int piecePrimes[6] = {5,7,11,13,17,19};
    int xPrimes[8] = {23, 29, 31, 47, 41, 43, 47, 53};
    int yPrimes[8] = {59, 61, 67, 71, 73, 79, 83, 89};
    int numPrimes[40] = {97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,283,293,307,311};

    int total = 0;
    int numPieces = 0;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (board[y][x][2] != 0) {
                total += playerPrimes[board[y][x][0]-1]*piecePrimes[board[y][x][1]-1]*xPrimes[x]*yPrimes[y]*numPrimes[numPieces];
                numPieces++;
            }
        }
    }

    return total;
}

int seenPosition(int position, int turnNum) {
    int timesSeen = 0;
    for (int i = 0; i < turnNum; i++) {
        if (position == seenPositions[i]) {
            timesSeen++;
        }
    }
    return timesSeen;
}

// returns board evaluation (white maximizing)
double evaluate(int board[8][8][3]) {

    int sign;
    double evaluation = 0;
    int x2, y2, i;
    double distanceEval = 0.5/9;
    double safeKingEval = 0.1;

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            
            if (board[y][x][0] != 0) {
                if (board[y][x][0] == 1) {
                    sign = 1;
                }
                else if (board[y][x][0] == 2) {
                    sign = -1;
                }

                // pawn
                if (board[y][x][1] == 1) {
                    evaluation += (1 + (pawnDistances[y][x])*distanceEval)*sign;
                }
                // knight
                else if (board[y][x][1] == 2) {
                    evaluation += (3 + (knightDistances[y][x])*distanceEval)*sign;
                }
                // bishop
                else if (board[y][x][1] == 3) {
                    evaluation += (3 + (bishopDistances[y][x])*distanceEval)*sign;
                }
                // rook
                else if (board[y][x][1] == 4) {
                    evaluation += (5 + (rookDistances[y][x])*distanceEval)*sign;
                }
                // queen
                else if (board[y][x][1] == 5) {
                    evaluation += (9 + (queenDistances[y][x])*distanceEval)*sign;
                }
                // king
                else if (board[y][x][1] == 6) {
                    evaluation += (1000 + (kingDistances[y][x])*distanceEval)*sign;
                    
                    // increase evaluation for every piece near king
                    for (i=0; i < 8; i++) {
                        x2 = x + kingMoves[i][0];
                        y2 = y + kingMoves[i][1];
                    
                        if ((x2 >= 0 && x2 < 8 && y2 >= 0 && y2 < 8 && board[y2][x2][0] == board[y][x][0]) || (x2 < 0 && x2 >= 8 && y2 < 0 && y2 >= 8)) {
                            evaluation += safeKingEval*sign;
                        }
                    }
                }
            }
        }
    }

    evaluation += random();

    return evaluation;
}

double evalShift(int board[8][8][3], double evaluation) {
    // add points for being in check
    if (inCheck(board, -1, -1, 1)) {
        evaluation -= checkAdvantage;
    }
    else if (inCheck(board, -1, -1, 2)) {
        evaluation += checkAdvantage;
    }

    return evaluation;
}

// checks if given king is in check
int inCheck(int board[8][8][3], int x, int y, int player) {
    // check if x and y are provided, if not find them
    int i;
    int j;
    int x2;
    int y2;
    int facing;

    if (x < 0 || y < 0) {
        for (i=0;i<8;i++) {
            for (j=0;j<8;j++) {
                if (board[i][j][1] == 6 && board[i][j][0] == player) {
                    x = j;
                    y = i;
                } 
            }
        }
    }

    if (player == 1) {
        facing = -1;
    }
    else if (player == 2) {
        facing = 1;
    }

    // check diagnols
    for (i=0; i < 4; i++) {
        for (j=1; j < 8; j++) {
            x2 = x + bishopMoves[i][0]*j;
            y2 = y + bishopMoves[i][1]*j;
            int ownKing = isOwnKing(board, x2, y2, player);
            if (isValid(board, x2, y2, player) || ownKing) {
                if (board[y2][x2][1] == 3 || board[y2][x2][1] == 5 || (j == 1 && board[y2][x2][1] == 6 && !ownKing)) {
                    return 1;
                } 
                else if (board[y2][x2][0] != 0 && !ownKing) {
                    break;
                }
            }
            else {
                break;
            }
        }
    }

    // check straights
    for (i=0; i < 4; i++) {
        for (j=1; j < 8; j++) {
            x2 = x + rookMoves[i][0]*j;
            y2 = y + rookMoves[i][1]*j;
            int ownKing = isOwnKing(board, x2, y2, player);
            if (isValid(board, x2, y2, player) || ownKing) {
                if (board[y2][x2][1] == 4 || board[y2][x2][1] == 5 || (j == 1 && board[y2][x2][1] == 6 && !ownKing)) {
                    return 1;
                }
                else if (board[y2][x2][0] != 0 && !ownKing) {
                    break;
                }
            }
            else {
                break;
            }
        }
    }

    // check knights
    for (i=0; i < 8; i++) {
        x2 = x + knightMoves[i][0];
        y2 = y + knightMoves[i][1];
        if (isValid(board, x2, y2, player) && board[y2][x2][1] == 2) {
            return 1;
        }
    }
    
    // check pawns
    for (i = -1; i < 1; i += 2) {
        x2 = x + i;
        y2 = y + facing;
        if (isValid(board, x2, y2, player) && board[y2][x2][1] == 1) {
            return 1;
        }
    }

    return 0;
}


// move piece
void movePiece(int board[8][8][3], int x1, int y1, int x2, int y2, int turnNum) {
    int i;

    if (y2 >= 0) {
        for (i = 0; i < 3; i++) {
            board[y2][x2][i] = board[y1][x1][i];
            board[y1][x1][i] = 0;
        }
        board[y2][x2][2] = turnNum;

        if ((y2 == 0 || y2 == 7) && board[y2][x2][1] == 1) {
            board[y2][x2][1] = 5;
        }
    }
    // en pessant
    else if (y2 == -1) {
        int facing;
        if (board[y1][x1][0] == 1) {
            facing = -1;
        }
        else{
            facing = 1;
        }
        for (i = 0; i < 3; i++) {
            board[y1][x2][i] = 0;
        }
        for (i = 0; i < 3; i++) {
            board[y1+facing][x2][i] = board[y1][x1][i];
            board[y1][x1][i] = 0;
        }
        board[y2][x2][2] = turnNum;
    }

    // castle
    else if (y2 == -2) {
        y2 = y1;
        for (i = 0; i < 3; i++) {
            board[y2][x2][i] = board[y1][x1][i];
            board[y1][x1][i] = 0;
        }
        board[y2][x2][2] = turnNum;
        
        if (x2 == 2) {
            x1 = 0;
            x2 = 3;
        }
        if (x2 == 6) {
            x1 = 7;
            x2 = 5;
        }

        for (i = 0; i < 3; i++) {
            board[y2][x2][i] = board[y1][x1][i];
            board[y1][x1][i] = 0;
        }
        board[y2][x2][2] = turnNum;
    }
}


// creates a copy of the given chess board
void copyBoard(int board[8][8][3], int boardCopy[8][8][3]) {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            for (int i = 0; i < 3; i++) {
                boardCopy[y][x][i] = board[y][x][i];
            }
        }
    }
}


// check if proposed move is valid
int isValid(int board[8][8][3], int x, int y, int player) {
    if (x >= 0 && x < 8 && y >= 0 && y < 8 && board[y][x][0] != player) {
        return 1;
    }
    return 0;
}

int isOwnKing(int board[8][8][3], int x, int y, int player) {
    if (x >= 0 && x < 8 && y >= 0 && y < 8 && board[y][x][1] == 6) {
        return 1;
    }

    return 0;
}

// generates all the possible moves for the given player, y2 = -1 = en passent, y2 = -2 = castle
void generateChildren(int board[8][8][3], int possibleMoves[][4], int player, int turnNum) {

    int i;
    int j;
    int y;
    int x;
    int x2;
    int y2;
    int y3;
    int index = 0;
    int facing;

    if (player == 1) {
        facing = -1;
    }
    else if (player == 2) {
        facing = 1;
    }

    for (y = 0; y < 8; y++) {
        for (x = 0; x < 8; x++) {
            // check if correct player
            if (board[y][x][0] == player) {
                
                // pawn
                if (board[y][x][1] == 1) {
                    // move 1 forward
                    x2 = x;
                    y2 = y + facing;
                    if (x2 >= 0 && x2 < 8 && y2 >= 0 && y2 < 8 && board[y2][x2][0] == 0) {
                        possibleMoves[index][0] = x;
                        possibleMoves[index][1] = y;
                        possibleMoves[index][2] = x2;
                        possibleMoves[index][3] = y2;
                        index++;
                    }
                    
                    // move 2 forward
                    if (board[y][x][2] == 0) {
                        x2 = x;
                        y2 = y + facing*2;
                        y3 = y + facing;
                        if (x2 >= 0 && x2 < 8 && y2 >= 0 && y2 < 8 && board[y2][x2][0] == 0 && board[y3][x2][0] == 0) {
                            possibleMoves[index][0] = x;
                            possibleMoves[index][1] = y;
                            possibleMoves[index][2] = x2;
                            possibleMoves[index][3] = y2;
                            index++;
                        }
                    }

                    // take
                    for (i=-1; i < 2; i += 2) {
                        x2 = x + i;
                        y2 = y + facing;
                        if (x2 >= 0 && x2 < 8 && y2 >= 0 && y2 < 8 && board[y2][x2][0] != 0 && board[y2][x2][0] != player) {
                            possibleMoves[index][0] = x;
                            possibleMoves[index][1] = y;
                            possibleMoves[index][2] = x2;
                            possibleMoves[index][3] = y2;
                            index++;
                        }
                    }

                    // en passent
                    for (i=-1; i < 2; i += 2) {
                        x2 = x + i;
                        y2 = y + facing;
                        if (x2 >= 0 && x2 < 8 && y2 >= 0 && y2 < 8 && board[y2][x2][0] == 0) {
                            if (board[y][x2][0] != 0 && board[y][x2][0] != player && board[y][x2][1] == 1 && board[y][x2][2] == turnNum-1) {
                                possibleMoves[index][0] = x;
                                possibleMoves[index][1] = y;
                                possibleMoves[index][2] = x2;
                                possibleMoves[index][3] = -1;
                                index++;
                            }
                        }
                    }

                }   

                // knight
                if (board[y][x][1] == 2) {
                    for (i=0; i < 8; i++) {
                        x2 = x + knightMoves[i][0];
                        y2 = y + knightMoves[i][1];
                        if (isValid(board, x2, y2, player)) {
                            possibleMoves[index][0] = x;
                            possibleMoves[index][1] = y;
                            possibleMoves[index][2] = x2;
                            possibleMoves[index][3] = y2;
                            index++;
                        }
                    }
                }

                // bishop
                if (board[y][x][1] == 3) {
                    for (i=0; i < 4; i++) {
                        for (j=1; j < 8; j++) {
                            x2 = x + bishopMoves[i][0]*j;
                            y2 = y + bishopMoves[i][1]*j;
                            if (isValid(board, x2, y2, player)) {
                                possibleMoves[index][0] = x;
                                possibleMoves[index][1] = y;
                                possibleMoves[index][2] = x2;
                                possibleMoves[index][3] = y2;
                                index++;
                                if (board[y2][x2][0] != 0) {
                                    break;
                                }
                            }
                            else {
                                break;
                            }
                        }
                    }
                }

                // rook
                if (board[y][x][1] == 4) {
                    for (i=0; i < 4; i++) {
                        for (j=1; j < 8; j++) {
                            x2 = x + rookMoves[i][0]*j;
                            y2 = y + rookMoves[i][1]*j;
                            if (isValid(board, x2, y2, player)) {
                                possibleMoves[index][0] = x;
                                possibleMoves[index][1] = y;
                                possibleMoves[index][2] = x2;
                                possibleMoves[index][3] = y2;
                                index++;
                                if (board[y2][x2][0] != 0) {
                                    break;
                                }
                            }
                            else {
                                break;
                            }
                        }
                    }
                }

                // queen
                if (board[y][x][1] == 5) {
                    for (i=0; i < 8; i++) {
                        for (j=1; j < 8; j++) {
                            x2 = x + queenMoves[i][0]*j;
                            y2 = y + queenMoves[i][1]*j;
                            if (isValid(board, x2, y2, player)) {
                                possibleMoves[index][0] = x;
                                possibleMoves[index][1] = y;
                                possibleMoves[index][2] = x2;
                                possibleMoves[index][3] = y2;
                                index++;
                                if (board[y2][x2][0] != 0) {
                                    break;
                                }
                            }
                            else {
                                break;
                            }
                        }
                    }
                }

                // king
                if (board[y][x][1] == 6) {
                    for (i=0; i < 8; i++) {
                        x2 = x + kingMoves[i][0];
                        y2 = y + kingMoves[i][1];
                        if (isValid(board, x2, y2, player) && !inCheck(board,x2,y2,player)) {
                            possibleMoves[index][0] = x;
                            possibleMoves[index][1] = y;
                            possibleMoves[index][2] = x2;
                            possibleMoves[index][3] = y2;
                            index++;
                        }
                    }
                    // castle
                    if (board[y][x][2] == 0 && board[y][0][1] == 4 && board[y][0][2] == 0 && board[y][1][0] == 0 && board[y][2][0] == 0 && board[y][3][0] == 0) {
                        if (inCheck(board,x,y,player) == 0 && inCheck(board,3,y,player) == 0) {
                            possibleMoves[index][0] = x;
                            possibleMoves[index][1] = y;
                            possibleMoves[index][2] = 2;
                            possibleMoves[index][3] = -2;
                            index++;
                        }
                    }
                    if (board[y][x][2] == 0 && board[y][7][1] == 4 && board[y][7][2] == 0 && board[y][6][0] == 0 && board[y][5][0] == 0) {
                        if (inCheck(board,x,y,player) == 0 && inCheck(board,5,y,player) == 0) {
                            possibleMoves[index][0] = x;
                            possibleMoves[index][1] = y;
                            possibleMoves[index][2] = 6;
                            possibleMoves[index][3] = -2;
                            index++;
                        }
                    }
                }
            }
        }
    }

    possibleMoves[index][0] = -1;
    possibleMoves[index][1] = -1;
    possibleMoves[index][2] = -1;
    possibleMoves[index][3] = -1;

}

int numMoves(int board[8][8][3], int player, int turnNum) {
    int possibleMoves[200][4];
    generateChildren(board, possibleMoves, player, turnNum);
    int numMoves = 0;
    for (int i = 0; i < 200; i++) {
        if (possibleMoves[i][0] == -1) {
            break;
        }
        numMoves++;
    }

    return numMoves;
}

// minimax function
double minimax(int board[8][8][3], int depth, int alpha, int beta, int player, int turnNum) {
    count++;

    int boardCopy[8][8][3];
    int possibleMoves[200][4];
    double eval;

    // check if it is the end of a branch
    double currentEval = evaluate(board);
    if (depth < 1) {
        return currentEval;
    }

    // if a checkmate happened, prioritize earlier mates
    if (abs(currentEval) > 100) {
        return currentEval*depth;
    }

    // evaluate for white (maximizing player)
    if (player == 1) {

        double maxEval = -1000000;
        generateChildren(board, possibleMoves, player, turnNum);
        for (int i = 0; i < 200; i++) {
            if (possibleMoves[i][0] == -1) {
                break;
            } 

            copyBoard(board, boardCopy);
            movePiece(boardCopy, possibleMoves[i][0], possibleMoves[i][1], possibleMoves[i][2], possibleMoves[i][3], turnNum);

            eval = minimax(boardCopy, depth - 1, alpha, beta, 2, turnNum+1);
            if (eval > maxEval) {
                maxEval = eval;
            }
            if (eval > alpha) {
                alpha = eval;
            }
            if (beta <= alpha) {
                break;
            }
        }

        return maxEval;
    }

    // evaluate for black (minimizing player)
    else {
        double minEval = 1000000;
        generateChildren(board, possibleMoves, player, turnNum);
        for (int i = 0; i < 200; i++) {
            if (possibleMoves[i][0] == -1) {
                break;
            } 

            copyBoard(board, boardCopy);
            movePiece(boardCopy, possibleMoves[i][0], possibleMoves[i][1], possibleMoves[i][2], possibleMoves[i][3], turnNum);

            eval = minimax(boardCopy, depth - 1, alpha, beta, 1, turnNum+1);
            if (eval < minEval) {
                minEval = eval;
            }
            if (eval < beta) {
                beta = eval;
            }
            if (beta <= alpha) {
                break;
            }
        }
        return minEval;
    }
}


// print the given board to the command line
void printBoard(int board[8][8][3]) {
    int x;
    int y;
    int colour;
    char symbol;
    int bold;

    for (int i = 0; i < 2; i++) {
        printf("\n");
    }

    for (y = 0; y < 8; y++) {
        
        printf("\033[0;30m%d   ", 8-y);

        for (x = 0; x < 8; x++) {
            colour = 0;
            symbol = '-';
            bold = 1;
            if (board[y][x][0] == 1) {
                colour = 2;
            }
            else if (board[y][x][0] == 2) {
                colour = 1;
            }
            if (colour != 0) {
                if (board[y][x][1] == 1) {
                    symbol = 'p';
                    bold = 0;
                }
                else if (board[y][x][1] == 2) {
                    symbol = 'n';
                }
                else if (board[y][x][1] == 3) {
                    symbol = 'b';
                }
                else if (board[y][x][1] == 4) {
                    symbol = 'r';
                }
                else if (board[y][x][1] == 5) {
                    symbol = 'q';
                }
                else if (board[y][x][1] == 6) {
                    symbol = 'k';
                }
            }

            printf("\033[%d;3%dm%c ", bold, colour, symbol);

        }
        printf("\n");
    }

    printf("\n\033[0;30m    a b c d e f g h ", labels[y]);
    printf("\n\n");

}

void writeBoard(int board[8][8][3]) {
    FILE *commFile;
    commFile = fopen("data.txt", "w");

    int i,j,k;

    // signify last edit by C
    fprintf(commFile, "c?");
    
    for (i = 0; i < 8; i++) {
        for  (j = 0; j < 8; j++) {
            for (k = 0; k < 3; k++) {
                fprintf(commFile, "%d", board[i][j][k]);
                if (k != 2) {
                    fprintf(commFile, ",");
                }
            }
            if (j != 7) {
                fprintf(commFile, ".");
            }
        }
        if (i != 7) {
            fprintf(commFile, "/");
        }
    }

    fclose(commFile);

}

int readBoard(int board[8][8][3]) {
    
    FILE *commFile;
    commFile = fopen("data.txt", "r");

    char _type;
    int _value;

    fscanf(commFile, "%c", &_type);
    if (_type == 'p') {
        int i = 0, j = 0,k = 0;

        while(fscanf(commFile, "%c", &_type) == 1) {
            fscanf(commFile, "%d", &_value);

            if (_type == ',') {
                k += 1;
            }
            else if (_type == '.') {
                k = 0;
                j += 1;
            }
            else if (_type == '/') {
                k = 0;
                j = 0;
                i += 1;
            }

            board[i][j][k] = _value;
        }
        fclose(commFile);
        return 1;

    }
    
    fclose(commFile);
    return 0;
}

// main function
int main(void) {
    
    int boardCopy[8][8][3];
    int possibleMoves[200][4];
    double eval;
    int turnNum = 1;
    int i;
    double minEval;
    double maxEval;
    int x1;
    int y1;
    int x2;
    int y2;
    char c1;
    char c2;
    int loop;
    char garbage;
    int startingDepth = 5;

    srand ( time ( NULL));

    for (int i = 0; i < 500; i++) {
        seenPositions[i] = -1;
    }

    while (1) {
        
        // ############################### PYTHON move (white) ############################################################################################################################
        int moveCount = numMoves(globalBoard, 1, turnNum);
        printf("WAITING FOR WHITE... %d POSSIBLE MOVES\n", moveCount);
        while (!readBoard(globalBoard)) {
            sleep(1);
        }
        seenPositions[turnNum] = hashBoard(globalBoard);
        turnNum++;

        // ############################### AI move (black) ############################################################################################################################
        count = 0;

        // check if game over
        if (abs(evaluate(globalBoard)) > 500) {
            break;
        }

        printf("BLACK THINKING... \n");

        minEval = 1000000;
        generateChildren(globalBoard, possibleMoves, 2, turnNum);
        x1 = possibleMoves[0][0];
        y1 = possibleMoves[0][1];
        x2 = possibleMoves[0][2];
        y2 = possibleMoves[0][3];

        for (i = 0; i < 200; i++) {
            if (possibleMoves[i][0] == -1) {
                break;
            }

            copyBoard(globalBoard, boardCopy);
            movePiece(boardCopy, possibleMoves[i][0], possibleMoves[i][1], possibleMoves[i][2], possibleMoves[i][3], turnNum);

            // prune if moving into check
            if (inCheck(boardCopy, -1, -1, 2)) {
                continue;
            }

            eval = minimax(boardCopy, startingDepth, -1000000, 1000000, 1, turnNum);
            eval = evalShift(boardCopy, eval);
            if (abs(eval) < 100 && numMoves(boardCopy, 1, turnNum+1) < 1) {
                eval = 0;
            }
            if (abs(eval) < 100 && seenPosition(hashBoard(boardCopy), turnNum) > 1) {
                eval = 0;
            }
            
            if (eval <= minEval) {
                minEval = eval;
                x1 = possibleMoves[i][0];
                y1 = possibleMoves[i][1];
                x2 = possibleMoves[i][2];
                y2 = possibleMoves[i][3];
            }
        }

        movePiece(globalBoard, x1, y1, x2, y2, turnNum);
        seenPositions[turnNum] = hashBoard(globalBoard);
        turnNum++;
        
        writeBoard(globalBoard);

        for (int i = 0; i < turnNum; i++) {
            printf("\n%d", seenPositions[i]);
        }
    }

    return 0;
}