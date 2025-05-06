#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <pigpio.h>

#include "solve.h"
#include "calculator.h"

int main() {

    // Khởi tạo pigpio
    if (gpioInitialise() < 0) {
        fprintf(stderr, "Khởi tạo pigpio thất bại\n");
        return 1;
    }

    char input[100];
    printf("Nhap bieu thuc: ");
    fgets(input, sizeof(input), stdin);
    
    // Loại bỏ ký tự xuống dòng nếu có
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strchr(input, 'x') != NULL) {
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start); // Bắt đầu đo thời gian

        double result = solve(input);

        clock_gettime(CLOCK_MONOTONIC, &end); // Kết thúc đo thời gian
        double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

        printf("Nghiem la: %f\n", result);
        printf("Thoi gian: %.4f giay\n", elapsed);
    }
    else {
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start); // Bắt đầu đo thời gian

        char postfix[MAX_EXPR] = "";
        toPostfix(input, postfix);
        double result = evaluatePostfix(postfix, 0); // Truyền 0 vì không có biến x

        clock_gettime(CLOCK_MONOTONIC, &end); // Kết thúc đo thời gian
        double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

        printf("Ket qua la: %f\n", result);
        printf("Thoi gian: %.4f giay\n", elapsed);
    }
        
    // Kết thúc pigpio
    gpioTerminate();
    return 0;
}