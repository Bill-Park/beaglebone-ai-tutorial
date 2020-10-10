#include "I2CDevice.h"
#include <unistd.h>
#include <iostream>

#define I2C_BUS        "/dev/i2c-3" // I2C bus device  
#define I2C_ADDR       0x27         // I2C slave address for the LCD module  

using namespace std ;
using namespace exploringBB ;
    
class LCD : public I2CDevice {
    private :
    	unsigned int I2CBus, I2CAddress ;
    	int status ;
    	
    public :
    	LCD(int I2CBus, int I2CAddress) ;
    	int writeHex(char value) ;
    	~LCD() ;
};

LCD::LCD(int I2CBus, int I2CAddress = 0x27) : I2CDevice(I2CBus, I2CAddress) {
	this->I2CAddress = I2CAddress ;
	this->I2CBus = I2CBus ;
}

int LCD::writeHex(char value) {
    status = this->write(value) ;
    usleep(1000) ;
    if (status != 1) {
        return 1 ;
    }
    return status ;
}

LCD::~LCD() {
}

int main(int argc, char* [])
{
    int led_num = 76 ;
    char buffer[10] = "" ;
    char buffer2[10] = "" ;
    int status ;
    int value ;
    LCD lcd = LCD(3, I2C_ADDR) ;
    
    usleep(15000) ; // 전원 공급 대기시간
    lcd.writeHex(0x34) ; // D7=0, D6=0, D5=1, D4=1, RS,RW=0 EN=1  
    lcd.writeHex(0x30) ; // D7=0, D6=0, D5=1, D4=1, RS,RW=0 EN=0  
    usleep(4100) ;
    lcd.writeHex(0x34) ;
    lcd.writeHex(0x30) ;
    usleep(100) ;
    lcd.writeHex(0x34) ;
    lcd.writeHex(0x30) ;
    usleep(4100) ;
    lcd.writeHex(0x24) ; // 4-bit mode
    lcd.writeHex(0x20) ;
  
   /* -------------------------------------------------------------------- * 
    * 4-bit mode initialization complete. Now configuring the function set * 
    * -------------------------------------------------------------------- */  
    usleep(40);          // wait 40usec
    lcd.writeHex(0x24) ; // keep 4-bit mode
    lcd.writeHex(0x20) ;
    lcd.writeHex(0x84) ;
    lcd.writeHex(0x80) ; // D3=2lines, D2=char5x8, function set
  
   /* -------------------------------------------------------------------- * 
    * Next turn display off                                                * 
    * -------------------------------------------------------------------- */  
    usleep(40);                // wait 40usec  
    lcd.writeHex(0x04) ;
    lcd.writeHex(0x00) ; // D7-D4=0  
    lcd.writeHex(0x84) ;
    lcd.writeHex(0x80) ; // D3=1 D2=display_off, D1=cursor_off, D0=cursor_blink  
  
   /* -------------------------------------------------------------------- * 
    * Display clear, cursor home                                           * 
    * -------------------------------------------------------------------- */  
    usleep(40);                // wait 40usec  
    lcd.writeHex(0x04) ;
    lcd.writeHex(0x00) ; // D7-D4=0
    lcd.writeHex(0x14) ;
    lcd.writeHex(0x80) ; // D0=display_clear
    
    /* -------------------------------------------------------------------- * 
    * Set cursor direction                                                 * 
    * -------------------------------------------------------------------- */  
    usleep(40);                // wait 40usec  
    lcd.writeHex(0x04) ;
    lcd.writeHex(0x00) ; // D7-D4=0  
    lcd.writeHex(0x64) ;
    lcd.writeHex(0x60) ; // print left to right  
  
   /* -------------------------------------------------------------------- * 
    * Turn on the display                                                  * 
    * -------------------------------------------------------------------- */  
    usleep(40);                // wait 40usec  
    lcd.writeHex(0x04) ;
    lcd.writeHex(0x00) ; // D7-D4=0
    lcd.writeHex(0xE4) ;
    lcd.writeHex(0xE0) ; // D3=1 D2=display_on, D1=cursor_on, D0=cursor_blink
    
    sleep(1) ;
    
    cout << "display on" << endl ;
    
    lcd.writeHex(0x4D) ;
    lcd.writeHex(0x49) ; // send 0100=4  
    lcd.writeHex(0x8D) ; 
    lcd.writeHex(0x89) ; // send 1000=8 = 0x48 ='H'
    
    lcd.writeHex(0x4D) ;
    lcd.writeHex(0x49) ; // send 0100=4  
    lcd.writeHex(0x5D) ;
    lcd.writeHex(0x59) ; // send 0101=1 = 0x41 ='E'  
    
    lcd.writeHex(0x4D) ;
    lcd.writeHex(0x49) ; // send 0100=4
    lcd.writeHex(0xCD) ;
    lcd.writeHex(0xC9) ; // send 1100=12 = 0x4D ='L'
   
    lcd.writeHex(0x4D) ;
    lcd.writeHex(0x49) ; // send 0100=4
    lcd.writeHex(0xCD) ;
    lcd.writeHex(0xC9) ; // send 1100=12 = 0x4D ='L'  
   
    lcd.writeHex(0x4D) ;
    lcd.writeHex(0x49) ; // send 0100=4  
    lcd.writeHex(0xFD) ;
    lcd.writeHex(0xF9) ; // send 1111=15 = 0x4F ='O'
    
    // 공백
    lcd.writeHex(0x1D) ;
    lcd.writeHex(0x19) ; // 0001
    lcd.writeHex(0x8D) ; 
    lcd.writeHex(0x89) ; // 1000
    
    // X
    // 0101 1101
    // 0101 1001
    // 1000 1101
    // 1000 1001
    lcd.writeHex(0x5D) ;
    lcd.writeHex(0x59) ; // 0101 
    lcd.writeHex(0x8D) ; 
    lcd.writeHex(0x89) ; // 1000

    
    usleep(1000) ;
    lcd.close() ;
    
    return 0 ;
}

