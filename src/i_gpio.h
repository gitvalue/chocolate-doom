//
// Copyright(C) 2026
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//    GPIO button input interface for hardware buttons on Raspberry Pi
//

#ifndef __I_GPIO__
#define __I_GPIO__

// Initialize GPIO button input
void I_InitGPIO(void);

// Poll GPIO buttons and post key events
void I_ReadGPIO(void);

// Shutdown GPIO
void I_ShutdownGPIO(void);

#endif
