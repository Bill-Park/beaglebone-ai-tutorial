#include "GPIO.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <fcntl.h>
#include <sys/ioctl.h>

using namespace std ;


int main(int argc, char* [])
{
    int led_num = 76 ;
    int btn_num = 74 ;
    char buffer[10] ;
    int status ;
    GPIO led = GPIO(led_num) ;
    
    led.set_dir("out") ;
    for (int i = 0; i < 5; i++) {
        led.write_value("1") ;
        usleep(500 * 1000) ;
        led.write_value("0") ;
        usleep(500 * 1000) ;
    }
    
    return 0 ;
}