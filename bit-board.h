#include <stdio.h>

#include <iostream>
using namespace std;

#define U64 unsigned long long

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

  a1, b1, c1, d1, e1, f1, g1, h1,
};

enum {
  black,white
};

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
  cout << "board:" << square << "\n";
  print_bit_board(bit_board);
  if (bit_board & not_H_file_board) attacks |= (bit_board >> 15);
  if (bit_board & not_A_file_board) attacks |= (bit_board >> 17);
  if (bit_board & not_A_file_board) attacks |= (bit_board << 15);
  if (bit_board & not_H_file_board) attacks |= (bit_board << 17);
  if (bit_board & not_GH_file_board) attacks |= (bit_board >> 6);
  if (bit_board & not_AB_file_board) attacks |= (bit_board >> 10);
  if (bit_board & not_AB_file_board) attacks |= (bit_board << 6);
  if (bit_board & not_GH_file_board) attacks |= (bit_board << 10);
  cout << "attack:" << square << "\n";
  print_bit_board(attacks);

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
  cout << "board:" << square << "\n";
  print_bit_board(bit_board);
  if (bit_board & not_H_file_board) attacks |= (bit_board >> 7);
  if (bit_board & not_H_file_board) attacks |= (bit_board << 1);
  if (bit_board & not_A_file_board) attacks |= (bit_board >> 1);
  if (bit_board & not_A_file_board) attacks |= (bit_board >> 9);
  if (bit_board & not_A_file_board) attacks |= (bit_board << 7);
  if (bit_board & not_H_file_board) attacks |= (bit_board << 9);
  attacks |= (bit_board << 8);
  attacks |= (bit_board >> 8);

  cout << "attack:" << square << "\n";
  print_bit_board(attacks);

  return attacks;
}

U64 mask_pawn_attacks(int square, int side) {
  U64 attacks = 0ULL;
  U64 bit_board = 0ULL;
  U64 not_A_file_board = 18374403900871474942ULL;
  U64 not_H_file_board = 9187201950435737471ULL;

  // set piece on board
  bit_board = set_bit(bit_board, square);
  cout << "board:" << square << "\n";
  print_bit_board(bit_board);

  // white
  if (side) {
    if (bit_board & not_H_file_board) attacks |= (bit_board >> 7);
    if (bit_board & not_A_file_board) attacks |= (bit_board >> 9);
  } else {
    if (bit_board & not_A_file_board) attacks |= (bit_board << 7);
    if (bit_board & not_H_file_board) attacks |= (bit_board << 9);
  }
  cout << "attack:" << square << "\n";

  print_bit_board(attacks);

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

  cout << "attack:" << square << "\n";

  print_bit_board(attacks);

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

  cout << "attack:" << square << "\n";

  print_bit_board(attacks);

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

  cout << "attack:" << square << "\n";

  print_bit_board(attacks);

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

  cout << "attack:" << square << "\n";

  print_bit_board(attacks);

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