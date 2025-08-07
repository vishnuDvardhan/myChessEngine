#include <stdio.h>

#include <chrono>
#include <cstring>
#include <iostream>
#include <unordered_map>

using namespace std;

#define U64 unsigned long long
// #define set_board(square, piece)                              \
//   {                                                           \
//     bitboards[piece] = set_bit(bitboards[piece], square);     \
//     occupancies[white] = set_bit(occupancies[white], square); \
//     occupancies[black] = set_bit(occupancies[black], square); \
//     occupancies[both] = set_bit(occupancies[both], square);   \
//   }
// #define pop_board(square, piece)                              \
//   {                                                           \
//     bitboards[piece] = pop_bit(bitboards[piece], square);     \
//     occupancies[white] = pop_bit(occupancies[white], square); \
//     occupancies[black] = pop_bit(occupancies[black], square); \
//     occupancies[both] = pop_bit(occupancies[both], square);   \
//   }

#define set_castle(state) (castle |= state)
#define unset_castle(state) (castle &= ~state);

#define encode_move(source, target, piece, promoted, capture, double_push, \
                    enpassan, castlin)                                     \
  (source) | (target << 6) | (piece << 12) | (promoted << 16) |            \
      (capture << 20) | (double_push << 21) | (enpassan << 22) |           \
      (castlin << 23)

#define get_move_source(mov) (mov & 0x3f)

#define get_move_target(mov) ((mov & 0xfc0) >> 6)

#define get_move_piece(mov) ((mov & 0xf000) >> 12)

#define get_move_promoted(mov) ((mov & 0xf0000) >> 16)

#define get_move_capture(mov) ((mov & 0x100000) ? 1 : 0)

#define get_move_double_push(mov) ((mov & 0x200000) ? 1 : 0)

#define get_move_enpassan(mov) ((mov & 0x400000) ? 1 : 0)

#define get_move_castling(mov) ((mov & 0x800000) ? 1 : 0)

inline int get_bit(U64 bit_board, int square) {
  return (bit_board & (1ULL << square)) ? 1 : 0;
}

inline U64 set_bit(U64 bit_board, int square) {
  return bit_board | (1ULL << square);
}

inline int count_bits(U64 bit_board) {
  int count = 0;
  while (bit_board > 0) {
    bit_board &= (bit_board - 1);
    count++;
  }

  return count;
}

inline int get_least_signi_bit_ind(U64 bit_board) {
  if (bit_board) {
    return count_bits((bit_board & -bit_board) - 1);
  }
  return -1;
}

// inline U64 pop_bit(U64 bit_board, int square) {
//   return get_bit(bit_board, square) ? bit_board ^ (1ULL << square) :
//   bit_board;
// }

inline U64 pop_bit(U64 bit_board, int square) {
  return bit_board & ~(1ULL << square);
}

void print_bit_board(U64 bit_board) {
  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      int square = rank * 8 + file;
      //   printf(" %2d ", square);
      if (!file) {
        printf("%d  ", 8 - rank);
      }
      printf(" %d ", get_bit(bit_board, square));
    }
    printf("\n");
  }
  printf("\n    a  b  c  d  e  f  g  h \n\n");
  printf("board value %llu\n\n", bit_board);
}
// clang-format off
enum {
  a8, b8, c8, d8, e8, f8, g8, h8,

  a7, b7, c7, d7, e7, f7, g7, h7,

  a6, b6, c6, d6, e6, f6, g6, h6,

  a5, b5, c5, d5, e5, f5, g5, h5,

  a4, b4, c4, d4, e4, f4, g4, h4,

  a3, b3, c3, d3, e3, f3, g3, h3,

  a2, b2, c2, d2, e2, f2, g2, h2,

  a1, b1, c1, d1, e1, f1, g1, h1, no_sq
};

enum {
  black,white,both
};

enum {
  wk=1,wq=2,bk=4,bq=8
};


// enum { 
//   rook,bishop
// };

const char* index_to_square[64] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};

const int castling_rights[64] = {
     7, 15, 15, 15,  3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};

char pieces[14] = "PNBRQKpnbrqk.";
enum {P,N,B,R,Q,K,p,n,b,r,q,k};



unordered_map<int, char> promoted_pieces  {
    {Q, 'q'},
    {R, 'r'},
    {B, 'b'},
    {N, 'n'},
    {q, 'q'},
    {r, 'r'},
    {b, 'b'},
    {n, 'n'},
    {0,' '}
};



enum   {
  wPawn,
  wKnight,
  wBishop,
  wRook,
  wQueen,
  wKing,
  bPawn,
  bKnight,
  bBishop,
  bRook,
  bQueen,
  bKing,
  empt,
  invalid
};

unordered_map<char, int> charToPiece{
    {' ', empt},   {'P', wPawn}, {'N',wKnight},
    {'B', wBishop}, {'R', wRook}, {'Q',wQueen},
    {'K', wKing},   {'p', bPawn}, {'n',bKnight},
    {'b', bBishop}, {'r', bRook}, {'q',bQueen},
    {'k', bKing}};

unordered_map<int, char> pieceToChar{
    {empt, '.'},   {wPawn, 'P'}, {wKnight, 'N'},
    {wBishop, 'B'}, {wRook, 'R'}, {wQueen, 'Q'},
    {wKing, 'K'},   {bPawn, 'p'}, {bKnight, 'n'},
    {bBishop, 'b'}, {bRook, 'r'}, {bQueen, 'q'},
    {bKing, 'k'}};


int bishop_relevant_occupancy_bits[64]=
{
    6,5,5,5,5,5,5,6,
    5,5,5,5,5,5,5,5,
    5,5,7,7,7,7,5,5,
    5,5,7,9,9,7,5,5,
    5,5,7,9,9,7,5,5,
    5,5,7,7,7,7,5,5,
    5,5,5,5,5,5,5,5,
    6,5,5,5,5,5,5,6,
};

int rook_relevant_occupancy_bits[64]=
{
    12,11,11,11,11,11,11,12,
    11,10,10,10,10,10,10,11,
    11,10,10,10,10,10,10,11,
    11,10,10,10,10,10,10,11,
    11,10,10,10,10,10,10,11,
    11,10,10,10,10,10,10,11,
    11,10,10,10,10,10,10,11,
    12,11,11,11,11,11,11,12,
};
// clang-format on

// attack boards
/*

      not A constant
8   0  1  1  1  1  1  1  1
7   0  1  1  1  1  1  1  1
6   0  1  1  1  1  1  1  1
5   0  1  1  1  1  1  1  1
4   0  1  1  1  1  1  1  1
3   0  1  1  1  1  1  1  1
2   0  1  1  1  1  1  1  1
1   0  1  1  1  1  1  1  1

    a  b  c  d  e  f  g  h

board value 18374403900871474942


      not H constant
8   1  1  1  1  1  1  1  0
7   1  1  1  1  1  1  1  0
6   1  1  1  1  1  1  1  0
5   1  1  1  1  1  1  1  0
4   1  1  1  1  1  1  1  0
3   1  1  1  1  1  1  1  0
2   1  1  1  1  1  1  1  0
1   1  1  1  1  1  1  1  0

    a  b  c  d  e  f  g  h

board value 9187201950435737471


      not GH constant
8   1  1  1  1  1  1  0  0
7   1  1  1  1  1  1  0  0
6   1  1  1  1  1  1  0  0
5   1  1  1  1  1  1  0  0
4   1  1  1  1  1  1  0  0
3   1  1  1  1  1  1  0  0
2   1  1  1  1  1  1  0  0
1   1  1  1  1  1  1  0  0

    a  b  c  d  e  f  g  h

board value 4557430888798830399

      not AB constant
8   0  0  1  1  1  1  1  1
7   0  0  1  1  1  1  1  1
6   0  0  1  1  1  1  1  1
5   0  0  1  1  1  1  1  1
4   0  0  1  1  1  1  1  1
3   0  0  1  1  1  1  1  1
2   0  0  1  1  1  1  1  1
1   0  0  1  1  1  1  1  1

    a  b  c  d  e  f  g  h

board value 18229723555195321596


      not 87 constant
8   0  0  0  0  0  0  0  0
7   0  0  0  0  0  0  0  0
6   1  1  1  1  1  1  1  1
5   1  1  1  1  1  1  1  1
4   1  1  1  1  1  1  1  1
3   1  1  1  1  1  1  1  1
2   1  1  1  1  1  1  1  1
1   1  1  1  1  1  1  1  1

    a  b  c  d  e  f  g  h

board value 18446744073709486080


      not 12 constant
8   1  1  1  1  1  1  1  1
7   1  1  1  1  1  1  1  1
6   1  1  1  1  1  1  1  1
5   1  1  1  1  1  1  1  1
4   1  1  1  1  1  1  1  1
3   1  1  1  1  1  1  1  1
2   0  0  0  0  0  0  0  0
1   0  0  0  0  0  0  0  0

    a  b  c  d  e  f  g  h

board value 281474976710655


*/

U64 pawn_attacks[2][64];
U64 knight_attacks[64];
U64 king_attacks[64];
U64 bishop_masks[64];
U64 rook_masks[64];
// bishop_attacks[square][index]
U64 bishop_attacks[64][512];
// rook_attacks[square][index]
U64 rook_attacks[64][4096];

U64 rook_magic_numbers[64];
U64 bishop_magic_numbers[64];

U64 bitboards[12];
U64 occupancies[3];
int side = white;
int enpassant = no_sq;
int castle;

void print_board() {
  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      int square = rank * 8 + file;
      if (!file) {
        printf("  %d  ", 8 - rank);
      }
      int piece = -1;

      for (int bitboard_count = P; bitboard_count <= k; bitboard_count++) {
        if (get_bit(bitboards[bitboard_count], square)) piece = bitboard_count;
      }

      printf("%c ", (piece == -1 ? '.' : pieceToChar[piece]));
    }
    printf("\n");
  }
  printf("\n     a b c d e f g h\n\n");
  printf("Side:       %s\n", (!side) ? "black" : "white");
  printf("eEnpassant: %s\n",
         (enpassant != no_sq) ? index_to_square[enpassant] : "no");
  printf("Castling  : %c%c%c%c\n\n", (castle & wk ? 'K' : '-'),
         (castle & wq ? 'Q' : '-'), (castle & bk ? 'k' : '-'),
         (castle & bq ? 'q' : '-'));
}
static inline void set_board(int square, int piece) {
  bitboards[piece] = set_bit(bitboards[piece], square);
  if (piece >= P && piece <= K)
    occupancies[white] = set_bit(occupancies[white], square);
  if (piece >= p && piece <= k)
    occupancies[black] = set_bit(occupancies[black], square);
  occupancies[both] = set_bit(occupancies[both], square);
}

static inline void pop_board(int square, int piece) {
  bitboards[piece] = pop_bit(bitboards[piece], square);
  if (piece >= P && piece <= K)
    occupancies[white] = pop_bit(occupancies[white], square);
  if (piece >= p && piece <= k)
    occupancies[black] = pop_bit(occupancies[black], square);
  occupancies[both] = pop_bit(occupancies[both], square);
}
void init_board() {
  set_board(a2, P);
  set_board(b2, P);
  set_board(c2, P);
  set_board(d2, P);
  set_board(e2, P);
  set_board(f2, P);
  set_board(g2, P);
  set_board(h2, P);

  // set white knights
  set_board(b1, N);
  set_board(g1, N);

  // set white bishops
  set_board(c1, B);
  set_board(f1, B);

  // set white rooks
  set_board(a1, R);
  set_board(h1, R);

  // set white queen & king
  set_board(d1, Q);
  set_board(e1, K);

  // set white pawns
  set_board(a7, p);
  set_board(b7, p);
  set_board(c7, p);
  set_board(d7, p);
  set_board(e7, p);
  set_board(f7, p);
  set_board(g7, p);
  set_board(h7, p);

  // set white knights
  set_board(b8, n);
  set_board(g8, n);

  // set white bishops
  set_board(c8, b);
  set_board(f8, b);

  // set white rooks
  set_board(a8, r);
  set_board(h8, r);

  // set white queen & king
  set_board(d8, q);
  set_board(e8, k);

  // init side
  side = black;

  // init enpassant
  enpassant = no_sq;

  // init castling
  set_castle(wk);
  set_castle(wq);
  set_castle(bq);
  set_castle(bk);
}

U64 mask_knight_attacks(int square) {
  U64 attacks = 0ULL;
  U64 bit_board = 0ULL;
  U64 not_A_file_board = 18374403900871474942ULL;
  U64 not_H_file_board = 9187201950435737471ULL;
  U64 not_GH_file_board = 4557430888798830399ULL;
  U64 not_AB_file_board = 18229723555195321596ULL;
  U64 not_12_rank_board = 281474976710655ULL;
  U64 not_78_rank_board = 18446744073709486080ULL;

  // set piece on board
  bit_board = set_bit(bit_board, square);
  // cout << "board:" << square << "\n";
  // print_bit_board(bit_board);
  if (bit_board & not_H_file_board) attacks |= (bit_board >> 15);
  if (bit_board & not_A_file_board) attacks |= (bit_board >> 17);
  if (bit_board & not_A_file_board) attacks |= (bit_board << 15);
  if (bit_board & not_H_file_board) attacks |= (bit_board << 17);
  if (bit_board & not_GH_file_board) attacks |= (bit_board >> 6);
  if (bit_board & not_AB_file_board) attacks |= (bit_board >> 10);
  if (bit_board & not_AB_file_board) attacks |= (bit_board << 6);
  if (bit_board & not_GH_file_board) attacks |= (bit_board << 10);
  // cout << "attack:" << square << "\n";
  // print_bit_board(attacks);

  return attacks;
}

U64 mask_king_attacks(int square) {
  U64 attacks = 0ULL;
  U64 bit_board = 0ULL;
  U64 not_A_file_board = 18374403900871474942ULL;
  U64 not_H_file_board = 9187201950435737471ULL;
  U64 not_GH_file_board = 4557430888798830399ULL;
  U64 not_AB_file_board = 18229723555195321596ULL;
  U64 not_12_rank_board = 281474976710655ULL;
  U64 not_78_rank_board = 18446744073709486080ULL;

  // set piece on board
  bit_board = set_bit(bit_board, square);
  // cout << "board:" << square << "\n";
  // print_bit_board(bit_board);
  if (bit_board & not_H_file_board) attacks |= (bit_board >> 7);
  if (bit_board & not_H_file_board) attacks |= (bit_board << 1);
  if (bit_board & not_A_file_board) attacks |= (bit_board >> 1);
  if (bit_board & not_A_file_board) attacks |= (bit_board >> 9);
  if (bit_board & not_A_file_board) attacks |= (bit_board << 7);
  if (bit_board & not_H_file_board) attacks |= (bit_board << 9);
  attacks |= (bit_board << 8);
  attacks |= (bit_board >> 8);

  // cout << "attack:" << square << "\n";
  // print_bit_board(attacks);

  return attacks;
}

U64 mask_pawn_attacks(int square, int side) {
  U64 attacks = 0ULL;
  U64 bit_board = 0ULL;
  U64 not_A_file_board = 18374403900871474942ULL;
  U64 not_H_file_board = 9187201950435737471ULL;

  // set piece on board
  bit_board = set_bit(bit_board, square);
  // cout << "board:" << square << "\n";
  // print_bit_board(bit_board);

  // white
  if (side) {
    if (bit_board & not_H_file_board) attacks |= (bit_board >> 7);
    if (bit_board & not_A_file_board) attacks |= (bit_board >> 9);
  } else {
    if (bit_board & not_A_file_board) attacks |= (bit_board << 7);
    if (bit_board & not_H_file_board) attacks |= (bit_board << 9);
  }
  // cout << "attack:" << square << "\n";

  // print_bit_board(attacks);

  return attacks;
}

U64 mask_bishop_attacks(int square) {
  U64 attacks = 0ULL;
  U64 bit_board = 0ULL;
  U64 not_A_file_board = 18374403900871474942ULL;
  U64 not_H_file_board = 9187201950435737471ULL;

  // set piece on board
  bit_board = set_bit(bit_board, square);
  // cout << "board:" << square << "\n";
  // print_bit_board(bit_board);
  int r, f;
  int target_rank = square / 8;
  int target_file = square % 8;
  for (r = target_rank + 1, f = target_file + 1; r <= 6 && f <= 6; r++, f++)
    attacks |= (1ULL << (r * 8 + f));
  for (r = target_rank - 1, f = target_file - 1; r >= 1 && f >= 1; r--, f--)
    attacks |= (1ULL << (r * 8 + f));
  for (r = target_rank - 1, f = target_file + 1; r >= 1 && f <= 6; r--, f++)
    attacks |= (1ULL << (r * 8 + f));
  for (r = target_rank + 1, f = target_file - 1; r <= 6 && f >= 1; r++, f--)
    attacks |= (1ULL << (r * 8 + f));

  // cout << "attack:" << square << "\n";

  // print_bit_board(attacks);

  return attacks;
}

U64 bishop_attacks_on_fly(int square, U64 blocker) {
  U64 attacks = 0ULL;
  U64 bit_board = 0ULL;
  U64 not_A_file_board = 18374403900871474942ULL;
  U64 not_H_file_board = 9187201950435737471ULL;

  // set piece on board
  bit_board = set_bit(bit_board, square);
  // cout << "board:" << square << "\n";
  // print_bit_board(bit_board);
  int r, f;
  int target_rank = square / 8;
  int target_file = square % 8;
  for (r = target_rank + 1, f = target_file + 1; r <= 7 && f <= 7; r++, f++) {
    attacks |= (1ULL << (r * 8 + f));
    if ((1ULL << (r * 8 + f)) & blocker) break;
  }
  for (r = target_rank - 1, f = target_file - 1; r >= 0 && f >= 0; r--, f--) {
    attacks |= (1ULL << (r * 8 + f));
    if ((1ULL << (r * 8 + f)) & blocker) break;
  }
  for (r = target_rank - 1, f = target_file + 1; r >= 0 && f <= 7; r--, f++) {
    attacks |= (1ULL << (r * 8 + f));
    if ((1ULL << (r * 8 + f)) & blocker) break;
  }
  for (r = target_rank + 1, f = target_file - 1; r <= 7 && f >= 0; r++, f--) {
    attacks |= (1ULL << (r * 8 + f));
    if ((1ULL << (r * 8 + f)) & blocker) break;
  }

  // cout << "attack:" << square << "\n";

  // print_bit_board(attacks);

  return attacks;
}

U64 mask_rook_attacks(int square) {
  U64 attacks = 0ULL;
  U64 bit_board = 0ULL;
  U64 not_A_file_board = 18374403900871474942ULL;
  U64 not_H_file_board = 9187201950435737471ULL;

  // set piece on board
  bit_board = set_bit(bit_board, square);
  // cout << "board:" << square << "\n";
  // print_bit_board(bit_board);
  int r, f;
  int target_rank = square / 8;
  int target_file = square % 8;
  for (r = target_rank + 1, f = target_file; r <= 6; r++)
    attacks |= (1ULL << (r * 8 + f));
  for (r = target_rank - 1, f = target_file; r >= 1; r--)
    attacks |= (1ULL << (r * 8 + f));
  for (r = target_rank, f = target_file + 1; f <= 6; f++)
    attacks |= (1ULL << (r * 8 + f));
  for (r = target_rank, f = target_file - 1; f >= 1; f--)
    attacks |= (1ULL << (r * 8 + f));

  // cout << "attack:" << square << "\n";

  // print_bit_board(attacks);

  return attacks;
}

U64 rook_attacks_on_fly(int square, U64 blocker) {
  U64 attacks = 0ULL;
  U64 bit_board = 0ULL;
  U64 not_A_file_board = 18374403900871474942ULL;
  U64 not_H_file_board = 9187201950435737471ULL;

  // set piece on board
  bit_board = set_bit(bit_board, square);
  // cout << "board:" << square << "\n";
  // print_bit_board(bit_board);
  int r, f;
  int target_rank = square / 8;
  int target_file = square % 8;
  for (r = target_rank + 1, f = target_file; r <= 7; r++) {
    attacks |= (1ULL << (r * 8 + f));
    if ((1ULL << (r * 8 + f)) & blocker) break;
  }

  for (r = target_rank - 1, f = target_file; r >= 0; r--) {
    attacks |= (1ULL << (r * 8 + f));
    if ((1ULL << (r * 8 + f)) & blocker) break;
  }

  for (r = target_rank, f = target_file + 1; f <= 7; f++) {
    attacks |= (1ULL << (r * 8 + f));
    if ((1ULL << (r * 8 + f)) & blocker) break;
  }

  for (r = target_rank, f = target_file - 1; f >= 0; f--) {
    attacks |= (1ULL << (r * 8 + f));
    if ((1ULL << (r * 8 + f)) & blocker) break;
  }

  // cout << "attack:" << square << "\n";

  // print_bit_board(attacks);

  return attacks;
}

void init_leaper_attacks() {
  // pawn attacks

  for (int square = 0; square < 64; square++) {
    pawn_attacks[white][square] = mask_pawn_attacks(square, white);
    pawn_attacks[black][square] = mask_pawn_attacks(square, black);

    knight_attacks[square] = mask_knight_attacks(square);
    king_attacks[square] = mask_king_attacks(square);
  }
}
U64 set_occupancy(int index, int bits_in_mask, U64 attack_mask) {
  U64 occupancy = 0ULL;
  for (int i = 0; i < bits_in_mask; i++) {
    int square = get_least_signi_bit_ind(attack_mask);
    attack_mask = pop_bit(attack_mask, square);
    if (index & (1 << i)) {
      occupancy |= (1ULL << square);
    }
  }

  return occupancy;
}

unsigned int state = 1804289383;
unsigned int get_random_32() {
  unsigned int num = state;
  num ^= num << 13;
  num ^= num >> 17;
  num ^= num << 5;
  state = num;
  return num;
}

U64 get_random_64() {
  U64 n1, n2, n3, n4;
  n1 = (U64)(get_random_32()) & 0xFFFF;
  n2 = (U64)(get_random_32()) & 0xFFFF;
  n3 = (U64)(get_random_32()) & 0xFFFF;
  n4 = (U64)(get_random_32()) & 0xFFFF;
  return n1 | n2 << 16 | n3 << 32 | n4 << 48;
}

U64 generate_candidate_magic_number() {
  return get_random_64() & get_random_64() & get_random_64();
}

U64 find_magic_number(int square, int relevant_occupancy_bits, int bishop) {
  U64 occupancies[4096];
  U64 attacks[4096];
  U64 used_attacks[4096];
  U64 attack_mask =
      bishop ? mask_bishop_attacks(square) : mask_rook_attacks(square);
  int occupancy_indices = 1 << relevant_occupancy_bits;
  for (int i = 0; i < occupancy_indices; i++) {
    occupancies[i] = set_occupancy(i, relevant_occupancy_bits, attack_mask);
    attacks[i] = bishop ? bishop_attacks_on_fly(square, occupancies[i])
                        : rook_attacks_on_fly(square, occupancies[i]);
  }

  for (int random_count = 0; random_count < 10000000000; random_count++) {
    U64 magic_number = generate_candidate_magic_number();
    // skip inappropriate number
    if (count_bits((attack_mask * magic_number) & 0xFF00000000000000) < 6) {
      continue;
    }

    memset(used_attacks, 0ULL, sizeof(used_attacks));

    int index, failed;

    for (index = 0, failed = 0; !failed && index < occupancy_indices; index++) {
      int magic_index = (int)((occupancies[index] * magic_number) >>
                              (64 - relevant_occupancy_bits));

      // if magic index works
      if (used_attacks[magic_index] == 0ULL) {
        // init used attacks
        used_attacks[magic_index] = attacks[index];
      } else if (used_attacks[magic_index] != attacks[index]) {
        failed = 1;
      }
    }
    if (!failed) {
      return magic_number;
    }
  }
  printf("magic number fails");
  return 0ULL;
}

// init magic numbers
void init_magic_numbers() {
  for (int square = 0; square < 64; square++) {
    rook_magic_numbers[square] =
        find_magic_number(square, rook_relevant_occupancy_bits[square], 0);
    // printf("ox%llxULL\n", rook_magic_numbers[square]);
  }
  // printf("\n\n");
  for (int square = 0; square < 64; square++) {
    bishop_magic_numbers[square] =
        find_magic_number(square, bishop_relevant_occupancy_bits[square], 1);
    // printf("ox%llxULL\n", bishop_magic_numbers[square]);
  }
}

void init_sliders_attacks(int bishop) {
  // loop over all squares
  for (int square = 0; square < 64; square++) {
    // init bishop and rook masks
    bishop_masks[square] = mask_bishop_attacks(square);
    rook_masks[square] = mask_rook_attacks(square);

    U64 attack_mask = bishop ? bishop_masks[square] : rook_masks[square];
    int relevant_bits_count = count_bits(attack_mask);
    int occupancy_indices = (1 << relevant_bits_count);

    for (int index = 0; index < occupancy_indices; index++) {
      if (bishop) {
        U64 occupancy = set_occupancy(index, relevant_bits_count, attack_mask);
        // init magic index
        int magic_index = (occupancy * bishop_magic_numbers[square]) >>
                          (64 - bishop_relevant_occupancy_bits[square]);
        bishop_attacks[square][magic_index] =
            bishop_attacks_on_fly(square, occupancy);
      } else {
        U64 occupancy = set_occupancy(index, relevant_bits_count, attack_mask);
        // init magic index
        int magic_index = (occupancy * rook_magic_numbers[square]) >>
                          (64 - rook_relevant_occupancy_bits[square]);
        rook_attacks[square][magic_index] =
            rook_attacks_on_fly(square, occupancy);
      }
    }
  }
}

static inline U64 get_bishop_attacks(int square, U64 occupancy) {
  // get bishop attacks assuming current board occupancy
  occupancy &= bishop_masks[square];
  occupancy *= bishop_magic_numbers[square];
  occupancy >>= (64 - bishop_relevant_occupancy_bits[square]);
  return bishop_attacks[square][occupancy];
}

static inline U64 get_rook_attacks(int square, U64 occupancy) {
  // get bishop attacks assuming current board occupancy
  occupancy &= rook_masks[square];
  occupancy *= rook_magic_numbers[square];
  occupancy >>= (64 - rook_relevant_occupancy_bits[square]);

  return rook_attacks[square][occupancy];
}

static inline U64 get_queen_attacks(int square, U64 occupancy) {
  // get bishop attacks assuming current board occupancy

  return get_bishop_attacks(square, occupancy) |
         get_rook_attacks(square, occupancy);
}

void init_all() {
  init_magic_numbers();
  init_leaper_attacks();
  // rook
  init_sliders_attacks(0);
  // bishop
  init_sliders_attacks(1);
}

void init_from_fen(char* fen) {
  memset(bitboards, 0, sizeof(bitboards));
  memset(occupancies, 0, sizeof(occupancies));
  side = 0;
  enpassant = no_sq;
  castle = 0;

  for (int rank = 0; rank < 8; rank++) {
    int file = 0;
    while (file < 8 && *fen != '/' && *fen != ' ') {
      int square = 8 * rank + file;
      if ((*fen >= 'a' && *fen <= 'z') || (*fen >= 'A' && *fen <= 'Z')) {
        int piece = charToPiece[*fen];
        set_board(square, piece);
        file++;
      } else if (*fen >= '1' && *fen <= '8') {
        int emptySquares = *fen - '0';
        file += emptySquares;
      }
      fen++;
    }
    if (*fen == '/') fen++;
  }

  if (*fen == ' ') fen++;

  side = (*fen == 'w' ? white : black);
  fen++;

  if (*fen == ' ') fen++;

  while (*fen != ' ' && *fen != '\0') {
    if (*fen == 'K')
      castle |= wk;
    else if (*fen == 'Q')
      castle |= wq;
    else if (*fen == 'k')
      castle |= bk;
    else if (*fen == 'q')
      castle |= bq;
    else if (*fen == '-')
      break;
    fen++;
  }

  if (*fen == ' ') fen++;

  if (*fen != '-') {
    int en_file = fen[0] - 'a';
    int en_rank = 8 - (fen[1] - '0');
    enpassant = en_rank * 8 + en_file;
  } else {
    enpassant = no_sq;
  }

  for (int piece = P; piece <= K; piece++) {
    occupancies[white] |= bitboards[piece];
  }
  // print_bit_board(occupancies[black]);
  // print_bit_board(occupancies[white]);
  for (int piece = p; piece <= k; piece++) {
    occupancies[black] |= bitboards[piece];
  }
  // print_bit_board(occupancies[black]);
  occupancies[both] = occupancies[white] | occupancies[black];
}

int is_square_attacked(int square, int side) {
  if ((side == white) && (pawn_attacks[black][square] & bitboards[P])) return 1;
  if ((side == black) && (pawn_attacks[white][square] & bitboards[p])) return 1;
  if (knight_attacks[square] & ((side == white ? bitboards[N] : bitboards[n])))
    return 1;
  if (king_attacks[square] & ((side == white ? bitboards[K] : bitboards[k])))
    return 1;
  if (get_bishop_attacks(square, occupancies[both]) &
      ((side == white ? bitboards[B] : bitboards[b])))
    return 1;
  if (get_rook_attacks(square, occupancies[both]) &
      ((side == white ? bitboards[R] : bitboards[r])))
    return 1;
  if (get_queen_attacks(square, occupancies[both]) &
      ((side == white ? bitboards[Q] : bitboards[q])))
    return 1;

  return 0;
}

void print_attacked_square(int side) {
  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      int square = rank * 8 + file;
      if (!file) printf(" %d  ", 8 - rank);
      printf("%d ", is_square_attacked(square, side) ? 1 : 0);
    }

    printf("\n");
  }
  printf("\n    a b c d e f g h \n");
}

typedef struct {
  int moves[256];
  int count;
  void add_move(int move) {
    moves[count] = move;
    count++;
  }
  void print_moves() {
    printf("\n    move    piece   capture   double    enpass    castling\n\n");

    for (int move_count = 0; move_count < count; move_count++) {
      int move = moves[move_count];
      printf("    %s%s%c   %c       %d         %d         %d         %d\n",
             index_to_square[get_move_source(move)],
             index_to_square[get_move_target(move)],
             promoted_pieces[get_move_promoted(move)],
             pieces[get_move_piece(move)], get_move_capture(move) ? 1 : 0,
             get_move_double_push(move) ? 1 : 0,
             get_move_enpassan(move) ? 1 : 0, get_move_castling(move) ? 1 : 0);
    }
    printf("\n\n    Total number of moves: %d\n\n", count);
  }

} moves;

void print_move(int move) {
  printf("%s%s%c\n", index_to_square[get_move_source(move)],
         index_to_square[get_move_target(move)],
         promoted_pieces[get_move_promoted(move)]);
}

#define copy_board()                           \
  U64 bitboards_copy[12], occupancies_copy[3]; \
  int side_copy, enpassant_copy, castle_copy;  \
  memcpy(bitboards_copy, bitboards, 96);       \
  memcpy(occupancies_copy, occupancies, 24);   \
  side_copy = side, enpassant_copy = enpassant, castle_copy = castle;

#define restore_board()                      \
  memcpy(bitboards, bitboards_copy, 96);     \
  memcpy(occupancies, occupancies_copy, 24); \
  side = side_copy, enpassant = enpassant_copy, castle = castle_copy;

enum { all_moves, only_captures };

inline int make_move(int move, int move_flag) {
  if (move_flag == all_moves) {
    copy_board();
    int source_square = get_move_source(move);
    int target_square = get_move_target(move);
    int piece = get_move_piece(move);
    int promoted = get_move_promoted(move);
    int capture = get_move_capture(move);
    int double_push = get_move_double_push(move);
    int enpass = get_move_enpassan(move);
    int castling = get_move_castling(move);
    pop_board(source_square, piece);

    if (capture) {
      int s = (side == white ? p : P), e = (side == white ? k : K);
      for (int piec = s; piec <= e; piec++) {
        if (get_bit(bitboards[piec], target_square)) {
          pop_board(target_square, piec);
          break;
        }
      }
    }
    if (promoted) {
      pop_board(target_square, ((side == white) ? P : p));
      set_board(target_square, promoted);
    }
    if (enpass) {
      (side == white) ? pop_board(target_square + 8, p)
                      : pop_board(target_square - 8, P);
    }
    enpassant = no_sq;
    if (double_push) {
      (side == white) ? (enpassant = target_square + 8)
                      : (enpassant = target_square - 8);
    }

    if (castling) {
      if (target_square == g1) {
        pop_board(h1, R);
        set_board(f1, R);
      } else if (target_square == c1) {
        pop_board(a1, R);
        set_board(d1, R);
      } else if (target_square == g8) {
        pop_board(h8, r);
        set_board(f8, r);
      } else if (target_square == c8) {
        pop_board(a8, r);
        set_board(d8, r);
      }
    }
    castle &= castling_rights[source_square];
    castle &= castling_rights[target_square];
    set_board(target_square, piece);
    side ^= 1;
    if (is_square_attacked((side == white)
                               ? get_least_signi_bit_ind(bitboards[k])
                               : get_least_signi_bit_ind(bitboards[K]),
                           side))

    {
      restore_board();
      return 0;
    } else {
      return 1;
    }

  } else {
    if (get_move_capture(move)) {
      make_move(move, all_moves);
    } else {
      return 0;
    }
  }
}

void generate_moves(moves& move_list) {
  move_list.count = 0;

  int source_square, target_square;
  U64 bitboard, attacks;
  for (int piece = P; piece <= k; piece++) {
    bitboard = bitboards[piece];
    if (side == white) {
      if (piece == P) {
        while (bitboard) {
          source_square = get_least_signi_bit_ind(bitboard);
          target_square = source_square - 8;
          if (!(target_square < a8) &&
              !get_bit(occupancies[both], target_square)) {
            if (source_square <= h7 && source_square >= a7) {
              move_list.add_move(encode_move(source_square, target_square,
                                             piece, Q, 0, 0, 0, 0));
              move_list.add_move(encode_move(source_square, target_square,
                                             piece, R, 0, 0, 0, 0));
              move_list.add_move(encode_move(source_square, target_square,
                                             piece, B, 0, 0, 0, 0));
              move_list.add_move(encode_move(source_square, target_square,
                                             piece, N, 0, 0, 0, 0));

            } else {
              if ((source_square >= a2 && source_square <= h2) &&
                  !get_bit(occupancies[both], target_square - 8))

                move_list.add_move(encode_move(source_square, target_square - 8,
                                               piece, 0, 0, 1, 0, 0));
              move_list.add_move(encode_move(source_square, target_square,
                                             piece, 0, 0, 0, 0, 0));
            }
          }
          U64 captures = pawn_attacks[side][source_square];
          while (captures) {
            target_square = get_least_signi_bit_ind(captures);
            if (get_bit(occupancies[black], target_square) ||
                (enpassant != no_sq && target_square == enpassant)) {
              if (source_square <= h7 && source_square >= a7) {
                // promotion
                move_list.add_move(encode_move(source_square, target_square,
                                               piece, Q, 1, 0, 0, 0));
                move_list.add_move(encode_move(source_square, target_square,
                                               piece, R, 1, 0, 0, 0));
                move_list.add_move(encode_move(source_square, target_square,
                                               piece, B, 1, 0, 0, 0));
                move_list.add_move(encode_move(source_square, target_square,
                                               piece, N, 1, 0, 0, 0));
              } else {
                move_list.add_move(
                    encode_move(source_square, target_square, piece, 0, 1, 0,
                                int(target_square == enpassant), 0));
              }
            }
            captures = pop_bit(captures, target_square);
          }
          bitboard = pop_bit(bitboard, source_square);
        }
      }
      if (piece == K) {
        if (castle & wk) {
          if (!get_bit(occupancies[both], f1) &&
              !get_bit(occupancies[both], g1)) {
            if (!is_square_attacked(e1, black) &&
                !is_square_attacked(f1, black))
              move_list.add_move(encode_move(e1, g1, piece, 0, 0, 0, 0, 1));
          }
        }

        if (castle & wq) {
          if (!get_bit(occupancies[both], d1) &&
              !get_bit(occupancies[both], c1) &&
              !get_bit(occupancies[both], b1)) {
            if (!is_square_attacked(e1, black) &&
                !is_square_attacked(d1, black))
              move_list.add_move(encode_move(e1, c1, piece, 0, 0, 0, 0, 1));
          }
        }
      }
    } else {
      if (piece == p) {
        while (bitboard) {
          source_square = get_least_signi_bit_ind(bitboard);
          target_square = source_square + 8;
          if (!(target_square > h1) &&
              !get_bit(occupancies[both], target_square)) {
            if (source_square <= h2 && source_square >= a2) {
              move_list.add_move(encode_move(source_square, target_square,
                                             piece, Q, 0, 0, 0, 0));
              move_list.add_move(encode_move(source_square, target_square,
                                             piece, R, 0, 0, 0, 0));
              move_list.add_move(encode_move(source_square, target_square,
                                             piece, B, 0, 0, 0, 0));
              move_list.add_move(encode_move(source_square, target_square,
                                             piece, N, 0, 0, 0, 0));

            } else {
              if ((source_square >= a7 && source_square <= h7) &&
                  !get_bit(occupancies[both], target_square + 8))

                move_list.add_move(encode_move(source_square, target_square + 8,
                                               piece, 0, 0, 1, 0, 0));
              move_list.add_move(encode_move(source_square, target_square,
                                             piece, 0, 0, 0, 0, 0));
            }
          }
          U64 captures = pawn_attacks[side][source_square];
          while (captures) {
            target_square = get_least_signi_bit_ind(captures);
            if (get_bit(occupancies[white], target_square) ||
                (enpassant != no_sq && target_square == enpassant)) {
              if (source_square <= h2 && source_square >= a2) {
                move_list.add_move(encode_move(source_square, target_square,
                                               piece, Q, 1, 0, 0, 0));
                move_list.add_move(encode_move(source_square, target_square,
                                               piece, R, 1, 0, 0, 0));
                move_list.add_move(encode_move(source_square, target_square,
                                               piece, B, 1, 0, 0, 0));
                move_list.add_move(encode_move(source_square, target_square,
                                               piece, N, 1, 0, 0, 0));
              } else {
                move_list.add_move(
                    encode_move(source_square, target_square, piece, 0, 1, 0,
                                int(target_square == enpassant), 0));
              }
            }
            captures = pop_bit(captures, target_square);
          }
          bitboard = pop_bit(bitboard, source_square);
        }
      }

      if (piece == k) {
        if (castle & bk) {
          if (!get_bit(occupancies[both], f8) &&
              !get_bit(occupancies[both], g8)) {
            if (!is_square_attacked(e8, white) &&
                !is_square_attacked(f8, white))
              move_list.add_move(encode_move(e8, g8, piece, 0, 0, 0, 0, 1));
          }
        }

        if (castle & bq) {
          if (!get_bit(occupancies[both], d8) &&
              !get_bit(occupancies[both], c8) &&
              !get_bit(occupancies[both], b8)) {
            if (!is_square_attacked(e8, white) &&
                !is_square_attacked(d8, white))
              move_list.add_move(encode_move(e8, c8, piece, 0, 0, 0, 0, 1));
          }
        }
      }
    }
    if ((side == white) ? piece == N : piece == n) {
      while (bitboard) {
        source_square = get_least_signi_bit_ind(bitboard);

        attacks = knight_attacks[source_square] &
                  ((side == white) ? ~occupancies[white] : ~occupancies[black]);

        while (attacks) {
          target_square = get_least_signi_bit_ind(attacks);

          if (!get_bit(
                  ((side == white) ? occupancies[black] : occupancies[white]),
                  target_square))
            move_list.add_move(encode_move(source_square, target_square, piece,
                                           0, 0, 0, 0, 0));

          else

            move_list.add_move(encode_move(source_square, target_square, piece,
                                           0, 1, 0, 0, 0));

          attacks = pop_bit(attacks, target_square);
        }

        bitboard = pop_bit(bitboard, source_square);
      }
    }

    if ((side == white) ? piece == B : piece == b) {
      while (bitboard) {
        source_square = get_least_signi_bit_ind(bitboard);

        attacks = get_bishop_attacks(source_square, occupancies[both]) &
                  ((side == white) ? ~occupancies[white] : ~occupancies[black]);

        while (attacks) {
          target_square = get_least_signi_bit_ind(attacks);

          if (!get_bit(
                  ((side == white) ? occupancies[black] : occupancies[white]),
                  target_square))
            move_list.add_move(encode_move(source_square, target_square, piece,
                                           0, 0, 0, 0, 0));

          else

            move_list.add_move(encode_move(source_square, target_square, piece,
                                           0, 1, 0, 0, 0));

          attacks = pop_bit(attacks, target_square);
        }

        bitboard = pop_bit(bitboard, source_square);
      }
    }

    if ((side == white) ? piece == R : piece == r) {
      while (bitboard) {
        source_square = get_least_signi_bit_ind(bitboard);

        attacks = get_rook_attacks(source_square, occupancies[both]) &
                  ((side == white) ? ~occupancies[white] : ~occupancies[black]);

        while (attacks) {
          target_square = get_least_signi_bit_ind(attacks);

          if (!get_bit(
                  ((side == white) ? occupancies[black] : occupancies[white]),
                  target_square))
            move_list.add_move(encode_move(source_square, target_square, piece,
                                           0, 0, 0, 0, 0));

          else
            move_list.add_move(encode_move(source_square, target_square, piece,
                                           0, 1, 0, 0, 0));

          attacks = pop_bit(attacks, target_square);
        }

        bitboard = pop_bit(bitboard, source_square);
      }
    }

    if ((side == white) ? piece == Q : piece == q) {
      while (bitboard) {
        source_square = get_least_signi_bit_ind(bitboard);

        attacks = get_queen_attacks(source_square, occupancies[both]) &
                  ((side == white) ? ~occupancies[white] : ~occupancies[black]);

        while (attacks) {
          target_square = get_least_signi_bit_ind(attacks);

          if (!get_bit(
                  ((side == white) ? occupancies[black] : occupancies[white]),
                  target_square))
            move_list.add_move(encode_move(source_square, target_square, piece,
                                           0, 0, 0, 0, 0));

          else
            move_list.add_move(encode_move(source_square, target_square, piece,
                                           0, 1, 0, 0, 0));

          attacks = pop_bit(attacks, target_square);
        }

        bitboard = pop_bit(bitboard, source_square);
      }
    }

    if ((side == white) ? piece == K : piece == k) {
      while (bitboard) {
        source_square = get_least_signi_bit_ind(bitboard);

        attacks = king_attacks[source_square] &
                  ((side == white) ? ~occupancies[white] : ~occupancies[black]);

        while (attacks) {
          target_square = get_least_signi_bit_ind(attacks);

          if (!get_bit(
                  ((side == white) ? occupancies[black] : occupancies[white]),
                  target_square))
            move_list.add_move(encode_move(source_square, target_square, piece,
                                           0, 0, 0, 0, 0));

          else
            move_list.add_move(encode_move(source_square, target_square, piece,
                                           0, 1, 0, 0, 0));

          attacks = pop_bit(attacks, target_square);
        }

        bitboard = pop_bit(bitboard, source_square);
      }
    }
  }
}

void generate_legal_moves(moves& legal_move_list) {
  // Clear the legal move list
  legal_move_list.count = 0;

  moves pseudo_legal_moves;
  pseudo_legal_moves.count = 0;
  generate_moves(pseudo_legal_moves);

  for (int i = 0; i < pseudo_legal_moves.count; i++) {
    int move = pseudo_legal_moves.moves[i];
    copy_board() if (make_move(move, all_moves)) {
      legal_move_list.add_move(move);

      restore_board()
    }
  }
}

long long count_legal_moves() {
  long long count = 0;
  moves move_list;
  move_list.count = 0;
  generate_moves(move_list);

  for (int i = 0; i < move_list.count; i++) {
    U64 bitboards_copy[12], occupancies_copy[3];
    int side_copy, enpassant_copy, castle_copy;
    memcpy(bitboards_copy, bitboards, 96);
    memcpy(occupancies_copy, occupancies, 24);
    side_copy = side, enpassant_copy = enpassant, castle_copy = castle;

    if (make_move(move_list.moves[i], all_moves)) {
      count++;
    }

    memcpy(bitboards, bitboards_copy, 96);
    memcpy(occupancies, occupancies_copy, 24);
    side = side_copy, enpassant = enpassant_copy, castle = castle_copy;
  }

  return count;
}

long long perft(int depth) {
  if (depth == 0) return 1;
  if (depth == 1) return count_legal_moves();

  long long count = 0;
  moves move_list;
  move_list.count = 0;
  generate_moves(move_list);

  int original_side = side;
  int king_square = (side == white) ? get_least_signi_bit_ind(bitboards[K])
                                    : get_least_signi_bit_ind(bitboards[k]);

  for (int i = 0; i < move_list.count; i++) {
    U64 orig_bitboards[12], orig_occupancies[3];
    int orig_side, orig_enpassant, orig_castle;
    memcpy(orig_bitboards, bitboards, 96);
    memcpy(orig_occupancies, occupancies, 24);
    orig_side = side, orig_enpassant = enpassant, orig_castle = castle;

    if (make_move(move_list.moves[i], all_moves)) {
      count += perft(depth - 1);
    }

    memcpy(bitboards, orig_bitboards, 96);
    memcpy(occupancies, orig_occupancies, 24);
    side = orig_side, enpassant = orig_enpassant, castle = orig_castle;
  }

  return count;
}

void measurePerft(int depth) {
  init_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  print_board();
  auto start = std::chrono::high_resolution_clock::now();
  long long nodes = perft(depth);
  auto end = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  double seconds = duration.count() / 1000.0;
  long long nps = (seconds > 0) ? static_cast<long long>(nodes / seconds) : 0;

  std::cout << "Perft(" << depth << ") results:" << std::endl;
  std::cout << "Time: " << duration.count() << " ms" << std::endl;
  std::cout << "Nodes: " << nodes << std::endl;
  std::cout << "Nodes per second: " << nps << std::endl;
}
