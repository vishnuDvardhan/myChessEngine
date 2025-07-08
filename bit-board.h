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

// clang-format on

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
  printf("\n    a  b  c  d  e  f  g  h \n");
}
