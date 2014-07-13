#if !defined(EVALUATE_H_INCLUDE)
#define EVALUATE_H_INCLUDE

extern int piece_value[16];
extern int cap_piece_value[16];
extern int pmoto_piece_value[8];

extern const int DPawn;
extern const int DLance;
extern const int DKnight;
extern const int DSilver;
extern const int DGold;
extern const int DBishop;
extern const int DRook;
extern const int DKing;
extern const int DPPawn;
extern const int DPLance;
extern const int DPKnight;
extern const int DPSilver;
extern const int DPBishop;
extern const int DPRook;

int eval_material(const Position &pos);

#endif