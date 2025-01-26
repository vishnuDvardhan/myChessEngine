
#include <iostream>
#include <string>
#include <vector>
using namespace std;

typedef pair<int, int> Square;

enum Pieces : int {
  empty,
  wPawn,
  wKnight,
  wBishop,
  wRook,
  wKing,
  wQueen,
  bPawn,
  bKnight,
  bBishop,
  bRook,
  bKing,
  bQueen
};
struct gameState {
  Pieces board[8][8];
};

gameState currentState;

Square getPieceLocation(Pieces piece) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (currentState.board[i][j] == piece) {
        return {i, j};
      }
    }
  }
  return {-1, -1};
}

int getDistance(int x1, int y1, int x2, int y2) {
  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  int distance = max(dx, dy);
  return distance;
}

struct Turn {
  Square from;
  Square to;
  Pieces promotionPiece = Pieces::empty;
  bool castling = false;

  Turn(Square f, Square t, Pieces p = Pieces::empty)
      : from(f), to(t), promotionPiece(p) {}

  bool operator==(const Turn& other) const {
    return from == other.from && to == other.to &&
           promotionPiece == other.promotionPiece;
  }

  std::string str() const {
    std::ostringstream os;
    os << "(" << from.first << ", " << from.second << "), "
       << "--> (" << to.first << ", " << to.second << ")";
    if (promotionPiece != Pieces::empty) {
      os << ", Promotion: " << static_cast<int>(promotionPiece);
    }
    os << "\n";
    return os.str();
  }

  friend std::ostream& operator<<(std::ostream& os, const Turn& turn) {
    os << turn.str();
    return os;
  }
};

struct Castling {
  bool wkingSide = true;
  bool wQueenSide = true;
  bool bKingSide = true;
  bool bQueenSide = true;
};

enum Colours : int { black, white };

std::string map_to_string(int i, int j) {
  char column = 'a' + j;
  char row = '1' + i;
  return std::string(1, column) + std::string(1, row);
}

char pieceTable[13] = {'.', 'P', 'N', 'B', 'R', 'K', 'Q',
                       'p', 'n', 'b', 'r', 'k', 'q'};

void initBoard() {
  for (int i = 0; i <= 7; i++) {
    currentState.board[1][i] = Pieces::wPawn;
    currentState.board[6][i] = Pieces::bPawn;
  }
  currentState.board[0][0] = Pieces::wRook;
  currentState.board[0][7] = Pieces::wRook;
  currentState.board[7][0] = Pieces::bRook;
  currentState.board[7][7] = Pieces::bRook;
  currentState.board[0][1] = Pieces::wBishop;
  currentState.board[0][6] = Pieces::wBishop;
  currentState.board[7][1] = Pieces::bBishop;
  currentState.board[7][6] = Pieces::bBishop;
  currentState.board[0][2] = Pieces::wKnight;
  currentState.board[0][5] = Pieces::wKnight;
  currentState.board[7][2] = Pieces::bKnight;
  currentState.board[7][5] = Pieces::bKnight;
  currentState.board[0][3] = Pieces::wQueen;
  currentState.board[7][3] = Pieces::bQueen;
  currentState.board[0][4] = Pieces::wKing;
  currentState.board[7][4] = Pieces::bKing;
}

void printBoard() {
  for (int i = 7; i >= 0; i--) {
    for (int j = 0; j < 8; j++) {
      cout << pieceTable[static_cast<int>(currentState.board[i][j])] << " ";
    }
    cout << "\n";
  }
  cout << "\n";
}

bool isWhite(int i, int j) {
  return currentState.board[i][j] == Pieces::wPawn ||
         currentState.board[i][j] == Pieces::wKnight ||
         currentState.board[i][j] == Pieces::wBishop ||
         currentState.board[i][j] == Pieces::wRook ||
         currentState.board[i][j] == Pieces::wQueen ||
         currentState.board[i][j] == Pieces::wKing;
}

bool isBlack(int i, int j) {
  return currentState.board[i][j] == Pieces::bPawn ||
         currentState.board[i][j] == Pieces::bKnight ||
         currentState.board[i][j] == Pieces::bBishop ||
         currentState.board[i][j] == Pieces::bRook ||
         currentState.board[i][j] == Pieces::bQueen ||
         currentState.board[i][j] == Pieces::bKing;
}

void GeneratePseudoCorrectMoves(Pieces p) {}

bool canLand(int i, int j, Colours c) {
  if (i >= 0 && i <= 7 && j >= 0 && j <= 7) {
    if (currentState.board[i][j] == Pieces::empty) {
      return true;
    }
    if (c == Colours::white) {
      return isBlack(i, j);
    }
    return isWhite(i, j);
  }
  return false;
}

bool canKingLand(int i, int j, Colours c, Square OppositeKingSquare) {
  if (i >= 0 && i <= 7 && j >= 0 && j <= 7) {
    if (canLand(i, j, c) && (getDistance(i, j, OppositeKingSquare.first,
                                         OppositeKingSquare.second) > 1)) {
      return true;
    }
  }
  return false;
}
bool canCapture(int i, int j, Colours c) {
  if (c == Colours::white) {
    return isBlack(i, j);
  }
  return isWhite(i, j);
}

bool canPawnLand(int i, int j, Colours c) {
  if (i >= 0 || i <= 7 || j >= 0 || j <= 7) {
    if (currentState.board[i][j] == Pieces::empty) {
      return true;
    }
  }
  return false;
}

bool canPawnCapture(int i, int j, Colours c) {
  if (i >= 0 || i <= 7 || j >= 0 || j <= 7) {
    if (currentState.board[i][j] == Pieces::empty) {
      return false;
    }
    if (c == Colours::white) {
      return isBlack(i, j);
    }
    return isWhite(i, j);
  }
  return false;
}

vector<Turn> kingToLocations(int i, int j, Colours c) {
  vector<Turn> toLocations;
  Pieces oppositeKing = c == Colours::white ? Pieces::bKing : Pieces::wKing;
  Square oppositeKingSquare = getPieceLocation(oppositeKing);

  if (canKingLand(i + 1, j + 1, c, oppositeKingSquare)) {
    toLocations.push_back({{i, j}, {i + 1, j + 1}});
  }
  if (canKingLand(i + 1, j, c, oppositeKingSquare)) {
    toLocations.push_back({{i, j}, {i + 1, j}});
  }
  if (canKingLand(i + 1, j - 1, c, oppositeKingSquare)) {
    toLocations.push_back({{i, j}, {i + 1, j - 1}});
  }
  if (canKingLand(i, j + 1, c, oppositeKingSquare)) {
    toLocations.push_back({{i, j}, {i, j + 1}});
  }
  if (canKingLand(i, j - 1, c, oppositeKingSquare)) {
    toLocations.push_back({{i, j}, {i, j - 1}});
  }
  if (canKingLand(i - 1, j + 1, c, oppositeKingSquare)) {
    toLocations.push_back({{i, j}, {i - 1, j + 1}});
  }
  if (canKingLand(i - 1, j, c, oppositeKingSquare)) {
    toLocations.push_back({{i, j}, {i - 1, j}});
  }
  if (canKingLand(i - 1, j - 1, c, oppositeKingSquare)) {
    toLocations.push_back({{i, j}, {i - 1, j - 1}});
  }
  return toLocations;
}

vector<Turn> pawnToLocations(int i, int j, Colours c) {
  vector<Turn> toLocations;
  if (c == Colours::white) {
    if (canPawnLand(i + 1, j, c)) {
      toLocations.push_back({{i, j}, {i + 1, j}});
      if (i == 1) {
        if (canPawnLand(i + 2, j, c)) {
          toLocations.push_back({{i, j}, {i + 2, j}});
        }
      }
    }
    if (canPawnCapture(i + 1, j + 1, c)) {
      toLocations.push_back({{i, j}, {i + 1, j + 1}});
    }
    if (canPawnCapture(i + 1, j - 1, c)) {
      toLocations.push_back({{i, j}, {i + 1, j - 1}});
    }
  }
  if (c == Colours::black) {
    if (canPawnLand(i - 1, j, c)) {
      toLocations.push_back({{i, j}, {i - 1, j}});
      if (i == 6) {
        if (canPawnLand(i - 2, j, c)) {
          toLocations.push_back({{i, j}, {i - 2, j}});
        }
      }
    }
    if (canPawnCapture(i - 1, j - 1, c)) {
      toLocations.push_back({{i, j}, {i - 1, j - 1}});
    }
    if (canPawnCapture(i - 1, j + 1, c)) {
      toLocations.push_back({{i, j}, {i - 1, j + 1}});
    }
  }
  return toLocations;
}

vector<Turn> knightToLocations(int i, int j, Colours c) {
  vector<Turn> toLocations;
  if (canLand(i + 2, j + 1, c)) {
    toLocations.push_back({{i, j}, {i + 2, j + 1}});
  }
  if (canLand(i + 2, j - 1, c)) {
    toLocations.push_back({{i, j}, {i + 2, j - 1}});
  }
  if (canLand(i + 1, j + 2, c)) {
    toLocations.push_back({{i, j}, {i + 1, j + 2}});
  }
  if (canLand(i + 1, j - 2, c)) {
    toLocations.push_back({{i, j}, {i + 1, j - 2}});
  }
  if (canLand(i - 2, j + 1, c)) {
    toLocations.push_back({{i, j}, {i - 2, j + 1}});
  }
  if (canLand(i - 2, j - 1, c)) {
    toLocations.push_back({{i, j}, {i - 2, j - 1}});
  }
  if (canLand(i - 1, j + 2, c)) {
    toLocations.push_back({{i, j}, {i - 1, j + 2}});
  }
  if (canLand(i - 1, j - 2, c)) {
    toLocations.push_back({{i, j}, {i - 1, j - 2}});
  }

  return toLocations;
}

vector<Turn> bishopToLocations(int i, int j, Colours c) {
  vector<Turn> toLocations;

  for (int k = 1; (i + k <= 7 && j + k <= 7); k++) {
    if (canLand(i + k, j + k, c)) {
      toLocations.push_back({{i, j}, {i + k, j + k}});
    } else {
      break;
    }
    if (canCapture(i + k, j + k, c)) {
      break;
    }
  }
  for (int k = 1; (i - k >= 0 && j + k <= 7); k++) {
    if (canLand(i - k, j + k, c)) {
      toLocations.push_back({{i, j}, {i - k, j + k}});
    } else {
      break;
    }
    if (canCapture(i - k, j + k, c)) {
      break;
    }
  }
  for (int k = 1; (i - k >= 0 && j - k >= 0); k++) {
    if (canLand(i - k, j - k, c)) {
      toLocations.push_back({{i, j}, {i - k, j - k}});
    } else {
      break;
    }
    if (canCapture(i - k, j - k, c)) {
      break;
    }
  }
  for (int k = 1; (i + k <= 7 && j - k >= 0); k++) {
    if (canLand(i + k, j - k, c)) {
      toLocations.push_back({{i, j}, {i + k, j - k}});
    } else {
      break;
    }
    if (canCapture(i + k, j - k, c)) {
      break;
    }
  }

  return toLocations;
}

vector<Turn> rookToLocations(int i, int j, Colours c) {
  vector<Turn> toLocations;

  for (int k = i + 1; k < 8; k++) {
    if (canLand(k, j, c)) {
      toLocations.push_back({{i, j}, {k, j}});
    } else {
      break;
    }
    if (canCapture(k, j, c)) {
      break;
    }
  }
  for (int k = i - 1; k >= 0; k--) {
    if (canLand(k, j, c)) {
      toLocations.push_back({{i, j}, {k, j}});
    } else {
      break;
    }
    if (canCapture(k, j, c)) {
      break;
    }
  }
  for (int k = j - 1; k >= 0; k--) {
    if (canLand(i, k, c)) {
      toLocations.push_back({{i, j}, {i, k}});
    } else {
      break;
    }
    if (canCapture(i, k, c)) {
      break;
    }
  }
  for (int k = j + 1; k < 8; k++) {
    if (canLand(i, k, c)) {
      toLocations.push_back({{i, j}, {i, k}});
    } else {
      break;
    }
    if (canCapture(i, k, c)) {
      break;
    }
  }
  return toLocations;
}
vector<Turn> queenToLocations(int i, int j, Colours c) {
  vector<Turn> toLocations;
  for (int k = i + 1; k < 8; k++) {
    if (canLand(k, j, c)) {
      toLocations.push_back({{i, j}, {k, j}});
    } else {
      break;
    }
    if (canCapture(k, j, c)) {
      break;
    }
  }
  for (int k = i - 1; k >= 0; k--) {
    if (canLand(k, j, c)) {
      toLocations.push_back({{i, j}, {k, j}});
    } else {
      break;
    }
    if (canCapture(k, j, c)) {
      break;
    }
  }
  for (int k = j - 1; k >= 0; k--) {
    if (canLand(i, k, c)) {
      toLocations.push_back({{i, j}, {i, k}});
    } else {
      break;
    }
    if (canCapture(i, k, c)) {
      break;
    }
  }
  for (int k = j + 1; k < 8; k++) {
    if (canLand(i, k, c)) {
      toLocations.push_back({{i, j}, {i, k}});
    } else {
      break;
    }
    if (canCapture(i, k, c)) {
      break;
    }
  }
  for (int k = 1; (k + i < 7 && k + j < 7); k++) {
    if (canLand(i + k, j + k, c)) {
      toLocations.push_back({{i, j}, {k + i, k + j}});
    } else {
      break;
    }
    if (canCapture(i + k, j + k, c)) {
      break;
    }
  }
  for (int k = 1; (i - k >= 0 && k + j < 7); k++) {
    if (canLand(i - k, j + k, c)) {
      toLocations.push_back({{i, j}, {i - k, k + j}});
    } else {
      break;
    }
    if (canCapture(i - k, j + k, c)) {
      break;
    }
  }
  for (int k = 1; (i - k >= 0 && j - k >= 0); k++) {
    if (canLand(i - k, j - k, c)) {
      toLocations.push_back({{i, j}, {i - k, j - k}});
    } else {
      break;
    }
    if (canCapture(i - k, j - k, c)) {
      break;
    }
  }
  for (int k = 1; (i + k < 7 && j - k >= 0); k++) {
    if (canLand(i + k, j - k, c)) {
      toLocations.push_back({{i, j}, {i + k, j - k}});
    } else {
      break;
    }
    if (canCapture(i + k, j - k, c)) {
      break;
    }
  }

  return toLocations;
}
