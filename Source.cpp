#include <iostream>
#include <cstring>
#include "check_true_false.h"

using namespace std;

int main(int argc, char ** argv) {
    char ** command_line = argv;
    if (argc != 4) {
        printf("Usage: %s [wumpus-rules-file] [additional-knowledge-file] [input_file]\n", command_line[0]);
        exit_function(0);
    }

    char buffer[200];
    char * input;
    FILE * input_file;

    // read wumpus rules
    input = command_line[1];
    input_file = fopen(input, "rb");
    if (input_file == 0) {
        printf("failed to open file %s\n", input);
        exit_function(0);
    }

    printf("Loading wumpus rules...\n");
    logical_expression * knowledge_base = new logical_expression();
    strcpy(knowledge_base->connective, "and");
    while (fgets(buffer, 200, input_file) != NULL)
    {
        // skip lines starting with # (comment lines), or empty lines
        if ((buffer[0] == '#') || (buffer[0] == 0) || (buffer[0] == 13) || (buffer[0] == 10))
        {
            continue;
        }
        logical_expression * subexpression = read_expression(buffer);
        knowledge_base->subexpressions.push_back(subexpression);
    }
    fclose(input_file);

    // read additional knowledge
    input = command_line[2];
    input_file = fopen(input, "rb");
    if (input_file == 0)
    {
        printf("failed to open file %s\n", input);
        exit_function(0);
    }

    printf("Loading additional knowledge...\n");
    while (fgets(buffer, 200, input_file) != NULL)
    {
        // skip lines starting with # (comment lines), or empty lines
        if ((buffer[0] == '#') || (buffer[0] == 0) || (buffer[0] == 13) || (buffer[0] == 10))
        {
            continue;
        }
        logical_expression * subexpression = read_expression(buffer);
        knowledge_base->subexpressions.push_back(subexpression);
    }
    fclose(input_file);

    if (valid_expression(knowledge_base) == 0)
    {
        printf("invalid knowledge base\n");
        exit_function(0);
    }

    print_expression(knowledge_base, "\n");

    // read statement whose entailment we want to determine.
    input = command_line[3];
    input_file = fopen(input, "rb");
    if (input_file == 0)
    {
        printf("failed to open file %s\n", input);
        exit_function(0);
    }

    printf("\n\nLoading statement...\n");
    fgets(buffer, 200, input_file);
    fclose(input_file);

    logical_expression * statement = read_expression(buffer);
    if (valid_expression(statement) == 0)
    {
        printf("invalid statement\n");
        exit_function(0);
    }

    print_expression(statement, "");

    check_true_false(knowledge_base, statement);

    delete knowledge_base;
    delete statement;
    exit_function(1);
}
