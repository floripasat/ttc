/*
 * main.c
 * 
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina.
 * 
 * This file is part of FloripaSat-TTC.
 * 
 * FloripaSat-TTC is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * FloripaSat-TTC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with FloripaSat-TTC. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Main file.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.7
 * 
 * \date 08/06/2017
 * 
 * \defgroup src Source
 * \{
 */

#include "beacon.h"

/**
 * \brief The main function.
 * 
 * After the initializations of the peripherals, the program stays
 * running in a infinite loop. If the program get out of the main
 * infinite loop (If any critical error occurs), there is a second
 * infinite loop to force an watchdog trigger.
 *
 * \image html beacon_main_flowchart.png "Beacon main flowchart" width=200px
 * 
 * \return None.
 */
void main()
{
    beacon_init();
    
    beacon_run();
    
    while(1);       // Safety infinite loop (If the code get out of the main loop, the watchdog will be triggered here)
}

//! \} End of src group
