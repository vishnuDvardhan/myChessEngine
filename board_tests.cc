#include <gtest/gtest.h>

#include "board.h"

TEST(TurnGenerationTests, KingTurns) {
  vector<Turn> kingTurnsActual;

  vector<Turn> kingTurnsExpected;

  kingTurnsActual = kingToLocations(6, 3, Colours::white);
  kingTurnsExpected = {{{6, 3}, {7, 2}},
                       {{6, 3}, {6, 2}},
                       {{6, 3}, {5, 4}},
                       {{6, 3}, {5, 3}},
                       {{6, 3}, {5, 2}}};
  ASSERT_EQ(kingTurnsActual, kingTurnsExpected);
}

TEST(TurnGenerationTests, RookTurns) {
  vector<Turn> rookTurnsActual = rookToLocations(1, 1, Colours::white);

  vector<Turn> rookTurnsExpected = {{{1, 1}, {2, 1}},
                                    {{1, 1}, {3, 1}},
                                    {{1, 1}, {4, 1}},
                                    {{1, 1}, {5, 1}},
                                    {{1, 1}, {6, 1}}};

  ASSERT_EQ(rookTurnsActual, rookTurnsExpected);
}

TEST(TurnGenerationTests, QueenTurns) {
  vector<Turn> queenTurnsActual = queenToLocations(1, 1, Colours::white);

  vector<Turn> queenTurnsExpected = {
      {{1, 1}, {2, 1}}, {{1, 1}, {3, 1}}, {{1, 1}, {4, 1}}, {{1, 1}, {5, 1}},
      {{1, 1}, {6, 1}}, {{1, 1}, {2, 2}}, {{1, 1}, {3, 3}}, {{1, 1}, {4, 4}},
      {{1, 1}, {5, 5}}, {{1, 1}, {6, 6}}, {{1, 1}, {2, 0}}};

  ASSERT_EQ(queenTurnsActual, queenTurnsExpected);
}

TEST(TurnGenerationTests, BishopTurns) {
  vector<Turn> bishopTurnsActual = bishopToLocations(1, 1, Colours::white);

  vector<Turn> bishopTurnsExpected = {{{1, 1}, {2, 2}}, {{1, 1}, {3, 3}},
                                      {{1, 1}, {4, 4}}, {{1, 1}, {5, 5}},
                                      {{1, 1}, {6, 6}}, {{1, 1}, {2, 0}}};

  ASSERT_EQ(bishopTurnsActual, bishopTurnsExpected);
}

TEST(TurnGenerationTests, PawnTurns) {
  vector<Turn> pawnTurnsActual = pawnToLocations(1, 1, Colours::white);

  vector<Turn> pawnTurnsExpected = {{{1, 1}, {2, 1}}, {{1, 1}, {3, 1}}};

  ASSERT_EQ(pawnTurnsActual, pawnTurnsExpected);
}

TEST(TurnGenerationTests, KnightsTurns) {
  vector<Turn> knightTurnsActual = knightToLocations(1, 1, Colours::white);

  vector<Turn> knightTurnsExpected = {
      {{1, 1}, {3, 2}}, {{1, 1}, {3, 0}}, {{1, 1}, {2, 3}}};

  ASSERT_EQ(knightTurnsActual, knightTurnsExpected);
}

TEST(TurnGenerationsTests, GenerateCorrectTurnTests) {
  vector<Turn> allTurnsActual = generateTurns(Colours::white);
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
  allTurnsActual = generateTurns(Colours::white);
  ASSERT_EQ(allTurnsActual, allTurnsExpected);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  initBoard();
  printBoard();

  return RUN_ALL_TESTS();
}
