
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

char pieceTable[13] = {'.', 'P', 'N', 'B', 'R', 'K', 'Q',
                       'p', 'n', 'b', 'r', 'k', 'q'};
typedef pair<int, int> Square;
ofstream outFile("perft_debug.txt");

enum Colours : int { black, white };

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

unordered_map<char, Pieces> charToPiece{
    {' ', Pieces::empty},   {'P', Pieces::wPawn}, {'N', Pieces::wKnight},
    {'B', Pieces::wBishop}, {'R', Pieces::wRook}, {'Q', Pieces::wQueen},
    {'K', Pieces::wKing},   {'p', Pieces::bPawn}, {'n', Pieces::bKnight},
    {'b', Pieces::bBishop}, {'r', Pieces::bRook}, {'q', Pieces::bQueen},
    {'k', Pieces::bKing}};

unordered_map<Pieces, char> pieceToChar{
    {Pieces::empty, ' '},   {Pieces::wPawn, 'P'}, {Pieces::wKnight, 'N'},
    {Pieces::wBishop, 'B'}, {Pieces::wRook, 'R'}, {Pieces::wQueen, 'Q'},
    {Pieces::wKing, 'K'},   {Pieces::bPawn, 'p'}, {Pieces::bKnight, 'n'},
    {Pieces::bBishop, 'b'}, {Pieces::bRook, 'r'}, {Pieces::bQueen, 'q'},
    {Pieces::bKing, 'k'}};

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
  Square wKingLocation = {-1, -1};
  Square bKingLocation = {-1, -1};
  Pieces board[8][8];
  Square enpassantSq = {-1, -1};
  Colours currentColour = Colours::white;
  bool canEnpassant = true;
  bool wCastle = true;
  bool bCastle = true;
  struct Castling {
    bool wkingSide = true;
    bool wQueenSide = true;
    bool bKingSide = true;
    bool bQueenSide = true;
  } castling;

  void printBoard() {
    std::cout << "    ";
    for (int col = 0; col < 8; col++) {
      std::cout << (char)(col + 97) << " ";
    }
    std::cout << "\n\n";

    // Print rows from bottom (0) to top (7)
    for (int i = 7; i >= 0; i--) {
      // Label each row on the left
      std::cout << " " << i + 1 << "  ";
      // Print the row contents from left to right
      for (int j = 0; j < 8; j++) {
        std::cout << pieceTable[static_cast<int>(board[i][j])] << " ";
      }
      std::cout << "\n";
    }
    std::cout << "\n";

    string currentColourString =
        (currentColour == Colours::white) ? "white" : "black";
    std::cout << "Current colour is " << currentColourString << std::endl;

    std::cout << "Castling rights:\n";
    std::cout << "  White King-side: " << (castling.wkingSide ? "Yes" : "No")
              << "\n";
    std::cout << "  White Queen-side: " << (castling.wQueenSide ? "Yes" : "No")
              << "\n";
    std::cout << "  Black King-side: " << (castling.bKingSide ? "Yes" : "No")
              << "\n";
    std::cout << "  Black Queen-side: " << (castling.bQueenSide ? "Yes" : "No")
              << "\n";

    std::cout << "Overall castling rights:\n";
    std::cout << "  White: "
              << ((castling.wkingSide || castling.wQueenSide) ? "Yes" : "No")
              << "\n";
    std::cout << "  Black: "
              << ((castling.bKingSide || castling.bQueenSide) ? "Yes" : "No")
              << "\n";
  }

  void printBoardDebug() {
    outFile << "    ";
    for (int col = 0; col < 8; col++) {
      outFile << col << " ";
    }
    outFile << "\n\n";

    // Print rows from bottom (0) to top (7)
    for (int i = 0; i < 8; i++) {
      // Label each row on the left
      outFile << " " << i << "  ";
      // Print the row contents from left to right
      for (int j = 0; j < 8; j++) {
        outFile << pieceTable[static_cast<int>(board[i][j])] << " ";
      }
      outFile << "\n";
    }
    outFile << "\n";
  }
};

// gameState currentState;

bool isEmpty(int i, int j, gameState currentState) {
  return currentState.board[i][j] == Pieces::empty;
}

bool isRook(int i, int j, gameState currentState) {
  return currentState.board[i][j] == Pieces::wRook ||
         currentState.board[i][j] == Pieces::bRook;
}

bool isKnight(int i, int j, gameState currentState) {
  return currentState.board[i][j] == Pieces::wKnight ||
         currentState.board[i][j] == Pieces::bKnight;
}

bool isPawn(int i, int j, gameState currentState) {
  return currentState.board[i][j] == Pieces::wPawn ||
         currentState.board[i][j] == Pieces::bPawn;
}

bool isBishop(int i, int j, gameState currentState) {
  return currentState.board[i][j] == Pieces::wBishop ||
         currentState.board[i][j] == Pieces::bBishop;
}

bool isQueen(int i, int j, gameState currentState) {
  return currentState.board[i][j] == Pieces::wQueen ||
         currentState.board[i][j] == Pieces::bQueen;
}

bool isKing(int i, int j, gameState currentState) {
  return currentState.board[i][j] == Pieces::wKing ||
         currentState.board[i][j] == Pieces::bKing;
}

Square getPieceLocation(Pieces piece, gameState currentState) {
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

  string numberToSquare() {
    if (promotionPiece == Pieces::empty) {
      return ((char)(from.second + 97) + to_string(from.first + 1) +
              (char)(to.second + 97) + to_string(to.first + 1));
    }
    return ((char)(from.second + 97) + to_string(from.first + 1) +
            (char)(to.second + 97) + to_string(to.first + 1)) +
           pieceTable[(promotionPiece)];
  }

  std::string str() const {
    std::ostringstream os;
    os << " { "
       << "from=(" << from.first << ", " << from.second << "), "
       << "to=(" << to.first << ", " << to.second << "), "
       << "promotionPiece=Pieces::" << pieceToString(promotionPiece) << ", "
       << "castling=" << (castling ? "true" : "false") << ", "
       << "isEnpassantInitiater=" << (isEnpassantInitiater ? "true" : "false")
       << ", "
       << "enpassantSquare=(" << enpassantSquare.first << ", "
       << enpassantSquare.second << "), "
       << "isEnpassantFinisher=" << (isEnpassantFinisher ? "true" : "false")
       << " }\n";
    return os.str();
  }

  // usefull for debugging
  //  std::string str() const {
  //    std::ostringstream os;
  //    os << "\n{{" << from.first << ", " << from.second << "}, "
  //       << "{" << to.first << ", " << to.second << "}, "
  //       << "Pieces::" << (pieceToString(promotionPiece)) << ", "
  //       << (castling ? "true" : "false") << ", "
  //       << (isEnpassantInitiater ? "true" : "false") << ", "
  //       << "{" << enpassantSquare.first << ", " << enpassantSquare.second
  //       << "}, " << (isEnpassantFinisher ? "true" : "false") << "}";

  //   return os.str();
  // }

  friend std::ostream& operator<<(std::ostream& os, const Turn& turn) {
    os << turn.str();
    return os;
  }
};

std::string map_to_string(int i, int j) {
  char column = 'a' + j;
  char row = '1' + i;
  return std::string(1, column) + std::string(1, row);
}

gameState initBoard() {
  gameState currentState;
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
  currentState.board[0][1] = Pieces::wKnight;
  currentState.board[0][6] = Pieces::wKnight;
  currentState.board[7][1] = Pieces::bKnight;
  currentState.board[7][6] = Pieces::bKnight;
  currentState.board[0][2] = Pieces::wBishop;
  currentState.board[0][5] = Pieces::wBishop;
  currentState.board[7][2] = Pieces::bBishop;
  currentState.board[7][5] = Pieces::bBishop;
  currentState.board[0][3] = Pieces::wQueen;
  currentState.board[7][3] = Pieces::bQueen;
  currentState.board[0][4] = Pieces::wKing;
  currentState.board[7][4] = Pieces::bKing;
  currentState.enpassantSq = {-1, -1};
  currentState.currentColour = Colours::white;
  return currentState;
}

void initBoardWithFen(gameState& currentState, string fen) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      currentState.board[i][j] = Pieces::empty;
    }
  }
  currentState.castling.wkingSide = false;
  currentState.castling.wQueenSide = false;
  currentState.castling.bKingSide = false;
  currentState.castling.bQueenSide = false;
  if (fen.empty()) {
    fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  }
  int i = 7;
  int j = 0;
  bool parseBoard = true;
  bool enPassantSq = false;
  for (const auto& ch : fen) {
    if (ch == ' ') {
      parseBoard = false;
    }
    if (parseBoard) {
      if (ch == '/') {
        i--;
        j = 0;
      } else if (ch >= '1' and ch <= '7') {
        j = j + int(ch) - 48;
      } else if (ch == '8') {
        j = 0;
      } else {
        currentState.board[i][j] = charToPiece[ch];
        if (currentState.board[i][j] == Pieces::wKing) {
          currentState.wKingLocation.first = i;
          currentState.wKingLocation.second = j;
        } else if (currentState.board[i][j] == Pieces::bKing) {
          currentState.bKingLocation.first = i;
          currentState.bKingLocation.second = j;
        }
        j++;
      }
    } else {
      if (ch == 'w') {
        currentState.currentColour = Colours::white;
      } else if (ch == 'b') {
        currentState.currentColour = Colours::black;

      } else if (ch == 'K') {
        currentState.castling.wkingSide = true;
      } else if (ch == 'Q') {
        currentState.castling.wQueenSide = true;
      } else if (ch == 'k') {
        currentState.castling.bKingSide = true;
      } else if (ch == 'q') {
        currentState.castling.bQueenSide = true;
      } else if (ch >= 'a' && ch <= 'h') {
        currentState.enpassantSq.second = int(ch) - 97;
        enPassantSq = true;
      } else if (enPassantSq &&
                     ((currentState.currentColour == Colours::white) &&
                      ch == '6') ||
                 (!(currentState.currentColour == Colours::white) &&
                  ch == '3')) {
        enPassantSq = false;
        currentState.enpassantSq.first = int(ch) - 49;
      }
    }
  }
}

void printBoard(gameState currentState) {
  for (int i = 7; i >= 0; i--) {
    for (int j = 0; j < 8; j++) {
      cout << pieceTable[static_cast<int>(currentState.board[i][j])] << " ";
    }
    cout << "\n";
  }
  cout << "\n";
}

bool isWhite(int i, int j, gameState currentState) {
  return currentState.board[i][j] == Pieces::wPawn ||
         currentState.board[i][j] == Pieces::wKnight ||
         currentState.board[i][j] == Pieces::wBishop ||
         currentState.board[i][j] == Pieces::wRook ||
         currentState.board[i][j] == Pieces::wQueen ||
         currentState.board[i][j] == Pieces::wKing;
}

bool isBlack(int i, int j, gameState currentState) {
  return currentState.board[i][j] == Pieces::bPawn ||
         currentState.board[i][j] == Pieces::bKnight ||
         currentState.board[i][j] == Pieces::bBishop ||
         currentState.board[i][j] == Pieces::bRook ||
         currentState.board[i][j] == Pieces::bQueen ||
         currentState.board[i][j] == Pieces::bKing;
}

bool canLand(int i, int j, Colours c, gameState currentState) {
  if (i >= 0 && i <= 7 && j >= 0 && j <= 7) {
    if (currentState.board[i][j] == Pieces::empty) {
      return true;
    }
    if (c == Colours::white) {
      return isBlack(i, j, currentState);
    }
    return isWhite(i, j, currentState);
  }
  return false;
}

bool canKingLand(int i, int j, Colours c, Square OppositeKingSquare,
                 gameState currentState) {
  if (i >= 0 && i <= 7 && j >= 0 && j <= 7) {
    if (canLand(i, j, c, currentState) &&
        (getDistance(i, j, OppositeKingSquare.first,
                     OppositeKingSquare.second) > 1)) {
      return true;
    }
  }
  return false;
}
bool canCapture(int i, int j, Colours c, gameState currentState) {
  if (c == Colours::white) {
    return isBlack(i, j, currentState);
  }
  return isWhite(i, j, currentState);
}

bool canPawnLand(int i, int j, Colours c, gameState currentState) {
  if (i >= 0 && i <= 7 && j >= 0 && j <= 7) {
    if (currentState.board[i][j] == Pieces::empty) {
      return true;
    }
  }
  return false;
}

bool canPawnCapture(int i, int j, Colours c, gameState currentState) {
  if (i >= 0 && i <= 7 && j >= 0 && j <= 7) {
    if (currentState.board[i][j] == Pieces::empty) {
      return false;
    }
    if (c == Colours::white) {
      return isBlack(i, j, currentState);
    }
    return isWhite(i, j, currentState);
  }
  return false;
}

bool canPawnEnpassant(int i, int j, Colours c, gameState currentState) {
  if (i >= 0 && i <= 7 && j >= 0 && j <= 7) {
    if ((i == currentState.enpassantSq.first) &&
        (j == currentState.enpassantSq.second)) {
      return isEmpty(i, j, currentState);
    }
  }
  return false;
}

vector<Turn> kingToLocations(int i, int j, Colours c, gameState currentState) {
  vector<Turn> toLocations;
  Pieces oppositeKing = c == Colours::white ? Pieces::bKing : Pieces::wKing;
  Square oppositeKingSquare = getPieceLocation(oppositeKing, currentState);

  if (canKingLand(i + 1, j + 1, c, oppositeKingSquare, currentState)) {
    toLocations.push_back({{i, j}, {i + 1, j + 1}});
  }
  if (canKingLand(i + 1, j, c, oppositeKingSquare, currentState)) {
    toLocations.push_back({{i, j}, {i + 1, j}});
  }
  if (canKingLand(i + 1, j - 1, c, oppositeKingSquare, currentState)) {
    toLocations.push_back({{i, j}, {i + 1, j - 1}});
  }
  if (canKingLand(i, j + 1, c, oppositeKingSquare, currentState)) {
    toLocations.push_back({{i, j}, {i, j + 1}});
  }
  if (canKingLand(i, j - 1, c, oppositeKingSquare, currentState)) {
    toLocations.push_back({{i, j}, {i, j - 1}});
  }
  if (canKingLand(i - 1, j + 1, c, oppositeKingSquare, currentState)) {
    toLocations.push_back({{i, j}, {i - 1, j + 1}});
  }
  if (canKingLand(i - 1, j, c, oppositeKingSquare, currentState)) {
    toLocations.push_back({{i, j}, {i - 1, j}});
  }
  if (canKingLand(i - 1, j - 1, c, oppositeKingSquare, currentState)) {
    toLocations.push_back({{i, j}, {i - 1, j - 1}});
  }
  return toLocations;
}

vector<Turn> pawnToLocations(int i, int j, Colours c, gameState currentState) {
  vector<Turn> toLocations;
  if (c == Colours::white) {
    if (canPawnLand(i + 1, j, c, currentState)) {
      if (i + 1 == 7) {
        toLocations.push_back({{i, j}, {i + 1, j}, Pieces::wQueen});
        toLocations.push_back({{i, j}, {i + 1, j}, Pieces::wRook});
        toLocations.push_back({{i, j}, {i + 1, j}, Pieces::wBishop});
        toLocations.push_back({{i, j}, {i + 1, j}, Pieces::wKnight});
      } else {
        toLocations.push_back({{i, j}, {i + 1, j}});
      }

      if (i == 1) {
        if (canPawnLand(i + 2, j, c, currentState)) {
          toLocations.push_back(
              {{i, j}, {i + 2, j}, Pieces::empty, false, true, {i + 1, j}});
        }
      }
    }
    if (canPawnCapture(i + 1, j + 1, c, currentState)) {
      if (i + 1 == 7) {
        toLocations.push_back({{i, j}, {i + 1, j + 1}, Pieces::wQueen});
        toLocations.push_back({{i, j}, {i + 1, j + 1}, Pieces::wRook});
        toLocations.push_back({{i, j}, {i + 1, j + 1}, Pieces::wBishop});
        toLocations.push_back({{i, j}, {i + 1, j + 1}, Pieces::wKnight});
      } else {
        toLocations.push_back({{i, j}, {i + 1, j + 1}});
      }
    }
    if (canPawnCapture(i + 1, j - 1, c, currentState)) {
      if (i + 1 == 7) {
        toLocations.push_back({{i, j}, {i + 1, j - 1}, Pieces::wQueen});
        toLocations.push_back({{i, j}, {i + 1, j - 1}, Pieces::wRook});
        toLocations.push_back({{i, j}, {i + 1, j - 1}, Pieces::wBishop});
        toLocations.push_back({{i, j}, {i + 1, j - 1}, Pieces::wKnight});
      } else {
        toLocations.push_back({{i, j}, {i + 1, j - 1}});
      }
    }
    if (canPawnEnpassant(i + 1, j - 1, c, currentState)) {
      toLocations.push_back({{i, j},
                             {i + 1, j - 1},
                             Pieces::empty,
                             false,
                             false,
                             {-1, -1},
                             true});
    }
    if (canPawnEnpassant(i + 1, j + 1, c, currentState)) {
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
    if (canPawnLand(i - 1, j, c, currentState)) {
      if (i - 1 == 0) {
        toLocations.push_back({{i, j}, {i - 1, j}, Pieces::bQueen});
        toLocations.push_back({{i, j}, {i - 1, j}, Pieces::bRook});
        toLocations.push_back({{i, j}, {i - 1, j}, Pieces::bBishop});
        toLocations.push_back({{i, j}, {i - 1, j}, Pieces::bKnight});
      } else {
        toLocations.push_back({{i, j}, {i - 1, j}});
      }
      if (i == 6) {
        if (canPawnLand(i - 2, j, c, currentState)) {
          toLocations.push_back(
              {{i, j}, {i - 2, j}, Pieces::empty, false, true, {i - 1, j}});
        }
      }
    }
    if (canPawnCapture(i - 1, j - 1, c, currentState)) {
      if (i - 1 == 0) {
        toLocations.push_back({{i, j}, {i - 1, j - 1}, Pieces::bQueen});
        toLocations.push_back({{i, j}, {i - 1, j - 1}, Pieces::bRook});
        toLocations.push_back({{i, j}, {i - 1, j - 1}, Pieces::bBishop});
        toLocations.push_back({{i, j}, {i - 1, j - 1}, Pieces::bKnight});
      } else {
        toLocations.push_back({{i, j}, {i - 1, j - 1}});
      }
    }
    if (canPawnCapture(i - 1, j + 1, c, currentState)) {
      if (i - 1 == 0) {
        toLocations.push_back({{i, j}, {i - 1, j + 1}, Pieces::bQueen});
        toLocations.push_back({{i, j}, {i - 1, j + 1}, Pieces::bRook});
        toLocations.push_back({{i, j}, {i - 1, j + 1}, Pieces::bBishop});
        toLocations.push_back({{i, j}, {i - 1, j + 1}, Pieces::bKnight});
      } else {
        toLocations.push_back({{i, j}, {i - 1, j + 1}});
      }
    }

    if (canPawnEnpassant(i - 1, j - 1, c, currentState)) {
      toLocations.push_back({{i, j},
                             {i - 1, j - 1},
                             Pieces::empty,
                             false,
                             false,
                             {-1, -1},
                             true});
    }
    if (canPawnEnpassant(i - 1, j + 1, c, currentState)) {
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

vector<Turn> knightToLocations(int i, int j, Colours c,
                               gameState currentState) {
  vector<Turn> toLocations;
  if (canLand(i + 2, j + 1, c, currentState)) {
    toLocations.push_back({{i, j}, {i + 2, j + 1}});
  }
  if (canLand(i + 2, j - 1, c, currentState)) {
    toLocations.push_back({{i, j}, {i + 2, j - 1}});
  }
  if (canLand(i + 1, j + 2, c, currentState)) {
    toLocations.push_back({{i, j}, {i + 1, j + 2}});
  }
  if (canLand(i + 1, j - 2, c, currentState)) {
    toLocations.push_back({{i, j}, {i + 1, j - 2}});
  }
  if (canLand(i - 2, j + 1, c, currentState)) {
    toLocations.push_back({{i, j}, {i - 2, j + 1}});
  }
  if (canLand(i - 2, j - 1, c, currentState)) {
    toLocations.push_back({{i, j}, {i - 2, j - 1}});
  }
  if (canLand(i - 1, j + 2, c, currentState)) {
    toLocations.push_back({{i, j}, {i - 1, j + 2}});
  }
  if (canLand(i - 1, j - 2, c, currentState)) {
    toLocations.push_back({{i, j}, {i - 1, j - 2}});
  }

  return toLocations;
}

vector<Turn> bishopToLocations(int i, int j, Colours c,
                               gameState currentState) {
  vector<Turn> toLocations;

  for (int k = 1; (i + k <= 7 && j + k <= 7); k++) {
    if (canLand(i + k, j + k, c, currentState)) {
      toLocations.push_back({{i, j}, {i + k, j + k}});
    } else {
      break;
    }
    if (canCapture(i + k, j + k, c, currentState)) {
      break;
    }
  }
  for (int k = 1; (i - k >= 0 && j + k <= 7); k++) {
    if (canLand(i - k, j + k, c, currentState)) {
      toLocations.push_back({{i, j}, {i - k, j + k}});
    } else {
      break;
    }
    if (canCapture(i - k, j + k, c, currentState)) {
      break;
    }
  }
  for (int k = 1; (i - k >= 0 && j - k >= 0); k++) {
    if (canLand(i - k, j - k, c, currentState)) {
      toLocations.push_back({{i, j}, {i - k, j - k}});
    } else {
      break;
    }
    if (canCapture(i - k, j - k, c, currentState)) {
      break;
    }
  }
  for (int k = 1; (i + k <= 7 && j - k >= 0); k++) {
    if (canLand(i + k, j - k, c, currentState)) {
      toLocations.push_back({{i, j}, {i + k, j - k}});
    } else {
      break;
    }
    if (canCapture(i + k, j - k, c, currentState)) {
      break;
    }
  }

  return toLocations;
}

vector<Turn> rookToLocations(int i, int j, Colours c, gameState currentState) {
  vector<Turn> toLocations;

  for (int k = i + 1; k < 8; k++) {
    if (canLand(k, j, c, currentState)) {
      toLocations.push_back({{i, j}, {k, j}});
    } else {
      break;
    }
    if (canCapture(k, j, c, currentState)) {
      break;
    }
  }
  for (int k = i - 1; k >= 0; k--) {
    if (canLand(k, j, c, currentState)) {
      toLocations.push_back({{i, j}, {k, j}});
    } else {
      break;
    }
    if (canCapture(k, j, c, currentState)) {
      break;
    }
  }
  for (int k = j - 1; k >= 0; k--) {
    if (canLand(i, k, c, currentState)) {
      toLocations.push_back({{i, j}, {i, k}});
    } else {
      break;
    }
    if (canCapture(i, k, c, currentState)) {
      break;
    }
  }
  for (int k = j + 1; k < 8; k++) {
    if (canLand(i, k, c, currentState)) {
      toLocations.push_back({{i, j}, {i, k}});
    } else {
      break;
    }
    if (canCapture(i, k, c, currentState)) {
      break;
    }
  }
  return toLocations;
}
vector<Turn> queenToLocations(int i, int j, Colours c, gameState currentState) {
  vector<Turn> toLocations;
  for (int k = i + 1; k < 8; k++) {
    if (canLand(k, j, c, currentState)) {
      toLocations.push_back({{i, j}, {k, j}});
    } else {
      break;
    }
    if (canCapture(k, j, c, currentState)) {
      break;
    }
  }
  for (int k = i - 1; k >= 0; k--) {
    if (canLand(k, j, c, currentState)) {
      toLocations.push_back({{i, j}, {k, j}});
    } else {
      break;
    }
    if (canCapture(k, j, c, currentState)) {
      break;
    }
  }
  for (int k = j - 1; k >= 0; k--) {
    if (canLand(i, k, c, currentState)) {
      toLocations.push_back({{i, j}, {i, k}});
    } else {
      break;
    }
    if (canCapture(i, k, c, currentState)) {
      break;
    }
  }
  for (int k = j + 1; k < 8; k++) {
    if (canLand(i, k, c, currentState)) {
      toLocations.push_back({{i, j}, {i, k}});
    } else {
      break;
    }
    if (canCapture(i, k, c, currentState)) {
      break;
    }
  }
  for (int k = 1; (k + i <= 7 && k + j <= 7); k++) {
    if (canLand(i + k, j + k, c, currentState)) {
      toLocations.push_back({{i, j}, {k + i, k + j}});
    } else {
      break;
    }
    if (canCapture(i + k, j + k, c, currentState)) {
      break;
    }
  }
  for (int k = 1; (i - k >= 0 && k + j <= 7); k++) {
    if (canLand(i - k, j + k, c, currentState)) {
      toLocations.push_back({{i, j}, {i - k, k + j}});
    } else {
      break;
    }
    if (canCapture(i - k, j + k, c, currentState)) {
      break;
    }
  }
  for (int k = 1; (i - k >= 0 && j - k >= 0); k++) {
    if (canLand(i - k, j - k, c, currentState)) {
      toLocations.push_back({{i, j}, {i - k, j - k}});
    } else {
      break;
    }
    if (canCapture(i - k, j - k, c, currentState)) {
      break;
    }
  }
  for (int k = 1; (i + k <= 7 && j - k >= 0); k++) {
    if (canLand(i + k, j - k, c, currentState)) {
      toLocations.push_back({{i, j}, {i + k, j - k}});
    } else {
      break;
    }
    if (canCapture(i + k, j - k, c, currentState)) {
      break;
    }
  }

  return toLocations;
}

bool canCaptureAPawn(int i, int j, Colours c, gameState currentState) {
  if (i >= 0 && i <= 7 && j >= 0 && j <= 7) {
    if (c == Colours::white) {
      return currentState.board[i][j] == Pieces::bPawn;
    }
    return currentState.board[i][j] == Pieces::wPawn;
  }
  return false;
}

bool canCaptureAKnight(int i, int j, Colours c, gameState currentState) {
  if (i >= 0 && i <= 7 && j >= 0 && j <= 7) {
    if (c == Colours::white) {
      return currentState.board[i][j] == Pieces::bKnight;
    }
    return currentState.board[i][j] == Pieces::wKnight;
  }
  return false;
}

bool canCaptureARookOrQueen(int i, int j, Colours c, gameState currentState) {
  if (i >= 0 && i <= 7 && j >= 0 && j <= 7) {
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

bool canCaptureABishopOrQueen(int i, int j, Colours c, gameState currentState) {
  if (i >= 0 && i <= 7 && j >= 0 && j <= 7) {
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

bool pawnCaptures(int i, int j, Colours c, gameState currentState) {
  if (i >= 0 && i <= 7 && j >= 0 && j <= 7) {
    if (Colours::white == c) {
      return canCaptureAPawn(i + 1, j + 1, c, currentState) ||
             canCaptureAPawn(i + 1, j - 1, c, currentState);
    }
    return canCaptureAPawn(i - 1, j + 1, c, currentState) ||
           canCaptureAPawn(i - 1, j - 1, c, currentState);
  }
  return false;
}

bool knightCaptures(int i, int j, Colours c, gameState currentState) {
  return (canCaptureAKnight(i + 1, j + 2, c, currentState) ||
          canCaptureAKnight(i + 1, j - 2, c, currentState) ||
          canCaptureAKnight(i + 2, j + 1, c, currentState) ||
          canCaptureAKnight(i + 2, j - 1, c, currentState) ||
          canCaptureAKnight(i - 1, j + 2, c, currentState) ||
          canCaptureAKnight(i - 1, j - 2, c, currentState) ||
          canCaptureAKnight(i - 2, j + 1, c, currentState) ||
          canCaptureAKnight(i - 2, j - 1, c, currentState));
}

bool rookOrQueenCaptures(int i, int j, Colours c, gameState currentState) {
  for (int k = i + 1; k < 8; k++) {
    if (isEmpty(k, j, currentState)) {
      continue;
    }
    if (canCaptureARookOrQueen(k, j, c, currentState)) {
      return true;
    }
    break;
  }
  for (int k = i - 1; k >= 0; k--) {
    if (isEmpty(k, j, currentState)) {
      continue;
    }
    if (canCaptureARookOrQueen(k, j, c, currentState)) {
      return true;
    }
    break;
  }
  for (int k = j - 1; k >= 0; k--) {
    if (isEmpty(i, k, currentState)) {
      continue;
    }
    if (canCaptureARookOrQueen(i, k, c, currentState)) {
      return true;
    }
    break;
  }

  for (int k = j + 1; k < 8; k++) {
    if (isEmpty(i, k, currentState)) {
      continue;
    }
    if (canCaptureARookOrQueen(i, k, c, currentState)) {
      return true;
    }
    break;
  }
  return false;
}

bool bishopOrQueenCaptures(int i, int j, Colours c, gameState currentState) {
  for (int k = 1; (i + k <= 7 && j + k <= 7); k++) {
    if (isEmpty(i + k, j + k, currentState)) {
      continue;
    }
    if (canCaptureABishopOrQueen(i + k, j + k, c, currentState)) {
      return true;
    }
    break;
  }

  for (int k = 1; (i - k >= 0 && j + k <= 7); k++) {
    if (isEmpty(i - k, j + k, currentState)) {
      continue;
    }
    if (canCaptureABishopOrQueen(i - k, j + k, c, currentState)) {
      return true;
    }
    break;
  }

  for (int k = 1; (i - k >= 0 && j - k >= 0); k++) {
    if (isEmpty(i - k, j - k, currentState)) {
      continue;
    }
    if (canCaptureABishopOrQueen(i - k, j - k, c, currentState)) {
      return true;
    }
    break;
  }
  for (int k = 1; (i + k <= 7 && j - k >= 0); k++) {
    if (isEmpty(i + k, j - k, currentState)) {
      continue;
    }
    if (canCaptureABishopOrQueen(i + k, j - k, c, currentState)) {
      return true;
    }
    break;
  }

  return false;
}

bool isInCheck(int i, int j, Colours currentColour, gameState currentState) {
  if (pawnCaptures(i, j, currentColour, currentState)) {
    // cout << "pawn captures\n";
    return true;
  }
  if (knightCaptures(i, j, currentColour, currentState)) {
    return true;
    // cout << "knight captures\n";
  }
  if (rookOrQueenCaptures(i, j, currentColour, currentState)) {
    // cout << "rook or queen captures\n";
    return true;
  }
  if (bishopOrQueenCaptures(i, j, currentColour, currentState)) {
    // cout << "bishop  or queen captures\n";
    return true;
  }
  return false;
}

vector<Turn> getCastlingTurns(Colours c, gameState currentState) {
  vector<Turn> toLocations;

  if (c == Colours::white) {
    if (currentState.wCastle) {
      if (currentState.castling.wQueenSide == true &&
          currentState.board[0][0] == Pieces::wRook &&
          currentState.board[0][1] == Pieces::empty &&
          currentState.board[0][2] == Pieces::empty &&
          currentState.board[0][3] == Pieces::empty) {
        if (!isInCheck(0, 2, c, currentState) &&
            !isInCheck(0, 3, c, currentState) &&
            // !isInCheck(0, 1, c, currentState) &&
            !isInCheck(0, 4, c, currentState)) {
          toLocations.push_back({{0, 4}, {0, 2}, Pieces::empty, true});
        }
      }
      if (currentState.castling.wkingSide == true &&
          currentState.board[0][7] == Pieces::wRook &&
          currentState.board[0][5] == Pieces::empty &&
          currentState.board[0][6] == Pieces::empty) {
        if (!isInCheck(0, 5, c, currentState) &&
            !isInCheck(0, 6, c, currentState) &&
            !isInCheck(0, 4, c, currentState)) {
          toLocations.push_back({{0, 4}, {0, 6}, Pieces::empty, true});
        }
      }
    }
  } else {
    if (currentState.bCastle) {
      if (currentState.castling.bQueenSide == true &&
          currentState.board[7][0] == Pieces::bRook &&
          currentState.board[7][1] == Pieces::empty &&
          currentState.board[7][2] == Pieces::empty &&
          currentState.board[7][3] == Pieces::empty) {
        if (!isInCheck(7, 2, c, currentState) &&
            !isInCheck(7, 3, c, currentState) &&
            // !isInCheck(7, 1, c, currentState) &&
            !isInCheck(7, 4, c, currentState)) {
          toLocations.push_back({{7, 4}, {7, 2}, Pieces::empty, true});
        }
      }
      if (currentState.castling.bKingSide == true &&
          currentState.board[7][7] == Pieces::bRook &&
          currentState.board[7][5] == Pieces::empty &&
          currentState.board[7][6] == Pieces::empty) {
        if (!isInCheck(7, 5, c, currentState) &&
            !isInCheck(7, 6, c, currentState) &&
            !isInCheck(7, 4, c, currentState)) {
          toLocations.push_back({{7, 4}, {7, 6}, Pieces::empty, true});
        }
      }
    }
  }
  return toLocations;
}

vector<Turn> getPseudoCorrect(Colours colour, gameState currentState) {
  vector<Turn> toLocations;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (isEmpty(i, j, currentState) ||
          (colour == Colours::white && !isWhite(i, j, currentState)) ||
          (colour == Colours::black && !isBlack(i, j, currentState))) {
        continue;
      }
      if (isPawn(i, j, currentState)) {
        auto pawnLocations = pawnToLocations(i, j, colour, currentState);
        toLocations.insert(toLocations.end(), pawnLocations.begin(),
                           pawnLocations.end());
      }
      if (isKnight(i, j, currentState)) {
        auto knightLocations = knightToLocations(i, j, colour, currentState);
        toLocations.insert(toLocations.end(), knightLocations.begin(),
                           knightLocations.end());
      }
      if (isBishop(i, j, currentState)) {
        auto bishopLocations = bishopToLocations(i, j, colour, currentState);
        toLocations.insert(toLocations.end(), bishopLocations.begin(),
                           bishopLocations.end());
      }
      if (isRook(i, j, currentState)) {
        auto rookLocations = rookToLocations(i, j, colour, currentState);
        toLocations.insert(toLocations.end(), rookLocations.begin(),
                           rookLocations.end());
      }
      if (isQueen(i, j, currentState)) {
        auto queenLocations = queenToLocations(i, j, colour, currentState);
        toLocations.insert(toLocations.end(), queenLocations.begin(),
                           queenLocations.end());
      }
      if (isKing(i, j, currentState)) {
        auto kingLocations = kingToLocations(i, j, colour, currentState);
        toLocations.insert(toLocations.end(), kingLocations.begin(),
                           kingLocations.end());
      }
    }
  }
  auto castlingTurns = getCastlingTurns(colour, currentState);
  toLocations.insert(toLocations.end(), castlingTurns.begin(),
                     castlingTurns.end());
  return toLocations;
}

void applyTurn(Turn turn, gameState& currentStat) {
  Pieces piece = (turn.promotionPiece == Pieces::empty)
                     ? (currentStat.board[turn.from.first][turn.from.second])
                     : turn.promotionPiece;
  int i = turn.to.first, j = turn.to.second;
  currentStat.board[turn.to.first][turn.to.second] = piece;
  currentStat.board[turn.from.first][turn.from.second] = Pieces::empty;
  currentStat.canEnpassant = turn.isEnpassantInitiater;
  currentStat.enpassantSq = turn.enpassantSquare;

  // Handle en passant
  if (turn.isEnpassantFinisher) {
    if (isWhite(turn.to.first, turn.to.second, currentStat)) {
      currentStat.board[i - 1][j] = Pieces::empty;
    } else {
      currentStat.board[i + 1][j] = Pieces::empty;
    }
  }

  // Handle castling
  if (turn.castling) {
    if (j == 2) {
      currentStat.board[i][3] = currentStat.board[i][0];
      currentStat.board[i][0] = Pieces::empty;
    } else if (j == 6) {
      currentStat.board[i][5] = currentStat.board[i][7];
      currentStat.board[i][7] = Pieces::empty;
    }
    if (i == 0) {
      currentStat.castling.wQueenSide = false;
      currentStat.castling.wkingSide = false;
    } else if (i == 7) {
      currentStat.castling.bQueenSide = false;
      currentStat.castling.bKingSide = false;
    }
  }

  // Make castling false when king or rook moves
  if (piece == Pieces::wKing) {
    currentStat.castling.wQueenSide = false;
    currentStat.castling.wkingSide = false;
  } else if (piece == Pieces::bKing) {
    currentStat.castling.bQueenSide = false;
    currentStat.castling.bKingSide = false;
  } else if (piece == Pieces::wRook) {
    if (turn.from.first == 0 && turn.from.second == 0) {
      currentStat.castling.wQueenSide = false;
    } else if (turn.from.first == 0 && turn.from.second == 7) {
      currentStat.castling.wkingSide = false;
    }
  } else if (piece == Pieces::bRook) {
    if (turn.from.first == 7 && turn.from.second == 0) {
      currentStat.castling.bQueenSide = false;
    } else if (turn.from.first == 7 && turn.from.second == 7) {
      currentStat.castling.bKingSide = false;
    }
  }

  // Update current colour
  if (currentStat.currentColour == Colours::white) {
    currentStat.currentColour = Colours::black;
  } else {
    currentStat.currentColour = Colours::white;
  }
}

vector<Turn> generateTurns(gameState& currentStat) {
  Colours c = currentStat.currentColour;
  vector<Turn> turns;

  for (auto& turn : getPseudoCorrect(c, currentStat)) {
    gameState originalState = currentStat;
    applyTurn(turn, currentStat);
    Pieces king = c == Colours::white ? Pieces::wKing : Pieces::bKing;
    Square kingLocation = getPieceLocation(king, currentStat);
    if (!isInCheck(kingLocation.first, kingLocation.second, c, currentStat)) {
      turns.push_back(turn);
    } else {
      // cout << "king is in check\n";
    }
    currentStat = originalState;
  }

  return turns;
}
std::vector<Turn> movesSoFar;

uint64_t perft(gameState& currentState, int depth, bool isRoot = true) {
  // if (isRoot) {
  //   cout << "Perft at root state" << "\n";
  //   currentState.printBoard();
  // }
  if (depth == 0) {
    // outFile << "Leaf at depth=0. Sequence of moves:\n";
    // for (auto& m : movesSoFar) outFile << "   " << m << "\n";

    // currentState.printBoardDebug();
    return 1;
  }

  uint64_t count = 0;
  auto turns = generateTurns(currentState);
  for (auto& turn : turns) {
    // movesSoFar.push_back(turn);

    gameState origState = currentState;
    applyTurn(turn, currentState);

    count += perft(currentState, depth - 1, false);

    currentState = origState;
    // movesSoFar.pop_back();
  }
  return count;
}

void perftDebug(gameState& currentState, int depth) {
  for (auto& turn : generateTurns(currentState)) {
    gameState origState = currentState;
    applyTurn(turn, currentState);
    uint64_t count = perft(currentState, depth - 1);

    std::cout << turn.numberToSquare() << ":" << count << "\n";

    currentState = origState;
  }
}
