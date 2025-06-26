#include <gtest/gtest.h>

#include "board.h"

TEST(TurnGenerationTests, KingTurns) {
  gameState currentState = initBoard();
  Turns toLocations;
  int n = 0;

  // Test king in open area - place at position {6, 3} (corresponds to c5 in
  // chess notation)
  currentState.board[6][3] = Pieces::wKing;
  currentState.board[2][6] = Pieces::empty;  // Remove original king
  currentState.wKingLocation = {6, 3};

  kingToLocations(currentState, 6, 3, true, toLocations, n);

  // Expected moves for king at c5: b4, b5, b6, c4, c6, d4, d5, d6
  vector<Turn> expectedTurns = {
      {{6, 3}, {7, 2}},  // b6
      {{6, 3}, {6, 2}},  // b5
      {{6, 3}, {5, 4}},  // d4
      {{6, 3}, {5, 3}},  // c4
      {{6, 3}, {5, 2}}   // b4
  };

  ASSERT_EQ(n, 5);
}

TEST(TurnGenerationTests, RookTurns) {
  gameState currentState = initBoard();
  Turns toLocations;
  int n = 0;

  // Place rook at position {3, 3} (corresponds to c2 in chess notation)
  currentState.board[3][3] = Pieces::wRook;
  rookToLocations(currentState, 3, 3, true, toLocations, n);

  // Expected moves for rook at c2
  vector<Turn> expectedTurns = {
      {{3, 3}, {2, 3}},  // c1
      {{3, 3}, {4, 3}},  // c3
      {{3, 3}, {5, 3}},  // c4
      {{3, 3}, {6, 3}},  // c5
      {{3, 3}, {7, 3}}   // c6
  };

  ASSERT_EQ(n, 5);
}

TEST(TurnGenerationTests, QueenTurns) {
  gameState currentState = initBoard();
  Turns toLocations;
  int n = 0;

  // Place queen at position {3, 3} (corresponds to c2 in chess notation)
  currentState.board[3][3] = Pieces::wQueen;
  queenToLocations(currentState, 3, 3, true, toLocations, n);

  // Queen combines rook and bishop moves
  vector<Turn> expectedTurns = {
      {{3, 3}, {2, 3}}, {{3, 3}, {4, 3}}, {{3, 3}, {5, 3}}, {{3, 3}, {6, 3}},
      {{3, 3}, {7, 3}}, {{3, 3}, {2, 2}}, {{3, 3}, {4, 4}}, {{3, 3}, {5, 5}},
      {{3, 3}, {6, 6}}, {{3, 3}, {7, 7}}, {{3, 3}, {2, 4}}};

  ASSERT_EQ(n, 11);
}

TEST(TurnGenerationTests, BishopTurns) {
  gameState currentState = initBoard();
  Turns toLocations;
  int n = 0;

  // Place bishop at position {3, 3} (corresponds to c2 in chess notation)
  currentState.board[3][3] = Pieces::wBishop;
  bishopToLocations(currentState, 3, 3, true, toLocations, n);

  vector<Turn> expectedTurns = {{{3, 3}, {2, 2}}, {{3, 3}, {4, 4}},
                                {{3, 3}, {5, 5}}, {{3, 3}, {6, 6}},
                                {{3, 3}, {7, 7}}, {{3, 3}, {2, 4}}};

  ASSERT_EQ(n, 6);
}

TEST(TurnGenerationTests, PawnTurns) {
  gameState currentState = initBoard();
  Turns toLocations;
  int n = 0;

  // Test pawn at starting position and add enemy piece for capture
  currentState.board[4][4] = Pieces::bPawn;  // Enemy pawn for capture test

  pawnToLocations(currentState, 3, 3, true, toLocations, n);

  vector<Turn> expectedTurns = {
      {{3, 3}, {4, 3}, Pieces::empty},  // Move forward one
      {{3, 3}, {5, 3}, Pieces::empty},  // Move forward two (from starting rank)
      {{3, 3}, {4, 4}, Pieces::empty}   // Capture enemy pawn
  };

  ASSERT_EQ(n, 3);
}

TEST(TurnGenerationTests, KnightsTurns) {
  gameState currentState = initBoard();
  Turns toLocations;
  int n = 0;

  // Place knight at position {3, 3} (corresponds to c2 in chess notation)
  currentState.board[3][3] = Pieces::wKnight;
  knightToLocations(currentState, 3, 3, true, toLocations, n);

  vector<Turn> expectedTurns = {
      {{3, 3}, {5, 4}}, {{3, 3}, {5, 2}}, {{3, 3}, {4, 5}}};

  ASSERT_EQ(n, 3);
}

TEST(TurnGenerationsTests, GenerateCorrectTurnTests) {
  gameState currentState = initBoard();
  Turns allTurns;
  int n = 0;

  // Add enemy bishop to create more complex scenario
  currentState.board[4][5] = Pieces::bBishop;

  generateTurns(currentState, allTurns, n);

  vector<Turn> expectedTurns = {
      {{2, 3}, {4, 4}, Pieces::empty}, {{2, 3}, {4, 2}, Pieces::empty},
      {{2, 8}, {4, 9}, Pieces::empty}, {{2, 8}, {4, 7}, Pieces::empty},
      {{3, 2}, {4, 2}, Pieces::empty}, {{3, 2}, {5, 2}, Pieces::empty},
      {{3, 3}, {4, 3}, Pieces::empty}, {{3, 3}, {5, 3}, Pieces::empty},
      {{3, 4}, {4, 4}, Pieces::empty}, {{3, 4}, {5, 4}, Pieces::empty},
      {{3, 4}, {4, 5}, Pieces::empty}, {{3, 6}, {4, 6}, Pieces::empty},
      {{3, 6}, {5, 6}, Pieces::empty}, {{3, 6}, {4, 5}, Pieces::empty},
      {{3, 7}, {4, 7}, Pieces::empty}, {{3, 7}, {5, 7}, Pieces::empty},
      {{3, 8}, {4, 8}, Pieces::empty}, {{3, 8}, {5, 8}, Pieces::empty},
      {{3, 9}, {4, 9}, Pieces::empty}, {{3, 9}, {5, 9}, Pieces::empty}};

  ASSERT_EQ(n, 20);
}

TEST(TurnGenerationsTests, Enpassanttest) {
  gameState currentState = initBoard();

  // Set up en passant scenario - simulate a white pawn moving two squares
  // Clear initial pawn positions
  currentState.board[3][4] = Pieces::empty;  // Remove white pawn from d2
  currentState.board[8][5] = Pieces::empty;  // Remove black pawn from e7

  // Place pawns for en passant scenario
  currentState.board[5][4] =
      Pieces::wPawn;  // White pawn at d4 (rank 4 = index 5)
  currentState.board[5][5] =
      Pieces::bPawn;  // Black pawn at e4 (rank 4 = index 5)

  // Set en passant square (the square behind the pawn that just moved two
  // squares)
  currentState.enpassantSq = {4, 4};  // d3 square (rank 3 = index 4)
  currentState.isWhitesTurn = false;  // Black to move

  Turns actualTurns;
  int n = 0;
  generateTurns(currentState, actualTurns, n);

  // Should include en passant capture
  bool foundEnPassant = false;
  for (int i = 0; i < n; i++) {
    if (actualTurns[i].enPassant) {
      foundEnPassant = true;
      break;
    }
  }
  ASSERT_TRUE(foundEnPassant);
}

TEST(TurnGenerationsTests, CastlingTest) {
  gameState currentState = initBoard();

  // Clear pieces for white queenside castling
  currentState.board[2][3] = Pieces::empty;  // b1
  currentState.board[2][4] = Pieces::empty;  // c1
  currentState.board[2][5] = Pieces::empty;  // d1

  Turns allTurns;
  int n = 0;
  generateTurns(currentState, allTurns, n);

  // Should include castling move
  bool foundCastling = false;
  for (int i = 0; i < n; i++) {
    if (allTurns[i].castling) {
      foundCastling = true;
      break;
    }
  }
  ASSERT_TRUE(foundCastling);

  // Test black castling
  currentState = initBoard();
  currentState.isWhitesTurn = false;
  currentState.board[9][3] = Pieces::empty;  // b8
  currentState.board[9][4] = Pieces::empty;  // c8
  currentState.board[9][5] = Pieces::empty;  // d8

  n = 0;
  generateTurns(currentState, allTurns, n);

  foundCastling = false;
  for (int i = 0; i < n; i++) {
    if (allTurns[i].castling) {
      foundCastling = true;
      break;
    }
  }
  ASSERT_TRUE(foundCastling);
}

TEST(TurnGenerationsTests, PromotionTurns) {
  gameState currentState = initBoard();

  // Clear the board for a clean promotion test
  for (int i = 2; i < 10; i++) {
    for (int j = 2; j < 10; j++) {
      currentState.board[i][j] = Pieces::empty;
    }
  }

  // Set up promotion scenario - black pawn on 2nd rank (index 3)
  currentState.board[3][2] =
      Pieces::bPawn;  // Black pawn on a2 (about to promote)
  currentState.board[2][6] = Pieces::wKing;  // White king on e1
  currentState.board[9][6] = Pieces::bKing;  // Black king on e8
  currentState.wKingLocation = {2, 6};
  currentState.bKingLocation = {9, 6};
  currentState.isWhitesTurn = false;  // Black to move

  Turns actualTurns;
  int n = 0;
  generateTurns(currentState, actualTurns, n);

  // Count promotion moves
  int promotionCount = 0;
  for (int i = 0; i < n; i++) {
    if (actualTurns[i].promotionPiece != Pieces::empty) {
      promotionCount++;
    }
  }

  // Should have 4 promotion options (Queen, Rook, Bishop, Knight)
  ASSERT_EQ(promotionCount, 4);
}

TEST(fen_test, fen_test) {
  gameState currentState;
  string fen = "rnbqkbnr/ppppppp1/7p/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  initBoardWithFen(currentState, fen);

  // Verify some key pieces
  ASSERT_EQ(currentState.board[9][2], Pieces::bRook);  // a8
  ASSERT_EQ(currentState.board[2][6], Pieces::wKing);  // e1
  ASSERT_EQ(currentState.isWhitesTurn, true);
}

TEST(FenTest, stateToFenTest) {
  gameState currentState;
  string fen = "rnbqkbnr/ppppppp1/7p/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  initBoardWithFen(currentState, fen);
  string generatedFen = toFen(currentState);
  cout << "Generated FEN: " << generatedFen << "\n";
  ASSERT_EQ(generatedFen, fen);
}

TEST(PerftTests, Test1) {
  gameState currentState = initBoard();
  string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  initBoardWithFen(currentState, fen);
  uint64_t nodes = perft(currentState, 6);
  ASSERT_EQ(nodes, 119060324);
}

TEST(PerftTests, Test2) {
  gameState currentState = initBoard();
  string fen =
      "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -";
  initBoardWithFen(currentState, fen);
  uint64_t nodes = perft(currentState, 5);
  ASSERT_EQ(nodes, 193690690);
}

TEST(PerftTests, Test3) {
  gameState currentState = initBoard();
  string fen = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -";
  initBoardWithFen(currentState, fen);
  uint64_t nodes = perft(currentState, 6);
  ASSERT_EQ(nodes, 11030083);
}

TEST(PerftTests, Test4) {
  gameState currentState = initBoard();
  string fen =
      "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
  initBoardWithFen(currentState, fen);
  uint64_t nodes = perft(currentState, 5);
  ASSERT_EQ(nodes, 15833292);
}

TEST(PerftTests, Test5) {
  gameState currentState = initBoard();
  string fen =
      "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1";
  initBoardWithFen(currentState, fen);
  uint64_t nodes = perft(currentState, 5);
  ASSERT_EQ(nodes, 15833292);
}

TEST(PerftTests, Test6) {
  gameState currentState = initBoard();
  string fen = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
  initBoardWithFen(currentState, fen);
  uint64_t nodes = perft(currentState, 5);
  ASSERT_EQ(nodes, 89941194);
}

TEST(PerftTests, Test7) {
  gameState currentState = initBoard();
  string fen =
      "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 "
      "10";
  initBoardWithFen(currentState, fen);
  uint64_t nodes = perft(currentState, 5);
  ASSERT_EQ(nodes, 164075551);
}

TEST(PerftTests, Test8) {
  gameState currentState = initBoard();
  string fen =
      "r3k2r/p1p1qpb1/bn1ppnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/1K1R3R b kq -";
  initBoardWithFen(currentState, fen);
  uint64_t nodes = perft(currentState, 5);
  ASSERT_EQ(nodes, 158391136);
}

TEST(PerftTests, Test9) {
  gameState currentState = initBoard();
  string fen = "3Q4/1Q4Q1/4Q3/2Q4R/Q4Q2/3Q4/1Q4Rp/1K1BBNNk w - - 0 1";
  initBoardWithFen(currentState, fen);
  uint64_t nodes = perft(currentState, 6);
  ASSERT_EQ(nodes, 24376626);
}

TEST(PerftTests, Test10) {
  gameState currentState = initBoard();
  string fen = "4k3/8/8/2b5/8/8/3P4/4K3 w -";
  initBoardWithFen(currentState, fen);
  uint64_t nodes = perft(currentState, 7);
  ASSERT_EQ(nodes, 4165765);
}

TEST(PerftTests, Test11) {
  gameState currentState = initBoard();
  string fen = "4kb2/8/8/8/8/8/3P4/4K3 b -";
  initBoardWithFen(currentState, fen);
  uint64_t nodes = perft(currentState, 7);
  ASSERT_EQ(nodes, 7981219);
}

TEST(PerftTests, Test12) {
  gameState currentState = initBoard();
  string fen = "8/2p5/3p4/KP6/5p1k/8/4P1P1/8 w - -";
  initBoardWithFen(currentState, fen);
  uint64_t nodes = perft(currentState, 7);
  ASSERT_EQ(nodes, 3734443);
}

TEST(PerftTests, Test13) {
  gameState currentState = initBoard();
  string fen = "8/PPP4k/8/8/8/8/4Kppp/8 w - - 0 1";
  initBoardWithFen(currentState, fen);
  uint64_t nodes = perft(currentState, 6);
  ASSERT_EQ(nodes, 34336777);
}

TEST(PerftTests, Test14) {
  gameState currentState = initBoard();
  string fen = "n1r1r1b1/1P1P1P1P/1Q6/3NBNK1/R7/4p1p1/3PBPkP/2R5 w - - 0 1";
  initBoardWithFen(currentState, fen);
  uint64_t nodes = perft(currentState, 4);
  ASSERT_EQ(nodes, 9421435);
}

TEST(PerftTests, Test15) {
  gameState currentState = initBoard();
  string fen =
      "r1n1n1b1/1P1P1P1P/1N1N1N2/2RnQrRq/2pKp3/3BNQbQ/k7/4Bq2 w - - 0 1";
  initBoardWithFen(currentState, fen);
  uint64_t nodes = perft(currentState, 4);
  ASSERT_EQ(nodes, 39508843);
}

TEST(PerftTests, Test16) {
  gameState currentState = initBoard();
  string fen = "rnbqqbnr/PPPPPPPP/8/3ppp2/3pkp2/3N1N2/4K3/8 w - - 0 1";
  initBoardWithFen(currentState, fen);
  uint64_t nodes = perft(currentState, 4);
  ASSERT_EQ(nodes, 1885519);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}