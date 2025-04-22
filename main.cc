#include <iostream>
#include <limits>
#include <string>

#include "Iterative-deepening-minimax-alphabeta_pruning.h"

Turn getUserMove(const gameState& state) {
  unordered_map<string, Turn> legalMovesMap;
  vector<Turn> legalMoves = generateTurns(const_cast<gameState&>(state));
  for (Turn& move : legalMoves) {
    legalMovesMap.emplace(move.numberToSquare(), move);
  }

  if (legalMoves.empty()) {
    cout << "No legal moves available.\n";
    exit(0);
  }

  cout << "Available Moves:\n";
  for (size_t i = 0; i < legalMoves.size(); ++i) {
    cout << i << ". " << legalMoves[i].numberToSquare() << " ";
  }

  string choice = "";
  while (true) {
    cout << "\nEnter chosen move: ";
    cin >> choice;

    if (legalMovesMap.find(choice) == legalMovesMap.end()) {
      cout << "Invalid move. Try again.\n";
    } else {
      break;
    }
  }

  return legalMovesMap[choice];
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

void play() {
  gameState state = initBoard();

  while (!gameOver(state)) {
    if (state.currentColour == Colours::white) {
      cout << "\nYour move (White):\n";
      Turn playerMove = getUserMove(state);
      cout << "You played: " << playerMove.numberToSquare() << "\n";
      applyTurn(playerMove, state);
    } else {
      cout << "\nAI is thinking (Black)...\n";
      Turn aiMove = findBestMove(state, 6);
      cout << "AI played: " << aiMove.numberToSquare() << "\n";
      applyTurn(aiMove, state);
    }

    state.printBoard();
  }
}

int main(int argc, char** argv) {
  play();

  return 0;
}