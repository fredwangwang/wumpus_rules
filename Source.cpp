#include <iostream>
#include "wumpus_logic.h"

using namespace std;

int main(int argc, char **argv) {
    wumpus_logic wumpus(argc, argv);
    wumpus.printKnowledgeBase();
    wumpus.printStatement();
    wumpus.checkTrueFalse();


    while (1) {
        int x, y;
        cout << "x?";
        cin >> x;
        cout << "y?";
        cin >> y;
        cell &c = wumpus.board.board[x][y];
        cout << "M=" << c.M << "\tP=" << c.P << endl;
        cout << "expre size = " << wumpus.board.board[x][y].expressions.size() << endl;
    }
}
