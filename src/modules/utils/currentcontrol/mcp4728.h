#ifndef MCP4728_H
#define MCP4728_H

#include "libs/Kernel.h"
#include "I2C.h" // mbed.h lib
#include "libs/utils.h"
#include "DigipotBase.h"
#include <string>
#include <math.h>

class MCP4728 : public DigipotBase {
    public:
        MCP4728(uint8_t mcp4728_dac_address, uint8_t mcp4726_dac_address)
        :i2c(new mbed::I2C(P0_27, P0_28)), currents{0.0}, adress_MCP4728(mcp4728_dac_address), adress_MCP4726(mcp4726_dac_address)
        {
            this->i2c->frequency(10000);
        }

        ~MCP4728(){
            delete this->i2c;
        }

        void set_current( int channel, float current )
        {
            current = min( (float) max( current, 0.0f ), this->max_current );
            currents[channel] = current;
			float voltage=current_to_voltage(current);
			if(channel<4){
				set_mcp4728(channel,voltage);
			}
			if(channel==4){
				set_mcp4726(voltage);
			}			
        }
		
        float get_current(int channel)
        {
            return currents[channel];
        }
		
    private:
		void set_mcp4728(uint8_t channel, float value)
		{
			uint8_t channels[4]={0x06,0x04,0x02,0x00};
			char upper=(uint16_t)(value*2000.0)/256;
			char lower =(uint16_t)(value*2000.0)%256;
	
			this->i2c->start();
			this->i2c->write(adress_MCP4728);
			this->i2c->write(0x40 | channels[channel]);
			this->i2c->write(0x80+upper);
			this->i2c->write(lower);
			this->i2c->stop();
		} 
		
		void set_mcp4726(float value){
			uint16_t val=(uint16_t)((value/(3.3/(float)4096)));
			char upper=(val)/256;
			char lower =(val)%256;
			this->i2c->start();
			this->i2c->write(adress_MCP4726);
			this->i2c->write(upper);
			this->i2c->write(lower);
			this->i2c->stop();
		}
		
        float current_to_voltage( float current ){
            return 8.0*0.05*current;
        }

        mbed::I2C* i2c;
        float currents[8];
		uint8_t adress_MCP4728;
		uint8_t adress_MCP4726;
};


#endif