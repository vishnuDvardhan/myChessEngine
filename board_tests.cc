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

  vector<Turn> pawnTurnsExpected = {
      {{1, 1}, {2, 1}, Pieces::empty, false, false, {-1, -1}, false},
      {{1, 1}, {3, 1}, Pieces::empty, false, true, {2, 1}, false},
      {{1, 1}, {2, 2}, Pieces::empty, false, false, {-1, -1}, false}};
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
  currentState.printBoard();
  cout << currentState.board[2][3] << "\n";
  vector<Turn> ptl = pawnToLocations(1, 3, Colours::white, currentState);
  for (const auto& turn : ptl) {
    cout << "From: (" << turn.from.first << ", " << turn.from.second << ") "
         << "To: (" << turn.to.first << ", " << turn.to.second << ")\n";
  }
  cout << "----------------------\n";
  vector<Turn> turnList = generateTurns(currentState);
  for (const auto& turn : turnList) {
    cout << "From: (" << turn.from.first << ", " << turn.from.second << ") "
         << "To: (" << turn.to.first << ", " << turn.to.second << ")\n";
  }
  vector<Turn> allTurnsActual = generateTurns(currentState);
  cout << "All turns\n";
  for (const auto& turn : allTurnsActual) {
    cout << "From: (" << turn.from.first << ", " << turn.from.second << ") "
         << "To: (" << turn.to.first << ", " << turn.to.second << ")\n";
  }
  vector<Turn> allTurnsExpected = {
      {{0, 2}, {2, 3}}, {{0, 2}, {2, 1}}, {{0, 5}, {2, 6}}, {{0, 5}, {2, 4}},
      {{1, 0}, {2, 0}}, {{1, 0}, {3, 0}}, {{1, 1}, {2, 1}}, {{1, 1}, {3, 1}},
      {{1, 2}, {2, 2}}, {{1, 2}, {3, 2}}, {{1, 3}, {2, 3}}, {{1, 3}, {3, 3}},
      {{1, 4}, {2, 4}}, {{1, 4}, {3, 4}}, {{1, 5}, {2, 5}}, {{1, 5}, {3, 5}},
      {{1, 6}, {2, 6}}, {{1, 6}, {3, 6}}, {{1, 7}, {2, 7}}, {{1, 7}, {3, 7}}};
  ASSERT_EQ(allTurnsActual, allTurnsExpected);
  currentState.board[2][3] = Pieces::bBishop;
  allTurnsExpected = {
      {{0, 2}, {2, 3}}, {{0, 2}, {2, 1}}, {{0, 5}, {2, 6}}, {{0, 5}, {2, 4}},
      {{1, 0}, {2, 0}}, {{1, 0}, {3, 0}}, {{1, 1}, {2, 1}}, {{1, 1}, {3, 1}},
      {{1, 2}, {2, 2}}, {{1, 2}, {3, 2}}, {{1, 2}, {2, 3}}, {{1, 4}, {2, 4}},
      {{1, 4}, {3, 4}}, {{1, 4}, {2, 3}}, {{1, 5}, {2, 5}}, {{1, 5}, {3, 5}},
      {{1, 6}, {2, 6}}, {{1, 6}, {3, 6}}, {{1, 7}, {2, 7}}, {{1, 7}, {3, 7}}};
  allTurnsActual = generateTurns(currentState);
  ASSERT_EQ(allTurnsActual, allTurnsExpected);
}

TEST(TurnGenerationsTests, Enpassanttest) {
  gameState currentState = initBoard();
  currentState.board[3][3] = Pieces::bPawn;
  applyTurn({{1, 2}, {3, 2}, Pieces::empty, false, true, {2, 2}, false},
            currentState);
  vector<Turn> actualTurns = generateTurns(currentState);
  vector<Turn> expectedTurns = {
      {{3, 3}, {2, 3}, Pieces::empty, false, false, {-1, -1}, false},
      {{3, 3}, {2, 2}, Pieces::empty, false, false, {-1, -1}, true},
      {{6, 0}, {5, 0}, Pieces::empty, false, false, {-1, -1}, false},
      {{6, 0}, {4, 0}, Pieces::empty, false, true, {5, 0}, false},
      {{6, 1}, {5, 1}, Pieces::empty, false, false, {-1, -1}, false},
      {{6, 1}, {4, 1}, Pieces::empty, false, true, {5, 1}, false},
      {{6, 2}, {5, 2}, Pieces::empty, false, false, {-1, -1}, false},
      {{6, 2}, {4, 2}, Pieces::empty, false, true, {5, 2}, false},
      {{6, 3}, {5, 3}, Pieces::empty, false, false, {-1, -1}, false},
      {{6, 3}, {4, 3}, Pieces::empty, false, true, {5, 3}, false},
      {{6, 4}, {5, 4}, Pieces::empty, false, false, {-1, -1}, false},
      {{6, 4}, {4, 4}, Pieces::empty, false, true, {5, 4}, false},
      {{6, 5}, {5, 5}, Pieces::empty, false, false, {-1, -1}, false},
      {{6, 5}, {4, 5}, Pieces::empty, false, true, {5, 5}, false},
      {{6, 6}, {5, 6}, Pieces::empty, false, false, {-1, -1}, false},
      {{6, 6}, {4, 6}, Pieces::empty, false, true, {5, 6}, false},
      {{6, 7}, {5, 7}, Pieces::empty, false, false, {-1, -1}, false},
      {{6, 7}, {4, 7}, Pieces::empty, false, true, {5, 7}, false},
      {{7, 2}, {5, 3}, Pieces::empty, false, false, {-1, -1}, false},
      {{7, 2}, {5, 1}, Pieces::empty, false, false, {-1, -1}, false},
      {{7, 5}, {5, 6}, Pieces::empty, false, false, {-1, -1}, false},
      {{7, 5}, {5, 4}, Pieces::empty, false, false, {-1, -1}, false}};
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
  vector<Turn> allTurnsExpected = {
      {{0, 0}, {0, 1}, Pieces::empty, false, false, {-1, -1}, false},
      {{0, 0}, {0, 2}, Pieces::empty, false, false, {-1, -1}, false},
      {{0, 0}, {0, 3}, Pieces::empty, false, false, {-1, -1}, false},
      {{0, 4}, {0, 3}, Pieces::empty, false, false, {-1, -1}, false},
      {{0, 5}, {2, 6}, Pieces::empty, false, false, {-1, -1}, false},
      {{0, 5}, {2, 4}, Pieces::empty, false, false, {-1, -1}, false},
      {{1, 0}, {2, 0}, Pieces::empty, false, false, {-1, -1}, false},
      {{1, 0}, {3, 0}, Pieces::empty, false, true, {2, 0}, false},
      {{1, 1}, {2, 1}, Pieces::empty, false, false, {-1, -1}, false},
      {{1, 1}, {3, 1}, Pieces::empty, false, true, {2, 1}, false},
      {{1, 2}, {2, 2}, Pieces::empty, false, false, {-1, -1}, false},
      {{1, 2}, {3, 2}, Pieces::empty, false, true, {2, 2}, false},
      {{1, 3}, {2, 3}, Pieces::empty, false, false, {-1, -1}, false},
      {{1, 3}, {3, 3}, Pieces::empty, false, true, {2, 2}, false},
      {{1, 4}, {2, 4}, Pieces::empty, false, false, {-1, -1}, false},
      {{1, 4}, {3, 4}, Pieces::empty, false, true, {2, 4}, false},
      {{1, 5}, {2, 5}, Pieces::empty, false, false, {-1, -1}, false},
      {{1, 5}, {3, 5}, Pieces::empty, false, true, {2, 5}, false},
      {{1, 6}, {2, 6}, Pieces::empty, false, false, {-1, -1}, false},
      {{1, 6}, {3, 6}, Pieces::empty, false, true, {2, 6}, false},
      {{1, 7}, {2, 7}, Pieces::empty, false, false, {-1, -1}, false},
      {{1, 7}, {3, 7}, Pieces::empty, false, true, {2, 7}, false},
      {{0, 4}, {0, 2}, Pieces::empty, true, false, {-1, -1}, false}};

  ASSERT_EQ(allTurnsActual, allTurnsExpected);
  currentState = initBoard();
  currentState.currentColour = Colours::black;
  currentState.board[7][1] = Pieces::empty;
  currentState.board[7][2] = Pieces::empty;
  currentState.board[7][3] = Pieces::empty;
  allTurnsActual = generateTurns(currentState);
  allTurnsExpected = {
      {{6, 0}, {5, 0}, Pieces::empty, false, false, {-1, -1}, false},
      {{6, 0}, {4, 0}, Pieces::empty, false, true, {5, 0}, false},
      {{6, 1}, {5, 1}, Pieces::empty, false, false, {-1, -1}, false},
      {{6, 1}, {4, 1}, Pieces::empty, false, true, {5, 1}, false},
      {{6, 2}, {5, 2}, Pieces::empty, false, false, {-1, -1}, false},
      {{6, 2}, {4, 2}, Pieces::empty, false, true, {5, 2}, false},
      {{6, 3}, {5, 3}, Pieces::empty, false, false, {-1, -1}, false},
      {{6, 3}, {4, 3}, Pieces::empty, false, true, {5, 3}, false},
      {{6, 4}, {5, 4}, Pieces::empty, false, false, {-1, -1}, false},
      {{6, 4}, {4, 4}, Pieces::empty, false, true, {5, 4}, false},
      {{6, 5}, {5, 5}, Pieces::empty, false, false, {-1, -1}, false},
      {{6, 5}, {4, 5}, Pieces::empty, false, true, {5, 5}, false},
      {{6, 6}, {5, 6}, Pieces::empty, false, false, {-1, -1}, false},
      {{6, 6}, {4, 6}, Pieces::empty, false, true, {5, 6}, false},
      {{6, 7}, {5, 7}, Pieces::empty, false, false, {-1, -1}, false},
      {{6, 7}, {4, 7}, Pieces::empty, false, true, {5, 7}, false},
      {{7, 0}, {7, 1}, Pieces::empty, false, false, {-1, -1}, false},
      {{7, 0}, {7, 2}, Pieces::empty, false, false, {-1, -1}, false},
      {{7, 0}, {7, 3}, Pieces::empty, false, false, {-1, -1}, false},
      {{7, 4}, {7, 3}, Pieces::empty, false, false, {-1, -1}, false},
      {{7, 5}, {5, 6}, Pieces::empty, false, false, {-1, -1}, false},
      {{7, 5}, {5, 4}, Pieces::empty, false, false, {-1, -1}, false},
      {{7, 4}, {7, 2}, Pieces::empty, true, false, {-1, -1}, false}};
  ASSERT_EQ(allTurnsActual, allTurnsExpected);
}

TEST(TurnGenerationsTests, PromotionTurns) {
  gameState currentState = initBoard();
  currentState.board[1][0] = Pieces::bPawn;
  currentState.currentColour = Colours::black;
  currentState.printBoard();

  vector<Turn> actualTurns = generateTurns(currentState);
  vector<Turn> expectedTurns = {
      {{1, 0}, {0, 1}, Pieces::bQueen, false, false, {-1, -1}, false},
      {{1, 0}, {0, 1}, Pieces::bRook, false, false, {-1, -1}, false},
      {{1, 0}, {0, 1}, Pieces::bBishop, false, false, {-1, -1}, false},
      {{1, 0}, {0, 1}, Pieces::bKnight, false, false, {-1, -1}, false},
      {{6, 0}, {5, 0}, Pieces::empty, false, false, {-1, -1}, false},
      {{6, 0}, {4, 0}, Pieces::empty, false, true, {5, 0}, false},
      {{6, 1}, {5, 1}, Pieces::empty, false, false, {-1, -1}, false},
      {{6, 1}, {4, 1}, Pieces::empty, false, true, {5, 1}, false},
      {{6, 2}, {5, 2}, Pieces::empty, false, false, {-1, -1}, false},
      {{6, 2}, {4, 2}, Pieces::empty, false, true, {5, 2}, false},
      {{6, 3}, {5, 3}, Pieces::empty, false, false, {-1, -1}, false},
      {{6, 3}, {4, 3}, Pieces::empty, false, true, {5, 3}, false},
      {{6, 4}, {5, 4}, Pieces::empty, false, false, {-1, -1}, false},
      {{6, 4}, {4, 4}, Pieces::empty, false, true, {5, 4}, false},
      {{6, 5}, {5, 5}, Pieces::empty, false, false, {-1, -1}, false},
      {{6, 5}, {4, 5}, Pieces::empty, false, true, {5, 5}, false},
      {{6, 6}, {5, 6}, Pieces::empty, false, false, {-1, -1}, false},
      {{6, 6}, {4, 6}, Pieces::empty, false, true, {5, 6}, false},
      {{6, 7}, {5, 7}, Pieces::empty, false, false, {-1, -1}, false},
      {{6, 7}, {4, 7}, Pieces::empty, false, true, {5, 7}, false},
      {{7, 2}, {5, 3}, Pieces::empty, false, false, {-1, -1}, false},
      {{7, 2}, {5, 1}, Pieces::empty, false, false, {-1, -1}, false},
      {{7, 5}, {5, 6}, Pieces::empty, false, false, {-1, -1}, false},
      {{7, 5}, {5, 4}, Pieces::empty, false, false, {-1, -1}, false}};
  ASSERT_EQ(actualTurns, expectedTurns);
}

TEST(PerftTest, PerftTest1) {
  gameState currentState = initBoard();

  currentState.printBoard();
  long long int nodes;

  currentState.printBoard();

  nodes = perft(currentState, 8);
  ASSERT_EQ(nodes, 84998978956);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
