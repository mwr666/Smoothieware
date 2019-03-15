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
    THEPANEL->setup_menu(5);
	
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
		case 1: if (THEPANEL->is_suspended())chckHotendTemp(); else THEPANEL->lcd->printf("Suspend to begin"); break;
		//case 2: if (THEPANEL->is_suspended())chckHotendTemp(); break;
        case 2: if (readyhot == true) THEPANEL->lcd->printf("Unload"); break;
		case 3: if (readyhot == true) THEPANEL->lcd->printf("Load"); break;
		case 4: if (readyhot == true) THEPANEL->lcd->printf("Purge"); break;
		
    }
}

void ExtruderScreen::clicked_menu_entry(uint16_t line)
{
    switch ( line ) {
        case 0: THEPANEL->enter_screen(this->parent); return;
		//case 1: send_command("\nsuspend"); break;
		case 1: if (THEPANEL->is_suspended())	{
					if (hot == true) send_command("\nM104_S0"); 
					else this->preheat(); hot = true;
				} else send_command("\nsuspend");
				break;   
        case 2: if (readyhot == true) send_command("M120\nG91\nG1 E-100 F6000\nM121\nG92 E0"); break;
		case 3: if (readyhot == true) send_command("M120\nG91\nG1 E80 F300\nM121\nG92 E0"); break;
		case 4: if (readyhot == true) send_command("M120\nG91\nG1 E10 F100\nM121\nG92 E0"); break;
		
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
