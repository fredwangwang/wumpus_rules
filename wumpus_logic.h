//
// Created by huan on 4/9/17.
//

#ifndef WUMPUS_WORLD_WUMPUS_LOGIC_H
#define WUMPUS_WORLD_WUMPUS_LOGIC_H

#include <iostream>
#include <cstring>
#include "check_true_false.h"
#include "wumpus_board.h"

using namespace std;

class wumpus_logic {
public:
    wumpus_logic(int argc, char **command_line) {
        if (argc != 4) {
            printf("Usage: %s [wumpus-rules-file] [additional-knowledge-file] [input_file]\n", command_line[0]);
            exit_function(0);
        }

        knowledge_base = new logical_expression();
        strcpy(knowledge_base->connective, "and");

        read_wumpus_rules(command_line[1]);
        read_additional_kb(command_line[2]);
        read_statement(command_line[3]);

        if (valid_expression(knowledge_base) == 0) {
            printf("invalid knowledge base\n");
            exit_function(0);
        }

        if (valid_expression(statement) == 0) {
            printf("invalid statement\n");
            exit_function(0);
        }
    }

    ~wumpus_logic() {
        delete knowledge_base;
        delete statement;
    }

    void printKnowledgeBase() {
        printf("Printing knowledge base...\n");
        print_expression(knowledge_base, "\n");
        printf("\n\n");
    }

    void printStatement() {
        printf("Printing statement...\n");
        print_expression(statement, "");
        printf("\n\n");
    }

    void checkTrueFalse() {
        board.fillBoard(knowledge_base);
        check_true_false(knowledge_base, statement);
    }

    wumpus_board board;

private:
    void read_wumpus_rules(char *input) {
        input_file = fopen(input, "rb");
        if (input_file == 0) {
            printf("failed to open file %s\n", input);
            exit_function(0);
        }
        printf("Loading wumpus rules...\n");

        while (fgets(buffer, 200, input_file) != NULL) {
            // skip lines starting with # (comment lines), or empty lines
            if ((buffer[0] == '#') || (buffer[0] == 0) || (buffer[0] == 13) || (buffer[0] == 10)) {
                continue;
            }
            logical_expression *subexpression = read_expression(buffer);
            knowledge_base->subexpressions.push_back(subexpression);
        }
        fclose(input_file);
    }

    void read_additional_kb(char *input) {
        input_file = fopen(input, "rb");
        if (input_file == 0) {
            printf("failed to open file %s\n", input);
            exit_function(0);
        }
        printf("Loading additional knowledge...\n");
        while (fgets(buffer, 200, input_file) != NULL) {
            // skip lines starting with # (comment lines), or empty lines
            if ((buffer[0] == '#') || (buffer[0] == 0) || (buffer[0] == 13) || (buffer[0] == 10)) {
                continue;
            }
            logical_expression *subexpression = read_expression(buffer);
            knowledge_base->subexpressions.push_back(subexpression);
        }
        fclose(input_file);
    }

    void read_statement(char *input) {
        input_file = fopen(input, "rb");
        if (input_file == 0) {
            printf("failed to open file %s\n", input);
            exit_function(0);
        }

        printf("Loading statement...\n");
        fgets(buffer, 200, input_file);
        fclose(input_file);

        statement = read_expression(buffer);
    }

    char buffer[200];
    FILE *input_file;

    logical_expression *knowledge_base;
    logical_expression *statement;


};


#endif //WUMPUS_WORLD_WUMPUS_LOGIC_H
