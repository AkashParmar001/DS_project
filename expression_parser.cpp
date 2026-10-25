#include "expression_parser.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <cctype>
#include <stdexcept>

using namespace std;

// ═══════════════════════════════════════════════════════════════
//  Utility
// ═══════════════════════════════════════════════════════════════
static void printLine(char c = '=', int w = 50) {
    for (int i = 0; i < w; i++) cout << c;
    cout << "\n";
}

// ═══════════════════════════════════════════════════════════════
//  CharStack
// ═══════════════════════════════════════════════════════════════
CharStack::CharStack() : top(-1) {}
bool CharStack::isEmpty() const { return top == -1; }
bool CharStack::isFull()  const { return top == EXPR_STACK_SIZE - 1; }

void CharStack::push(char c) {
    if (!isFull()) data[++top] = c;
}

char CharStack::pop() {
    if (isEmpty()) return '\0';
    return data[top--];
}

char CharStack::peek() const {
    if (isEmpty()) return '\0';
    return data[top];
}

// ═══════════════════════════════════════════════════════════════
//  DoubleStack
// ═══════════════════════════════════════════════════════════════
DoubleStack::DoubleStack() : top(-1) {}
bool   DoubleStack::isEmpty() const { return top == -1; }
bool   DoubleStack::isFull()  const { return top == EXPR_STACK_SIZE - 1; }

void   DoubleStack::push(double v) { if (!isFull()) data[++top] = v; }
double DoubleStack::pop()          { return isEmpty() ? 0.0 : data[top--]; }
double DoubleStack::peek() const   { return isEmpty() ? 0.0 : data[top]; }

// ═══════════════════════════════════════════════════════════════
//  Helpers
// ═══════════════════════════════════════════════════════════════
int precedence(char op) {
    if (op == '^')            return 3;
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

bool isOperand(char c) {
    return isdigit(c) || isalpha(c);
}

string reverseStr(const string& s) {
    string r = s;
    int l = 0, ri = (int)r.size() - 1;
    while (l < ri) { swap(r[l++], r[ri--]); }
    return r;
}

double applyOp(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) { cout << "[ERROR] Division by zero.\n"; return 0; }
            return a / b;
        case '^': return pow(a, b);
    }
    return 0;
}

// ═══════════════════════════════════════════════════════════════
//  Parenthesis Checker
//  Supports: ( ) [ ] { }
// ═══════════════════════════════════════════════════════════════
bool checkParentheses(const string& expr) {
    CharStack stk;
    cout << "\n  Checking: \"" << expr << "\"\n";
    cout << "  Step-by-step:\n";

    for (int i = 0; i < (int)expr.size(); i++) {
        char c = expr[i];
        if (c == '(' || c == '[' || c == '{') {
            stk.push(c);
            cout << "    [" << i << "] Push '" << c << "'\n";
        } else if (c == ')' || c == ']' || c == '}') {
            if (stk.isEmpty()) {
                cout << "    [" << i << "] '" << c
                     << "' — no matching opener. INVALID.\n";
                return false;
            }
            char top = stk.pop();
            bool match = (c == ')' && top == '(') ||
                         (c == ']' && top == '[') ||
                         (c == '}' && top == '{');
            cout << "    [" << i << "] Pop '" << top
                 << "' for '" << c << "' — " << (match ? "match" : "MISMATCH") << "\n";
            if (!match) return false;
        }
    }
    if (!stk.isEmpty()) {
        cout << "    Unmatched openers remain. INVALID.\n";
        return false;
    }
    return true;
}

// ═══════════════════════════════════════════════════════════════
//  Infix → Postfix  (Shunting-Yard algorithm)
//  Supports single-character operands and operators + - * / ^
//  Uses standard left-associativity; ^ is right-associative
// ═══════════════════════════════════════════════════════════════
string infixToPostfix(const string& infix) {
    CharStack stk;
    string    output;

    cout << "\n  Infix  : " << infix << "\n";
    cout << "  Step-by-step conversion:\n";
    cout << "  " << left << setw(8) << "Token"
         << setw(20) << "Stack"
         << "Output\n";
    cout << "  " << string(40, '-') << "\n";

    for (char c : infix) {
        if (c == ' ') continue;

        // helper: print current state
        auto printState = [&](const string& token) {
            // build stack string
            string sstr;
            for (int i = 0; i <= stk.top; i++) sstr += stk.data[i];
            cout << "  " << setw(8) << token
                 << setw(20) << sstr
                 << output << "\n";
        };

        if (isOperand(c)) {
            output += c;
            printState(string(1, c));
        } else if (c == '(') {
            stk.push(c);
            printState("(");
        } else if (c == ')') {
            while (!stk.isEmpty() && stk.peek() != '(') {
                output += stk.pop();
            }
            if (!stk.isEmpty()) stk.pop(); // discard '('
            printState(")");
        } else if (isOperator(c)) {
            // ^ is right-associative: pop only if strictly greater
            while (!stk.isEmpty() && stk.peek() != '(' &&
                   ((c != '^' && precedence(stk.peek()) >= precedence(c)) ||
                    (c == '^' && precedence(stk.peek()) >  precedence(c)))) {
                output += stk.pop();
            }
            stk.push(c);
            printState(string(1, c));
        }
    }
    while (!stk.isEmpty()) {
        output += stk.pop();
    }

    cout << "  " << string(40, '-') << "\n";
    cout << "  Postfix: " << output << "\n";
    return output;
}

// ═══════════════════════════════════════════════════════════════
//  Infix → Prefix
//  Method: reverse infix → swap brackets → postfix → reverse
// ═══════════════════════════════════════════════════════════════
string infixToPrefix(const string& infix) {
    // Step 1: reverse the infix string
    string rev = reverseStr(infix);

    // Step 2: swap ( and )
    for (char& c : rev) {
        if (c == '(') c = ')';
        else if (c == ')') c = '(';
    }

    cout << "\n  Infix            : " << infix  << "\n";
    cout << "  Reversed & swapped: " << rev    << "\n";

    // Step 3: convert reversed string to postfix (right-associative ^ fix)
    CharStack stk;
    string    output;
    for (char c : rev) {
        if (c == ' ') continue;
        if (isOperand(c)) {
            output += c;
        } else if (c == '(') {
            stk.push(c);
        } else if (c == ')') {
            while (!stk.isEmpty() && stk.peek() != '(')
                output += stk.pop();
            if (!stk.isEmpty()) stk.pop();
        } else if (isOperator(c)) {
            // for prefix, ^ is left-associative in this reversed pass
            while (!stk.isEmpty() && stk.peek() != '(' &&
                   precedence(stk.peek()) > precedence(c)) {
                output += stk.pop();
            }
            stk.push(c);
        }
    }
    while (!stk.isEmpty()) output += stk.pop();

    // Step 4: reverse the postfix of reversed = prefix
    string prefix = reverseStr(output);
    cout << "  Prefix           : " << prefix  << "\n";
    return prefix;
}

// ═══════════════════════════════════════════════════════════════
//  Postfix Evaluation
//  Operands must be single digits (0-9) for simplicity in demos
// ═══════════════════════════════════════════════════════════════
double evaluatePostfix(const string& postfix) {
    DoubleStack stk;
    cout << "\n  Postfix: " << postfix << "\n";
    cout << "  Step-by-step evaluation:\n";

    for (char c : postfix) {
        if (c == ' ') continue;
        if (isdigit(c)) {
            double val = c - '0';
            stk.push(val);
            cout << "    Push " << val << "\n";
        } else if (isOperator(c)) {
            double b = stk.pop();
            double a = stk.pop();
            double res = applyOp(a, b, c);
            stk.push(res);
            cout << "    " << a << " " << c << " " << b << " = " << res << "  Push " << res << "\n";
        }
    }
    double result = stk.pop();
    cout << "  Result : " << result << "\n";
    return result;
}

// ═══════════════════════════════════════════════════════════════
//  Prefix Evaluation  (right-to-left scan)
// ═══════════════════════════════════════════════════════════════
double evaluatePrefix(const string& prefix) {
    DoubleStack stk;
    cout << "\n  Prefix: " << prefix << "\n";
    cout << "  Step-by-step evaluation (right to left):\n";

    for (int i = (int)prefix.size() - 1; i >= 0; i--) {
        char c = prefix[i];
        if (c == ' ') continue;
        if (isdigit(c)) {
            double val = c - '0';
            stk.push(val);
            cout << "    Push " << val << "\n";
        } else if (isOperator(c)) {
            double a = stk.pop();
            double b = stk.pop();
            double res = applyOp(a, b, c);
            stk.push(res);
            cout << "    " << a << " " << c << " " << b << " = " << res << "  Push " << res << "\n";
        }
    }
    double result = stk.pop();
    cout << "  Result : " << result << "\n";
    return result;
}

// ═══════════════════════════════════════════════════════════════
//  Tower of Hanoi  (recursive)
// ═══════════════════════════════════════════════════════════════
void hanoi(int n, char from, char to, char aux, int& moves) {
    if (n == 0) return;
    hanoi(n - 1, from, aux, to, moves);
    moves++;
    cout << "  Move disk " << setw(2) << n
         << " : " << from << "  -->  " << to << "\n";
    hanoi(n - 1, aux, to, from, moves);
}

// ═══════════════════════════════════════════════════════════════
//  EXPRESSION CALCULATOR MENU
// ═══════════════════════════════════════════════════════════════
void expressionCalculatorMenu() {
    int choice;
    do {
        printLine();
        cout << "   EXPRESSION CALCULATOR & STACK DEMOS\n";
        printLine();
        cout << " 1. Parenthesis Checker\n";
        cout << " 2. Infix  -> Postfix Conversion\n";
        cout << " 3. Infix  -> Prefix  Conversion\n";
        cout << " 4. Evaluate Postfix Expression\n";
        cout << " 5. Evaluate Prefix  Expression\n";
        cout << " 6. Infix  -> Postfix -> Evaluate  (full pipeline)\n";
        cout << " 7. Tower of Hanoi\n";
        cout << " 0. Back\n";
        printLine();
        cout << "Hint: Use single-character operands (a-z, 0-9)\n";
        cout << "      and operators + - * / ^\n";
        printLine('-');
        cout << "Choice: ";
        cin >> choice;

        string expr;
        int n;

        switch (choice) {

        case 1:
            cout << "\nEnter expression to check parentheses: ";
            cin.ignore();
            getline(cin, expr);
            if (checkParentheses(expr))
                cout << "\n  RESULT: Parentheses are BALANCED.\n";
            else
                cout << "\n  RESULT: Parentheses are NOT balanced.\n";
            break;

        case 2:
            cout << "\nEnter infix expression: ";
            cin.ignore();
            getline(cin, expr);
            infixToPostfix(expr);
            break;

        case 3:
            cout << "\nEnter infix expression: ";
            cin.ignore();
            getline(cin, expr);
            infixToPrefix(expr);
            break;

        case 4:
            cout << "\nEnter postfix expression (single-digit operands): ";
            cin.ignore();
            getline(cin, expr);
            evaluatePostfix(expr);
            break;

        case 5:
            cout << "\nEnter prefix expression (single-digit operands): ";
            cin.ignore();
            getline(cin, expr);
            evaluatePrefix(expr);
            break;

        case 6: {
            cout << "\nEnter infix expression (single-digit operands): ";
            cin.ignore();
            getline(cin, expr);
            printLine('-');
            cout << "  Step 1: Check parentheses\n";
            bool balanced = checkParentheses(expr);
            if (!balanced) {
                cout << "  [ABORTED] Expression has unbalanced parentheses.\n";
                break;
            }
            printLine('-');
            cout << "  Step 2: Convert infix to postfix\n";
            string postfix = infixToPostfix(expr);
            printLine('-');
            cout << "  Step 3: Evaluate postfix\n";
            double result = evaluatePostfix(postfix);
            printLine('-');
            cout << "  Final Result: " << expr << " = " << result << "\n";
            break;
        }

        case 7:
            cout << "\nNumber of disks (1-10): ";
            cin >> n;
            if (n < 1 || n > 10) { cout << "[ERROR] Enter 1-10.\n"; break; }
            {
                int moves = 0;
                printLine('-');
                cout << "  Tower of Hanoi — " << n << " disk(s)\n";
                cout << "  Pegs: A (source)  B (auxiliary)  C (destination)\n";
                printLine('-');
                hanoi(n, 'A', 'C', 'B', moves);
                printLine('-');
                cout << "  Total moves    : " << moves << "\n";
                cout << "  Formula        : 2^" << n << " - 1 = " << ((1 << n) - 1) << "\n";
                cout << "  Complexity     : O(2^n)\n";
            }
            break;

        case 0: break;
        default: cout << "[ERROR] Invalid choice.\n";
        }

        if (choice != 0) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }

    } while (choice != 0);
}
