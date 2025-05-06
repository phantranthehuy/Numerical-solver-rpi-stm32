#ifndef SOLVE_H
#define SOLVE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_EXPR 256
#define MAX_STACK 100

/**
 * Tìm nghiệm của phương trình f(x) = 0 bằng phương pháp Newton-Raphson
 * 
 * @param f Chuỗi biểu thức cần giải nghiệm (dạng f(x) = 0)
 * @return Nghiệm của phương trình, hoặc NAN nếu không tìm thấy nghiệm
 */
double solve(char* f);

/**
 * Chuyển biểu thức trung tố sang hậu tố
 * 
 * @param infix Chuỗi biểu thức trung tố đầu vào
 * @param postfix Chuỗi biểu thức hậu tố đầu ra
 */
void toPostfix(const char* infix, char* postfix);

/**
 * Tính giá trị của biểu thức hậu tố với giá trị x cho trước
 * 
 * @param postfix Chuỗi biểu thức hậu tố
 * @param xValue Giá trị của biến x
 * @return Kết quả tính toán hoặc NAN nếu có lỗi
 */
double evaluatePostfix(const char* postfix, double xValue);

/**
 * Tính đạo hàm của biểu thức hậu tố tại điểm x
 * 
 * @param postfix Chuỗi biểu thức hậu tố
 * @param x Điểm cần tính đạo hàm
 * @return Giá trị đạo hàm hoặc NAN nếu có lỗi
 */
double derivativePostfix(const char* postfix, double x);

#endif /* SOLVE_H */