#include <gtest/gtest.h>

#include "board.h"

TEST(TurnGenerationTests, KingTurns) {
  vector<Turn> kingTurnsActual;

  vector<Turn> kingTurnsExpected;
  gameState currentState = initBoard();

  kingTurnsActual = kingToLocations(6, 3, Colours::white, currentState);
  kingTurnsExpected = {{{6, 3}, {7, 2}},
                       {{6, 3}, {6, 2}},
                       {{6, 3}, {5, 4}},
                       {{6, 3}, {5, 3}},
                       {{6, 3}, {5, 2}}};
  ASSERT_EQ(kingTurnsActual, kingTurnsExpected);
}

TEST(TurnGenerationTests, RookTurns) {
  gameState currentState = initBoard();

  vector<Turn> rookTurnsActual =
      rookToLocations(1, 1, Colours::white, currentState);

  vector<Turn> rookTurnsExpected = {{{1, 1}, {2, 1}},
                                    {{1, 1}, {3, 1}},
                                    {{1, 1}, {4, 1}},
                                    {{1, 1}, {5, 1}},
                                    {{1, 1}, {6, 1}}};

  ASSERT_EQ(rookTurnsActual, rookTurnsExpected);
}

TEST(TurnGenerationTests, QueenTurns) {
  gameState currentState = initBoard();

  vector<Turn> queenTurnsActual =
      queenToLocations(1, 1, Colours::white, currentState);

  vector<Turn> queenTurnsExpected = {
      {{1, 1}, {2, 1}}, {{1, 1}, {3, 1}}, {{1, 1}, {4, 1}}, {{1, 1}, {5, 1}},
      {{1, 1}, {6, 1}}, {{1, 1}, {2, 2}}, {{1, 1}, {3, 3}}, {{1, 1}, {4, 4}},
      {{1, 1}, {5, 5}}, {{1, 1}, {6, 6}}, {{1, 1}, {2, 0}}};

  ASSERT_EQ(queenTurnsActual, queenTurnsExpected);
}

TEST(TurnGenerationTests, BishopTurns) {
  gameState currentState = initBoard();
  vector<Turn> bishopTurnsActual =
      bishopToLocations(1, 1, Colours::white, currentState);

  vector<Turn> bishopTurnsExpected = {{{1, 1}, {2, 2}}, {{1, 1}, {3, 3}},
                                      {{1, 1}, {4, 4}}, {{1, 1}, {5, 5}},
                                      {{1, 1}, {6, 6}}, {{1, 1}, {2, 0}}};

  ASSERT_EQ(bishopTurnsActual, bishopTurnsExpected);
}

TEST(TurnGenerationTests, PawnTurns) {
  gameState currentState = initBoard();
  cout << "Initial board\n";
  currentState.board[2][2] = Pieces::bPawn;

  vector<Turn> pawnTurnsActual =
      pawnToLocations(1, 1, Colours::white, currentState);

  vector<Turn> pawnTurnsExpected = {{{1, 1}, {2, 1}, Pieces::empty},
                                    {{1, 1}, {3, 1}, Pieces::empty},
                                    {{1, 1}, {2, 2}, Pieces::empty}};
  gameState origState = currentState;

  ASSERT_EQ(pawnTurnsActual, pawnTurnsExpected);
}

TEST(TurnGenerationTests, KnightsTurns) {
  gameState currentState = initBoard();
  vector<Turn> knightTurnsActual =
      knightToLocations(1, 1, Colours::white, currentState);

  vector<Turn> knightTurnsExpected = {
      {{1, 1}, {3, 2}}, {{1, 1}, {3, 0}}, {{1, 1}, {2, 3}}};

  ASSERT_EQ(knightTurnsActual, knightTurnsExpected);
}

TEST(TurnGenerationsTests, GenerateCorrectTurnTests) {
  gameState currentState = initBoard();

  vector<Turn> allTurnsActual = generateTurns(currentState);

  vector<Turn> allTurnsExpected;
  currentState.board[2][3] = Pieces::bBishop;
  allTurnsExpected = {
      {{0, 1}, {2, 2}, Pieces::empty}, {{0, 1}, {2, 0}, Pieces::empty},
      {{0, 6}, {2, 7}, Pieces::empty}, {{0, 6}, {2, 5}, Pieces::empty},
      {{1, 0}, {2, 0}, Pieces::empty}, {{1, 0}, {3, 0}, Pieces::empty},
      {{1, 1}, {2, 1}, Pieces::empty}, {{1, 1}, {3, 1}, Pieces::empty},
      {{1, 2}, {2, 2}, Pieces::empty}, {{1, 2}, {3, 2}, Pieces::empty},
      {{1, 2}, {2, 3}, Pieces::empty}, {{1, 4}, {2, 4}, Pieces::empty},
      {{1, 4}, {3, 4}, Pieces::empty}, {{1, 4}, {2, 3}, Pieces::empty},
      {{1, 5}, {2, 5}, Pieces::empty}, {{1, 5}, {3, 5}, Pieces::empty},
      {{1, 6}, {2, 6}, Pieces::empty}, {{1, 6}, {3, 6}, Pieces::empty},
      {{1, 7}, {2, 7}, Pieces::empty}, {{1, 7}, {3, 7}, Pieces::empty}};

  allTurnsActual = generateTurns(currentState);
  ASSERT_EQ(allTurnsActual, allTurnsExpected);
}

TEST(TurnGenerationsTests, Enpassanttest) {
  gameState currentState = initBoard();
  currentState.board[3][3] = Pieces::bPawn;
  applyTurn({{1, 2}, {3, 2}, Pieces::empty}, currentState);
  vector<Turn> actualTurns = generateTurns(currentState);
  vector<Turn> expectedTurns = {
      {{3, 3}, {2, 3}, Pieces::empty}, {{3, 3}, {2, 2}, Pieces::empty},
      {{6, 0}, {5, 0}, Pieces::empty}, {{6, 0}, {4, 0}, Pieces::empty},
      {{6, 1}, {5, 1}, Pieces::empty}, {{6, 1}, {4, 1}, Pieces::empty},
      {{6, 2}, {5, 2}, Pieces::empty}, {{6, 2}, {4, 2}, Pieces::empty},
      {{6, 3}, {5, 3}, Pieces::empty}, {{6, 3}, {4, 3}, Pieces::empty},
      {{6, 4}, {5, 4}, Pieces::empty}, {{6, 4}, {4, 4}, Pieces::empty},
      {{6, 5}, {5, 5}, Pieces::empty}, {{6, 5}, {4, 5}, Pieces::empty},
      {{6, 6}, {5, 6}, Pieces::empty}, {{6, 6}, {4, 6}, Pieces::empty},
      {{6, 7}, {5, 7}, Pieces::empty}, {{6, 7}, {4, 7}, Pieces::empty},
      {{7, 1}, {5, 2}, Pieces::empty}, {{7, 1}, {5, 0}, Pieces::empty},
      {{7, 6}, {5, 7}, Pieces::empty}, {{7, 6}, {5, 5}, Pieces::empty}};

  ASSERT_EQ(actualTurns, expectedTurns);
}

TEST(TurnGenerationsTests, CastlingTest) {
  gameState currentState = initBoard();
  currentState.board[0][1] = Pieces::empty;
  currentState.board[0][2] = Pieces::empty;
  currentState.board[0][3] = Pieces::empty;
  currentState.printBoard();
  cout << "current colors is " << currentState.currentColour;
  currentState.currentColour = Colours::white;
  vector<Turn> allTurnsActual = generateTurns(currentState);
  cout << "\n1\n";
  vector<Turn> allTurnsExpected = {
      {{0, 0}, {0, 1}, Pieces::empty}, {{0, 0}, {0, 2}, Pieces::empty},
      {{0, 0}, {0, 3}, Pieces::empty}, {{0, 4}, {0, 3}, Pieces::empty},
      {{0, 6}, {2, 7}, Pieces::empty}, {{0, 6}, {2, 5}, Pieces::empty},
      {{1, 0}, {2, 0}, Pieces::empty}, {{1, 0}, {3, 0}, Pieces::empty},
      {{1, 1}, {2, 1}, Pieces::empty}, {{1, 1}, {3, 1}, Pieces::empty},
      {{1, 2}, {2, 2}, Pieces::empty}, {{1, 2}, {3, 2}, Pieces::empty},
      {{1, 3}, {2, 3}, Pieces::empty}, {{1, 3}, {3, 3}, Pieces::empty},
      {{1, 4}, {2, 4}, Pieces::empty}, {{1, 4}, {3, 4}, Pieces::empty},
      {{1, 5}, {2, 5}, Pieces::empty}, {{1, 5}, {3, 5}, Pieces::empty},
      {{1, 6}, {2, 6}, Pieces::empty}, {{1, 6}, {3, 6}, Pieces::empty},
      {{1, 7}, {2, 7}, Pieces::empty}, {{1, 7}, {3, 7}, Pieces::empty},
      {{0, 4}, {0, 2}, Pieces::empty}};

  ASSERT_EQ(allTurnsActual, allTurnsExpected);
  cout << "\n2\n";
  currentState = initBoard();
  currentState.currentColour = Colours::black;
  currentState.board[7][1] = Pieces::empty;
  currentState.board[7][2] = Pieces::empty;
  currentState.board[7][3] = Pieces::empty;
  allTurnsActual = generateTurns(currentState);
  allTurnsExpected = {
      {{6, 0}, {5, 0}, Pieces::empty}, {{6, 0}, {4, 0}, Pieces::empty},
      {{6, 1}, {5, 1}, Pieces::empty}, {{6, 1}, {4, 1}, Pieces::empty},
      {{6, 2}, {5, 2}, Pieces::empty}, {{6, 2}, {4, 2}, Pieces::empty},
      {{6, 3}, {5, 3}, Pieces::empty}, {{6, 3}, {4, 3}, Pieces::empty},
      {{6, 4}, {5, 4}, Pieces::empty}, {{6, 4}, {4, 4}, Pieces::empty},
      {{6, 5}, {5, 5}, Pieces::empty}, {{6, 5}, {4, 5}, Pieces::empty},
      {{6, 6}, {5, 6}, Pieces::empty}, {{6, 6}, {4, 6}, Pieces::empty},
      {{6, 7}, {5, 7}, Pieces::empty}, {{6, 7}, {4, 7}, Pieces::empty},
      {{7, 0}, {7, 1}, Pieces::empty}, {{7, 0}, {7, 2}, Pieces::empty},
      {{7, 0}, {7, 3}, Pieces::empty}, {{7, 4}, {7, 3}, Pieces::empty},
      {{7, 6}, {5, 7}, Pieces::empty}, {{7, 6}, {5, 5}, Pieces::empty},
      {{7, 4}, {7, 2}, Pieces::empty}};
  ASSERT_EQ(allTurnsActual, allTurnsExpected);
}

TEST(TurnGenerationsTests, PromotionTurns) {
  gameState currentState = initBoard();
  currentState.board[1][0] = Pieces::bPawn;
  currentState.currentColour = Colours::black;
  currentState.printBoard();

  vector<Turn> actualTurns = generateTurns(currentState);
  vector<Turn> expectedTurns = {
      {{1, 0}, {0, 1}, Pieces::bQueen},  {{1, 0}, {0, 1}, Pieces::bRook},
      {{1, 0}, {0, 1}, Pieces::bBishop}, {{1, 0}, {0, 1}, Pieces::bKnight},
      {{6, 0}, {5, 0}, Pieces::empty},   {{6, 0}, {4, 0}, Pieces::empty},
      {{6, 1}, {5, 1}, Pieces::empty},   {{6, 1}, {4, 1}, Pieces::empty},
      {{6, 2}, {5, 2}, Pieces::empty},   {{6, 2}, {4, 2}, Pieces::empty},
      {{6, 3}, {5, 3}, Pieces::empty},   {{6, 3}, {4, 3}, Pieces::empty},
      {{6, 4}, {5, 4}, Pieces::empty},   {{6, 4}, {4, 4}, Pieces::empty},
      {{6, 5}, {5, 5}, Pieces::empty},   {{6, 5}, {4, 5}, Pieces::empty},
      {{6, 6}, {5, 6}, Pieces::empty},   {{6, 6}, {4, 6}, Pieces::empty},
      {{6, 7}, {5, 7}, Pieces::empty},   {{6, 7}, {4, 7}, Pieces::empty},
      {{7, 1}, {5, 2}, Pieces::empty},   {{7, 1}, {5, 0}, Pieces::empty},
      {{7, 6}, {5, 7}, Pieces::empty},   {{7, 6}, {5, 5}, Pieces::empty}};

  ASSERT_EQ(actualTurns, expectedTurns);
}

TEST(fen_test, fen_test) {
  gameState currentState = initBoard();
  string fen = "rnbqkbnr/ppppppp1/7p/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  initBoardWithFen(currentState, fen);
  currentState.printBoard();
}

TEST(FenTest, stateToFenTest) {
  gameState currentState = initBoard();
  string fen = "rnbqkbnr/ppppppp1/7p/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  initBoardWithFen(currentState, fen);
  currentState.printBoard();
  string generatedFen = toFen(currentState);
  cout << "Generated FEN: " << generatedFen << "\n";
  ASSERT_EQ(generatedFen, fen);
}

TEST(PerftTests, Test1) {
  gameState currentState = initBoard();
  string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  initBoardWithFen(currentState, fen);
  long long int nodes = perft(currentState, 6);
  ASSERT_EQ(nodes, 119060324);
}

TEST(PerftTests, Test2) {
  gameState currentState = initBoard();
  string fen =
      "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -";
  initBoardWithFen(currentState, fen);
  long long int nodes = perft(currentState, 5);
  ASSERT_EQ(nodes, 193690690);
}

TEST(PerftTests, Test3) {
  gameState currentState = initBoard();
  string fen = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -";
  initBoardWithFen(currentState, fen);
  long long int nodes = perft(currentState, 6);
  ASSERT_EQ(nodes, 11030083);
}

TEST(PerftTests, Test4) {
  gameState currentState = initBoard();
  string fen =
      "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
  initBoardWithFen(currentState, fen);
  long long int nodes = perft(currentState, 5);
  ASSERT_EQ(nodes, 15833292);
}

TEST(PerftTests, Test5) {
  gameState currentState = initBoard();
  string fen =
      "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1";
  initBoardWithFen(currentState, fen);
  long long int nodes = perft(currentState, 5);
  ASSERT_EQ(nodes, 15833292);
}

TEST(PerftTests, Test6) {
  gameState currentState = initBoard();
  string fen = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
  initBoardWithFen(currentState, fen);
  long long int nodes = perft(currentState, 5);
  ASSERT_EQ(nodes, 89941194);
}

TEST(PerftTests, Test7) {
  gameState currentState = initBoard();
  string fen =
      "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 "
      "10";
  initBoardWithFen(currentState, fen);
  long long int nodes = perft(currentState, 5);
  ASSERT_EQ(nodes, 164075551);
}

TEST(PerftTests, Test8) {
  gameState currentState = initBoard();
  string fen =
      "r3k2r/p1p1qpb1/bn1ppnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/1K1R3R b kq -";
  initBoardWithFen(currentState, fen);
  long long int nodes = perft(currentState, 5);
  ASSERT_EQ(nodes, 158391136);
}

TEST(PerftTests, Test9) {
  gameState currentState = initBoard();
  string fen = "3Q4/1Q4Q1/4Q3/2Q4R/Q4Q2/3Q4/1Q4Rp/1K1BBNNk w - - 0 1";
  initBoardWithFen(currentState, fen);
  long long int nodes = perft(currentState, 6);
  ASSERT_EQ(nodes, 24376626);
}

TEST(PerftTests, Test10) {
  gameState currentState = initBoard();
  string fen = "4k3/8/8/2b5/8/8/3P4/4K3 w -";
  initBoardWithFen(currentState, fen);
  long long int nodes = perft(currentState, 7);
  ASSERT_EQ(nodes, 4165765);
}

TEST(PerftTests, Test11) {
  gameState currentState = initBoard();
  string fen = "4kb2/8/8/8/8/8/3P4/4K3 b -";
  initBoardWithFen(currentState, fen);
  long long int nodes = perft(currentState, 7);
  ASSERT_EQ(nodes, 7981219);
}

TEST(PerftTests, Test12) {
  gameState currentState = initBoard();
  string fen = "8/2p5/3p4/KP6/5p1k/8/4P1P1/8 w - -";
  initBoardWithFen(currentState, fen);
  long long int nodes = perft(currentState, 7);
  ASSERT_EQ(nodes, 3734443);
}

TEST(PerftTests, Test13) {
  gameState currentState = initBoard();
  string fen = "8/PPP4k/8/8/8/8/4Kppp/8 w - - 0 1";
  initBoardWithFen(currentState, fen);
  long long int nodes = perft(currentState, 6);
  ASSERT_EQ(nodes, 34336777);
}

TEST(PerftTests, Test14) {
  gameState currentState = initBoard();
  string fen = "n1r1r1b1/1P1P1P1P/1Q6/3NBNK1/R7/4p1p1/3PBPkP/2R5 w - - 0 1";
  initBoardWithFen(currentState, fen);
  long long int nodes = perft(currentState, 4);
  ASSERT_EQ(nodes, 9421435);
}

TEST(PerftTests, Test15) {
  gameState currentState = initBoard();
  string fen =
      "r1n1n1b1/1P1P1P1P/1N1N1N2/2RnQrRq/2pKp3/3BNQbQ/k7/4Bq2 w - - 0 1";
  initBoardWithFen(currentState, fen);
  long long int nodes = perft(currentState, 4);
  ASSERT_EQ(nodes, 39508843);
}

TEST(PerftTests, Test16) {
  gameState currentState = initBoard();
  string fen = "rnbqqbnr/PPPPPPPP/8/3ppp2/3pkp2/3N1N2/4K3/8 w - - 0 1";
  initBoardWithFen(currentState, fen);
  long long int nodes = perft(currentState, 4);
  ASSERT_EQ(nodes, 1885519);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
