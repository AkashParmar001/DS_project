#ifndef EXPRESSION_PARSER_H
#define EXPRESSION_PARSER_H

#include <string>

// ─────────────────────────────────────────────────────────────────
//  Constants
// ─────────────────────────────────────────────────────────────────
static const int EXPR_STACK_SIZE = 100;

// ═══════════════════════════════════════════════════════════════
//  Char Stack  —  used internally by expression algorithms
// ═══════════════════════════════════════════════════════════════
struct CharStack {
    char data[EXPR_STACK_SIZE];
    int  top;

    CharStack();
    bool isEmpty() const;
    bool isFull()  const;
    void push(char c);
    char pop();
    char peek() const;
};

// ═══════════════════════════════════════════════════════════════
//  Double Stack  —  used for expression evaluation
// ═══════════════════════════════════════════════════════════════
struct DoubleStack {
    double data[EXPR_STACK_SIZE];
    int    top;

    DoubleStack();
    bool   isEmpty() const;
    bool   isFull()  const;
    void   push(double v);
    double pop();
    double peek() const;
};

// ═══════════════════════════════════════════════════════════════
//  Expression Parser — free functions
// ═══════════════════════════════════════════════════════════════

// Operator precedence (used by shunting-yard)
int  precedence(char op);
bool isOperator(char c);
bool isOperand (char c);

// Parenthesis checker
bool checkParentheses(const std::string& expr);

// Conversions
std::string infixToPostfix(const std::string& infix);
std::string infixToPrefix (const std::string& infix);

// Reverse a string (helper)
std::string reverseStr(const std::string& s);

// Evaluation
double evaluatePostfix(const std::string& postfix);
double evaluatePrefix (const std::string& prefix);

// Apply a single operator
double applyOp(double a, double b, char op);

// Tower of Hanoi
void hanoi(int n, char from, char to, char aux, int& moves);

// ═══════════════════════════════════════════════════════════════
//  Menu entry point (called from main.cpp)
// ═══════════════════════════════════════════════════════════════
void expressionCalculatorMenu();

#endif // EXPRESSION_PARSER_H
