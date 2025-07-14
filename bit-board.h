#include <stdio.h>

#define U64 unsigned long long

inline int get_bit(U64 bit_board, int square) {
  return (bit_board & (1ULL << square)) ? 1 : 0;
}

inline U64 set_bit(U64 bit_board, int square) {
  return bit_board | (1ULL << square);
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

  a1, b1, c1, d1, e1, f1, g1, h1,
};

enum {
  black,white
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


*/

U64 pawn_attacks[2][64];

U64 mask_pawn_attacks(int square, int side) {
  U64 attacks = 0ULL;
  U64 bit_board = 0ULL;
  U64 not_A_file_board = 18374403900871474942ULL;
  U64 not_H_file_board = 9187201950435737471ULL;

  // set piece on board
  bit_board = set_bit(bit_board, square);
  print_bit_board(bit_board);

  // white
  if (side) {
    if (bit_board & not_H_file_board) attacks |= (bit_board >> 7);
    if (bit_board & not_A_file_board) attacks |= (bit_board >> 9);
  } else {
    if (bit_board & not_A_file_board) attacks |= (bit_board << 7);
    if (bit_board & not_H_file_board) attacks |= (bit_board << 9);
  }
  print_bit_board(attacks);

  return attacks;
}

void init_leaper_attacks() {
  // pawn attacks
  for (int side = 0; side < 2; side++) {
    for (int square = 0; square < 64; square++) {
      pawn_attacks[side][square] = mask_pawn_attacks(square, side);
    }
  }
}
