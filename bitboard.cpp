#include "bitboard.hpp"

void Clear()
{
#if defined _WIN32
    system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
#elif defined (__APPLE__)
    system("clear");
#endif
}

void bit_display(uint64_t mask){
    char c;
    for(int y = 7; y > -1; y--){
        std::cout << "   -------------------------------\n"<< y + 1<< " |";
        for(int i = 0; i < 8; i++){
            if(mask & (1UL << (8 * y + i))) c = '*';
            else c = ' ';
            std::cout << " " <<  c << " |";
        }
        std::cout << std::endl;
    }
    std::cout << "   -------------------------------\n  ";
    std::cout << "  a   b   c   d   e   f   g   h\n";
}  



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
    // make sure to pass a legal FEN string
    int Index = 55;
    int i = 0;
    for(; (fen[i] != ' '); i++){
        if(fen[i] > 65){
            // Certain that it is an alphabet
            board[map(fen[i])] |= 1L << ++Index;
        }
        else if(fen[i] > 48){
            // Certain that it is a number
            Index += fen[i] - 48;
        }
        else Index -= 16;
    }
    std::cout << (uint64_t) (((1l << 62) | (1l << 63) | (1l << 60))) << std::endl;
    std::cout << fen[i + 3]<< std::endl;
    i += 3;
    while(fen[i] != ' '){
        switch (fen[i])
        {
        case 'K':
        Castling[1] = 208;
        break;
        case 'Q':
        Castling[0] = 21;
        break;
        case 'k':
        Castling[3] = 14987979559889010688ul;
        break;
        case 'q':
        Castling[2] = 1513209474796486656;
        break;
        default:
            break;
        }
        i++;
    }
    UpdateBoardState();
    std:: cout << fen[i + 1] << std::endl;
    i++;
    if(fen[i] != '-'){
        int a = (fen[i + 1] - '1') * 8 + (fen[i] - 'a');
        std::cout << (fen[i + 1] - '1')<< std::endl;
        std::cout << a << std::endl;
        Enpassant = 1ul << a;
    }
    DisplayBoard();
    for(int i = 0; i < 4; i++) bit_display(Castling[i]); 
    std::cout << "Enpassant is : " << Enpassant << std::endl;
}

uint64_t bitboard::GenerateMoves(uint64_t pos, char PieceID){
    return ~0UL;
}

void bitboard::UpdateBoardState(){
    for(int i = 0; i < 6; i++){
        white |= board[i];
        black |= board[6 + i];
    }
    EntireBoard = white | black;
    Enpassant = 0;
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
        // If you can castle
        if(PieceID == Blackturn * 6){
            // If it is a King
            if((Castling[Blackturn * 2] & Position) && (Castling[Blackturn * 2] & Destination)){
                MakeMove(2 * Blackturn + 2, Destination >> 2, Destination << 1, Skip);
            }
            else if((Castling[Blackturn * 2 + 1] & Position) && (Castling[Blackturn * 2 + 1] & Destination)){
                MakeMove(2 * Blackturn + 2, Destination << 1, Destination >> 1, Skip);
            }
            Castling[Blackturn * 2] = Castling[Blackturn * 2 + 1] = 0;
        }
        else if(PieceID == Blackturn * 6 + 2){
            // If it is a rook
            if(Castling[Blackturn * 2] & (Position)) Castling[Blackturn * 2] = 0;
            else Castling[Blackturn * 2 + 1] = 0;
        }
    }

    board[PieceID] &= ~Position;

    if(PieceID == Blackturn * 6 + 5){
        switch (f)
        {
        case None:
            break;

        case PromotionToQ:
            PieceID = 1;
            break;
        
        case PromotionToR:
            PieceID = 2;
            break;

        case PromotionToB:
            PieceID = 3;
            break;

        case PromotionToN:
            PieceID = 4;
            break;

        case EnpassantFlag:
            board[Blackturn * 6 + 5] &= (Blackturn)? ~(Enpassant << 8): ~(Enpassant >> 8);
            break;

        }
    }
    if(Destination & EntireBoard){
        // Capture move
        board[getPieceID(Destination, (Blackturn)? 0: 6)] &= ~Destination;
    }
    board[PieceID] |= Destination;
    UpdateBoardState();
}

void bitboard::DisplayBoard(uint64_t MovesGenerated){
    char Index = 55;
    char indicator;
    int PieceID;
    for(int y = 7; y > -1; y--){
        std::cout << "   ---------------------------------------\n"<< y + 1<< " |";
        for(int i = 0; i < 8; i++){
            if(EntireBoard & (1UL << ++Index)){   // checking for the existence of a piece
                for(PieceID = 0; PieceID < 12; PieceID++){
                    if(board[PieceID] & (1UL << Index)) break;
                }
            }
            else PieceID = 12;  // No piece found at the position
            if(MovesGenerated & (1UL << Index)) indicator = '*';
            else indicator = ' ';
            std::cout << " " <<  Symbols[PieceID] << " "<< indicator <<"|";
        }
        Index -= 16;
        std::cout << std::endl;
    }
    std::cout << "   ---------------------------------------\n  ";
    std::cout << "  a    b    c    d    e    f    g    h\n\n";
}

bool bitboard::Interface(const char Move[], bool WhiteTurn){
    for(int i = 0; i < 4; i++){
        if(i % 2){
            if(Move[i] < '1'|| Move[i] > '8'){
                std::cout << "Invalid Numeric Index. Range is 1-8.\n";
                return false;
            }
        }
        else {
            if(Move[i] < 'a' || Move[i] > 'h'){
                std::cout << "Invalid Alphabet Index. Range is A-H.\n";
                return false;
            }
        }
    }
    
    uint64_t PositionMask = 1ul << ((Move[1] - '1') * 8 +  Move[0] - 'a');
    if(WhiteTurn){
        if(PositionMask & ~white){ std:: cout << "\nSelect a white piece.\n"; return false;}
    }
    else if(PositionMask & ~black){ std:: cout << "\nSelect a Black piece.\n"; return false;}

    uint64_t DestinationMask = 1ul << ((Move[3] - '1') * 8 +  Move[2] - 'a');
    char PieceID = getPieceID(PositionMask, (WhiteTurn)? 0: 6);
    uint64_t GeneratedMoves = GenerateMoves(PositionMask, PieceID);
    flag f = None;
    
    if(DestinationMask & GeneratedMoves){
        // Legal Move
        if(PieceID == 5 || PieceID == 11){
            if(DestinationMask & 18374686479671623935ul){
                // Promotion Move
                std:: cout << "That's is a promotion move!!!\n" 
                << "Enter a character from { q, r, b, n }\nInput (default=q): ";
                char c;
                f = PromotionToQ;
                std:: cin >> c;
                switch (c)
                {
                case 'r':
                    f = PromotionToR;
                    break;
                case 'b':
                    f = PromotionToB;
                    break;
                case 'n':
                    f = PromotionToN;
                    break;
                default:
                    std::cout << "Taking default value.\n";
                }
            }           
        }
    }
    else{
        // Illegal move
        std:: cout << "Illegal move.\n";
        return false;
    }
    Clear();
    DisplayBoard(GeneratedMoves);
    MakeMove(PieceID, PositionMask, DestinationMask, f);
    return true;
}
int bitboard::getPieceID(uint64_t pos, char turn){
    for(int i = 0; i < 6; i++) if(board[turn + i] & pos) return turn + i;
    return -1;
}
bool bitboard::TheEnd(){
    return false;
}
void King::Generate(){
    // for()
}
int main(){
    //bitboard b("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ h8 1 8");
    bitboard b;
    //b.MakeMove(10, 1, 1ul << 12);
    bool IsWhite = true;
    char Move[5]; // mandatorily sized
    b.DisplayBoard ();
    // while(!b.TheEnd()){
    //     for(int i = 0; ; i++){
    //         std::cin.get(Move[i % 5]);
    //         if(Move[i % 5] == 10) break;
    //     }
    //     if(b.Interface(Move, IsWhite)){
    //         IsWhite = !IsWhite;
    //         b.DisplayBoard();
    //     }
    // }
    
    

    return 0;
}