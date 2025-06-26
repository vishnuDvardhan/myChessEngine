
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

char pieceTable[13] = {'.', 'P', 'N', 'B', 'R', 'K', 'Q',
                       'p', 'n', 'b', 'r', 'k', 'q'};
typedef pair<int, int> Square;
ofstream outFile("perft_debug.txt");

enum Colours : int { black, white };

enum Pieces : uint16_t {
  empty = 0,
  wPawn = 1 << 1,
  wKnight = 1 << 2,
  wBishop = 1 << 3,
  wRook = 1 << 4,
  wQueen = 1 << 5,
  wKing = 1 << 6,
  bPawn = 1 << 7,
  bKnight = 1 << 8,
  bBishop = 1 << 9,
  bRook = 1 << 10,
  bQueen = 1 << 11,
  bKing = 1 << 12,
  invalid = 1 << 13,
};

struct Castling {
  bool wKingSide = false;
  bool wQueenSide = false;
  bool bKingSide = false;
  bool bQueenSide = false;
};

int pieceValue(Pieces p) {
  switch (p) {
    case wPawn:
      return 1;
    case wKnight:
      return 3;
    case wBishop:
      return 3;
    case wRook:
      return 5;
    case wQueen:
      return 9;
    case wKing:
      return 0;
    case bPawn:
      return -1;
    case bKnight:
      return -3;
    case bBishop:
      return -3;
    case bRook:
      return -5;
    case bQueen:
      return -9;
    case bKing:
      return 0;
    default:
      return 0;
  }
}

unordered_map<char, Pieces> charToPiece{
    {' ', Pieces::empty},   {'P', Pieces::wPawn}, {'N', Pieces::wKnight},
    {'B', Pieces::wBishop}, {'R', Pieces::wRook}, {'Q', Pieces::wQueen},
    {'K', Pieces::wKing},   {'p', Pieces::bPawn}, {'n', Pieces::bKnight},
    {'b', Pieces::bBishop}, {'r', Pieces::bRook}, {'q', Pieces::bQueen},
    {'k', Pieces::bKing}};

unordered_map<Pieces, char> pieceToChar{
    {Pieces::empty, '.'},   {Pieces::wPawn, 'P'}, {Pieces::wKnight, 'N'},
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
  Pieces board[12][12];
  Square enpassantSq = {-1, -1};
  bool isWhitesTurn = true;
  bool canEnpassant = true;
  bool wCastle = true;
  bool bCastle = true;
  struct Castling {
    bool wkingSide = true;
    bool wQueenSide = true;
    bool bKingSide = true;
    bool bQueenSide = true;
  } castling;

  void printBoardDebug() {
    outFile << "    ";
    for (int col = 0; col < 8; col++) {
      outFile << (char)(col + 97) << " ";
    }
    outFile << "\n\n";

    for (int i = 9; i >= 2; i--) {
      outFile << " " << (i - 1) << "  ";
      for (int j = 2; j < 10; j++) {
        outFile << pieceToChar[board[i][j]] << " ";
      }
      outFile << "\n";
    }
    outFile << "\n";
  }
};

struct Turn {
  Square from;
  Square to;
  Pieces promotionPiece = Pieces::empty;
  bool castling = false;
  Square enPassantSq = {-1, -1};
  bool enPassant = false;
  Pieces capturedPiece = Pieces::empty;
  // Default constructor
  Turn() : from({-1, -1}), to({-1, -1}), promotionPiece(Pieces::empty) {}

  Turn(Square f, Square t, Pieces p = Pieces::empty, bool c = false,
       Square enPassantSq = {-1, -1}, bool enPassant = false)
      : from(f),
        to(t),
        promotionPiece(p),
        castling(c),
        enPassantSq(enPassantSq),
        enPassant(enPassant) {}

  Turn(const std::string& move) {
    if (move.length() < 4 || move.length() > 5) {
      throw std::invalid_argument("Invalid move string");
    }

    from.second = move[0] - 'a' + 2;
    from.first = move[1] - '1' + 2;
    to.second = move[2] - 'a' + 2;
    to.first = move[3] - '1' + 2;

    if (move.length() == 5) {
      char promotionChar = move[4];
      promotionPiece = charToPiece[promotionChar];
    } else {
      promotionPiece = Pieces::empty;
    }
  }

  bool operator==(const Turn& other) const {
    return from == other.from && to == other.to &&
           promotionPiece == other.promotionPiece;
  }

  string numberToSquare() {
    if (promotionPiece == Pieces::empty) {
      return ((char)(from.second + 95) + to_string(from.first - 1) +
              (char)(to.second + 95) + to_string(to.first - 1));
    }
    return ((char)(from.second + 95) + to_string(from.first - 1) +
            (char)(to.second + 95) + to_string(to.first - 1)) +
           pieceToChar[promotionPiece];
  }

  std::string str() const {
    std::ostringstream os;
    os << "\n{{" << from.first << ", " << from.second << "}, "
       << "{" << to.first << ", " << to.second << "}, "
       << "Pieces::" << pieceToString(promotionPiece) << "}";
    return os.str();
  }

  friend std::ostream& operator<<(std::ostream& os, const Turn& turn) {
    os << turn.str();
    return os;
  }
};
typedef Turn Turns[255];

// Basic piece detection functions
bool isEmpty(int i, int j, gameState currentState);
bool isWhite(int i, int j, gameState currentState);
bool isBlack(int i, int j, gameState currentState);
bool isPawn(int i, int j, gameState currentState);
bool isKnight(int i, int j, gameState currentState);
bool isBishop(int i, int j, gameState currentState);
bool isRook(int i, int j, gameState currentState);
bool isQueen(int i, int j, gameState currentState);
bool isKing(int i, int j, gameState currentState);

// Check detection functions
bool pawnCaptures(gameState& currentState, int i, int j, bool isWhitesTurn);
bool knightCaptures(gameState& currentState, int i, int j, bool isWhitesTurn);
bool rookOrQueenCaptures(gameState& currentState, int i, int j,
                         bool isWhitesTurn);
bool bishopOrQueenCaptures(gameState& currentState, int i, int j,
                           bool isWhitesTurn);
bool isInCheck(gameState& currentState, int i, int j, bool isWhitesTurn,
               bool includeKnightCapturesValidation = true,
               bool includePawnCapturesValidation = true);

// Helper functions
bool isWhiteOrInvalid(gameState& currentState, int i, int j);
bool isBlackOrInvalid(gameState& currentState, int i, int j);
bool canWKnightLand(gameState& currentState, int i, int j);
bool canBKnightLand(gameState& currentState, int i, int j);
bool canWKingLand(gameState& currentState, int i, int j,
                  Square otherKingLocation);
bool canBKingLand(gameState& currentState, int i, int j,
                  Square otherKingLocation);
bool canPawnLandAndCaptureEnPassant(gameState& currentState, int i, int j);

// Utility functions
Square getKingLocation(gameState& currentState, bool isWhitesTurn);
Square getPieceLocation(Pieces piece, gameState& currentState);
Square getEnpassantSq(gameState& currentState, int i, int j, bool isWhitesTurn);
int getDistance(int x1, int y1, int x2, int y2);

// Movement generation functions
void pawnToLocations(gameState& currentState, int i, int j, bool isWhitesTurn,
                     Turns& toLocations, int& n);
void knightToLocations(gameState& currentState, int i, int j, bool isWhitesTurn,
                       Turns& toLocations, int& n);
void bishopToLocations(gameState& currentState, int i, int j, bool isWhitesTurn,
                       Turns& toLocations, int& n);
void rookToLocations(gameState& currentState, int i, int j, bool isWhitesTurn,
                     Turns& toLocations, int& n);
void queenToLocations(gameState& currentState, int i, int j, bool isWhitesTurn,
                      Turns& toLocations, int& n);
void kingToLocations(gameState& currentState, int i, int j, bool isWhitesTurn,
                     Turns& toLocations, int& n);
void getCastlingTurns(gameState& currentState, Turns& toLocations, int& n);

// Main generation functions
void getPseudoCorrect(gameState& currentState, Turns& toLocations, int& n);
void generateTurns(gameState& currentState, Turns& correctToLocations, int& m);
void applyTurn(gameState& currentState, Turn& turn);

bool isEmpty(int i, int j, gameState currentState) {
  return !(currentState.board[i][j] ^ Pieces::empty);
}

bool isRook(int i, int j, gameState currentState) {
  return (currentState.board[i][j] & 0b10000010000);
}

bool isKnight(int i, int j, gameState currentState) {
  return (currentState.board[i][j] & 0b100000100);
}

bool isPawn(int i, int j, gameState currentState) {
  return (currentState.board[i][j] & 0b10000010);
}

bool isBishop(int i, int j, gameState currentState) {
  return (currentState.board[i][j] & 0b1000001000);
}

bool isQueen(int i, int j, gameState currentState) {
  return (currentState.board[i][j] & 0b100000100000);
}

bool isKing(int i, int j, gameState currentState) {
  return (currentState.board[i][j] & 0b1000001000000);
}

bool canWKnightLand(gameState& currentState, int i, int j) {
  return isEmpty(i, j, currentState) || isBlack(i, j, currentState);
}

bool canBKnightLand(gameState& currentState, int i, int j) {
  return isEmpty(i, j, currentState) || isWhite(i, j, currentState);
}

Square getPieceLocation(Pieces piece, gameState& currentState) {
  for (int i = 2; i <= 9; i++) {
    for (int j = 2; j <= 9; j++) {
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

std::string map_to_string(int i, int j) {
  char column = 'a' + j;
  char row = '1' + i;
  return std::string(1, column) + std::string(1, row);
}

gameState initBoard() {
  gameState currentState;

  for (int i = 0; i < 12; i++) {
    for (int j = 0; j < 12; j++) {
      currentState.board[i][j] = Pieces::invalid;
    }
  }

  for (int i = 2; i < 10; i++) {
    for (int j = 2; j < 10; j++) {
      currentState.board[i][j] = Pieces::empty;
    }
  }

  for (int i = 0; i <= 7; i++) {
    currentState.board[3][i + 2] = Pieces::wPawn;
    currentState.board[8][i + 2] = Pieces::bPawn;
  }

  currentState.board[2][2] = Pieces::wRook;
  currentState.board[2][9] = Pieces::wRook;
  currentState.board[2][3] = Pieces::wKnight;
  currentState.board[2][8] = Pieces::wKnight;
  currentState.board[2][4] = Pieces::wBishop;
  currentState.board[2][7] = Pieces::wBishop;
  currentState.board[2][5] = Pieces::wQueen;
  currentState.board[2][6] = Pieces::wKing;

  currentState.board[9][2] = Pieces::bRook;
  currentState.board[9][9] = Pieces::bRook;
  currentState.board[9][3] = Pieces::bKnight;
  currentState.board[9][8] = Pieces::bKnight;
  currentState.board[9][4] = Pieces::bBishop;
  currentState.board[9][7] = Pieces::bBishop;
  currentState.board[9][5] = Pieces::bQueen;
  currentState.board[9][6] = Pieces::bKing;

  currentState.wKingLocation = {2, 6};
  currentState.bKingLocation = {9, 6};
  currentState.enpassantSq = {-1, -1};
  currentState.isWhitesTurn = true;

  return currentState;
}

void initBoardWithFen(gameState& currentState, string fen) {
  for (int i = 0; i < 12; i++) {
    for (int j = 0; j < 12; j++) {
      currentState.board[i][j] = Pieces::invalid;
    }
  }

  for (int i = 2; i < 10; i++) {
    for (int j = 2; j < 10; j++) {
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

  int i = 9;
  int j = 2;
  bool parseBoard = true;
  bool enPassantSq = false;

  for (const auto& ch : fen) {
    if (ch == ' ') {
      parseBoard = false;
    }
    if (parseBoard) {
      if (ch == '/') {
        i--;
        j = 2;
      } else if (ch >= '1' and ch <= '7') {
        j = j + int(ch) - 48;
      } else if (ch == '8') {
        j = 2;
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
        currentState.isWhitesTurn = true;
      } else if (ch == 'b') {
        currentState.isWhitesTurn = false;
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
      } else if (enPassantSq && ((currentState.isWhitesTurn && ch == '6') ||
                                 (!currentState.isWhitesTurn && ch == '3'))) {
        enPassantSq = false;
        currentState.enpassantSq.first = int(ch) - 49;
      }
    }
  }
}
void printBoard(gameState currentState) {
  for (int i = 9; i >= 2; i--) {
    for (int j = 2; j < 10; j++) {
      cout << pieceToChar[currentState.board[i][j]] << " ";
    }
    cout << "\n";
  }
  cout << "\n";
}

bool isWhite(int i, int j, gameState currentState) {
  return (currentState.board[i][j] & 0b1111110);
}

bool isBlack(int i, int j, gameState currentState) {
  return (currentState.board[i][j] & 0b1111110000000);
}

bool isWhiteOrInvalid(gameState& currentState, int i, int j) {
  return (currentState.board[i][j] & 0b10000001111110);
}

bool isBlackOrInvalid(gameState& currentState, int i, int j) {
  return (currentState.board[i][j] & 0b11111110000000);
}

Square getKingLocation(gameState& currentState, bool isWhitesTurn) {
  if (isWhitesTurn) {
    return currentState.wKingLocation;
  }
  return currentState.bKingLocation;
}

bool canPawnLandAndCaptureEnPassant(gameState& currentState, int i, int j) {
  return currentState.enpassantSq.first == i &&
         currentState.enpassantSq.second == j;
}

Square getEnpassantSq(gameState& currentState, int i, int j,
                      bool isWhitesTurn) {
  Square enpassantSq = {-1, -1};
  if (isWhitesTurn) {
    if ((currentState.board[i][j + 1] == Pieces::bPawn) ||
        currentState.board[i][j - 1] == Pieces::bPawn) {
      enpassantSq = {i - 1, j};
    }
  } else {
    if ((currentState.board[i][j + 1] == Pieces::wPawn) ||
        currentState.board[i][j - 1] == Pieces::wPawn) {
      enpassantSq = {i + 1, j};
    }
  }
  return enpassantSq;
}

bool canWKingLand(gameState& currentState, int i, int j,
                  Square otherKingLocation) {
  return ((isEmpty(i, j, currentState) || isBlack(i, j, currentState)) &&
          getDistance(i, j, otherKingLocation.first, otherKingLocation.second) >
              1 &&
          !knightCaptures(currentState, i, j, true) &&
          !pawnCaptures(currentState, i, j, true));
}

bool canBKingLand(gameState& currentState, int i, int j,
                  Square otherKingLocation) {
  return ((isEmpty(i, j, currentState) || isWhite(i, j, currentState)) &&
          getDistance(i, j, otherKingLocation.first, otherKingLocation.second) >
              1 &&
          !knightCaptures(currentState, i, j, false) &&
          !pawnCaptures(currentState, i, j, false));
}

void kingToLocations(gameState& currentState, int i, int j, bool isWhitesTurn,
                     Turns& toLocations, int& n) {
  auto otherKingLocation = getKingLocation(currentState, !isWhitesTurn);
  if (isWhitesTurn) {
    if (canWKingLand(currentState, i - 1, j - 1, otherKingLocation)) {
      toLocations[n++] = {{i, j}, {i - 1, j - 1}};
    }
    if (canWKingLand(currentState, i - 1, j + 1, otherKingLocation)) {
      toLocations[n++] = {{i, j}, {i - 1, j + 1}};
    }
    if (canWKingLand(currentState, i + 1, j - 1, otherKingLocation)) {
      toLocations[n++] = {{i, j}, {i + 1, j - 1}};
    }
    if (canWKingLand(currentState, i + 1, j + 1, otherKingLocation)) {
      toLocations[n++] = {{i, j}, {i + 1, j + 1}};
    }
    if (canWKingLand(currentState, i, j - 1, otherKingLocation)) {
      toLocations[n++] = {{i, j}, {i, j - 1}};
    }
    if (canWKingLand(currentState, i, j + 1, otherKingLocation)) {
      toLocations[n++] = {{i, j}, {i, j + 1}};
    }
    if (canWKingLand(currentState, i - 1, j, otherKingLocation)) {
      toLocations[n++] = {{i, j}, {i - 1, j}};
    }
    if (canWKingLand(currentState, i + 1, j, otherKingLocation)) {
      toLocations[n++] = {{i, j}, {i + 1, j}};
    }
  } else {
    if (canBKingLand(currentState, i - 1, j - 1, otherKingLocation)) {
      toLocations[n++] = {{i, j}, {i - 1, j - 1}};
    }
    if (canBKingLand(currentState, i - 1, j + 1, otherKingLocation)) {
      toLocations[n++] = {{i, j}, {i - 1, j + 1}};
    }
    if (canBKingLand(currentState, i + 1, j - 1, otherKingLocation)) {
      toLocations[n++] = {{i, j}, {i + 1, j - 1}};
    }
    if (canBKingLand(currentState, i + 1, j + 1, otherKingLocation)) {
      toLocations[n++] = {{i, j}, {i + 1, j + 1}};
    }
    if (canBKingLand(currentState, i, j - 1, otherKingLocation)) {
      toLocations[n++] = {{i, j}, {i, j - 1}};
    }
    if (canBKingLand(currentState, i, j + 1, otherKingLocation)) {
      toLocations[n++] = {{i, j}, {i, j + 1}};
    }
    if (canBKingLand(currentState, i - 1, j, otherKingLocation)) {
      toLocations[n++] = {{i, j}, {i - 1, j}};
    }
    if (canBKingLand(currentState, i + 1, j, otherKingLocation)) {
      toLocations[n++] = {{i, j}, {i + 1, j}};
    }
  }
}

void pawnToLocations(gameState& currentState, int i, int j, bool isWhitesTurn,
                     Turns& toLocations, int& n) {
  if (isWhitesTurn) {
    if (isEmpty(i + 1, j, currentState)) {
      if (i != 8) {
        toLocations[n++] = {{i, j}, {i + 1, j}};
      } else {
        toLocations[n++] = {{i, j}, {i + 1, j}, Pieces::wKnight};
        toLocations[n++] = {{i, j}, {i + 1, j}, Pieces::wBishop};
        toLocations[n++] = {{i, j}, {i + 1, j}, Pieces::wRook};
        toLocations[n++] = {{i, j}, {i + 1, j}, Pieces::wQueen};
      }
      if (i == 3 && isEmpty(i + 2, j, currentState)) {
        Square enpassantSq =
            getEnpassantSq(currentState, i + 2, j, isWhitesTurn);
        toLocations[n++] = {
            {i, j}, {i + 2, j}, Pieces::empty, false, enpassantSq};
      }
    }
    if (isBlack(i + 1, j + 1, currentState)) {
      if (i != 8) {
        toLocations[n++] = {{i, j}, {i + 1, j + 1}};
      } else {
        toLocations[n++] = {{i, j}, {i + 1, j + 1}, Pieces::wKnight};
        toLocations[n++] = {{i, j}, {i + 1, j + 1}, Pieces::wBishop};
        toLocations[n++] = {{i, j}, {i + 1, j + 1}, Pieces::wRook};
        toLocations[n++] = {{i, j}, {i + 1, j + 1}, Pieces::wQueen};
      }
    }
    if (canPawnLandAndCaptureEnPassant(currentState, i + 1, j + 1)) {
      toLocations[n++] = {{i, j}, {i + 1, j + 1}, Pieces::empty,
                          false,  {-1, -1},       true};
    }
    if (isBlack(i + 1, j - 1, currentState)) {
      if (i != 8) {
        toLocations[n++] = {{i, j}, {i + 1, j - 1}};
      } else {
        toLocations[n++] = {{i, j}, {i + 1, j - 1}, Pieces::wKnight};
        toLocations[n++] = {{i, j}, {i + 1, j - 1}, Pieces::wBishop};
        toLocations[n++] = {{i, j}, {i + 1, j - 1}, Pieces::wRook};
        toLocations[n++] = {{i, j}, {i + 1, j - 1}, Pieces::wQueen};
      }
    }
    if (canPawnLandAndCaptureEnPassant(currentState, i + 1, j - 1)) {
      toLocations[n++] = {{i, j}, {i + 1, j - 1}, Pieces::empty,
                          false,  {-1, -1},       true};
    }
  } else {
    if (isEmpty(i - 1, j, currentState)) {
      if (i != 3) {
        toLocations[n++] = {{i, j}, {i - 1, j}};
      } else {
        toLocations[n++] = {{i, j}, {i - 1, j}, Pieces::bKnight};
        toLocations[n++] = {{i, j}, {i - 1, j}, Pieces::bBishop};
        toLocations[n++] = {{i, j}, {i - 1, j}, Pieces::bRook};
        toLocations[n++] = {{i, j}, {i - 1, j}, Pieces::bQueen};
      }
      if (i == 8 && isEmpty(i - 2, j, currentState)) {
        Square enpassantSq =
            getEnpassantSq(currentState, i - 2, j, isWhitesTurn);
        toLocations[n++] = {
            {i, j}, {i - 2, j}, Pieces::empty, false, enpassantSq};
      }
    }
    if (isWhite(i - 1, j + 1, currentState)) {
      if (i != 3) {
        toLocations[n++] = {{i, j}, {i - 1, j + 1}};
      } else {
        toLocations[n++] = {{i, j}, {i - 1, j + 1}, Pieces::bKnight};
        toLocations[n++] = {{i, j}, {i - 1, j + 1}, Pieces::bBishop};
        toLocations[n++] = {{i, j}, {i - 1, j + 1}, Pieces::bRook};
        toLocations[n++] = {{i, j}, {i - 1, j + 1}, Pieces::bQueen};
      }
    }
    if (canPawnLandAndCaptureEnPassant(currentState, i - 1, j + 1)) {
      toLocations[n++] = {{i, j}, {i - 1, j + 1}, Pieces::empty,
                          false,  {-1, -1},       true};
    }
    if (isWhite(i - 1, j - 1, currentState)) {
      if (i != 3) {
        toLocations[n++] = {{i, j}, {i - 1, j - 1}};
      } else {
        toLocations[n++] = {{i, j}, {i - 1, j - 1}, Pieces::bKnight};
        toLocations[n++] = {{i, j}, {i - 1, j - 1}, Pieces::bBishop};
        toLocations[n++] = {{i, j}, {i - 1, j - 1}, Pieces::bRook};
        toLocations[n++] = {{i, j}, {i - 1, j - 1}, Pieces::bQueen};
      }
    }
    if (canPawnLandAndCaptureEnPassant(currentState, i - 1, j - 1)) {
      toLocations[n++] = {{i, j}, {i - 1, j - 1}, Pieces::empty,
                          false,  {-1, -1},       true};
    }
  }
}

void knightToLocations(gameState& currentState, int i, int j, bool isWhitesTurn,
                       Turns& toLocations, int& n) {
  if (isWhitesTurn) {
    if (canWKnightLand(currentState, i - 1, j - 2)) {
      toLocations[n++] = {{i, j}, {i - 1, j - 2}};
    }
    if (canWKnightLand(currentState, i - 1, j + 2)) {
      toLocations[n++] = {{i, j}, {i - 1, j + 2}};
    }
    if (canWKnightLand(currentState, i - 2, j - 1)) {
      toLocations[n++] = {{i, j}, {i - 2, j - 1}};
    }
    if (canWKnightLand(currentState, i - 2, j + 1)) {
      toLocations[n++] = {{i, j}, {i - 2, j + 1}};
    }
    if (canWKnightLand(currentState, i + 1, j - 2)) {
      toLocations[n++] = {{i, j}, {i + 1, j - 2}};
    }
    if (canWKnightLand(currentState, i + 1, j + 2)) {
      toLocations[n++] = {{i, j}, {i + 1, j + 2}};
    }
    if (canWKnightLand(currentState, i + 2, j - 1)) {
      toLocations[n++] = {{i, j}, {i + 2, j - 1}};
    }
    if (canWKnightLand(currentState, i + 2, j + 1)) {
      toLocations[n++] = {{i, j}, {i + 2, j + 1}};
    }
  } else {
    if (canBKnightLand(currentState, i - 1, j - 2)) {
      toLocations[n++] = {{i, j}, {i - 1, j - 2}};
    }
    if (canBKnightLand(currentState, i - 1, j + 2)) {
      toLocations[n++] = {{i, j}, {i - 1, j + 2}};
    }
    if (canBKnightLand(currentState, i - 2, j - 1)) {
      toLocations[n++] = {{i, j}, {i - 2, j - 1}};
    }
    if (canBKnightLand(currentState, i - 2, j + 1)) {
      toLocations[n++] = {{i, j}, {i - 2, j + 1}};
    }
    if (canBKnightLand(currentState, i + 1, j - 2)) {
      toLocations[n++] = {{i, j}, {i + 1, j - 2}};
    }
    if (canBKnightLand(currentState, i + 1, j + 2)) {
      toLocations[n++] = {{i, j}, {i + 1, j + 2}};
    }
    if (canBKnightLand(currentState, i + 2, j - 1)) {
      toLocations[n++] = {{i, j}, {i + 2, j - 1}};
    }
    if (canBKnightLand(currentState, i + 2, j + 1)) {
      toLocations[n++] = {{i, j}, {i + 2, j + 1}};
    }
  }
}
void bishopToLocations(gameState& currentState, int i, int j, bool isWhitesTurn,
                       Turns& toLocations, int& n) {
  if (isWhitesTurn) {
    for (int k = 1;; k++) {
      if (isWhiteOrInvalid(currentState, i + k, j + k)) break;
      toLocations[n++] = {{i, j}, {i + k, j + k}};
      if (isBlack(i + k, j + k, currentState)) break;
    }
    for (int k = 1;; k++) {
      if (isWhiteOrInvalid(currentState, i - k, j + k)) break;
      toLocations[n++] = {{i, j}, {i - k, j + k}};
      if (isBlack(i - k, j + k, currentState)) break;
    }
    for (int k = 1;; k++) {
      if (isWhiteOrInvalid(currentState, i + k, j - k)) break;
      toLocations[n++] = {{i, j}, {i + k, j - k}};
      if (isBlack(i + k, j - k, currentState)) break;
    }
    for (int k = 1;; k++) {
      if (isWhiteOrInvalid(currentState, i - k, j - k)) break;
      toLocations[n++] = {{i, j}, {i - k, j - k}};
      if (isBlack(i - k, j - k, currentState)) break;
    }
  } else {
    for (int k = 1;; k++) {
      if (isBlackOrInvalid(currentState, i + k, j + k)) break;
      toLocations[n++] = {{i, j}, {i + k, j + k}};
      if (isWhite(i + k, j + k, currentState)) break;
    }
    for (int k = 1;; k++) {
      if (isBlackOrInvalid(currentState, i - k, j + k)) break;
      toLocations[n++] = {{i, j}, {i - k, j + k}};
      if (isWhite(i - k, j + k, currentState)) break;
    }
    for (int k = 1;; k++) {
      if (isBlackOrInvalid(currentState, i + k, j - k)) break;
      toLocations[n++] = {{i, j}, {i + k, j - k}};
      if (isWhite(i + k, j - k, currentState)) break;
    }
    for (int k = 1;; k++) {
      if (isBlackOrInvalid(currentState, i - k, j - k)) break;
      toLocations[n++] = {{i, j}, {i - k, j - k}};
      if (isWhite(i - k, j - k, currentState)) break;
    }
  }
}

void rookToLocations(gameState& currentState, int i, int j, bool isWhitesTurn,
                     Turns& toLocations, int& n) {
  if (isWhitesTurn) {
    for (int k = i - 1;; k--) {
      if (isWhiteOrInvalid(currentState, k, j)) break;
      toLocations[n++] = {{i, j}, {k, j}};
      if (isBlack(k, j, currentState)) break;
    }
    for (int k = i + 1;; k++) {
      if (isWhiteOrInvalid(currentState, k, j)) break;
      toLocations[n++] = {{i, j}, {k, j}};
      if (isBlack(k, j, currentState)) break;
    }
    for (int k = j - 1;; k--) {
      if (isWhiteOrInvalid(currentState, i, k)) break;
      toLocations[n++] = {{i, j}, {i, k}};
      if (isBlack(i, k, currentState)) break;
    }
    for (int k = j + 1;; k++) {
      if (isWhiteOrInvalid(currentState, i, k)) break;
      toLocations[n++] = {{i, j}, {i, k}};
      if (isBlack(i, k, currentState)) break;
    }
  } else {
    for (int k = i - 1;; k--) {
      if (isBlackOrInvalid(currentState, k, j)) break;
      toLocations[n++] = {{i, j}, {k, j}};
      if (isWhite(k, j, currentState)) break;
    }
    for (int k = i + 1;; k++) {
      if (isBlackOrInvalid(currentState, k, j)) break;
      toLocations[n++] = {{i, j}, {k, j}};
      if (isWhite(k, j, currentState)) break;
    }
    for (int k = j - 1;; k--) {
      if (isBlackOrInvalid(currentState, i, k)) break;
      toLocations[n++] = {{i, j}, {i, k}};
      if (isWhite(i, k, currentState)) break;
    }
    for (int k = j + 1;; k++) {
      if (isBlackOrInvalid(currentState, i, k)) break;
      toLocations[n++] = {{i, j}, {i, k}};
      if (isWhite(i, k, currentState)) break;
    }
  }
}

void queenToLocations(gameState& currentState, int i, int j, bool isWhitesTurn,
                      Turns& toLocations, int& n) {
  rookToLocations(currentState, i, j, isWhitesTurn, toLocations, n);
  bishopToLocations(currentState, i, j, isWhitesTurn, toLocations, n);
}

bool pawnCaptures(gameState& currentState, int i, int j, bool isWhitesTurn) {
  if (isWhitesTurn) {
    if (currentState.board[i + 1][j + 1] & Pieces::bPawn) return true;
    if (currentState.board[i + 1][j - 1] & Pieces::bPawn) return true;
  } else {
    if (currentState.board[i - 1][j + 1] & Pieces::wPawn) return true;
    if (currentState.board[i - 1][j - 1] & Pieces::wPawn) return true;
  }
  return false;
}

bool knightCaptures(gameState& currentState, int i, int j, bool isWhitesTurn) {
  if (isWhitesTurn) {
    return (currentState.board[i - 1][j - 2] & Pieces::bKnight) ||
           (currentState.board[i - 1][j + 2] & Pieces::bKnight) ||
           (currentState.board[i - 2][j - 1] & Pieces::bKnight) ||
           (currentState.board[i - 2][j + 1] & Pieces::bKnight) ||
           (currentState.board[i + 1][j - 2] & Pieces::bKnight) ||
           (currentState.board[i + 1][j + 2] & Pieces::bKnight) ||
           (currentState.board[i + 2][j - 1] & Pieces::bKnight) ||
           (currentState.board[i + 2][j + 1] & Pieces::bKnight);
  } else {
    return (currentState.board[i - 1][j - 2] & Pieces::wKnight) ||
           (currentState.board[i - 1][j + 2] & Pieces::wKnight) ||
           (currentState.board[i - 2][j - 1] & Pieces::wKnight) ||
           (currentState.board[i - 2][j + 1] & Pieces::wKnight) ||
           (currentState.board[i + 1][j - 2] & Pieces::wKnight) ||
           (currentState.board[i + 1][j + 2] & Pieces::wKnight) ||
           (currentState.board[i + 2][j - 1] & Pieces::wKnight) ||
           (currentState.board[i + 2][j + 1] & Pieces::wKnight);
  }
}

bool rookOrQueenCaptures(gameState& currentState, int i, int j,
                         bool isWhitesTurn) {
  if (isWhitesTurn) {
    for (int k = i - 1;; k--) {
      if (isEmpty(k, j, currentState)) continue;
      if (currentState.board[k][j] & (Pieces::bRook | Pieces::bQueen))
        return true;
      break;
    }
    for (int k = i + 1;; k++) {
      if (isEmpty(k, j, currentState)) continue;
      if (currentState.board[k][j] & (Pieces::bRook | Pieces::bQueen))
        return true;
      break;
    }
    for (int k = j - 1;; k--) {
      if (isEmpty(i, k, currentState)) continue;
      if (currentState.board[i][k] & (Pieces::bRook | Pieces::bQueen))
        return true;
      break;
    }
    for (int k = j + 1;; k++) {
      if (isEmpty(i, k, currentState)) continue;
      if (currentState.board[i][k] & (Pieces::bRook | Pieces::bQueen))
        return true;
      break;
    }
  } else {
    for (int k = i - 1;; k--) {
      if (isEmpty(k, j, currentState)) continue;
      if (currentState.board[k][j] & (Pieces::wRook | Pieces::wQueen))
        return true;
      break;
    }
    for (int k = i + 1;; k++) {
      if (isEmpty(k, j, currentState)) continue;
      if (currentState.board[k][j] & (Pieces::wRook | Pieces::wQueen))
        return true;
      break;
    }
    for (int k = j - 1;; k--) {
      if (isEmpty(i, k, currentState)) continue;
      if (currentState.board[i][k] & (Pieces::wRook | Pieces::wQueen))
        return true;
      break;
    }
    for (int k = j + 1;; k++) {
      if (isEmpty(i, k, currentState)) continue;
      if (currentState.board[i][k] & (Pieces::wRook | Pieces::wQueen))
        return true;
      break;
    }
  }
  return false;
}

bool bishopOrQueenCaptures(gameState& currentState, int i, int j,
                           bool isWhitesTurn) {
  if (isWhitesTurn) {
    for (int k = 1;; k++) {
      if (isEmpty(i + k, j + k, currentState)) continue;
      if (currentState.board[i + k][j + k] & (Pieces::bBishop | Pieces::bQueen))
        return true;
      break;
    }
    for (int k = 1;; k++) {
      if (isEmpty(i - k, j + k, currentState)) continue;
      if (currentState.board[i - k][j + k] & (Pieces::bBishop | Pieces::bQueen))
        return true;
      break;
    }
    for (int k = 1;; k++) {
      if (isEmpty(i + k, j - k, currentState)) continue;
      if (currentState.board[i + k][j - k] & (Pieces::bBishop | Pieces::bQueen))
        return true;
      break;
    }
    for (int k = 1;; k++) {
      if (isEmpty(i - k, j - k, currentState)) continue;
      if (currentState.board[i - k][j - k] & (Pieces::bBishop | Pieces::bQueen))
        return true;
      break;
    }
  } else {
    for (int k = 1;; k++) {
      if (isEmpty(i + k, j + k, currentState)) continue;
      if (currentState.board[i + k][j + k] & (Pieces::wBishop | Pieces::wQueen))
        return true;
      break;
    }
    for (int k = 1;; k++) {
      if (isEmpty(i - k, j + k, currentState)) continue;
      if (currentState.board[i - k][j + k] & (Pieces::wBishop | Pieces::wQueen))
        return true;
      break;
    }
    for (int k = 1;; k++) {
      if (isEmpty(i + k, j - k, currentState)) continue;
      if (currentState.board[i + k][j - k] & (Pieces::wBishop | Pieces::wQueen))
        return true;
      break;
    }
    for (int k = 1;; k++) {
      if (isEmpty(i - k, j - k, currentState)) continue;
      if (currentState.board[i - k][j - k] & (Pieces::wBishop | Pieces::wQueen))
        return true;
      break;
    }
  }
  return false;
}
bool isInCheck(gameState& currentState, int i, int j, bool isWhitesTurn,
               bool includeKnightCapturesValidation,
               bool includePawnCapturesValidation) {
  if (includePawnCapturesValidation &&
      pawnCaptures(currentState, i, j, isWhitesTurn)) {
    return true;
  }
  if (includeKnightCapturesValidation &&
      knightCaptures(currentState, i, j, isWhitesTurn)) {
    return true;
  }
  if (rookOrQueenCaptures(currentState, i, j, isWhitesTurn)) {
    return true;
  }
  if (bishopOrQueenCaptures(currentState, i, j, isWhitesTurn)) {
    return true;
  }
  return false;
}

void getCastlingTurns(gameState& currentState, Turns& toLocations, int& n) {
  bool isWhitesTurn = currentState.isWhitesTurn;
  if (isWhitesTurn) {
    if (currentState.castling.wkingSide &&
        currentState.board[2][7] == Pieces::empty &&
        currentState.board[2][8] == Pieces::empty &&
        currentState.board[2][9] == Pieces::wRook &&
        !isInCheck(currentState, 2, 6, isWhitesTurn) &&
        !isInCheck(currentState, 2, 7, isWhitesTurn) &&
        !isInCheck(currentState, 2, 8, isWhitesTurn)) {
      toLocations[n++] = {{2, 6}, {2, 8}, Pieces::empty, true};
    }
    if (currentState.castling.wQueenSide &&
        currentState.board[2][5] == Pieces::empty &&
        currentState.board[2][4] == Pieces::empty &&
        currentState.board[2][3] == Pieces::empty &&
        currentState.board[2][2] == Pieces::wRook &&
        !isInCheck(currentState, 2, 6, isWhitesTurn) &&
        !isInCheck(currentState, 2, 5, isWhitesTurn) &&
        !isInCheck(currentState, 2, 4, isWhitesTurn)) {
      toLocations[n++] = {{2, 6}, {2, 4}, Pieces::empty, true};
    }
  } else {
    if (currentState.castling.bKingSide &&
        currentState.board[9][7] == Pieces::empty &&
        currentState.board[9][8] == Pieces::empty &&
        currentState.board[9][9] == Pieces::bRook &&
        !isInCheck(currentState, 9, 6, isWhitesTurn) &&
        !isInCheck(currentState, 9, 7, isWhitesTurn) &&
        !isInCheck(currentState, 9, 8, isWhitesTurn)) {
      toLocations[n++] = {{9, 6}, {9, 8}, Pieces::empty, true};
    }
    if (currentState.castling.bQueenSide &&
        currentState.board[9][5] == Pieces::empty &&
        currentState.board[9][4] == Pieces::empty &&
        currentState.board[9][3] == Pieces::empty &&
        currentState.board[9][2] == Pieces::bRook &&
        !isInCheck(currentState, 9, 6, isWhitesTurn) &&
        !isInCheck(currentState, 9, 5, isWhitesTurn) &&
        !isInCheck(currentState, 9, 4, isWhitesTurn)) {
      toLocations[n++] = {{9, 6}, {9, 4}, Pieces::empty, true};
    }
  }
}

void getPseudoCorrect(gameState& currentState, Turns& toLocations, int& n) {
  bool isWhitesTurn = currentState.isWhitesTurn;
  n = 0;

  for (int i = 2; i < 10; i++) {
    for (int j = 2; j < 10; j++) {
      if (isWhitesTurn && !isWhite(i, j, currentState)) continue;
      if (!isWhitesTurn && !isBlack(i, j, currentState)) continue;

      if (isPawn(i, j, currentState)) {
        pawnToLocations(currentState, i, j, isWhitesTurn, toLocations, n);
      }
      if (isKnight(i, j, currentState)) {
        knightToLocations(currentState, i, j, isWhitesTurn, toLocations, n);
      }
      if (isBishop(i, j, currentState)) {
        bishopToLocations(currentState, i, j, isWhitesTurn, toLocations, n);
      }
      if (isRook(i, j, currentState)) {
        rookToLocations(currentState, i, j, isWhitesTurn, toLocations, n);
      }
      if (isQueen(i, j, currentState)) {
        queenToLocations(currentState, i, j, isWhitesTurn, toLocations, n);
      }
      if (isKing(i, j, currentState)) {
        kingToLocations(currentState, i, j, isWhitesTurn, toLocations, n);
      }
    }
  }
  getCastlingTurns(currentState, toLocations, n);
}
void applyTurn(gameState& currentState, Turn& turn) {
  Square from = turn.from;
  Square to = turn.to;

  turn.capturedPiece = currentState.board[to.first][to.second];
  currentState.board[to.first][to.second] =
      currentState.board[from.first][from.second];

  // Update king locations
  if (currentState.board[to.first][to.second] == Pieces::wKing) {
    currentState.wKingLocation.first = to.first;
    currentState.wKingLocation.second = to.second;
  } else if (currentState.board[to.first][to.second] == Pieces::bKing) {
    currentState.bKingLocation.first = to.first;
    currentState.bKingLocation.second = to.second;
  }

  currentState.board[from.first][from.second] = Pieces::empty;

  if (turn.promotionPiece != Pieces::empty) {
    currentState.board[to.first][to.second] = turn.promotionPiece;
  }

  currentState.enpassantSq = turn.enPassantSq;

  if (turn.enPassant) {
    currentState.board[from.first][to.second] = Pieces::empty;
  }

  if (from.first == 2) {     // White pieces
    if (from.second == 6) {  // King
      currentState.castling.wkingSide = false;
      currentState.castling.wQueenSide = false;
    }
    if (from.second == 2) {  // Queenside rook
      currentState.castling.wQueenSide = false;
    }
    if (from.second == 9) {  // Kingside rook
      currentState.castling.wkingSide = false;
    }
  }
  if (from.first == 9) {     // Black pieces
    if (from.second == 6) {  // King
      currentState.castling.bKingSide = false;
      currentState.castling.bQueenSide = false;
    }
    if (from.second == 2) {  // Queenside rook
      currentState.castling.bQueenSide = false;
    }
    if (from.second == 9) {  // Kingside rook
      currentState.castling.bKingSide = false;
    }
  }

  // Handle castling
  if (turn.castling) {
    if (from.first == 2) {   // White castling
      if (to.second == 8) {  // Kingside
        currentState.board[2][7] = Pieces::wRook;
        currentState.board[2][9] = Pieces::empty;
      } else if (to.second == 4) {  // Queenside
        currentState.board[2][5] = Pieces::wRook;
        currentState.board[2][2] = Pieces::empty;
      }
    } else {                 // Black castling
      if (to.second == 8) {  // Kingside
        currentState.board[9][7] = Pieces::bRook;
        currentState.board[9][9] = Pieces::empty;
      } else if (to.second == 4) {  // Queenside
        currentState.board[9][5] = Pieces::bRook;
        currentState.board[9][2] = Pieces::empty;
      }
    }
  }

  currentState.isWhitesTurn = !currentState.isWhitesTurn;
}
void generateTurns(gameState& currentState, Turns& correctToLocations, int& m) {
  bool isWhitesTurn = currentState.isWhitesTurn;
  Turns toLocations;
  int n = 0;
  getPseudoCorrect(currentState, toLocations, n);

  m = 0;
  for (int i = 0; i < n; i++) {
    gameState origState = currentState;
    applyTurn(currentState, toLocations[i]);

    if (toLocations[i].castling) {
      correctToLocations[m++] = toLocations[i];
    } else {
      Square sq = getKingLocation(currentState, isWhitesTurn);
      if (!isInCheck(currentState, sq.first, sq.second, isWhitesTurn)) {
        correctToLocations[m++] = toLocations[i];
      }
    }
    currentState = origState;
  }
}
std::vector<Turn> movesSoFar;

uint64_t getCountCorrectTurns(gameState& currentState) {
  bool isWhitesTurn = currentState.isWhitesTurn;
  uint64_t count = 0;
  Turns toLocations;
  int n = 0;
  getPseudoCorrect(currentState, toLocations, n);

  auto kingLocation = getKingLocation(currentState, isWhitesTurn);
  bool includeKnightCapturesValidation = knightCaptures(
      currentState, kingLocation.first, kingLocation.second, isWhitesTurn);
  bool includePawnCapturesValidation = pawnCaptures(
      currentState, kingLocation.first, kingLocation.second, isWhitesTurn);

  for (int i = 0; i < n; i++) {
    auto origWKingLocation = currentState.wKingLocation;
    auto origBKingLocation = currentState.bKingLocation;
    auto origEnPassantSq = currentState.enpassantSq;
    gameState::Castling origCastling = currentState.castling;

    applyTurn(currentState, toLocations[i]);

    if (toLocations[i].castling) {
      count++;
    } else {
      Square sq = getKingLocation(currentState, isWhitesTurn);
      if (!isInCheck(currentState, sq.first, sq.second, isWhitesTurn,
                     includeKnightCapturesValidation,
                     includePawnCapturesValidation)) {
        count++;
      }
    }

    // undo
    currentState.wKingLocation = origWKingLocation;
    currentState.bKingLocation = origBKingLocation;
    currentState.enpassantSq = origEnPassantSq;
    currentState.castling = origCastling;
    currentState.isWhitesTurn = !currentState.isWhitesTurn;

    Square from = toLocations[i].from;
    Square to = toLocations[i].to;
    currentState.board[from.first][from.second] =
        currentState.board[to.first][to.second];
    currentState.board[to.first][to.second] = toLocations[i].capturedPiece;

    if (toLocations[i].promotionPiece != Pieces::empty) {
      currentState.board[from.first][from.second] =
          currentState.isWhitesTurn ? Pieces::wPawn : Pieces::bPawn;
    }

    if (toLocations[i].enPassant) {
      currentState.board[from.first][to.second] =
          currentState.isWhitesTurn ? Pieces::bPawn : Pieces::wPawn;
    }

    if (toLocations[i].castling) {
      if (from.first == 2) {
        if (to.second == 8) {
          currentState.board[2][7] = Pieces::empty;
          currentState.board[2][9] = Pieces::wRook;
        } else if (to.second == 4) {
          currentState.board[2][5] = Pieces::empty;
          currentState.board[2][2] = Pieces::wRook;
        }
      } else {
        if (to.second == 8) {
          currentState.board[9][7] = Pieces::empty;
          currentState.board[9][9] = Pieces::bRook;
        } else if (to.second == 4) {
          currentState.board[9][5] = Pieces::empty;
          currentState.board[9][2] = Pieces::bRook;
        }
      }
    }
  }
  return count;
}

uint64_t perft(gameState& currentState, int depth) {
  if (depth == 0) return 1;
  if (depth == 1) return getCountCorrectTurns(currentState);

  uint64_t count = 0;
  Turns turns;
  int n = 0;
  getPseudoCorrect(currentState, turns, n);

  bool isWhitesTurn = currentState.isWhitesTurn;
  auto kingLocation = getKingLocation(currentState, isWhitesTurn);
  bool includeKnightCapturesValidation = knightCaptures(
      currentState, kingLocation.first, kingLocation.second, isWhitesTurn);
  bool includePawnCapturesValidation = pawnCaptures(
      currentState, kingLocation.first, kingLocation.second, isWhitesTurn);

  for (int i = 0; i < n; i++) {
    Square origWKingLocation = currentState.wKingLocation;
    Square origBKingLocation = currentState.bKingLocation;
    Square origEnPassantSq = currentState.enpassantSq;
    gameState::Castling origCastling = currentState.castling;

    applyTurn(currentState, turns[i]);

    if (turns[i].castling) {
      count += perft(currentState, depth - 1);
    } else {
      Square sq = getKingLocation(currentState, isWhitesTurn);
      if (!isInCheck(currentState, sq.first, sq.second, isWhitesTurn,
                     includeKnightCapturesValidation,
                     includePawnCapturesValidation)) {
        count += perft(currentState, depth - 1);
      }
    }

    // Undo move
    currentState.wKingLocation = origWKingLocation;
    currentState.bKingLocation = origBKingLocation;
    currentState.enpassantSq = origEnPassantSq;
    currentState.castling = origCastling;
    currentState.isWhitesTurn = !currentState.isWhitesTurn;

    Square from = turns[i].from;
    Square to = turns[i].to;
    currentState.board[from.first][from.second] =
        currentState.board[to.first][to.second];
    currentState.board[to.first][to.second] = turns[i].capturedPiece;

    if (turns[i].promotionPiece != Pieces::empty) {
      currentState.board[from.first][from.second] =
          currentState.isWhitesTurn ? Pieces::wPawn : Pieces::bPawn;
    }

    if (turns[i].enPassant) {
      currentState.board[from.first][to.second] =
          currentState.isWhitesTurn ? Pieces::bPawn : Pieces::wPawn;
    }

    if (turns[i].castling) {
      if (from.first == 2) {
        if (to.second == 8) {
          currentState.board[2][7] = Pieces::empty;
          currentState.board[2][9] = Pieces::wRook;
        } else if (to.second == 4) {
          currentState.board[2][5] = Pieces::empty;
          currentState.board[2][2] = Pieces::wRook;
        }
      } else {
        if (to.second == 8) {
          currentState.board[9][7] = Pieces::empty;
          currentState.board[9][9] = Pieces::bRook;
        } else if (to.second == 4) {
          currentState.board[9][5] = Pieces::empty;
          currentState.board[9][2] = Pieces::bRook;
        }
      }
    }
  }
  return count;
}

std::string toFen(const gameState& gs) {
  std::stringstream fen;

  for (int row = 9; row >= 2; row--) {
    int emptyCount = 0;
    for (int col = 2; col < 10; col++) {
      Pieces piece = gs.board[row][col];
      if (piece == Pieces::empty) {
        emptyCount++;
      } else {
        if (emptyCount > 0) {
          fen << emptyCount;
          emptyCount = 0;
        }
        fen << pieceToChar[piece];
      }
    }
    if (emptyCount > 0) {
      fen << emptyCount;
    }
    if (row > 2) fen << '/';
  }
  fen << ' ';

  fen << (gs.isWhitesTurn ? 'w' : 'b') << ' ';

  if (gs.castling.wkingSide || gs.castling.wQueenSide ||
      gs.castling.bKingSide || gs.castling.bQueenSide) {
    std::string castlingRights = "";
    if (gs.castling.wkingSide) castlingRights += "K";
    if (gs.castling.wQueenSide) castlingRights += "Q";
    if (gs.castling.bKingSide) castlingRights += "k";
    if (gs.castling.bQueenSide) castlingRights += "q";
    fen << (castlingRights.empty() ? "-" : castlingRights);
  } else {
    fen << "-";
  }
  fen << ' ';

  if (gs.enpassantSq.first != -1 && gs.enpassantSq.second != -1) {
    char col = 'a' + gs.enpassantSq.first;
    int row = gs.enpassantSq.second + 1;
    fen << col << row;
  } else {
    fen << "-";
  }
  fen << ' ';

  fen << "0 ";

  fen << "1";

  return fen.str();
}

int evaluateBoard(gameState& gs) {
  int evaluationScore = 0;

  for (int i = 2; i <= 9; i++) {
    for (int j = 2; j <= 9; j++) {
      Pieces piece = gs.board[i][j];
      evaluationScore += pieceValue(piece);
    }
  }

  Turns turns;
  int n = 0;
  generateTurns(gs, turns, n);
  if (n == 0) {
    Square kingLocation = getKingLocation(gs, gs.isWhitesTurn);
    if (isInCheck(gs, kingLocation.first, kingLocation.second,
                  gs.isWhitesTurn)) {
      return gs.isWhitesTurn ? -100000 : 100000;
    }
  }

  return evaluationScore;
}

void perftDebug(gameState& currentState, int depth) {
  Turns turns;
  int n = 0;
  generateTurns(currentState, turns, n);

  for (int i = 0; i < n; i++) {
    gameState origState = currentState;
    applyTurn(currentState, turns[i]);
    uint64_t count = perft(currentState, depth - 1);

    std::cout << turns[i].numberToSquare() << ":" << count << "\n";

    currentState = origState;
  }
}
