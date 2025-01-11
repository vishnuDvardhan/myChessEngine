#include <gtest/gtest.h>

#include "board.h"

TEST(TurnGenerationTests, KingTurns) {
  vector<pair<int, int>> kingTurnsActual =
      kingToLocations(1, 1, Colours::white);

  vector<pair<int, int>> kingTurnsExpected = {{2, 2}, {2, 1}, {2, 0}};

  ASSERT_EQ(kingTurnsActual, kingTurnsExpected);
}

TEST(TurnGenerationTests, RookTurns) {
  vector<pair<int, int>> rookTurnsActual =
      rookToLocations(1, 1, Colours::white);

  vector<pair<int, int>> rookTurnsExpected = {
      {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}};

  ASSERT_EQ(rookTurnsActual, rookTurnsExpected);
}

TEST(TurnGenerationTests, QueenTurns) {
  vector<pair<int, int>> queenTurnsActual =
      queenToLocations(1, 1, Colours::white);

  vector<pair<int, int>> queenTurnsExpected = {{2, 1}, {3, 1}, {4, 1}, {5, 1},
                                               {6, 1}, {2, 2}, {3, 3}, {4, 4},
                                               {5, 5}, {6, 6}, {2, 0}};

  ASSERT_EQ(queenTurnsActual, queenTurnsExpected);
}

TEST(TurnGenerationTests, BishopTurns) {
  vector<pair<int, int>> bishopTurnsActual =
      bishopToLocations(1, 1, Colours::white);

  vector<pair<int, int>> bishopTurnsExpected = {{2, 2}, {3, 3}, {4, 4},
                                                {5, 5}, {6, 6}, {2, 0}};

  ASSERT_EQ(bishopTurnsActual, bishopTurnsExpected);
}

TEST(TurnGenerationTests, PawnTurns) {
  vector<pair<int, int>> pawnTurnsActual =
      pawnToLocations(1, 1, Colours::white);

  vector<pair<int, int>> pawnTurnsExpected = {{2, 1}, {3, 1}};

  ASSERT_EQ(pawnTurnsActual, pawnTurnsExpected);
}

TEST(TurnGenerationTests, KnightsTurns) {
  vector<pair<int, int>> knightTurnsActual =
      knightToLocations(1, 1, Colours::white);

  vector<pair<int, int>> knightTurnsExpected = {{3, 2}, {3, 0}, {2, 3}};

  ASSERT_EQ(knightTurnsActual, knightTurnsExpected);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  initBoard();

  return RUN_ALL_TESTS();
}