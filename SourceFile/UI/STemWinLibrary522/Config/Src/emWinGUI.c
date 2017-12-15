

#include "emWinGUI.h"


void emWinInit(void)
{
    System.emWin.emWinDispStringAt=GUI_DispStringAt;
	System.emWin.emWinSetPenSize=GUI_SetPenSize;
    System.emWin.emWinBackColor=GUI_SetBkColor;
   // System.emWin.emWinFontSet=GUI_SetFont;
    System.emWin.emWinPointColor=GUI_SetColor;
    System.emWin.emWinClear=GUI_Clear;
}







































