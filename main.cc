#include <iostream>
#include <limits>
#include <string>

#include "board.h"

int minimax(gameState& state, int depth, bool maximizingPlayer) {
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

      int eval = minimax(state, depth - 1, false);
      maxEval = max(maxEval, eval);

      state = backup;
    }
    return maxEval;

  } else {
    int minEval = 1000000;
    for (Turn& move : moves) {
      gameState backup = state;
      applyTurn(move, state);
      state.currentColour = Colours::white;

      int eval = minimax(state, depth - 1, true);
      minEval = min(minEval, eval);

      state = backup;
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

    int score = minimax(state, depth - 1, !maximizing);

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

Turn getUserMove(const gameState& state) {
  vector<Turn> legalMoves = generateTurns(const_cast<gameState&>(state));

  if (legalMoves.empty()) {
    cout << "No legal moves available.\n";
    exit(0);
  }

  cout << "Available Moves:\n";
  for (size_t i = 0; i < legalMoves.size(); ++i) {
    cout << "[" << i << "] " << legalMoves[i].numberToSquare() << "\n";
  }

  int choice = -1;
  while (true) {
    cout << "Enter the index of your chosen move: ";
    cin >> choice;

    if (cin.fail() || choice < 0 || choice >= (int)legalMoves.size()) {
      cout << "Invalid index. Try again.\n";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } else {
      break;
    }
  }

  return legalMoves[choice];
}

bool gameOver(const gameState& state) {
  Colours c = state.currentColour;
  Pieces king = (c == Colours::white) ? Pieces::wKing : Pieces::bKing;

  vector<Turn> legalMoves = generateTurns(const_cast<gameState&>(state));
  if (!legalMoves.empty()) return false;

  Square kingLoc = getPieceLocation(king, state);
  if (isInCheck(kingLoc.first, kingLoc.second, c, state)) {
    cout << (c == Colours::white ? "Checkmate! Black wins.\n"
                                 : "Checkmate! White wins.\n");
  } else {
    cout << "Stalemate! It's a draw.\n";
  }
  return true;
}

int main(int argc, char** argv) {
  gameState state = initBoard();

  while (!gameOver(state)) {
    if (state.currentColour == Colours::white) {
      cout << "\nYour move (White):\n";
      Turn playerMove = getUserMove(state);
      cout << "You played: " << playerMove.numberToSquare() << "\n";
      applyTurn(playerMove, state);
    } else {
      cout << "\nAI is thinking (Black)...\n";
      Turn aiMove = findBestMove(state, 5);
      cout << "AI played: " << aiMove.numberToSquare() << "\n";
      applyTurn(aiMove, state);
    }

    state.printBoard();
  }

  return 0;
}