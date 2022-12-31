#include<iostream>
#include <chrono>
#include <unistd.h>
 
enum flag {
    PromotionToQ,
    PromotionToR,
    PromotionToB,
    PromotionToN,
    EnpassantFlag,
    Skip,
    None
};

class bitboard
{
private:
    uint64_t board[12] = {};
    uint64_t white = 0;
    uint64_t black = 0;
    uint64_t EntireBoard = 0;
    bool IsMoved[6]; // for castling purposes i.e, is king moved?, is rook moved?
    uint64_t Castling[4] = {};
    uint64_t Enpassant;

public:
    bitboard();
    bitboard(const char fen[]);
    void LoadFEN(const char fen[]);
    void UpdateBoardState();
    uint64_t GenerateMoves();
    int getPiece(uint64_t pos, char turn);
    void MakeMove(char PieceID, uint64_t Position, uint64_t Destination, flag f = None);
    void DisplayBoard();
    void Interface(const char Move[]);

    const char Symbols[13] = 
    {'K', 'Q', 'R', 'B', 'N', 'P',
     'k', 'q', 'r', 'b', 'n', 'p',
     ' '};

};