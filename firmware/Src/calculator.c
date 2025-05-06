#include "calculator.h"

int Precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

double applyOperator(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return pow(a, b);
        default: return 0;
    }
}

double evaluateExpression(const char *expr) {
    int len = strlen(expr);
    double values[len];  // Stack chứa toán hạng
    char ops[len];       // Stack chứa toán tử
    int valTop = -1, opTop = -1;
    
    int i = 0;
    while (i < len) {
        // Bỏ qua khoảng trắng
        if (expr[i] == ' ') {
            i++;
            continue;
        }
        
        // Xử lý số (bao gồm số âm)
        if (isdigit(expr[i]) || (expr[i] == '-' && (i == 0 || expr[i-1] == '(' || 
            expr[i-1] == '+' || expr[i-1] == '-' || expr[i-1] == '*' || expr[i-1] == '/' || expr[i-1] == '^'))) {
            double num = 0;
            int decimal = 0, divisor = 1;
            int negative = (expr[i] == '-') ? -1 : 1;
            if (expr[i] == '-') i++;

            while (i < len && (isdigit(expr[i]) || expr[i] == '.')) {
                if (expr[i] == '.') decimal = 1;
                else {
                    if (decimal) {
                        divisor *= 10;
                        num += (expr[i] - '0') / (double)divisor;
                    } else num = num * 10 + (expr[i] - '0');
                }
                i++;
            }
            values[++valTop] = num * negative;
            continue;
        }
        
        // Xử lý dấu mở ngoặc
        if (expr[i] == '(') {
            ops[++opTop] = expr[i];
        }
        // Xử lý dấu đóng ngoặc
        else if (expr[i] == ')') {
            while (opTop >= 0 && ops[opTop] != '(') {
                double b = values[valTop--];
                double a = values[valTop--];
                values[++valTop] = applyOperator(a, b, ops[opTop--]);
            }
            opTop--; // Loại bỏ dấu '(' khỏi stack
        }
        // Xử lý toán tử
        else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/' || expr[i] == '^') {
            while (opTop >= 0 && Precedence(ops[opTop]) >= Precedence(expr[i])) {
                double b = values[valTop--];
                double a = values[valTop--];
                values[++valTop] = applyOperator(a, b, ops[opTop--]);
            }
            ops[++opTop] = expr[i];
        }
        i++;
    }
    
    // Xử lý các toán tử còn lại
    while (opTop >= 0) {
        double b = values[valTop--];
        double a = values[valTop--];
        values[++valTop] = applyOperator(a, b, ops[opTop--]);
    }
    
    return values[valTop];
}