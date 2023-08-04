#include <bits/stdc++.h>
using namespace std;

#define ln '\n'

vector<string> threeAddCode;

bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c =='^' || c == '%');
}

bool isOperand(char c) {
    return isalnum(c);
}

int getPrecedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    else if (op == '*' || op == '/')
        return 2;
    else if (op == '^' || op == '%') return 3;
    else
        return 0;
}

// Function to convert infix expression to postfix
string infixToPostfix(const string& infix) {
    string postfix;
    stack<char> operatorStack;

    for (char c : infix) {
        if (isOperand(c)) {
            postfix += c;
        } 
        else if (isOperator(c)) {
            while (!operatorStack.empty() && operatorStack.top() != '(' && getPrecedence(c) <= getPrecedence(operatorStack.top())) {
                postfix += operatorStack.top();
                operatorStack.pop();
            }
            operatorStack.push(c);
        } 
        else if (c == '(') {
            operatorStack.push(c);
        } 
        else if (c == ')') {
            while (!operatorStack.empty() && operatorStack.top() != '(') {
                postfix += operatorStack.top();
                operatorStack.pop();
            }
            if (!operatorStack.empty() && operatorStack.top() == '(') {
                operatorStack.pop();
            }
        }
    }

    while (!operatorStack.empty()) {
        postfix += operatorStack.top();
        operatorStack.pop();
    }

    return postfix;
}

void postfixToInfix(const string& postfix) {
    stack<string> operandStack;
    int varno = 0;
    for (char c : postfix) {
        if (isOperand(c)) {
            string operand(1, c);
            operandStack.push(operand);
        } 
        else {
            string operand2 = operandStack.top();
            operandStack.pop();

            string operand1 = operandStack.top();
            operandStack.pop();

            string expression = operand1 + c + operand2;
            string temp = "t" + to_string(varno++);
            threeAddCode.push_back(expression);
            operandStack.push(temp);
        }
    }

    // return operandStack.top();
}


int main() {
    string infixExpression;
    cout << "Enter the expression: ";
    getline(cin, infixExpression);

    string postfixExpression = infixToPostfix(infixExpression);

    postfixToInfix(postfixExpression);

    for (int i=0; i<threeAddCode.size(); i++){
        if (i == threeAddCode.size()-1) cout<<threeAddCode[i]<<ln;
        else cout<<"t"<<i<<" = "<<threeAddCode[i]<<ln;
    }

    return 0;
}
