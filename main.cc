#include <iostream>
#include <string>
#include "board.h"

using namespace std;

int main() {
    initBoard();
    printBoard();


    auto rookLocations = rookToLocations(0, 0, Colours::white);
    for (const auto& sq : rookLocations) {
        std::cout << map_to_string(sq.first, sq.second) << "\n";
    }
    std::cout << "\n";

}