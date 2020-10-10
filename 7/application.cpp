#include <iostream>
#include "ADXL345.h"
#include <unistd.h>
#include <pthread.h>

using namespace std ;
using namespace exploringBB ;

int main() 
{
	float roll, pitch, yaw ;
	float x, y, z ;
	ADXL345 sensor(3, 0x53);
	sensor.setResolution(ADXL345::NORMAL);
	sensor.setRange(ADXL345::PLUSMINUS_4_G);
    // sensor.displayPitchAndRoll();
    
    while(1) {
	    sensor.readSensorState() ;
	    
	    roll = sensor.getRoll() ;
	    pitch = sensor.getPitch() ;
	    yaw = sensor.getYaw() ;
	    x = sensor.getAccelerationX() ;
	    y = sensor.getAccelerationY() ;
	    z = sensor.getAccelerationZ() ;

	    cout << "Roll " << roll << " Pitch " << pitch << " Yaw " << yaw << "    \r" << endl ;
	    cout << "X " << x << " Y " << y << " Z " << z << "    \r\e[A" << flush ;
	    usleep(300 * 1000) ;
    }
	
	return 0 ;
}
