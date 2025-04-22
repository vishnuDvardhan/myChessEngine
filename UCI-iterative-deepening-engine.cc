#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Iterative-deepening-minimax-alphabeta_pruning.h"

using namespace std;

gameState currentState;

void applyMovesFromUci(const vector<string>& moves) {
  for (const string& moveStr : moves) {
    Turn userMove;
    try {
      userMove = Turn(moveStr);
      if (currentState.currentColour == Colours::white &&
          moveStr.length() > 4) {
        string modifiableMoveStr = moveStr;
        modifiableMoveStr[4] = toupper(modifiableMoveStr[4]);
        userMove = Turn(modifiableMoveStr);
      }
    } catch (...) {
      cerr << "[error] invalid move string: " << moveStr << endl;
      cerr << "legal moves: ";
      vector<Turn> legalMoves = generateTurns(currentState);
      for (Turn& move : legalMoves) {
        cerr << move.numberToSquare() << " ";
      }
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
