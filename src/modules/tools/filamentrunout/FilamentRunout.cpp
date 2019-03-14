
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
#include "utils.h"
#include "Gcode.h"
#include "Panel.h"


FilamentRunout::FilamentRunout()
{
}

FilamentRunout::~FilamentRunout()
{
}

void FilamentRunout::on_module_loaded()
{
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




