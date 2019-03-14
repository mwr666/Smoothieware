/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/

#include "libs/Kernel.h"
#include "libs/SerialMessage.h"
#include "Panel.h"
#include "PanelScreen.h"
#include "LcdBase.h"
#include "ExtruderScreen.h"
#include "libs/nuts_bolts.h"
#include "libs/utils.h"
#include "ModifyValuesScreen.h"
#include "PublicData.h"
#include "ExtruderPublicAccess.h"
#include "TemperatureControlPublicAccess.h"
#include "TemperatureControlPool.h"
#include "checksumm.h"

#include <string>


using namespace std;

#define extruder_checksum CHECKSUM("extruder")

ExtruderScreen::ExtruderScreen()
{
	hot = false;
	readyhot = false;
}

static struct pad_temperature getTemperatures(uint16_t heater_cs)
{
    struct pad_temperature temp;
    PublicData::get_value( temperature_control_checksum, current_temperature_checksum, heater_cs, &temp );
    return temp;
}

void ExtruderScreen::on_enter()
{
	// enumerate temperature controls
    temp_controllers.clear();
    std::vector<struct pad_temperature> controllers;
    bool ok = PublicData::get_value(temperature_control_checksum, poll_controls_checksum, &controllers);
    if (ok) {
        for (auto &c : controllers) {
            temp_controllers.push_back(c.id);
        }
	}
	deftemp = THEPANEL->get_default_hotend_temp();
    THEPANEL->enter_menu_mode();
	    // if no heaters or extruder then don't show related menu items
    THEPANEL->setup_menu(6);
	
    this->refresh_menu();
}


void ExtruderScreen::on_refresh()
{
//    if ( THEPANEL->menu_change() ) {
        this->refresh_menu();
//    }
    if ( THEPANEL->click() ) {
        this->clicked_menu_entry(THEPANEL->get_menu_current_line());
    }
}

void ExtruderScreen::display_menu_line(uint16_t line)
{
    switch ( line ) {
        case 0: THEPANEL->lcd->printf("Back");  break;
		case 1: THEPANEL->lcd->printf("Suspend"); break;
		case 2: chckHotendTemp(); break;
//        case 2: THEPANEL->lcd->printf("Extrude 5mm"); break;
//        case 3: THEPANEL->lcd->printf("Retract 5mm");  break;	
        case 3: if (readyhot == true) THEPANEL->lcd->printf("Unload"); break;
		case 4: if (readyhot == true) THEPANEL->lcd->printf("Load"); break;
		case 5: if (readyhot == true) THEPANEL->lcd->printf("Purge"); break;
		
//        case 3: THEPANEL->lcd->printf("Settings...");  break;
    }
}

void ExtruderScreen::clicked_menu_entry(uint16_t line)
{
    switch ( line ) {
        case 0: THEPANEL->enter_screen(this->parent); return;
		case 1: send_command("\nsuspend"); break;
		case 2: if (hot == true) send_command("\nM104_S0"); else this->preheat(); hot = true; break; 
//        case 2: send_command("M120\nG91\nG1 E5 F200\nM121"); break;
//        case 3: send_command("M120\nG91\nG1 E-5 F200\nM121"); break;      
        case 3: if (readyhot == true) send_command("M120\nG91\nG1 E-100 F6000\nM121\nG92 E0"); break;
		case 4: if (readyhot == true) send_command("M120\nG91\nG1 E80 F300\nM121\nG92 E0"); break;
		case 5: if (readyhot == true) send_command("M120\nG91\nG1 E10 F100\nM121\nG92 E0"); break;
		
//      case 3: setupConfigSettings(); break; // lazy load
    }
}

void ExtruderScreen::preheat()
{
    PublicData::set_value( temperature_control_checksum, hotend_checksum, &deftemp );
}

void ExtruderScreen::chckHotendTemp()
{
	for(auto id : temp_controllers) {
            struct pad_temperature c= getTemperatures(id);
      //      if(c.current_temperature > 50) is_hot= true; // anything is hot
            if(c.designator.front() == 'T') { // a hotend by convention
				int t= std::min(999, (int)roundf(c.current_temperature)); 
				if (t >= deftemp){
				readyhot=true;
				}
				else{
				readyhot=false;
				}
				int tt= roundf(c.target_temperature);
				if (tt == deftemp){
				THEPANEL->lcd->printf("Cooldown");
				hot=true;
				}
				else{
				THEPANEL->lcd->printf("Preheat");
				hot=false;
				}
				THEPANEL->lcd->setCursor(1, 7);
				if ((hot == true) && (t < deftemp)){
				THEPANEL->lcd->printf(" Heating up:%03d/%03d", t, tt);
				}
				else{
				THEPANEL->lcd->printf("Nozzle temp:%03d/%03d", t, tt);
				}					
			}
	}
}

/*
void ExtruderScreen::setupConfigSettings()
{
    auto mvs= new ModifyValuesScreen(true);  // self delete on exit
    mvs->set_parent(this);

    mvs->addMenuItem("E steps/mm",
        // gets steps/mm for currently active extruder
        []() -> float { pad_extruder_t rd; if(PublicData::get_value( extruder_checksum, (void *)&rd )) return rd.steps_per_mm; else return 0.0F; },
        [this](float v) { send_gcode("M92", 'E', v); },
        0.1F,
        1.0F
        );

    mvs->addMenuItem("Filament diameter",
        // gets filament diameter for currently active extruder
        []() -> float { pad_extruder_t rd; if(PublicData::get_value( extruder_checksum, (void *)&rd )) return rd.filament_diameter; else return 0.0F; },
        [this](float v) { send_gcode("M200", 'D', v); },
        0.01F,
        0.0F,
        4.0F
        );

    // flow rate
    mvs->addMenuItem("Flow rate", // menu name
        []() -> float { pad_extruder_t rd; if(PublicData::get_value( extruder_checksum, (void *)&rd )) return rd.flow_rate*100.0F; else return 100.0F; }, // getter as fraction
        [this](float fr) { send_gcode("M221", 'S', fr); }, // setter in percent
        1.0F, // increment
        1.0F  // Min
        );

    mvs->addMenuItem("Accel", // menu name
        []() -> float { pad_extruder_t rd; if(PublicData::get_value( extruder_checksum, (void *)&rd )) return rd.accleration; else return 0; }, // getter
        [this](float acc) { send_gcode("M204", 'E', acc); }, // setter
        10.0F, // increment
        1.0F   // Min
        );

    mvs->addMenuItem("Retract len", // menu name
        []() -> float { pad_extruder_t rd; if(PublicData::get_value( extruder_checksum, (void *)&rd )) return rd.retract_length; else return 0; }, // getter
        [this](float l) { send_gcode("M207", 'S', l); }, // setter
        0.1F, // increment
        0.0F  // Min
        );

    THEPANEL->enter_screen(mvs);
}
*/