#pragma once

#include <vector>
#include <set>

using namespace std;

class logical_expression {
public:
    char symbol[20]; // null if sentence is a more complex expression
    char connective[20]; // null if sentice is a symbol
    vector<logical_expression *> subexpressions;

    logical_expression();

    ~logical_expression();
};


void print_expression(logical_expression *expression, char *separator);

logical_expression *read_expression(char *input_string);

logical_expression *read_expression(char *input_string, long &counter);

long read_subexpressions(char *input_string, long &counter,
                         vector<logical_expression *> &subexpressions);

void read_word(char *input_string, long &counter, char *connective);

int valid_expression(logical_expression *expression);

int valid_symbol(char *symbol);

void exit_function(int value);

/**
 * This function extract all symbols without repitition from the given expression, and store the result
 * to the second parameter
 * @param exp
 * @param result
 */
void extract_symbol(logical_expression *exp, set<string> &result);


/**
 * helper function to find all the literals to reduce computation.
 * return true iff all the literals doesn't contradict each other.
 * @param kb
 * @param literal
 * @param remain
 * @return
 */
bool find_literal(logical_expression *kb, map<string, bool> &literal, vector<logical_expression *> &remain);

/**
 * Given the logic expression and the boolean assignment to each variable,
 * determine whether the logic expression is evaluated to true or false
 * @param exp
 * @param assignment
 * @return
 */
bool determine_expression(logical_expression *exp, map<string, bool> &assignment);

/**
 * given the knowledge base, statement, and given boolean assignment,
 * determine whether the kb entails the statement
 * @param kb
 * @param statement
 * @param assignment
 * @return
 */
bool entails(logical_expression *kb, logical_expression *statement, map<string, bool> &assignment);

/**
 * The brute force engine
 * @param known_literal
 * @param kb
 * @param statement
 * @param syms
 * @return 1   - definitely true
 * @return 0   - possibly
 * @return -1  - definitely false
 * @return 2   - both
 */
int engine(map<string, bool> &known_literal, logical_expression *kb, logical_expression *statement, set<string> &syms);

void check_true_false(logical_expression *knowledge_base, logical_expression *statement);



