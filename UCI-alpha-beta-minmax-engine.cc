#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "board.h"

using namespace std;

gameState currentState;

int minimax(gameState& state, int depth, int alpha, int beta,
            bool maximizingPlayer) {
  if (depth == 0) return evaluateBoard(state);

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

    if ((maximizing && score > bestScore) ||
        (!maximizing && score < bestScore)) {
      bestScore = score;
      bestMove = move;
    }

    state = backup;
  }

  return bestMove;
}

void applyMovesFromUci(const vector<string>& moves) {
  for (const string& moveStr : moves) {
    Turn userMove;
    try {
      userMove = Turn(moveStr);
    } catch (...) {
      cerr << "[error] invalid move string: " << moveStr << endl;
      continue;
    }

    vector<Turn> legalMoves = generateTurns(currentState);
    bool applied = false;
    for (const Turn& move : legalMoves) {
      if (move == userMove) {
        applyTurn(move, currentState);
        applied = true;
        break;
      }
    }

    if (!applied) {
      cerr << "[error] move not legal: " << moveStr << endl;
    }
  }
}

void uciLoop() {
  string line;
  while (getline(cin, line)) {
    istringstream iss(line);
    string command;
    iss >> command;

    if (command == "uci") {
      cout << "id name SimpleMinimaxEngine\n";
      cout << "id author VishnuDVardhan\n";
      cout << "uciok\n";
      cout.flush();
    } else if (command == "isready") {
      cout << "readyok\n";
      cout.flush();
    } else if (command == "ucinewgame") {
      currentState = initBoard();
    } else if (command == "position") {
      string token;
      iss >> token;

      if (token == "startpos") {
        currentState = initBoard();
        if (iss >> token && token == "moves") {
          string moveStr;
          vector<string> moves;
          while (iss >> moveStr) moves.push_back(moveStr);
          applyMovesFromUci(moves);
        }
      } else if (token == "fen") {
        string fen, part;
        int parts = 0;
        while (iss >> part && parts < 6) {
          fen += part + " ";
          parts++;
        }

        initBoardWithFen(currentState, fen);

        if (iss >> token && token == "moves") {
          string moveStr;
          vector<string> moves;
          while (iss >> moveStr) moves.push_back(moveStr);
          applyMovesFromUci(moves);
        }
      }
    } else if (command == "go") {
      Turn bestMove = findBestMove(currentState, 5);
      cout << "bestmove " << bestMove.numberToSquare() << endl;
      cout.flush();
    } else if (command == "quit") {
      break;
    }
  }
}

int main() {
  currentState = initBoard();
  uciLoop();
  return 0;
}
