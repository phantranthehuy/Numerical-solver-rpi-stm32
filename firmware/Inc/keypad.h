#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>

// Định nghĩa các chân cho LCD
#define LCD_RS  25
#define LCD_RW  24
#define LCD_E   23
#define LCD_D0  22
#define LCD_D1  21
#define LCD_D2  20
#define LCD_D3  19
#define LCD_D4  18
#define LCD_D5  17
#define LCD_D6  16
#define LCD_D7  13

// Định nghĩa các chân cho bàn phím
#define ROW1    4
#define ROW2    5
#define ROW3    6
#define ROW4    12
#define ROW5    26
#define COL1    7
#define COL2    8
#define COL3    9
#define COL4    10
#define COL5    11

// Khởi tạo bàn phím
void keypadInit(void);

// Đọc bàn phím
char keyread(void);

// Nhận dữ liệu từ bàn phím
char getKeypadInput(void);

// Gửi dữ liệu hoặc lệnh đến LCD
void lcd_write_byte(uint8_t data, uint8_t mode);

// Gửi dữ liệu đến LCD
void lcd_write_data(uint8_t data);

// Gửi lệnh đến LCD
void lcd_write_cmd(uint8_t data);

// In chuỗi và kết quả lên LCD
void lcd_print(char* f, double result);

// Khởi tạo LCD
void lcd_init(void);

#endif /* KEYPAD_H */