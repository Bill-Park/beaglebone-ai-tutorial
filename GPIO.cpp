#include "GPIO.h"
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <sys/ioctl.h>

GPIO::GPIO(int init_num) {
    path = default_path + "gpio" + to_string(init_num) ;
    num = init_num ;
    num_str = to_string(num) ;
    gpio_export() ;
}

GPIO::~GPIO() {
    gpio_unexport() ;
}

int GPIO::read_value(char* buffer, int len) {
    string value_path = path + "/value" ;
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

