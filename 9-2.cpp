#include "GPIO.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <chrono>

using namespace std ;
using namespace std::chrono ;

int main(int argc, char* [])
{
    int led_num = 76 ;
    int btn_num = 74 ;
    char buffer[10] ;
    int status ;
    
    system_clock::time_point start, end, timeout ;
    microseconds micro ;
    
    GPIO led = GPIO(led_num) ;
    GPIO echo = GPIO(74) ;
    GPIO trig = GPIO(75) ;
    
    status = trig.set_dir("out") ;
    cout << "trig out " << status << endl ;
    if (status < 0) return -1 ;
    status = echo.set_dir("in") ;
    cout << "echo in " << status << endl ;
    if (status < 0) return -1 ;
    
    trig.write_value("1") ;
    usleep(10) ;
    trig.write_value("0") ;
    
    timeout = system_clock::now() ; // 최대 대기 시간을 위한 값
    while (1) { // HIGH까지 대기
        echo.read_value(buffer, 1) ;
        if (buffer[0] == '1') {
            start = system_clock::now() ;
            break ;
        } else {
            micro = duration_cast<microseconds>(system_clock::now() - timeout) ;
            if (micro.count() > 1000 * 1000) {
                cout << "HIGH timeout" << micro.count() << endl ;
                return -1 ;
            }
        }
    }
    cout << "LOW " << endl ;
    
    timeout = system_clock::now() ; // 최대 대기 시간을 위한 값
    while (1) { // LOW까지 대기
        echo.read_value(buffer, 1) ;
        if (buffer[0] == '0') {
            end = system_clock::now() ;
            break ;
        } else {
            micro = duration_cast<microseconds>(system_clock::now() - timeout) ;
            if (micro.count() > 1000 * 1000) {
                cout << "LOW timeout" << endl ;
                return -1 ;
            }
        }
    }
    
    micro = duration_cast<microseconds>(end - start) ;
    
    cout << "length " << micro.count() / 58 << " cm" << endl ;
    
    
    return 0 ;
}