#include "solve.h"

// ==== Stack cho toán tử ====
typedef struct {
    char data[MAX_STACK];
    int top;
} CharStack;

void initCharStack(CharStack *s) { s->top = -1; }
int isEmptyChar(CharStack *s) { return s->top == -1; }
char peekChar(CharStack *s) { return s->data[s->top]; }
void pushChar(CharStack *s, char c) { s->data[++s->top] = c; }
char popChar(CharStack *s) { return s->data[s->top--]; }

// ==== Stack cho số thực ====
typedef struct {
    double data[MAX_STACK];
    int top;
} DoubleStack;

void initDoubleStack(DoubleStack *s) { s->top = -1; }
int isEmptyDouble(DoubleStack *s) { return s->top == -1; }
void pushDouble(DoubleStack *s, double val) { s->data[++s->top] = val; }
double popDouble(DoubleStack *s) { return s->data[s->top--]; }

// ==== Hàm hỗ trợ ====
int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '=';
}

int precedence(char op) {
    switch(op) {
        case '=': return 0;
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^': return 3;
        default: return -1;
    }
}

int isRightAssociative(char op) {
    return op == '^';
}

void toPostfix(const char* infix, char* postfix) {
    CharStack opStack;
    initCharStack(&opStack);
    char token[MAX_EXPR];
    int i = 0, j = 0, k;

    while (infix[i] != '\0') {
        if (isspace(infix[i])) {
            i++;
            continue;
        }

        // === Nếu là số hoặc biến ===
        if (isdigit(infix[i]) || infix[i] == '.' || infix[i] == 'x') {
            k = 0;
            while (isdigit(infix[i]) || infix[i] == '.' || infix[i] == 'x') {
                token[k++] = infix[i++];
            }
            token[k] = '\0';
            strcat(postfix, token);
            strcat(postfix, " ");
        }
        // === Nếu là dấu mở ngoặc ===
        else if (infix[i] == '(') {
            pushChar(&opStack, infix[i]);
            i++;
        }
        // === Nếu là dấu đóng ngoặc ===
        else if (infix[i] == ')') {
            while (!isEmptyChar(&opStack) && peekChar(&opStack) != '(') {
                char op[2] = {popChar(&opStack), '\0'};
                strcat(postfix, op);
                strcat(postfix, " ");
            }
            if (!isEmptyChar(&opStack) && peekChar(&opStack) == '(') {
                popChar(&opStack); // bỏ '('
            }
            i++;
        }
        // === Nếu là toán tử ===
        else if (isOperator(infix[i])) {
            char currOp = infix[i];
            while (!isEmptyChar(&opStack) && isOperator(peekChar(&opStack))) {
                char topOp = peekChar(&opStack);
                if ((isRightAssociative(currOp) && precedence(currOp) < precedence(topOp)) ||
                    (!isRightAssociative(currOp) && precedence(currOp) <= precedence(topOp))) {
                    char op[2] = {popChar(&opStack), '\0'};
                    strcat(postfix, op);
                    strcat(postfix, " ");
                } else {
                    break;
                }
            }
            pushChar(&opStack, currOp);
            i++;
        }
        else {
            // Ký tự không hợp lệ
            printf("Lỗi: Ký tự không hợp lệ '%c'\n", infix[i]);
            return;
        }
    }

    // Đưa toán tử còn lại ra hậu tố
    while (!isEmptyChar(&opStack)) {
        char op = popChar(&opStack);
        if (op == '(' || op == ')') {
            printf("Lỗi: ngoặc không khớp\n");
            return;
        }
        char tmp[2] = {op, '\0'};
        strcat(postfix, tmp);
        strcat(postfix, " ");
    }
}

double evaluatePostfix(const char* postfix, double xValue) {
    DoubleStack valStack;
    initDoubleStack(&valStack);

    char token[64];
    int i = 0, j;

    while (postfix[i] != '\0') {
        if (isspace(postfix[i])) {
            i++;
            continue;
        }

        // === Đọc token (số, biến, hoặc toán tử) ===
        j = 0;
        while (postfix[i] != '\0' && !isspace(postfix[i])) {
            token[j++] = postfix[i++];
        }
        token[j] = '\0';

        // === Nếu là số ===
        if (isdigit(token[0]) || (token[0] == '.' && isdigit(token[1]))) {
            pushDouble(&valStack, atof(token));
        }
        // === Nếu là biến x ===
        else if (strcmp(token, "x") == 0) {
            pushDouble(&valStack, xValue);
        }
        // === Nếu là toán tử ===
        else if (strlen(token) == 1 && strchr("+-*/^=", token[0])) {
            if (valStack.top < 1) {
                printf("Lỗi: thiếu toán hạng\n");
                return NAN;
            }
            double b = popDouble(&valStack);
            double a = popDouble(&valStack);
            double result;

            switch (token[0]) {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/': 
                    if (b == 0) {
                        printf("Lỗi: chia cho 0\n");
                        return NAN;
                    }
                    result = a / b; 
                    break;
                case '^': result = pow(a, b); break;
                case '=': result = (fabs(a - b) < 1e-6) ? 1.0 : 0.0; break;
                default: 
                    printf("Lỗi: toán tử không hợp lệ '%s'\n", token);
                    return NAN;
            }
            pushDouble(&valStack, result);
        }
        else {
            printf("Lỗi: token không hợp lệ '%s'\n", token);
            return NAN;
        }
    }

    if (valStack.top != 0) {
        printf("Lỗi: biểu thức không hợp lệ (dư toán hạng?)\n");
        return NAN;
    }

    return popDouble(&valStack);
}

double derivativePostfix(const char* postfix, double x) {
    const double h = 1e-5;  // độ lệch nhỏ
    double fxh = evaluatePostfix(postfix, x + h);
    double fxmh = evaluatePostfix(postfix, x - h);
    if (isnan(fxh) || isnan(fxmh)) {
        printf("Lỗi: không thể tính đạo hàm tại x = %.5f\n", x);
        return NAN;
    }
    return (fxh - fxmh) / (2 * h);
}

int isNegate(double a, double b) {
    if (a <= 0 && b >= 0) return 1;
    if (a >= 0 && b <= 0) return 1;
    return 0;
}

int find_bracket(char* postfix, double* a, double* b) {   
    double step = 1;
    double x1 = 0, x2 = x1 + step, x3 = x1 - step, x4 = 0;

    while (x3 >= -1e5) {
        double f3 = evaluatePostfix(postfix, x3);
        double f4 = evaluatePostfix(postfix, x4);

        if (isNegate(f3, f4)) {
            *a = x3;
            *b = x4;
            return 1;
        }
        x4 = x3;
        x3 -= step;
    }
    
    while (x2 <= 1e5) {
        double f1 = evaluatePostfix(postfix, x1);
        double f2 = evaluatePostfix(postfix, x2);
        
        if (isNegate(f1, f2)) {
            *a = x1;
            *b = x2;
            return 1; // Tìm thấy khoảng
        }

        x1 = x2;
        x2 += step;
    }
    return 0;
}

double solve(char* f) {
    char postfix[512] = "";
    double tol = 1e-10;
    int max_iter = 1000;
    double x0 = 1;

    toPostfix(f, postfix);
    double f_der_x = derivativePostfix(postfix, x0);
    if (fabs(f_der_x) < 1e-10) { 
        // Tránh chia cho 0
        double a, b;
        if (find_bracket(postfix, &a, &b)) {
            x0 = (a + b)/2;
        } 
        else return NAN;
    }
    
    for (int iter = 0; iter < max_iter; iter++) {
        double f_x = evaluatePostfix(postfix, x0);
        double f_der_x = derivativePostfix(postfix, x0);
        
        if (fabs(f_der_x) < 1e-10) { 
            // Tránh chia cho 0
            return NAN;
        }
        
        double x1 = x0 - f_x / f_der_x;
        
        if (fabs(x1 - x0) < tol) return x1;        
        x0 = x1;
    }
    return NAN;
}