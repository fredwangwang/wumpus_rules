//
// Created by huan on 4/9/17.
//

#ifndef WUMPUS_WORLD_WUMPUS_BOARD_H
#define WUMPUS_WORLD_WUMPUS_BOARD_H

#define TRUE 1
#define FALSE 0
#define UNKNOWN -1
#define stench 0
#define monster 1
#define breeze 2
#define pit 3

#include <vector>
#include "check_true_false.h"

using namespace std;

struct cell {
    int S;
    int M;
    int B;
    int P;

    vector<logical_expression *> expressions;

    cell() {
        S = M = B = P = UNKNOWN;
    }
};

class wumpus_board {
public:
    wumpus_board() {
        init_board();
    }

    void fillBoard(logical_expression *kb) {
        int x, y, type;
        for (logical_expression *exp : kb->subexpressions) {
            if (strcmp(exp->symbol, "") != 0) {  // the expression is a symbol
                parse_symbol(exp->symbol, x, y, type);
                place_statement_to_block(x, y, type);
                cout << "parsing symbol" << endl;
            } else {
                parse_connective(exp);
            }
        }
    }

    vector<vector<cell>> board;

private:
    void init_board() {
        board.resize(5);
        for (vector<cell> &s: board) {
            s.resize(5);
        }
    }

    void parse_symbol(char *sym, int &x, int &y, int &type) {
        char v;
        switch (sym[0]) {
            case 'S':
                type = stench;
                break;
            case 'M':
                type = monster;
                break;
            case 'B':
                type = breeze;
                break;
            case 'P':
                type = pit;
                break;
            default:
                break;
        }

        v = sym[2];
        x = atoi(&v);

        v = sym[4];
        y = atoi(&v);
    }

    void parse_connective(logical_expression *exp) {
        int x, y, type;
        if (strcmp(exp->connective, "not") == 0) {
            parse_symbol(exp->subexpressions[0]->symbol, x, y, type);
            place_statement_to_block(x, y, type, false);
        } else {
            add_expressions_to_block(exp, exp->subexpressions);
        }
    }

    void add_expressions_to_block(logical_expression *exp, vector<logical_expression *> sub) {
        int x, y, type;
        for (logical_expression *subexp:sub) {
            if (strcmp(subexp->symbol, "") != 0) {
                parse_symbol(subexp->symbol, x, y, type);
                cout << x << " " << y << endl;
                board[x][y].expressions.push_back(exp);
            } else {
                add_expressions_to_block(exp, subexp->subexpressions);
            }
        }
    }

    void place_statement_to_block(const int x, const int y, const int type, bool True = true) {
        int value = -1;
        if (True) value = TRUE;
        else value = FALSE;
        switch (type) {
            case stench:
                board[x][y].S = value;
                break;
            case monster:
                board[x][y].M = value;
                break;
            case breeze:
                board[x][y].B = value;
                break;
            case pit:
                board[x][y].P = value;
                break;
            default:
                break;
        }
    }


};


#endif //WUMPUS_WORLD_WUMPUS_BOARD_H
