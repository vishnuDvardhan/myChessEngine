#include <algorithm>  // for std::find and std::iter_swap
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
    return inCheck ? (maximizingPlayer ? -100000 : 100000) : 0;
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
      if (beta <= alpha) break;
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
      if (beta <= alpha) break;
    }
    return minEval;
  }
}

Turn findMove(gameState& state, int depth,
              const Turn* preferredMove = nullptr) {
  vector<Turn> moves = generateTurns(state);
  Turn bestMove = {{-1, -1}, {-1, -1}};
  if (moves.empty()) return bestMove;

  if (preferredMove) {
    auto it = std::find(moves.begin(), moves.end(), *preferredMove);
    if (it != moves.end()) {
      std::iter_swap(moves.begin(), it);
    }
  }

  Colours originalColour = state.currentColour;
  bool maximizing = (originalColour == Colours::white);
  int bestScore = maximizing ? -1000000 : 1000000;

  for (Turn& move : moves) {
    gameState backup = state;
    applyTurn(move, state);
    state.currentColour =
        (originalColour == Colours::white) ? Colours::black : Colours::white;

    int score = minimax(state, depth - 1, -100000, 100000, !maximizing);

    if ((maximizing && score > bestScore) ||
        (!maximizing && score < bestScore)) {
      bestScore = score;
      bestMove = move;
    }

    state = backup;
  }

  return bestMove;
}

Turn findBestMove(gameState& state, int maxDepth) {
  Turn bestMove = {{-1, -1}, {-1, -1}};
  for (int depth = 1; depth <= maxDepth; ++depth) {
    bestMove = findMove(state, depth, &bestMove);
  }
  return bestMove;
}
