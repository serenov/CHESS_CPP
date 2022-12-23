#include "bitboard.hpp"

int map(const char a){
    int isBlack = 0;
    if(a > 97) isBlack = 1;
    if((a & 223) == 'P') return isBlack * 6 + 5;
    if((a & 223) == 'N') return isBlack * 6 + 4;
    if((a & 223) == 'B') return isBlack * 6 + 3;
    if((a & 223) == 'R') return isBlack * 6 + 2;
    if((a & 223) == 'Q') return isBlack * 6 + 1;
    if((a & 223) == 'K') return isBlack * 6;
    return 0;
}

bitboard::bitboard(){
    const char fen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; 
    LoadFEN(fen);
}

bitboard::bitboard(const char fen[]){
    LoadFEN(fen);
}

void bitboard::LoadFEN(const char fen[]){
    int Index = 55;
    for(int i = 0; (fen[i] != ' '); i++){
        if(fen[i] > 65){
            // Sure that it is an alphabet
            board[map(fen[i])] |= 1L << ++Index;
        }
        else if(fen[i] > 48){
            // Sure that it is a number
            Index += fen[i] - 48;
        }
        else Index -= 16;
    }
    UpdateBoardState();
    DisplayBoard();
}

uint64_t bitboard::GenerateMoves(){
    return !(1L);
}

void bitboard::UpdateBoardState(){
    for(int i = 0; i < 6; i++){
        white |= board[i];
        black |= board[6 + i];
    }
    EntireBoard = white | black;
}

// void bitboard::MakeMove(char PieceID, uint64_t Position, uint64_t Destination, const char){

//     char Offset = (PieceID > 5)? 3: 0;
//     if(notMoved[Offset]){
//         if(PieceID == Offset * 2){
//             if((Position | Destination) & 1L){
//                 if(Position > Destination)
//                     MakeMove(2 * Offset + 2, Destination << 1, Destination >> 1);
//                 else
//                     MakeMove(2 * Offset + 2, Destination >> 2, Destination << 1);
//             };
//             notMoved[Offset] = false;
//         }
//         if(PieceID == Offset * 2 + 2){
//             if(Position & 1L) notMoved[Offset + 2] = false;
//             else notMoved[Offset + 1] = false;
//         }
//     }
//     if(PieceID == Offset * 2 + 5){
//         if();
//     }

//     board[PieceID] &= !Position;
//     board[PieceID] |= Destination;
// }
 
void bitboard::MakeMove(char PieceID, uint64_t Position, uint64_t Destination, flag f){
    char Blackturn = (PieceID > 5)? 1: 0;

    if(Castling[Blackturn * 2] | Castling[Blackturn * 2 + 1]){
        if(PieceID == Blackturn * 6){
            if(Castling[Blackturn * 2] & (Position | Destination)){
                MakeMove(2 * Blackturn + 2, Destination >> 2, Destination << 1, Skip);
            }
            else if(Castling[Blackturn * 2 + 1] & (Position | Destination)){
                MakeMove(2 * Blackturn + 2, Destination << 1, Destination >> 1, Skip);
            }
            Castling[Blackturn * 2] = Castling[Blackturn * 2 + 1] = 0;
        }
        else if(PieceID == Blackturn * 6 + 2){
            if()
        }
    }

    board[PieceID] &= !Position;

    if(PieceID == Blackturn * 6 + 5){
        // Handle pawn's move
        if(f < None);
        // Handling promotions
        if(f == PromotionToQ){
            PieceID = Blackturn * 6 + 1;
        }
        else if(f == PromotionToR){
            PieceID = Blackturn * 6 + 2;
        }
        else if(f == PromotionToB){
            PieceID = Blackturn * 6 + 3;
        }
        else if(f == PromotionToN){
            PieceID = Blackturn * 6 + 4;
        }
        // Handling Enpassant
    }
    board[PieceID] |= Destination;
}

void bitboard::DisplayBoard(){
    char Index = 55;
    int PieceID;
    for(int y = 7; y > -1; y--){
        std::cout << "   -------------------------------\n"<< y + 1<< " |";
        for(int i = 0; i < 8; i++){
            if(EntireBoard & (1L << ++Index)){   // checking the existence of a piece
                for(PieceID = 0; PieceID < 12; PieceID++){
                    if(board[PieceID] & (1L << Index)) break;
                }
            }
            else PieceID = 12;  // No piece found at the position
            std::cout << " " <<  Symbols[PieceID] << " |";
        }
        Index -= 16;
        std::cout << std::endl;
    }
    std::cout << "   -------------------------------\n  ";
    std::cout << "  a   b   c   d   e   f   g   h\n";
}



int main(){
    // bitboard b("8/2K1P3/4p3/p1b2pp1/4R2P/2kPB2q/2b5/n1N5 w - - 0 1");
    bitboard b;
    int count = 0;
    return 0;
}