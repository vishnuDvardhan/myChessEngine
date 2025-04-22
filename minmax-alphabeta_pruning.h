#include <iostream>
#include <limits>
#include <string>

#include "board.h"

int minimax(gameState& state, int depth, int alpha, int beta,
            bool maximizingPlayer) {
  if (depth == 0) {
    return evaluateBoard(state);
  }

  vector<Turn> moves = generateTurns(state);

  if (moves.empty()) {
    Pieces king = maximizingPlayer ? Pieces::wKing : Pieces::bKing;
    Square kingLoc = getPieceLocation(king, state);
    bool inCheck =
        isInCheck(kingLoc.first, kingLoc.second,
                  maximizingPlayer ? Colours::white : Colours::black, state);

    if (inCheck)
      return maximizingPlayer ? -100000 : 100000;
    else
      return 0;
  }

  if (maximizingPlayer) {
    int maxEval = -1000000;
    for (Turn& move : moves) {
      gameState backup = state;
      applyTurn(move, state);
      state.currentColour = Colours::black;

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
    for (Turn& move : moves) {
      gameState backup = state;
      applyTurn(move, state);
      state.currentColour = Colours::white;

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
  vector<Turn> moves = generateTurns(state);
  Turn bestMove = {{-1, -1}, {-1, -1}};
  if (moves.empty()) return bestMove;

  Colours originalColour = state.currentColour;
  bool maximizing = (originalColour == Colours::white);
  int bestScore = maximizing ? -1000000 : 1000000;

  for (Turn& move : moves) {
    gameState backup = state;
    applyTurn(move, state);
    state.currentColour =
        (originalColour == Colours::white) ? Colours::black : Colours::white;

    int score = minimax(state, depth - 1, -100000, 100000, !maximizing);

    if (maximizing && score > bestScore) {
      bestScore = score;
      bestMove = move;
    } else if (!maximizing && score < bestScore) {
      bestScore = score;
      bestMove = move;
    }

    state = backup;
  }

  return bestMove;
}