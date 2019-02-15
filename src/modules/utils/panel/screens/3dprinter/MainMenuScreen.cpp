/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/

#include "libs/Kernel.h"
#include "Panel.h"
#include "PanelScreen.h"
#include "LcdBase.h"
#include "MainMenuScreen.h"
#include "WatchScreen.h"
#include "FileScreen.h"
#include "JogScreen.h"
#include "ControlScreen.h"
#include "PrepareScreen.h"
#include "ProbeScreen.h"
#include "libs/nuts_bolts.h"
#include "libs/utils.h"
#include "modules/utils/player/PlayerPublicAccess.h"
#include "PublicData.h"
#include "checksumm.h"
#include "ModifyValuesScreen.h"
#include "Robot.h"
#include "Planner.h"
#include "StepperMotor.h"
#include "EndstopsPublicAccess.h"
#include "ExtruderPublicAccess.h"

#include <string>
using namespace std;

#define extruder_checksum CHECKSUM("extruder")

MainMenuScreen::MainMenuScreen()
{
    // Children screens
    this->jog_screen     = (new JogScreen()     )->set_parent(this);
    this->watch_screen   = (new WatchScreen()   )->set_parent(this);
    this->file_screen    = (new FileScreen()    )->set_parent(this);
    this->prepare_screen = (new PrepareScreen() )->set_parent(this);
    this->set_parent(this->watch_screen);
}

// setup and enter the configure screen
void MainMenuScreen::setupConfigureScreen()
{
    auto mvs= new ModifyValuesScreen(true); // delete itself on exit
    mvs->set_parent(this);

    // acceleration
    mvs->addMenuItem("axis Accel", // menu name
        []() -> float { return THEROBOT->get_default_acceleration(); }, // getter
        [this](float acc) { send_gcode("M204", 'S', acc); }, // setter
        10.0F, // increment
        1.0F, // Min
        10000.0F // Max
        );
		
	 mvs->addMenuItem("extr Accel", // menu name
        []() -> float { pad_extruder_t rd; if(PublicData::get_value( extruder_checksum, (void *)&rd )) return rd.accleration; else return 0; }, // getter
        [this](float acc) { send_gcode("M204", 'E', acc); }, // setter
        10.0F, // increment
        1.0F   // Min
        );

    // steps/mm
    mvs->addMenuItem("X steps/mm",
        []() -> float { return THEROBOT->actuators[0]->get_steps_per_mm(); },
        [](float v) { THEROBOT->actuators[0]->change_steps_per_mm(v); },
        0.1F,
        1.0F
        );

    mvs->addMenuItem("Y steps/mm",
        []() -> float { return THEROBOT->actuators[1]->get_steps_per_mm(); },
        [](float v) { THEROBOT->actuators[1]->change_steps_per_mm(v); },
        0.1F,
        1.0F
        );

    mvs->addMenuItem("Z steps/mm",
        []() -> float { return THEROBOT->actuators[2]->get_steps_per_mm(); },
        [](float v) { THEROBOT->actuators[2]->change_steps_per_mm(v); },
        0.1F,
        1.0F
        );
		
	mvs->addMenuItem("E steps/mm",
        // gets steps/mm for currently active extruder
        []() -> float { pad_extruder_t rd; if(PublicData::get_value( extruder_checksum, (void *)&rd )) return rd.steps_per_mm; else return 0.0F; },
        [this](float v) { send_gcode("M92", 'E', v); },
        0.1F,
        1.0F
        );
		
    // flow rate
    mvs->addMenuItem("Flow rate", // menu name
        []() -> float { pad_extruder_t rd; if(PublicData::get_value( extruder_checksum, (void *)&rd )) return rd.flow_rate*100.0F; else return 100.0F; }, // getter as fraction
        [this](float fr) { send_gcode("M221", 'S', fr); }, // setter in percent
        1.0F, // increment
        1.0F  // Min
        );	

    mvs->addMenuItem("Retract len", // menu name
        []() -> float { pad_extruder_t rd; if(PublicData::get_value( extruder_checksum, (void *)&rd )) return rd.retract_length; else return 0; }, // getter
        [this](float l) { send_gcode("M207", 'S', l); }, // setter
        0.1F, // increment
        0.0F  // Min
        );
		
	mvs->addMenuItem("Filament diameter",
        // gets filament diameter for currently active extruder
        []() -> float { pad_extruder_t rd; if(PublicData::get_value( extruder_checksum, (void *)&rd )) return rd.filament_diameter; else return 0.0F; },
        [this](float v) { send_gcode("M200", 'D', v); },
        0.01F,
        0.0F,
        4.0F
        );

    mvs->addMenuItem("Z Home Ofs",
        []() -> float { float rd[3]; PublicData::get_value( endstops_checksum, home_offset_checksum, rd ); return rd[2]; },
        [this](float v) { send_gcode("M206", 'Z', v); },
        0.01F
        );

    mvs->addMenuItem("LCD Contrast",
        []() -> float { return THEPANEL->lcd->getContrast(); },
        [this](float v) { THEPANEL->lcd->setContrast(v); },
        1,
        0,
        255,
        true // instant update
        );

    THEPANEL->enter_screen(mvs);
}

void MainMenuScreen::on_enter()
{
    THEPANEL->enter_menu_mode();
    THEPANEL->setup_menu(6);
    this->refresh_menu();
}

void MainMenuScreen::on_refresh()
{
    if ( THEPANEL->menu_change() ) {
        this->refresh_menu();
    }
    if ( THEPANEL->click() ) {
        this->clicked_menu_entry(THEPANEL->get_menu_current_line());
    }
}

void MainMenuScreen::display_menu_line(uint16_t line)
{
    switch ( line ) {
        case 0: THEPANEL->lcd->printf("Info Screen"); break;
        case 1: if(THEKERNEL->is_halted()) THEPANEL->lcd->printf("Clear HALT"); else THEPANEL->lcd->printf(THEPANEL->is_playing() ? "Abort" : "Play file"); break;
		case 2: THEPANEL->lcd->printf("Prepare"); break;
        case 3: THEPANEL->lcd->printf("Set axis"); break;
//        case 4: THEPANEL->lcd->printf("Custom"); break;
		case 4: THEPANEL->lcd->printf("Calibrate"); break;
        case 5: THEPANEL->lcd->printf("Configure"); break;
 //       case 6: THEPANEL->lcd->printf("Probe"); break;
    }
}

void MainMenuScreen::clicked_menu_entry(uint16_t line)
{
    switch ( line ) {
        case 0: THEPANEL->enter_screen(this->watch_screen); break;
        case 1:
            if(THEKERNEL->is_halted()){
                send_command("M999");
                THEPANEL->enter_screen(this->watch_screen);
            }else if(THEPANEL->is_playing()) abort_playing();
             else THEPANEL->enter_screen(this->file_screen); break;
		case 2: THEPANEL->enter_screen(this->prepare_screen ); break;
        case 3: THEPANEL->enter_screen(this->jog_screen     ); break;
        case 4: THEPANEL->enter_screen(THEPANEL->custom_screen ); break;
        case 5: setupConfigureScreen(); break;
//        case 6: THEPANEL->enter_screen((new ProbeScreen())->set_parent(this)); break;
    }
}

void MainMenuScreen::abort_playing()
{
    //PublicData::set_value(player_checksum, abort_play_checksum, NULL);
    send_command("abort");
    THEPANEL->enter_screen(this->watch_screen);
}

