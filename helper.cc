#include <iostream>
#include <string>
#include <thread>

#include "bit-board.h"
#include "board.h"

using namespace std;

void measurePerft(const string& fen, int depth) {
  gameState currentState;
  initBoardWithFen(currentState, fen);

  auto start = std::chrono::high_resolution_clock::now();
  uint64_t nodes = perft(currentState, depth);
  auto end = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  double seconds = duration.count() / 1000.0;
  uint64_t nps = (seconds > 0) ? static_cast<uint64_t>(nodes / seconds) : 0;

  std::cout << "Perft(" << depth << ") results:" << std::endl;
  std::cout << "Time: " << duration.count() << " ms" << std::endl;
  std::cout << "Nodes: " << nodes << std::endl;
  std::cout << "Nodes per second: " << nps << std::endl;
}

int main(int argc, char** argv) {
  // string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  U64 bitboard = 6ULL;
  print_bit_board(bitboard);
  bitboard = set_bit(bitboard, a1);
  print_bit_board(set_bit(bitboard, a1));
  bitboard = pop_bit(bitboard, a1);
  print_bit_board(bitboard);

  // measurePerft(fen, 6);
}
