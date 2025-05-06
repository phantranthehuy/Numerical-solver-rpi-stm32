#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/**
 * Đánh giá độ ưu tiên của toán tử
 * 
 * @param op Toán tử cần đánh giá
 * @return Giá trị độ ưu tiên của toán tử
 */
int Precedence(char op);

/**
 * Áp dụng toán tử lên hai toán hạng
 * 
 * @param a Toán hạng thứ nhất
 * @param b Toán hạng thứ hai
 * @param op Toán tử
 * @return Kết quả thực hiện phép tính
 */
double applyOperator(double a, double b, char op);

/**
 * Tính toán biểu thức số học
 * 
 * @param expr Chuỗi biểu thức cần tính toán
 * @return Kết quả tính toán
 */
double evaluateExpression(const char *expr);

#endif /* CALCULATOR_H */