#include <stdio.h>
#include <ctype.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <bitset>
#include <map>

#include "check_true_false.h"

logical_expression::logical_expression() {
    symbol[0] = 0;
    connective[0] = 0;
}

logical_expression::~logical_expression() {
    unsigned long counter;
    for (counter = 0; counter < subexpressions.size(); counter++)
        delete subexpressions[counter];
}

void print_expression(logical_expression *expression, char *separator) {
    if (expression == 0)
        printf("\nINVALID\n");
    else if (strcmp(expression->symbol, "") != 0)
        printf("%s", expression->symbol);
    else {
        printf("(%s", expression->connective);
        unsigned long counter;
        for (counter = 0; counter < expression->subexpressions.size(); counter++) {
            printf(" ");
            print_expression(expression->subexpressions[counter], "");
            printf(separator);
        }
        printf(")");
    }
}

logical_expression *read_expression(char *input_string) {
    long counter = 0;
    return read_expression(input_string, counter);
}

logical_expression *read_expression(char *input_string, long &counter) {
    logical_expression *result = new logical_expression();
    long length = strlen(input_string);
    while (1) {
        if (counter >= length)
            break;
        if (isspace(input_string[counter])) {   // skip whitespace 
            counter++;
            continue;
        } else if (input_string[counter] == '(') {
            counter++;
            read_word(input_string, counter, result->connective);
            read_subexpressions(input_string, counter, result->subexpressions);
            break;
        } else {
            read_word(input_string, counter, result->symbol);
            break;
        }
    }

    return result;
}

long read_subexpressions(char *input_string, long &counter, vector<logical_expression *> &subexpressions) {
    long length = strlen(input_string);
    while (1) {
        if (counter >= length) {
            printf("\nunexpected end of input\n");
            return 0;
        }
        if (isspace(input_string[counter])) {  // skip whitespace 
            counter++;
            continue;
        }
        if (input_string[counter] == ')') { // we are done 
            counter++;
            return 1;
        } else {
            logical_expression *expression = read_expression(input_string, counter);
            subexpressions.push_back(expression);
        }
    }
}

void read_word(char *input_string, long &counter, char *target) {
    unsigned long second_counter = 0;
    while (1) {
        if (counter >= strlen(input_string))
            break;
        if ((isalpha(input_string[counter])) || (input_string[counter] == '_') || (isdigit(input_string[counter]))) {
            target[second_counter] = input_string[counter];
            counter++;
            second_counter++;
        } else if ((input_string[counter] == ')') || (isspace(input_string[counter])))
            break;
        else {
            printf("unexpected character %c\n", input_string[counter]);
            exit_function(0);
        }
    }

    target[second_counter] = 0;
}

int valid_expression(logical_expression *expression) {
    if (strcmp(expression->symbol, "") != 0)
        return valid_symbol(expression->symbol);

    // [if] or [iff] is expected
    if ((strcmp(expression->connective, "if") == 0) || (strcmp(expression->connective, "iff") == 0)) {
        if (expression->subexpressions.size() != 2) {
            printf("error: connective \"%s\" with %li arguments\n",
                   expression->connective, expression->subexpressions.size());
            return 0;
        }
    } else if (strcmp(expression->connective, "not") == 0) {
        if (expression->subexpressions.size() != 1) {
            printf("error: connective \"%s\" with %li arguments\n",
                   expression->connective, expression->subexpressions.size());
            return 0;
        }
    } else if ((strcmp(expression->connective, "and") != 0) &&
               (strcmp(expression->connective, "or") != 0) &&
               (strcmp(expression->connective, "xor") != 0)) {
        printf("error: unknown connective %s\n", expression->connective);
        return 0;
    }

    unsigned long counter;
    for (counter = 0; counter < expression->subexpressions.size(); counter++)
        // if any of the expression failed to pass validation, then valid_expression fails. Return 0
        if (valid_expression(expression->subexpressions[counter]) == 0)
            return 0;

    return 1;
}

int valid_symbol(char *symbol) {
    if ((symbol == 0) || (strlen(symbol) == 0))
        return 0;
    unsigned long counter;
    for (counter = 0; counter < strlen(symbol); counter++)
        if ((symbol[counter] != '_') && (!(isalpha(symbol[counter]))) && (!(isdigit(symbol[counter]))))
            return 0;

    return 1;
}


void exit_function(int value) {
    //	char str_buf[200]; 
    //	printf("Press <ENTER> to exit.\n");
    //	gets(str_buf);
    exit(value);
}


void extract_symbol(logical_expression *exp, set<string> &result) {
    if (exp->symbol[0] != 0) {
        char *symbol = exp->symbol;
        //cout << symbol << endl;
        result.insert(string(symbol));
    } else {
        for (logical_expression *subexp:exp->subexpressions) {
            extract_symbol(subexp, result);
        }
    }
}


bool find_literal(logical_expression *kb, map<string, bool> &literal, vector<logical_expression *> &remain) {
    bool result = true;
    for (logical_expression *exp : kb->subexpressions) {
        if (strcmp(exp->symbol, "") != 0) {  // the expression is a symbol
            if (literal.find(string(exp->symbol)) != literal.end())
                return false;
            literal.insert(make_pair(string(exp->symbol), true));
        } else if (strcmp(exp->connective, "not") == 0) {
            if (literal.find(string(exp->subexpressions[0]->symbol)) != literal.end())
                return false;
            literal.insert(make_pair(string(exp->subexpressions[0]->symbol), false));
        } else {
            remain.push_back(exp);
        }
    }
    return result;
}


bool determine_expression(logical_expression *exp, map<string, bool> &assignment) {
    bool result = false;
    if (exp->symbol[0] != 0) {
        string sym = exp->symbol;
        return assignment[sym];
    } else {
        string conn = exp->connective;
        if (conn == "and") {
            result = true;
            for (logical_expression *subexp:exp->subexpressions)
                result &= determine_expression(subexp, assignment);
        } else if (conn == "or") {
            for (logical_expression *subexp:exp->subexpressions)
                result |= determine_expression(subexp, assignment);
        } else if (conn == "xor") {
            int count = 0;
            for (logical_expression *subexp:exp->subexpressions)
                count += determine_expression(subexp, assignment);
            if (count == 1) result = true;
        } else if (conn == "not") {
            string ssym = exp->subexpressions[0]->symbol;
            result = !assignment[ssym];
        } else if (conn == "if") {
            result = (!determine_expression(exp->subexpressions[0], assignment) ||
                      determine_expression(exp->subexpressions[1], assignment));
        } else if (conn == "iff") {
            result = ((!determine_expression(exp->subexpressions[0], assignment) ||
                       determine_expression(exp->subexpressions[1], assignment)) &&
                      (!determine_expression(exp->subexpressions[1], assignment) ||
                       determine_expression(exp->subexpressions[0], assignment)));
        }
    }
    return result;
}


bool entails(logical_expression *kb, logical_expression *statement, map<string, bool> &assignment) {
    if (!determine_expression(kb, assignment)) return true;
    else return determine_expression(statement, assignment);
}


int engine(map<string, bool> &known_literal, logical_expression *kb, logical_expression *statement, set<string> &syms) {
    unsigned long long i = 0, temp = 1;
    vector<string> vsyms(syms.begin(), syms.end());
    cout << vsyms.size() << endl;
    temp <<= vsyms.size();
    cout << temp << endl;
    bitset<64> perm;
    bitset<64> term = (temp - 1);

    logical_expression *neg_statement = new logical_expression;
    strcpy(neg_statement->connective, "not");
    neg_statement->subexpressions.push_back(statement);

    bool entails_statement = true, entails_neg_statement = true;
    for (i = 0; perm != term; i++) {
        // if (i % 100 == 0) cout << i << endl;
        perm = i;
        //assignments
        for (int j = 0; j < vsyms.size(); j++)
            known_literal[vsyms[j]] = perm[j];
        entails_statement &= entails(kb, statement, known_literal);
        entails_neg_statement &= entails(kb, neg_statement, known_literal);
    }

    int result = 99;
    if (entails_statement && !entails_neg_statement) result = 1;
    else if (!entails_statement && !entails_neg_statement) result = 0;
    else if (!entails_statement && entails_neg_statement) result = -1;
    else if (entails_statement && entails_neg_statement) result = 2;

    neg_statement->subexpressions.clear();      // avoid accidently remove original statement
    delete neg_statement;

    return result;
}

void check_true_false(logical_expression *knowledge_base, logical_expression *statement) {
    map<string, bool> literal;
    vector<logical_expression *> expression;

    if (!find_literal(knowledge_base, literal, expression)) {
        printf("\nboth true and false\n");
        FILE *fp = fopen("result.txt", "wb");
        if (fp == 0) {
            printf("something is wrong, cannot open result.txt for writing\n");
        } else {
            fprintf(fp, "both true and false");
            fclose(fp);
        }
        return;
    }

    set<string> syms;
    extract_symbol(knowledge_base, syms);
    for (auto it = literal.begin(); it != literal.end(); it++) {
        syms.erase(it->first);
    }

    int result = engine(literal, knowledge_base, statement, syms);

    FILE *fp = fopen("result.txt", "wb");
    if (fp == 0) {
        printf("something is wrong, cannot open result.txt for writing\n");
        return;
    }

    switch (result) {
        case 1:
            printf("\ndefinitely true\n");
            fprintf(fp, "definitely true");
            break;
        case 0:
            printf("\npossibly true, possibly false\n");
            fprintf(fp, "possibly true, possibly false");
            break;
        case -1:
            printf("\ndefinitely false\n");
            fprintf(fp, "definitely false");
            break;
        case 2:
            printf("\nboth true and false\n");
            fprintf(fp, "both true and false");
            break;
        default:
            abort();
    }

    fclose(fp);
}
