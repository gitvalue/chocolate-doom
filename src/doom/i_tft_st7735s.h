//
// ST7735S TFT display support for Raspberry Pi / WiringPi.
//

#ifndef __I_TFT_ST7735S__
#define __I_TFT_ST7735S__

#include "doomtype.h"

boolean I_TFT_ST7735S_Init(void);
boolean I_TFT_ST7735S_IsInitialized(void);
boolean I_TFT_ST7735S_Demo(void);
boolean I_TFT_ST7735S_RunDemo(void);
void I_TFT_ST7735S_Shutdown(void);

#endif
