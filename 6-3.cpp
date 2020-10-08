#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <asm/termbits.h>

using namespace std ;

class GPIO
{
    private :
        string default_path = "/sys/class/gpio/" ;
        string path, export_path, unexport_path ;
        int num ;
        string num_str ;
        int f_temp, status ;
        int gpio_export() ;
        int gpio_unexport() ;
    public :
        GPIO(int init_num) ;
        int read_value(char* buffer, int len, string d_path) ;
        int write_value(string value) ;
        int set_dir(string dir) ;
        void init_path(string user_path) ;
        ~GPIO() ;
} ;

GPIO::GPIO(int init_num) {
    if (init_num >= 0) {
        path = default_path + "gpio" + to_string(init_num) ;
        num = init_num ;
        num_str = to_string(num) ;
        gpio_export() ;    
    }
}

GPIO::~GPIO() {
    gpio_unexport() ;
}

int GPIO::read_value(char* buffer, int len, string d_path = "") {
    string value_path ;
    if (d_path == "") {
        value_path = path + "/value" ;
    } else {
        value_path = path + d_path ;
    }
    f_temp = open(value_path.c_str(), O_RDONLY  | O_CLOEXEC) ;
    if (f_temp < 0) return f_temp ; // 파일 열기
    
    status = read(f_temp, buffer, len) ;
    if (status < 0) return status ;
    
    close(f_temp) ;
    return 0 ;
}

int GPIO::write_value(string value) {
    string value_path = path + "/value" ;
    f_temp = open(value_path.c_str(), O_WRONLY  | O_CLOEXEC) ;
    if (f_temp < 0) return f_temp ; // 파일 열기
    
    status = write(f_temp, value.c_str(), value.length()) ;
    if (status < 0) return status ; // 파일에 값 쓰기
    
    close(f_temp) ; // 파일 닫기
    return 0 ;
}


int GPIO::set_dir(string dir) {
    string dir_path = path + "/direction" ;
    f_temp = open(dir_path.c_str(), O_WRONLY  | O_CLOEXEC) ;
    if (f_temp < 0) return f_temp ; // 파일 열기
    
    status = write(f_temp, dir.c_str(), dir.length()) ;
    if (status < 0) return status ; // 파일에 값 쓰기
    
    close(f_temp) ; // 파일 닫기
    return 0 ;
}

void GPIO::init_path(string user_path) {
    path = user_path ;
}


int GPIO::gpio_export() {
    export_path = default_path + "export" ;
    
    f_temp = open(export_path.c_str(), O_WRONLY  | O_CLOEXEC) ;
    if (f_temp < 0) return f_temp ; // 파일 열기
    
    status = write(f_temp, num_str.c_str(), num_str.length()) ;
    if (status < 0) return status ; // 파일에 값 쓰기
    
    close(f_temp) ; // 파일 닫기
    return 0 ;
}

int GPIO::gpio_unexport() {
    unexport_path = default_path + "unexport" ;
    
    f_temp = open(unexport_path.c_str(), O_WRONLY  | O_CLOEXEC) ;
    if (f_temp < 0) return f_temp ; // 파일 열기
    
    status = write(f_temp, num_str.c_str(), num_str.length()) ;
    if (status < 0) return status ; // 파일에 값 쓰기
    
    close(f_temp) ; // 파일 닫기
    return 0 ;
}

class UART
{
    private :
        struct termios2 uartTerm ;
        string default_path = "/dev/" ;
        string path ;
        int baudrates ;
        int f_uart, status ;
    public :
        UART(int num, int baud) ;
        int write_string(string data) ;
        int read_bytes(char* buffer, int len) ;
        ~UART() ;
} ;

UART::UART(int num, int baud) {
    path = default_path + "ttyS" + to_string(num) ;
    baudrates = baud ;
    
    f_uart = open(path.c_str(), O_RDWR | O_NOCTTY | O_NDELAY) ;
    
    ioctl(f_uart, TCGETS2, &uartTerm) ; // 정보 읽어오기
    
    uartTerm.c_cflag &= ~CBAUD ; // 기존 baud 무시
    uartTerm.c_cflag |= BOTHER ; // baud 커스텀 가능하도록 변경
    
    uartTerm.c_cflag |= CS8 ;    // 8비트
    uartTerm.c_cflag |= CLOCAL ; // 제어신호 없음
    uartTerm.c_cflag |= CREAD ;  // 읽기 쓰기 가능
    
    uartTerm.c_ospeed = baudrates ; // 통신속도 설정
    uartTerm.c_ispeed = baudrates ;
    
    ioctl(f_uart, TCSETS2, &uartTerm) ; // 정보 설정하기
}

int UART::write_string(string data) {
    status = write(f_uart, data.c_str(), data.length()) ;
    return status ;
}

int UART::read_bytes(char* buffer, int len) {
    status = read(f_uart, buffer, len) ;
    return status ;
}

UART::~UART()
{
    close(f_uart) ;
}

int main(int argc, char* [])
{
    int led_num = 76 ;
    char buffer[10] = "" ;
    char buffer2[10] = "" ;
    int status ;
    int value ;
    GPIO led = GPIO(led_num) ;
    GPIO adc = GPIO(-1) ;
    UART uart = UART(2, 9600) ;
    
    // uart.write_string("test") ;
    // sleep(5) ;
    // uart.read_bytes(buffer, 5) ;
    // cout << "read " << buffer << endl ;
    
    adc.init_path("/sys/bus/iio/devices/iio:device0/") ;
    status = led.set_dir("out") ;
    cout << "status " << status << endl ;
    if (status < 0) return -1 ;
    
    while (1) {
        adc.read_value(buffer, 5, "in_voltage0_raw") ;
        uart.write_string(string(buffer) + "\r\n") ;
        
        uart.read_bytes(buffer2, 5) ;
        
        if (buffer2[0] == '1') {
            led.write_value("1") ;
        } else if(buffer2[0] == '0') {
            led.write_value("0") ;
        }
        usleep(500 * 1000) ;
    }
    
    return 0 ;
}