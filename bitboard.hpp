#include<iostream>
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
    uint64_t Castling[4] = {};
    uint64_t Enpassant;
    bool WhiteToPlay = true;


public:
    bitboard(const char fen[]);
    bitboard();
    void LoadFEN(const char fen[]); // Only legal fen strings are allowed. Game glitches otherwise.
    void UpdateBoardState();
    uint64_t GenerateMoves(uint64_t pos, char PieceID, int x, int y);
    int getPieceID(uint64_t pos, char turn);
    void MakeMove(char PieceID, uint64_t Position, uint64_t Destination, flag f);
    bool TheEnd();
    void DisplayBoard(uint64_t GenerateMoves = 0);  
    bool Interface(const char Move[], bool WhiteTurn);
    uint64_t RookMoves(int x, int y);
    uint64_t BishopMoves(int x, int y);
    uint64_t PawnMoves(int x, int y, bool forKing = false);
    void game();
    uint64_t AttackMask(int PiecesID);

    const char Symbols[13] = 
    {'K', 'Q', 'R', 'B', 'N', 'P',
     'k', 'q', 'r', 'b', 'n', 'p',
     ' '};
    const uint64_t KingMoves[64] = {
        770UL, 
        1797UL, 
        3594UL, 
        7188UL, 
        14376UL, 
        28752UL, 
        57504UL, 
        49216UL, 
        197123UL, 
        460039UL, 
        920078UL, 
        1840156UL, 
        3680312UL, 
        7360624UL, 
        14721248UL, 
        12599488UL, 
        50463488UL, 
        117769984UL, 
        235539968UL, 
        471079936UL, 
        942159872UL, 
        1884319744UL, 
        3768639488UL, 
        3225468928UL, 
        12918652928UL, 
        30149115904UL, 
        60298231808UL, 
        120596463616UL, 
        241192927232UL, 
        482385854464UL, 
        964771708928UL, 
        825720045568UL, 
        3307175149568UL, 
        7718173671424UL, 
        15436347342848UL, 
        30872694685696UL, 
        61745389371392UL, 
        123490778742784UL, 
        246981557485568UL, 
        211384331665408UL, 
        846636838289408UL, 
        1975852459884544UL, 
        3951704919769088UL, 
        7903409839538176UL, 
        15806819679076352UL, 
        31613639358152704UL, 
        63227278716305408UL, 
        54114388906344448UL, 
        216739030602088448UL, 
        505818229730443264UL, 
        1011636459460886528UL, 
        2023272918921773056UL, 
        4046545837843546112UL, 
        8093091675687092224UL, 
        16186183351374184448UL, 
        13853283560024178688UL, 
        144959613005987840UL, 
        362258295026614272UL, 
        724516590053228544UL, 
        1449033180106457088UL, 
        2898066360212914176UL, 
        5796132720425828352UL, 
        11592265440851656704UL, 
        4665729213955833856UL
    };
    const uint64_t KnightMoves[64] = {
        132096UL, 
        329728UL, 
        659712UL, 
        1319424UL, 
        2638848UL, 
        5277696UL, 
        10489856UL, 
        4202496UL, 
        33816580UL, 
        84410376UL, 
        168886289UL, 
        337772578UL, 
        675545156UL, 
        1351090312UL, 
        2685403152UL, 
        1075839008UL, 
        8657044482UL, 
        21609056261UL, 
        43234889994UL, 
        86469779988UL, 
        172939559976UL, 
        345879119952UL, 
        687463207072UL, 
        275414786112UL, 
        2216203387392UL, 
        5531918402816UL, 
        11068131838464UL, 
        22136263676928UL, 
        44272527353856UL, 
        88545054707712UL, 
        175990581010432UL, 
        70506185244672UL, 
        567348067172352UL, 
        1416171111120896UL, 
        2833441750646784UL, 
        5666883501293568UL, 
        11333767002587136UL, 
        22667534005174272UL, 
        45053588738670592UL, 
        18049583422636032UL, 
        145241105196122112UL, 
        362539804446949376UL, 
        725361088165576704UL, 
        1450722176331153408UL, 
        2901444352662306816UL, 
        5802888705324613632UL, 
        11533718717099671552UL, 
        4620693356194824192UL, 
        288234782788157440UL, 
        576469569871282176UL, 
        1224997833292120064UL, 
        2449995666584240128UL, 
        4899991333168480256UL, 
        9799982666336960512UL, 
        1152939783987658752UL, 
        2305878468463689728UL, 
        1128098930098176UL, 
        2257297371824128UL, 
        4796069720358912UL, 
        9592139440717824UL, 
        19184278881435648UL, 
        38368557762871296UL, 
        4679521487814656UL, 
        9077567998918656UL
    };


};