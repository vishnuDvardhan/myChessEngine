#include <gtest/gtest.h>

#include <iostream>
#include <string>

// Include your bitboard engine header
#include "bit-board.h"

using namespace std;

TEST(PerftTests, Test1) {
  init_all();
  memset(bitboards, 0, sizeof(bitboards));
  memset(occupancies, 0, sizeof(occupancies));
  string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  init_from_fen(const_cast<char*>(fen.c_str()));
  uint64_t nodes = perft(6);
  ASSERT_EQ(nodes, 119060324);
}

TEST(PerftTests, Test2) {
  init_all();
  memset(bitboards, 0, sizeof(bitboards));
  memset(occupancies, 0, sizeof(occupancies));
  string fen =
      "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -";
  init_from_fen(const_cast<char*>(fen.c_str()));
  uint64_t nodes = perft(5);
  ASSERT_EQ(nodes, 193690690);
}

TEST(PerftTests, Test3) {
  init_all();
  memset(bitboards, 0, sizeof(bitboards));
  memset(occupancies, 0, sizeof(occupancies));
  string fen = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -";
  init_from_fen(const_cast<char*>(fen.c_str()));
  uint64_t nodes = perft(6);
  ASSERT_EQ(nodes, 11030083);
}

TEST(PerftTests, Test4) {
  init_all();
  memset(bitboards, 0, sizeof(bitboards));
  memset(occupancies, 0, sizeof(occupancies));
  string fen =
      "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
  init_from_fen(const_cast<char*>(fen.c_str()));
  uint64_t nodes = perft(5);
  ASSERT_EQ(nodes, 15833292);
}

TEST(PerftTests, Test5) {
  init_all();
  memset(bitboards, 0, sizeof(bitboards));
  memset(occupancies, 0, sizeof(occupancies));
  string fen =
      "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1";
  init_from_fen(const_cast<char*>(fen.c_str()));
  uint64_t nodes = perft(5);
  ASSERT_EQ(nodes, 15833292);
}

TEST(PerftTests, Test6) {
  init_all();
  memset(bitboards, 0, sizeof(bitboards));
  memset(occupancies, 0, sizeof(occupancies));
  string fen = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
  init_from_fen(const_cast<char*>(fen.c_str()));
  uint64_t nodes = perft(5);
  ASSERT_EQ(nodes, 89941194);
}

TEST(PerftTests, Test7) {
  init_all();
  memset(bitboards, 0, sizeof(bitboards));
  memset(occupancies, 0, sizeof(occupancies));
  string fen =
      "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 "
      "10";
  init_from_fen(const_cast<char*>(fen.c_str()));
  uint64_t nodes = perft(5);
  ASSERT_EQ(nodes, 164075551);
}

TEST(PerftTests, Test8) {
  init_all();
  memset(bitboards, 0, sizeof(bitboards));
  memset(occupancies, 0, sizeof(occupancies));
  string fen =
      "r3k2r/p1p1qpb1/bn1ppnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/1K1R3R b kq -";
  init_from_fen(const_cast<char*>(fen.c_str()));
  uint64_t nodes = perft(5);
  ASSERT_EQ(nodes, 158391136);
}

TEST(PerftTests, Test9) {
  init_all();
  memset(bitboards, 0, sizeof(bitboards));
  memset(occupancies, 0, sizeof(occupancies));
  string fen = "3Q4/1Q4Q1/4Q3/2Q4R/Q4Q2/3Q4/1Q4Rp/1K1BBNNk w - - 0 1";
  init_from_fen(const_cast<char*>(fen.c_str()));
  uint64_t nodes = perft(6);
  ASSERT_EQ(nodes, 24376626);
}

TEST(PerftTests, Test10) {
  init_all();
  memset(bitboards, 0, sizeof(bitboards));
  memset(occupancies, 0, sizeof(occupancies));
  string fen = "4k3/8/8/2b5/8/8/3P4/4K3 w -";
  init_from_fen(const_cast<char*>(fen.c_str()));
  uint64_t nodes = perft(7);
  ASSERT_EQ(nodes, 4165765);
}

TEST(PerftTests, Test11) {
  init_all();
  memset(bitboards, 0, sizeof(bitboards));
  memset(occupancies, 0, sizeof(occupancies));
  string fen = "4kb2/8/8/8/8/8/3P4/4K3 b -";
  init_from_fen(const_cast<char*>(fen.c_str()));
  uint64_t nodes = perft(7);
  ASSERT_EQ(nodes, 7981219);
}

TEST(PerftTests, Test12) {
  init_all();
  memset(bitboards, 0, sizeof(bitboards));
  memset(occupancies, 0, sizeof(occupancies));
  string fen = "8/2p5/3p4/KP6/5p1k/8/4P1P1/8 w - -";
  init_from_fen(const_cast<char*>(fen.c_str()));
  uint64_t nodes = perft(7);
  ASSERT_EQ(nodes, 3734443);
}

TEST(PerftTests, Test13) {
  init_all();
  memset(bitboards, 0, sizeof(bitboards));
  memset(occupancies, 0, sizeof(occupancies));
  string fen = "8/PPP4k/8/8/8/8/4Kppp/8 w - - 0 1";
  init_from_fen(const_cast<char*>(fen.c_str()));
  uint64_t nodes = perft(6);
  ASSERT_EQ(nodes, 34336777);
}

TEST(PerftTests, Test14) {
  init_all();
  memset(bitboards, 0, sizeof(bitboards));
  memset(occupancies, 0, sizeof(occupancies));
  string fen = "n1r1r1b1/1P1P1P1P/1Q6/3NBNK1/R7/4p1p1/3PBPkP/2R5 w - - 0 1";
  init_from_fen(const_cast<char*>(fen.c_str()));
  uint64_t nodes = perft(4);
  ASSERT_EQ(nodes, 9421435);
}

TEST(PerftTests, Test15) {
  init_all();
  memset(bitboards, 0, sizeof(bitboards));
  memset(occupancies, 0, sizeof(occupancies));
  string fen =
      "r1n1n1b1/1P1P1P1P/1N1N1N2/2RnQrRq/2pKp3/3BNQbQ/k7/4Bq2 w - - 0 1";
  init_from_fen(const_cast<char*>(fen.c_str()));
  uint64_t nodes = perft(4);
  ASSERT_EQ(nodes, 39508843);
}

TEST(PerftTests, Test16) {
  init_all();
  memset(bitboards, 0, sizeof(bitboards));
  memset(occupancies, 0, sizeof(occupancies));
  string fen = "rnbqqbnr/PPPPPPPP/8/3ppp2/3pkp2/3N1N2/4K3/8 w - - 0 1";
  init_from_fen(const_cast<char*>(fen.c_str()));
  uint64_t nodes = perft(4);
  ASSERT_EQ(nodes, 1885519);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}