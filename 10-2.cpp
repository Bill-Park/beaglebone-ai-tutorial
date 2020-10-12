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
    char buffer[10] = "" ;
    int status ;
    int value ;
    int value_pulse ;
    GPIO adc = GPIO(-1) ;
    GPIO servo = GPIO(-1) ;
    
    adc.init_path("/sys/bus/iio/devices/iio:device0/") ;
    
    servo.init_path("/sys/class/pwm/pwm-0:0/") ;
    servo.write_value("20000000", "period") ;
    servo.write_value("1", "enable") ;
    
    while (1) {
        adc.read_value(buffer, 5, "in_voltage0_raw") ;
        value = atoi(buffer) ;
        value_pulse = (600000 + (1700000 * (float)((float)value / 4095))) ;
        cout << "input " << value << "   \r" << endl ;
        cout << "pulse " << value_pulse << "   \r\e[A" << flush ;
        servo.write_value(to_string(value_pulse), "duty_cycle") ;
        usleep(100 * 1000) ;
    }
    
    return 0 ;
}