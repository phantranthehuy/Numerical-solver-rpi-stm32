#include "keypad.h"
#include "calculator.h"
#include "solve.h"

// Định nghĩa bàn phím
char keypad[5][5] = {
    {'1', '2', '3', '^', 'S'},      
    {'4', '5', '6', '/', ' '},      
    {'7', '8', '9', '*', ' '},
    {'.', '0', 'x', '-', ' '},
    {'(', ')', 'C', '+', '='}
};

// Mảng các chân hàng
int rowPins[5] = {ROW1, ROW2, ROW3, ROW4, ROW5};
// Mảng các chân cột
int colPins[5] = {COL1, COL2, COL3, COL4, COL5};

/*--------------------------------------------------KEYPAD INPUT FUNCTIONS--------------------------------------------------- */
// Khởi tạo bàn phím
void keypadInit() {
    // Thiết lập chân cột là output
    for (int i = 0; i < 5; i++) {
        gpioSetMode(colPins[i], PI_OUTPUT);
        gpioWrite(colPins[i], 1);
    }
    
    // Thiết lập chân hàng là input với điện trở kéo lên
    for (int i = 0; i < 5; i++) {
        gpioSetMode(rowPins[i], PI_INPUT);
        gpioSetPullUpDown(rowPins[i], PI_PUD_UP);
    }
}

// Đọc bàn phím
char keyread() {
    // Quét bàn phím ma trận
    for (int col = 0; col < 5; col++) {
        // Đặt cột hiện tại thành LOW
        gpioWrite(colPins[col], 0);
        gpioDelay(5);  // Delay nhỏ để ổn định (5 microseconds)
        
        // Kiểm tra từng hàng
        for (int row = 0; row < 5; row++) {
            if (gpioRead(rowPins[row]) == 0) {
                // Đợi nhả phím (chống dội đơn giản)
                gpioDelay(20);
                if (gpioRead(rowPins[row]) == 0) {
                    while (gpioRead(rowPins[row]) == 0);
                    // Đặt lại cột
                    gpioWrite(colPins[col], 1);
                    return keypad[row][col];
                }
            }
        }
        
        // Đặt lại cột thành HIGH
        gpioWrite(colPins[col], 1);
    }
    return 0; // Không có phím nào được nhấn
}

// Nhận dữ liệu từ bàn phím
char getKeypadInput() {
    char key = keyread();
    if (key != 0) {
        return key;
    }
    return 0; // Không có phím nào được nhấn
}

/*--------------------------------------------------LCD FUNCTIONS--------------------------------------------------- */
// Gửi dữ liệu hoặc lệnh đến LCD
void lcd_write_byte(uint8_t data, uint8_t mode) {
    // mode = 0: lệnh, mode = 1: dữ liệu
    gpioWrite(LCD_RS, mode);
    gpioWrite(LCD_RW, 0); // Write mode
    
    // Gửi dữ liệu đến các chân dữ liệu
    gpioWrite(LCD_D0, data & 0x01);
    gpioWrite(LCD_D1, (data >> 1) & 0x01);
    gpioWrite(LCD_D2, (data >> 2) & 0x01);
    gpioWrite(LCD_D3, (data >> 3) & 0x01);
    gpioWrite(LCD_D4, (data >> 4) & 0x01);
    gpioWrite(LCD_D5, (data >> 5) & 0x01);
    gpioWrite(LCD_D6, (data >> 6) & 0x01);
    gpioWrite(LCD_D7, (data >> 7) & 0x01);
    
    // Tạo xung enable
    gpioWrite(LCD_E, 0);
    gpioDelay(1);
    gpioWrite(LCD_E, 1);
    gpioDelay(1);
    gpioWrite(LCD_E, 0);
    gpioDelay(1);
}

// Gửi dữ liệu đến LCD
void lcd_write_data(uint8_t data) {
    lcd_write_byte(data, 1);
}

// Gửi lệnh đến LCD
void lcd_write_cmd(uint8_t data) {
    lcd_write_byte(data, 0);
}

// In chuỗi và kết quả lên LCD
void lcd_print(char* f, double result) {
    char result_str[16];
    sprintf(result_str, "%.6g", result); // Chuyển đổi kết quả thành chuỗi với độ chính xác phù hợp
    
    // In biểu thức lên dòng đầu tiên
    lcd_write_cmd(0x80); // Đặt con trỏ vào dòng đầu tiên
    gpioDelay(1);
    for (int i = 0; i < strlen(f) && i < 16; i++) {
        lcd_write_data(f[i]);
        gpioDelay(1);
    }
    
    // In kết quả lên dòng thứ hai
    lcd_write_cmd(0xC0); // Đặt con trỏ vào dòng thứ hai
    gpioDelay(1);
    for (int i = 0; i < strlen(result_str) && i < 16; i++) {
        lcd_write_data(result_str[i]);
        gpioDelay(1);
    }
}

// Khởi tạo LCD
void lcd_init(void) {
    // Thiết lập các chân LCD là output
    gpioSetMode(LCD_RS, PI_OUTPUT);
    gpioSetMode(LCD_RW, PI_OUTPUT);
    gpioSetMode(LCD_E, PI_OUTPUT);
    gpioSetMode(LCD_D0, PI_OUTPUT);
    gpioSetMode