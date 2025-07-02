#include <iostream>
#include <limits>
#include <string>

#include "board.h"

int minimax(gameState& state, int depth, int alpha, int beta,
            bool maximizingPlayer) {
  if (depth == 0) {
    return evaluateBoard(state);
  }

  Turns movesArray;
  int numMoves = 0;
  generateTurns(state, movesArray, numMoves);

  if (numMoves == 0) {
    Square kingLoc = getKingLocation(state, maximizingPlayer);
    bool inCheck =
        isInCheck(state, kingLoc.first, kingLoc.second, maximizingPlayer);

    if (inCheck)
      return maximizingPlayer ? -100000 : 100000;
    else
      return 0;
  }

  if (maximizingPlayer) {
    int maxEval = -1000000;
    for (int i = 0; i < numMoves; i++) {
      gameState backup = state;
      applyTurn(state, movesArray[i]);

      int eval = minimax(state, depth - 1, alpha, beta, false);
      maxEval = max(maxEval, eval);
      alpha = max(alpha, eval);

      state = backup;

      if (beta <= alpha) {
        break;  // Beta cut-off
      }
    }
    return maxEval;
  } else {
    int minEval = 1000000;
    for (int i = 0; i < numMoves; i++) {
      gameState backup = state;
      applyTurn(state, movesArray[i]);

      int eval = minimax(state, depth - 1, alpha, beta, true);
      minEval = min(minEval, eval);
      beta = min(beta, eval);

      state = backup;

      if (beta <= alpha) {
        break;  // Alpha cut-off
      }
    }
    return minEval;
  }
}

Turn findBestMove(gameState& state, int depth) {
  Turns movesArray;
  int numMoves = 0;
  generateTurns(state, movesArray, numMoves);

  Turn bestMove = {{-1, -1}, {-1, -1}};
  if (numMoves == 0) return bestMove;

  bool maximizing = state.isWhitesTurn;
  int bestScore = maximizing ? -1000000 : 1000000;

  for (int i = 0; i < numMoves; i++) {
    gameState backup = state;
    applyTurn(state, movesArray[i]);

    int score = minimax(state, depth - 1, -100000, 100000, !maximizing);

    if (maximizing && score > bestScore) {
      bestScore = score;
      bestMove = movesArray[i];
    } else if (!maximizing && score < bestScore) {
      bestScore = score;
      bestMove = movesArray[i];
    }

    state = backup;
  }

  return bestMove;
}