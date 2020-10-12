#include "GPIO.h"
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>

#include <fcntl.h>
#include <sys/ioctl.h>

using namespace std ;


int main(int argc, char* [])
{
    int led_num = 76 ;
    char buffer[10] = "" ;
    int status ;
    int value ;
    int value_pulse ;
    GPIO led = GPIO(led_num) ;
    GPIO adc = GPIO(-1) ;
    
    adc.init_path("/sys/bus/iio/devices/iio:device0/") ;
    status = led.set_dir("out") ;
    cout << "status " << status << endl ;
    if (status < 0) return -1 ;
    
    while (1) {
        adc.read_value(buffer, 5, "in_voltage0_raw") ;
        value = atoi(buffer) ;
        value_pulse = (600000 + (1700000 * (float)((float)value / 4095))) ;
        cout << "input " << value << "   \r" << endl ;
        cout << "pulse " << value_pulse << "   \r\e[A" << flush ;
        usleep(100 * 1000) ;
    }
    
    return 0 ;
}