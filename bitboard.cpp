#include "bitboard.hpp"
#define NOT_FILE_A 18374403900871474942ul
#define NOT_FILE_H 18374403900871474942ul
#define RANK_2_7 71776119061282560ul

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
    char fen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
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
    i++;
    if(fen[i] == 'b') WhiteToPlay = false;
    i += 2;
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
    i++;
    if(fen[i] != '-'){
        int a = (fen[i + 1] - '1') * 8 + (fen[i] - 'a');
        std::cout << (fen[i + 1] - '1')<< std::endl;
        std::cout << (fen[i] - 'a')<< std::endl;
        std::cout << a << std::endl;
        Enpassant = 1ul << a;
    }
    else Enpassant = 0;
}

uint64_t bitboard::GenerateMoves(uint64_t pos, char PieceID, int x, int y){
    uint64_t ResultantMoves = (PieceID / 6)? ~black: ~white;
    switch (PieceID % 6)
    {
    case 0:
        ResultantMoves &= KingMoves[8 * y + x];
        if(Castling[(PieceID/6) * 2]) ResultantMoves |= 1ul << y * 8 + x - 2;
        if(Castling[(PieceID/6) * 2 + 1]) ResultantMoves |= 1ul << y * 8 + x + 2;
        break;
    case 1:
        ResultantMoves &= RookMoves(x, y) | BishopMoves(x, y);
        break;
    case 2:
        ResultantMoves &= RookMoves(x, y);
        break;
    case 3:
        ResultantMoves &= BishopMoves(x, y);
        break;
    case 4:
        ResultantMoves &= KnightMoves[8 * y + x];
        break;
    case 5:
        ResultantMoves = PawnMoves(x, y);
        break;
    }
    return ResultantMoves;
}

void bitboard::UpdateBoardState(){
    white = 0;
    black = 0;
    for(int i = 0; i < 6; i++){
        white |= board[i];
        black |= board[6 + i];
    }
    EntireBoard = white | black;
    Enpassant = 0;
}

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
            board[!Blackturn * 6 + 5] &= (Blackturn)? ~(Enpassant << 8): ~(Enpassant >> 8);
            break;

        }
    }

    if(Destination & EntireBoard){
        // Capture move
        char CapturedPID = getPieceID(Destination, (Blackturn)? 0: 6);
        board[CapturedPID] &= ~Destination;
        if(CapturedPID % 6 == 2){
            if(Destination & 0x8100000000000081){
                if(Castling[(!Blackturn) * 2] & Destination)
                Castling[!Blackturn * 2] = 0;
                else if(Castling[(!Blackturn) * 2 + 1] & Destination)
                Castling[(!Blackturn) * 2 + 1] = 0;
            }
        }
    }
    board[PieceID] |= Destination;
    UpdateBoardState();
    if(PieceID % 6 == 5 && Position & RANK_2_7){
        if((Position >> 16)  & Destination) Enpassant = Position >> 8;
        else if((Position << 16) & Destination) Enpassant = Position << 8;
    }
    else Enpassant = 0;
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
    int x = Move[0] - 'a';
    int y = Move[1] - '1';
    
    uint64_t PositionMask = 1ul << ((Move[1] - '1') * 8 +  Move[0] - 'a');
    if(WhiteTurn){
        if(PositionMask & ~white){ std:: cout << "\nSelect a white piece.\n"; return false;}
    }
    else if(PositionMask & ~black){ std:: cout << "\nSelect a Black piece.\n"; return false;}

    uint64_t DestinationMask = 1ul << ((Move[3] - '1') * 8 +  Move[2] - 'a');
    char PieceID = getPieceID(PositionMask, (WhiteTurn)? 0: 6);
    uint64_t GeneratedMoves = GenerateMoves(PositionMask, PieceID, x, y);
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
            else if (DestinationMask & Enpassant){
                // Enpassant Move
                 f = EnpassantFlag;         
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
uint64_t bitboard::RookMoves(int x, int y){
    uint64_t poskey = 1ul << (y * 8 + x + 8);
    uint64_t GeneratedMoves = 0;

    for(int i = y + 1; i < 8; i++){
        GeneratedMoves |= poskey;
        if(EntireBoard & poskey) break;
        poskey <<= 8;
    }
    poskey = 1ul << (y * 8 + x - 8);
    for(int i = 0; i < y; i++){
        GeneratedMoves |= poskey;
        if(EntireBoard & poskey) break;
        poskey >>= 8;
    }
    poskey = 1ul << (y * 8 + x + 1);
    for(int i = x + 1; i < 8; i++){
        GeneratedMoves |= poskey;
        if(EntireBoard & poskey) break;
        poskey <<= 1;
    }
    poskey = 1ul << (y * 8 + x - 1);
    for(int i = 0; i < (x - 1); i++){
        GeneratedMoves |= poskey;
        if(EntireBoard & poskey) break;
        poskey >>= 1;
    }
    return GeneratedMoves;
}
uint64_t bitboard::BishopMoves(int x, int y){
    int size = x - y;
    int currentpos = ((x) < (y))? x:y;
    uint64_t poskey = 1ul << (y * 8 + x + 9);
    uint64_t GeneratedMoves = 0;

    size = (size < 0)? 8 + size: 8 - size;

    for(int i = currentpos + 1; i < size; i++){
        GeneratedMoves |= poskey;
        if(EntireBoard & poskey) break;
        poskey <<= 9;
    }

    poskey = 1ul << (y * 8 + x - 9);

    for(int i = 0; i < currentpos; i++){
        GeneratedMoves |= poskey;
        if(EntireBoard & poskey) break;
        poskey >>= 9;
    }
    
    size = (7 - x) + y;
    size = (size < 0)? 8 + size: 8 - size;
    currentpos = ((7 - x) < (y))? 7 - x: y;

    poskey = 1ul << (y * 8 + x + 7);
    for(int i = currentpos + 1; i < size; i++){
        GeneratedMoves |= poskey;
        if(EntireBoard & poskey)break;
        poskey <<= 7;
    }
    bit_display(GeneratedMoves);

    poskey = 1ul << (y * 8 + x - 7);

    for(int i = 0; i < currentpos; i++){
        GeneratedMoves |= poskey;
        if(EntireBoard & poskey) break;
        poskey >>= 7;
    }
    bit_display(GeneratedMoves);
    return GeneratedMoves;
}
uint64_t bitboard::PawnMoves(int x, int y){

    uint64_t poskey = 1ul << 8 * y + x;
    uint64_t temp = 0;

    int offset = (board[5] & poskey)?  1: -1;
    uint64_t GeneratedMoves = 0;

    temp = (offset == -1)? poskey >> 8:poskey << 8;

    if(temp & ~(EntireBoard)){
        GeneratedMoves |= temp;
        if(RANK_2_7 & poskey){
            GeneratedMoves |= (offset == -1)? temp >> 8: temp << 8;
            GeneratedMoves &= ~EntireBoard;
        }
    }

    if(NOT_FILE_A & poskey)temp = (offset == -1)? poskey >> 9: poskey << 7;
    if(NOT_FILE_H & poskey)temp |= (offset == -1)? poskey >> 7: poskey << 9;
    poskey = (offset == -1)? white: black;
    bit_display(temp);
    bit_display(poskey);

    GeneratedMoves |= poskey & temp;

    if(temp & Enpassant) GeneratedMoves |= Enpassant;

    return GeneratedMoves;
}

void bitboard::game(){
    char Move[5]; // mandatorily sized
    DisplayBoard ();
    while(!TheEnd()){
        for(int i = 0; ; i++){
            std::cin.get(Move[i % 5]);
            if(Move[i % 5] == 10) break;
        }
        if(Interface(Move, WhiteToPlay)){
            WhiteToPlay = !WhiteToPlay;
            DisplayBoard();
        }
    }   

}


int main(){
    // bitboard b;
    bitboard b("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Qnp/PPPBBPPP/R3K2R w KQkq -");
    b.game();
    return 0;
}