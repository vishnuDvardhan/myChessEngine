
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

std::string pieceToString(Pieces piece) {
  switch (piece) {
    case Pieces::empty:
      return "empty";
    case Pieces::wPawn:
      return "wPawn";
    case Pieces::wKnight:
      return "wKnight";
    case Pieces::wBishop:
      return "wBishop";
    case Pieces::wRook:
      return "wRook";
    case Pieces::wQueen:
      return "wQueen";
    case Pieces::wKing:
      return "wKing";
    case Pieces::bPawn:
      return "bPawn";
    case Pieces::bKnight:
      return "bKnight";
    case Pieces::bBishop:
      return "bBishop";
    case Pieces::bRook:
      return "bRook";
    case Pieces::bQueen:
      return "bQueen";
    case Pieces::bKing:
      return "bKing";
    default:
      return "unknown";
  }
}
struct gameState {
  Pieces board[8][8];
  Square enpassantSq = {-1, -1};
  bool canEnpassant = true;
  bool wCastle = true;
  bool bCastle = true;
  struct Castling {
    bool wkingSide = true;
    bool wQueenSide = true;
    bool bKingSide = true;
    bool bQueenSide = true;
  };
};

gameState currentState;

bool isEmpty(int i, int j) { return currentState.board[i][j] == Pieces::empty; }

bool isRook(int i, int j) {
  return currentState.board[i][j] == Pieces::wRook ||
         currentState.board[i][j] == Pieces::bRook;
}

bool isKnight(int i, int j) {
  return currentState.board[i][j] == Pieces::wKnight ||
         currentState.board[i][j] == Pieces::bKnight;
}

bool isPawn(int i, int j) {
  return currentState.board[i][j] == Pieces::wPawn ||
         currentState.board[i][j] == Pieces::bPawn;
}

bool isBishop(int i, int j) {
  return currentState.board[i][j] == Pieces::wBishop ||
         currentState.board[i][j] == Pieces::bBishop;
}

bool isQueen(int i, int j) {
  return currentState.board[i][j] == Pieces::wQueen ||
         currentState.board[i][j] == Pieces::bQueen;
}

bool isKing(int i, int j) {
  return currentState.board[i][j] == Pieces::wKing ||
         currentState.board[i][j] == Pieces::bKing;
}

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
  bool isEnpassantInitiater = false;
  bool isEnpassantFinisher = false;
  Square enpassantSquare = {-1, -1};

  Turn(Square f, Square t, Pieces p = Pieces::empty, bool c = false,
       bool enpassant = false, Square enpassantSq = {-1, -1},
       bool enpassantFinisher = false)
      : from(f),
        to(t),
        promotionPiece(p),
        castling(c),
        isEnpassantInitiater(enpassant),
        enpassantSquare(enpassantSq),
        isEnpassantFinisher(enpassantFinisher) {}

  bool operator==(const Turn& other) const {
    return from == other.from && to == other.to &&
           promotionPiece == other.promotionPiece;
  }

  std::string str() const {
    std::ostringstream os;
    os << "\n{{" << from.first << ", " << from.second << "}, "
       << "{" << to.first << ", " << to.second << "}, "
       << "Pieces::" << (pieceToString(promotionPiece)) << ", "
       << (castling ? "true" : "false") << ", "
       << (isEnpassantInitiater ? "true" : "false") << ", "
       << "{" << enpassantSquare.first << ", " << enpassantSquare.second
       << "}, " << (isEnpassantFinisher ? "true" : "false") << "}";

    return os.str();
  }

  friend std::ostream& operator<<(std::ostream& os, const Turn& turn) {
    os << turn.str();
    return os;
  }
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
  for (int i = 2; i < 6; i++) {
    for (int j = 0; j <= 7; j++) {
      currentState.board[i][j] = Pieces::empty;
    }
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
  currentState.enpassantSq = {-1, -1};
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
  if (i >= 0 && i <= 7 && j >= 0 && j <= 7) {
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

bool canPawnEnpassant(int i, int j, Colours c) {
  if (i >= 0 && i <= 7 && j >= 0 && j <= 7) {
    if ((i == currentState.enpassantSq.first) &&
        (j == currentState.enpassantSq.second)) {
      return isEmpty(i, j);
    }
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
      if (i + 1 == 7) {
        toLocations.push_back({{i, j}, {i + 1, j}, Pieces::wQueen});
        toLocations.push_back({{i, j}, {i + 1, j}, Pieces::wRook});
        toLocations.push_back({{i, j}, {i + 1, j}, Pieces::wBishop});
        toLocations.push_back({{i, j}, {i + 1, j}, Pieces::wKnight});
      } else {
        toLocations.push_back({{i, j}, {i + 1, j}});
      }

      if (i == 1) {
        if (canPawnLand(i + 2, j, c)) {
          toLocations.push_back(
              {{i, j}, {i + 2, j}, Pieces::empty, false, true, {i + 1, j}});
        }
      }
    }
    if (canPawnCapture(i + 1, j + 1, c)) {
      if (i + 1 == 7) {
        toLocations.push_back({{i, j}, {i + 1, j + 1}, Pieces::wQueen});
        toLocations.push_back({{i, j}, {i + 1, j + 1}, Pieces::wRook});
        toLocations.push_back({{i, j}, {i + 1, j + 1}, Pieces::wBishop});
        toLocations.push_back({{i, j}, {i + 1, j + 1}, Pieces::wKnight});
      } else {
        toLocations.push_back({{i, j}, {i + 1, j + 1}});
      }
    }
    if (canPawnCapture(i + 1, j - 1, c)) {
      if (i == 7) {
        toLocations.push_back({{i, j}, {i + 1, j - 1}, Pieces::wQueen});
        toLocations.push_back({{i, j}, {i + 1, j - 1}, Pieces::wRook});
        toLocations.push_back({{i, j}, {i + 1, j - 1}, Pieces::wBishop});
        toLocations.push_back({{i, j}, {i + 1, j - 1}, Pieces::wKnight});
      } else {
        toLocations.push_back({{i, j}, {i + 1, j - 1}});
      }
    }
    if (canPawnEnpassant(i + 1, j - 1, c)) {
      toLocations.push_back({{i, j},
                             {i + 1, j - 1},
                             Pieces::empty,
                             false,
                             false,
                             {-1, -1},
                             true});
    }
    if (canPawnEnpassant(i + 1, j + 1, c)) {
      toLocations.push_back({{i, j},
                             {i + 1, j + 1},
                             Pieces::empty,
                             false,
                             false,
                             {-1, -1},
                             true});
    }
  }
  if (c == Colours::black) {
    if (canPawnLand(i - 1, j, c)) {
      if (i - 1 == 0) {
        toLocations.push_back({{i, j}, {i - 1, j}, Pieces::bQueen});
        toLocations.push_back({{i, j}, {i - 1, j}, Pieces::bRook});
        toLocations.push_back({{i, j}, {i - 1, j}, Pieces::bBishop});
        toLocations.push_back({{i, j}, {i - 1, j}, Pieces::bKnight});
      } else {
        toLocations.push_back({{i, j}, {i - 1, j}});
      }
      if (i == 6) {
        if (canPawnLand(i - 2, j, c)) {
          toLocations.push_back(
              {{i, j}, {i - 2, j}, Pieces::empty, false, true, {i - 1, j}});
        }
      }
    }
    if (canPawnCapture(i - 1, j - 1, c)) {
      if (i - 1 == 0) {
        toLocations.push_back({{i, j}, {i - 1, j - 1}, Pieces::bQueen});
        toLocations.push_back({{i, j}, {i - 1, j - 1}, Pieces::bRook});
        toLocations.push_back({{i, j}, {i - 1, j - 1}, Pieces::bBishop});
        toLocations.push_back({{i, j}, {i - 1, j - 1}, Pieces::bKnight});
      } else {
        toLocations.push_back({{i, j}, {i - 1, j - 1}});
      }
    }
    if (canPawnCapture(i - 1, j + 1, c)) {
      if (i - 1 == 0) {
        toLocations.push_back({{i, j}, {i - 1, j + 1}, Pieces::bQueen});
        toLocations.push_back({{i, j}, {i - 1, j + 1}, Pieces::bRook});
        toLocations.push_back({{i, j}, {i - 1, j + 1}, Pieces::bBishop});
        toLocations.push_back({{i, j}, {i - 1, j + 1}, Pieces::bKnight});
      } else {
        toLocations.push_back({{i, j}, {i - 1, j + 1}});
      }
    }
    if (canPawnEnpassant(i - 1, j - 1, c)) {
      toLocations.push_back({{i, j},
                             {i - 1, j - 1},
                             Pieces::empty,
                             false,
                             false,
                             {-1, -1},
                             true});
    }
    if (canPawnEnpassant(i - 1, j + 1, c)) {
      toLocations.push_back({{i, j},
                             {i - 1, j + 1},
                             Pieces::empty,
                             false,
                             false,
                             {-1, -1},
                             true});
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

bool canCaptureAPawn(int i, int j, Colours c) {
  if (i >= 1 && i <= 7 && j >= 1 && j <= 7) {
    if (c == Colours::white) {
      return currentState.board[i][j] == Pieces::bPawn;
    }
    return currentState.board[i][j] == Pieces::wPawn;
  }
  return false;
}

bool canCaptureAKnight(int i, int j, Colours c) {
  if (i >= 1 && i <= 7 && j >= 1 && j <= 7) {
    if (c == Colours::white) {
      return currentState.board[i][j] == Pieces::bKnight;
    }
    return currentState.board[i][j] == Pieces::wKnight;
  }
  return false;
}

bool canCaptureARookOrQueen(int i, int j, Colours c) {
  if (i >= 1 && i <= 7 && j >= 1 && j <= 7) {
    if (c == Colours::white) {
      return (currentState.board[i][j] == Pieces::bRook) ||
             (currentState.board[i][j] == Pieces::bQueen);
    }
    return (currentState.board[i][j] == Pieces::wRook) ||
           (currentState.board[i][j] == Pieces::wQueen);
    ;
  }
  return false;
}

bool canCaptureABishopOrQueen(int i, int j, Colours c) {
  if (i >= 1 && i <= 7 && j >= 1 && j <= 7) {
    if (c == Colours::white) {
      return (currentState.board[i][j] == Pieces::bBishop) ||
             (currentState.board[i][j] == Pieces::bQueen);
    }
    return (currentState.board[i][j] == Pieces::wBishop) ||
           (currentState.board[i][j] == Pieces::wQueen);
    ;
  }
  return false;
}

bool pawnCaptures(int i, int j, Colours c) {
  if (i >= 1 && i <= 7 && j >= 1 && j <= 7) {
    if (Colours::white == c) {
      return canCaptureAPawn(i + 1, j + 1, c) ||
             canCaptureAPawn(i + 1, j - 1, c);
    }
    return canCaptureAPawn(i - 1, j + 1, c) || canCaptureAPawn(i - 1, j - 1, c);
  }
  return false;
}

bool knightCaptures(int i, int j, Colours c) {
  return (canCaptureAKnight(i + 1, j + 2, c) ||
          canCaptureAKnight(i + 1, j - 2, c) ||
          canCaptureAKnight(i + 2, j + 1, c) ||
          canCaptureAKnight(i + 2, j - 1, c) ||
          canCaptureAKnight(i - 1, j + 2, c) ||
          canCaptureAKnight(i - 1, j - 2, c) ||
          canCaptureAKnight(i - 2, j + 1, c) ||
          canCaptureAKnight(i - 2, j - 1, c));
}

bool rookOrQueenCaptures(int i, int j, Colours c) {
  for (int k = i + 1; k < 8; k++) {
    if (!canLand(k, j, c)) {
      break;
    } else if (canCaptureARookOrQueen(k, j, c)) {
      return true;
    }
  }
  for (int k = i - 1; k < 8; k--) {
    if (!canLand(k, j, c)) {
      break;
    } else if (canCaptureARookOrQueen(k, j, c)) {
      return true;
    }
  }
  for (int k = j - 1; k < 8; k--) {
    if (!canLand(k, j, c)) {
      break;
    } else if (canCaptureARookOrQueen(i, k, c)) {
      return true;
    }
  }
  for (int k = j + 1; k < 8; k++) {
    if (!canLand(k, j, c)) {
      break;
    } else if (canCaptureARookOrQueen(i, k, c)) {
      return true;
    }
  }
  return false;
}

bool bishopOrQueenCaptures(int i, int j, Colours c) {
  for (int k = 1; (i + k <= 7 && j + k <= 7); k++) {
    if (!canLand(i + k, j + k, c)) {
      break;
    }
    if (canCaptureABishopOrQueen(i + k, j + k, c)) {
      return true;
    }
  }
  for (int k = 1; (i - k >= 0 && j + k <= 7); k++) {
    if (!canLand(i - k, j + k, c)) {
      break;
    }
    if (canCapture(i - k, j + k, c)) {
      return true;
    }
  }
  for (int k = 1; (i - k >= 0 && j - k >= 0); k++) {
    if (!canLand(i - k, j - k, c)) {
      break;
    }
    if (canCapture(i - k, j - k, c)) {
      return true;
    }
  }
  for (int k = 1; (i + k <= 7 && j - k >= 0); k++) {
    if (canLand(i + k, j - k, c)) {
    } else {
      break;
    }
    if (canCapture(i + k, j - k, c)) {
      break;
    }
  }
  return false;
}

bool isInCheck(int i, int j, Colours currentColour) {
  if (pawnCaptures(i, j, currentColour)) {
    return true;
  }
  if (knightCaptures(i, j, currentColour)) {
    return true;
  }
  if (rookOrQueenCaptures(i, j, currentColour)) {
    return true;
  }
  if (bishopOrQueenCaptures(i, j, currentColour)) {
    return true;
  }
  return false;
}

vector<Turn> getCastlingTurns(Colours c) {
  vector<Turn> toLocations;

  if (c == Colours::white) {
    if (currentState.wCastle) {
      if (currentState.board[0][1] == Pieces::empty &&
          currentState.board[0][2] == Pieces::empty &&
          currentState.board[0][3] == Pieces::empty) {
        if (!isInCheck(0, 2, c) && !isInCheck(0, 3, c) && !isInCheck(0, 1, c) &&
            !isInCheck(0, 4, c)) {
          toLocations.push_back({{0, 4}, {0, 2}, Pieces::empty, true});
        }
      }
      if (currentState.board[0][5] == Pieces::empty &&
          currentState.board[0][6] == Pieces::empty) {
        if (!isInCheck(0, 5, c) && !isInCheck(0, 6, c) && !isInCheck(0, 4, c)) {
          toLocations.push_back({{0, 4}, {0, 6}, Pieces::empty, true});
        }
      }
    }
  } else {
    if (currentState.bCastle) {
      if (currentState.board[7][1] == Pieces::empty &&
          currentState.board[7][2] == Pieces::empty &&
          currentState.board[7][3] == Pieces::empty) {
        if (!isInCheck(7, 2, c) && !isInCheck(7, 3, c) && !isInCheck(7, 1, c) &&
            !isInCheck(7, 4, c)) {
          toLocations.push_back({{7, 4}, {7, 2}, Pieces::empty, true});
        }
      }
      if (currentState.board[7][5] == Pieces::empty &&
          currentState.board[7][6] == Pieces::empty) {
        if (!isInCheck(7, 5, c) && !isInCheck(7, 6, c) && !isInCheck(7, 4, c)) {
          toLocations.push_back({{7, 4}, {7, 6}, Pieces::empty, true});
        }
      }
    }
  }
  return toLocations;
}

vector<Turn> getPseudoCorrect(Colours colour) {
  vector<Turn> toLocations;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (isEmpty(i, j) || (colour == Colours::white && !isWhite(i, j)) ||
          (colour == Colours::black && !isBlack(i, j))) {
        continue;
      }
      if (isPawn(i, j)) {
        auto pawnLocations = pawnToLocations(i, j, colour);
        toLocations.insert(toLocations.end(), pawnLocations.begin(),
                           pawnLocations.end());
      }
      if (isKnight(i, j)) {
        auto knightLocations = knightToLocations(i, j, colour);
        toLocations.insert(toLocations.end(), knightLocations.begin(),
                           knightLocations.end());
      }
      if (isBishop(i, j)) {
        auto bishopLocations = bishopToLocations(i, j, colour);
        toLocations.insert(toLocations.end(), bishopLocations.begin(),
                           bishopLocations.end());
      }
      if (isRook(i, j)) {
        auto rookLocations = rookToLocations(i, j, colour);
        toLocations.insert(toLocations.end(), rookLocations.begin(),
                           rookLocations.end());
      }
      if (isQueen(i, j)) {
        auto queenLocations = queenToLocations(i, j, colour);
        toLocations.insert(toLocations.end(), queenLocations.begin(),
                           queenLocations.end());
      }
      if (isKing(i, j)) {
        auto kingLocations = kingToLocations(i, j, colour);
        toLocations.insert(toLocations.end(), kingLocations.begin(),
                           kingLocations.end());
      }
    }
  }
  auto castlingTurns = getCastlingTurns(colour);
  toLocations.insert(toLocations.end(), castlingTurns.begin(),
                     castlingTurns.end());
  return toLocations;
}

void applyTurn(Turn turn) {
  Pieces piece = (turn.promotionPiece == Pieces::empty)
                     ? (currentState.board[turn.from.first][turn.from.second])
                     : turn.promotionPiece;
  int i = turn.to.first, j = turn.to.second;
  currentState.board[turn.to.first][turn.to.second] = piece;
  currentState.board[turn.from.first][turn.from.second] = Pieces::empty;
  currentState.canEnpassant = turn.isEnpassantInitiater;
  currentState.enpassantSq = turn.enpassantSquare;
  if (turn.isEnpassantFinisher) {
    if (isWhite(turn.to.first, turn.to.second)) {
      currentState.board[i - 1][j] = Pieces::empty;
    } else {
      currentState.board[i + 1][j] = Pieces::empty;
    }
  }
  if (turn.castling) {
    if (j == 2) {
      currentState.board[i][3] = currentState.board[i][0];
      currentState.board[i][0] = Pieces::empty;
    }
    if (j == 6) {
      currentState.board[i][5] = currentState.board[i][7];
      currentState.board[i][7] = Pieces::empty;
    }
  }
}

vector<Turn> generateTurns(Colours c) {
  vector<Turn> turns;
  for (auto& turn : getPseudoCorrect(c)) {
    gameState originalState = currentState;
    applyTurn(turn);
    Pieces king = c == Colours::white ? Pieces::wKing : Pieces::bKing;
    Square kingLocation = getPieceLocation(king);
    if (!isInCheck(kingLocation.first, kingLocation.second, c)) {
      turns.push_back(turn);
    }
    currentState = originalState;
  }
  return turns;
}