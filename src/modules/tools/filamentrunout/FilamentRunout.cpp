
/*
    Handles a filament runout sensor, that send M701 over serial when printer has no filament 
    only if not suspended. 
	Handy if You want to change filament without triggering the sensor
*/

#include "FilamentRunout.h"
#include "Kernel.h"
#include "StreamOutputPool.h"
#include "StreamOutput.h"
#include "SerialMessage.h"
#include "Gcode.h"
#include "Panel.h"

#include "Config.h"
#include "checksumm.h"
#include "ConfigValue.h"

#define filament_runout_checksum  	CHECKSUM("filament_runout")
#define enable_checksum             CHECKSUM("enable")

FilamentRunout::FilamentRunout()
{
}

FilamentRunout::~FilamentRunout()
{
}

void FilamentRunout::on_module_loaded()
{
	    // if the module is disabled -> do nothing
    if(!THEKERNEL->config->value( filament_runout_checksum, enable_checksum )->by_default(false)->as_bool()) {
        // as this module is not needed free up the resource
        delete this;
        return;
    }
    this->register_for_event(ON_GCODE_RECEIVED);
}


void FilamentRunout::send_command(std::string msg, StreamOutput *stream)
{
    struct SerialMessage message;
    message.message = msg;
    message.stream = stream;
    THEKERNEL->call_event(ON_CONSOLE_LINE_RECEIVED, &message );
}

void FilamentRunout::on_gcode_received(void *argument)
{
	
	if (THEPANEL->is_suspended() == false)	{
	
		Gcode *gcode = static_cast<Gcode *>(argument);
		if (gcode->has_m) {
			if (gcode->m == 701) {
			  this->send_command( "suspend", &(StreamOutput::NullStream) );
			}	 
		}
	}
}




