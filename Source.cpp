#include <iostream>
#include "wumpus_logic.h"

using namespace std;

int main(int argc, char **argv) {
    wumpus_logic wumpus(argc, argv);
    wumpus.printKnowledgeBase();
    wumpus.printStatement();
    wumpus.checkTrueFalse();
}
