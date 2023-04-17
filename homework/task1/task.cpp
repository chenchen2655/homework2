// 基于栈数据结构实现字符串计算器
#include <iostream>
#include "Vector/vector.h"
#include <string>
#include <cctype>
using namespace std;

// 定义栈数据结构
template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(T data, Node* next = nullptr) : data(data), next(next) {}
    };
    Node* top;
    int size;
public:
    Stack() : top(nullptr), size(0) {}
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }
    bool isEmpty() const {
        return top == nullptr;
    }
    int getSize() const {
        return size;
    }
    void push(T data) {
        top = new Node(data, top);
        size++;
    }
    void pop() {
        if (isEmpty()) {
            throw "Stack is empty";
        }
        Node* temp = top;
        top = top->next;
        delete temp;
        size--;
    }
    T getTop() const {
        if (isEmpty()) {
            throw "Stack is empty";
        }
        return top->data;
    }
};

// 定义运算符优先级
int getPriority(char op) {
    switch (op) {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    case '^':
        return 3;
    case '(':
        return 0;
    default:
        throw "Invalid operator";
    }
}

// 定义运算符结合性
bool isLeftAssociative(char op) {
    switch (op) {
    case '+':
    case '-':
    case '*':
    case '/':
        return true;
    case '^':
        return false;
    default:
        throw "Invalid operator";
    }
}

// 定义运算符操作
int operate(int a, int b, char op) {
    switch (op) {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        if (b == 0) {
            throw "Division by zero";
        }
        return a / b;
    case '^':
        return pow(a, b);
    default:
        throw "Invalid operator";
    }
}

// 定义字符串计算器函数
int calculate(string s) {
    // 去除空格
    s.erase(remove(s.begin(), s.end(), ' '), s.end());

    // 定义两个栈，一个用于存储操作数，一个用于存储运算符
    Stack<int> operands;
    Stack<char> operators;

    // 遍历输入字符串
    for (int i = 0; i < s.length(); i++) {

        // 如果是数字，就把它转换成整数并压入操作数栈
        if (isdigit(s[i])) {
            int num = 0;
            while (i < s.length() && isdigit(s[i])) {
                num = num * 10 + (s[i] - '0');
                i++;
            }
            i--;
            operands.push(num);

            // 如果是左括号，就压入运算符栈
        }
        else if (s[i] == '(') {
            operators.push(s[i]);

            // 如果是右括号，就弹出运算符栈直到遇到左括号，并计算结果
        }
        else if (s[i] == ')') {
            while (!operators.isEmpty() && operators.getTop() != '(') {
                // 弹出两个操作数和一个运算符
                int b = operands.getTop();
                operands.pop();
                int a = operands.getTop();
                operands.pop();
                char op = operators.getTop();
                operators.pop();
                // 计算结果并压入操作数栈
                int result = operate(a, b, op);
                operands.push(result);
            }
            // 如果运算符栈为空或者没有遇到左括号，说明输入的表达式无效
            if (operators.isEmpty() || operators.getTop() != '(') {
                throw "Invalid expression";
            }
            // 弹出左括号
            operators.pop();

            // 如果是运算符，就比较它和运算符栈顶的优先级和结合性
        }
        else if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == '^') {
            while (!operators.isEmpty() &&
                (getPriority(s[i]) < getPriority(operators.getTop()) ||
                    (getPriority(s[i]) == getPriority(operators.getTop()) && isLeftAssociative(s[i])))) {
                // 弹出两个操作数和一个运算符
                int b = operands.getTop();
                operands.pop();
                int a = operands.getTop();
                operands.pop();
                char op = operators.getTop();
                operators.pop();
                // 计算结果并压入操作数栈
                int result = operate(a, b, op);
                operands.push(result);
            }
            // 压入当前运算符
            operators.push(s[i]);

            // 如果是其他字符，说明输入的表达式无效
        }
        else {
            throw "Invalid expression";
        }
    }

    // 遍历完输入字符串后，如果运算符栈不为空，就继续计算结果
    while (!operators.isEmpty()) {
        // 弹出两个操作数和一个运算符
        int b = operands.getTop();
        operands.pop();
        int a = operands.getTop();
        operands.pop();
        char op = operators.getTop();
        operators.pop();
        // 计算结果并压入操作数栈
        int result = operate(a, b, op);
        operands.push(result);
    }

    // 如果操作数栈只有一个元素，那么它就是最终的结果，否则说明输入的表达式无效
    if (operands.getSize() == 1) {
        return operands.getTop();
    }
    else {
        throw "Invalid expression";
    }
}

// 主函数，用于测试字符串计算器函数
int main() {
    string s;
    cout << "请输入一个表达式：" << endl;
    getline(cin, s);
    try {
        int result = calculate(s);
        cout << "计算结果是：" << result << endl;
    }
    catch (const char* msg) {
        cout << msg << endl;
    }
    return 0;
}
