#pragma once

#include "Module.h"
#include "Panel.h"

#include <stdint.h>
#include <string>


class StreamOutput;

class FilamentRunout: public Module
{
public:
    FilamentRunout();
    ~FilamentRunout();
    void on_module_loaded();
    void on_gcode_received(void *argument);

private:
    void send_command(std::string msg, StreamOutput *stream);

};
