#include <stdio.h>

#include <cstring>
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

  a1, b1, c1, d1, e1, f1, g1, h1, no_sq
};

enum {
  black,white,both
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
int side = -1;
int enpassant = no_sq;

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

void init_all() {
  init_magic_numbers();
  init_leaper_attacks();
  // rook
  init_sliders_attacks(0);
  // bishop
  init_sliders_attacks(1);
}