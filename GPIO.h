#ifndef GPIO_H_
#define GPIO_H_
#include <string>

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
        int read_value(char* buffer, int len) ;
        int write_value(string value) ;
        int set_dir(string dir) ;
        ~GPIO() ;
} ;

#endif /* GPIO_H_ */