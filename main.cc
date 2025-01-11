#include <iostream>
#include <string>

#include "board.h"

using namespace std;

int main() {
  initBoard();
  printBoard();

  auto bishopLocations = bishopToLocations(1, 1, Colours::white);
  for (const auto &sq : bishopLocations) {
    std::cout << map_to_string(sq.first, sq.second) << "\n";
  }
  std::cout << "\n";
}