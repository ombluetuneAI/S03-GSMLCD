/*********************************************************************
*          Portions COPYRIGHT 2014 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2013  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.22 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUIDEMO_IconView.c      
Purpose     : Washing machine demo
---------------------------END-OF-HEADER------------------------------
*/

/**
  ******************************************************************************
  * @file    GUIDEMO_IconView.c
  * @author  MCD Application Team
  * @version V1.1.2
  * @date    28-January-2014
  * @brief   Washing machine demo
  ******************************************************************************
  * @attention
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


#include <stddef.h>

#include "GUIDemo.h"

#if (SHOW_GUIDEMO_ICONVIEW && GUI_WINSUPPORT && GUI_SUPPORT_MEMDEV)

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define MSG_MOVE (WM_USER + 0)

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct {
  const GUI_BITMAP * pBitmap;
  const char       * pText;
  const char       * pExplanation;
} BITMAP_ITEM;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
/*********************************************************************
*
*       _bmBrowser
*
* Purpose:
*   Icon bitmap with alpha channel
*/
static GUI_CONST_STORAGE unsigned long _acBrowser[] = {
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFA931000, 0xCE8C0D00, 0x9B880B00, 0x70880800, 
        0x4B8A0B00, 0x2E911200, 0x24931600, 0x24941600, 0x2A921600, 0x408B1100, 0x62860F00, 0x89821200, 0xB67A1400, 0xE85D1300, 0xFB000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 
        0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xDA8D0C00, 0x8B890600, 0x2E901000, 0x059C2310, 0x00B1462B, 0x00C26648, 

};

GUI_CONST_STORAGE GUI_BITMAP _bmBrowser = {
  48, /* XSize */
  48, /* YSize */
  192, /* BytesPerLine */
  32, /* BitsPerPixel */
  (unsigned char *)_acBrowser,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMP8888
};

/*********************************************************************
*
*       _bmClock
*
* Purpose:
*   Icon bitmap with alpha channel
*/
static GUI_CONST_STORAGE unsigned long _acClock[] = {
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 

};

GUI_CONST_STORAGE GUI_BITMAP _bmClock = {
  48, /* XSize */
  48, /* YSize */
  192, /* BytesPerLine */
  32, /* BitsPerPixel */
  (unsigned char *)_acClock,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMP8888
};

/*********************************************************************
*
*       _bmDate
*
* Purpose:
*   Icon bitmap with alpha channel
*/
static GUI_CONST_STORAGE unsigned long _acDate[] = {
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,

};

GUI_CONST_STORAGE GUI_BITMAP _bmDate = {
  48, /* XSize */
  48, /* YSize */
  192, /* BytesPerLine */
  32, /* BitsPerPixel */
  (unsigned char *)_acDate,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMP8888
};

/*********************************************************************
*
*       _bmEmail
*
* Purpose:
*   Icon bitmap with alpha channel
*/
static GUI_CONST_STORAGE unsigned long _acEmail[] = {
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,

};

GUI_CONST_STORAGE GUI_BITMAP _bmEmail = {
  48, /* XSize */
  48, /* YSize */
  192, /* BytesPerLine */
  32, /* BitsPerPixel */
  (unsigned char *)_acEmail,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMP8888
};

/*********************************************************************
*
*       _bmPassword
*
* Purpose:
*   Icon bitmap with alpha channel
*/
static GUI_CONST_STORAGE unsigned long _acPassword[] = {
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 

};

GUI_CONST_STORAGE GUI_BITMAP _bmPassword = {
  48, /* XSize */
  48, /* YSize */
  192, /* BytesPerLine */
  32, /* BitsPerPixel */
  (unsigned char *)_acPassword,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMP8888
};

/*********************************************************************
*
*       _bmRead
*
* Purpose:
*   Icon bitmap with alpha channel
*/
static GUI_CONST_STORAGE unsigned long _acRead[] = {
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 

};

GUI_CONST_STORAGE GUI_BITMAP _bmRead = {
  48, /* XSize */
  48, /* YSize */
  192, /* BytesPerLine */
  32, /* BitsPerPixel */
  (unsigned char *)_acRead,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMP8888
};

/*********************************************************************
*
*       _bmRemote
*
* Purpose:
*   Icon bitmap with alpha channel
*/
static GUI_CONST_STORAGE unsigned long _acRemote[] = {
  0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xD50B0B0B, 0x9F0A0A0A, 0x6E0A0A0A, 0x3C0C0C0C, 

  0xFF000000, 0xFF000000, 0xFF000000, 0xF9000000, 0xB7151515, 0x09323232, 0x402E2E2E, 0xD1000000, 0xF8000000, 0xFA000000, 0xEC000000, 0xE9000000, 0xF7000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0x4E000062, 

};

GUI_CONST_STORAGE GUI_BITMAP _bmRemote = {
  48, /* XSize */
  48, /* YSize */
  192, /* BytesPerLine */
  32, /* BitsPerPixel */
  (unsigned char *)_acRemote,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMP8888
};

/*********************************************************************
*
*       _bmSystem
*
* Purpose:
*   Icon bitmap with alpha channel
*/
static GUI_CONST_STORAGE unsigned long _acSystem[] = {
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 



};

GUI_CONST_STORAGE GUI_BITMAP _bmSystem = {
  48, /* XSize */
  48, /* YSize */
  192, /* BytesPerLine */
  32, /* BitsPerPixel */
  (unsigned char *)_acSystem,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMP8888
};

/*********************************************************************
*
*       _bmWrite
*
* Purpose:
*   Icon bitmap with alpha channel
*/
static GUI_CONST_STORAGE unsigned long _acWrite[] = {
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 

 


};

GUI_CONST_STORAGE GUI_BITMAP _bmWrite = {
  48, /* XSize */
  48, /* YSize */
  192, /* BytesPerLine */
  32, /* BitsPerPixel */
  (unsigned char *)_acWrite,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMP8888
};

/*********************************************************************
*
*       _bmToucan
*
* Purpose:
*   Toucan with alpha channel
*/
static GUI_CONST_STORAGE unsigned long _acToucan[] = {

  0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 


};

GUI_CONST_STORAGE GUI_BITMAP _bmToucan = {
  158, /* XSize */
  150, /* YSize */
  632, /* BytesPerLine */
  32, /* BitsPerPixel */
  (unsigned char *)_acToucan,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMP8888
};

/*********************************************************************
*
*       _bmHund
*
* Purpose:
*   Background bitmap
*/
static GUI_CONST_STORAGE GUI_COLOR _ColorsHund[] = {
 0xFF000000, 0xFF000000, 0xFF000000,
};

static GUI_CONST_STORAGE GUI_LOGPALETTE PalHund = {
  253,	/* number of entries */
  0, 	/* No transparency */
  &_ColorsHund[0]
};

static GUI_CONST_STORAGE unsigned char acHund[] = {
 0xFF000000, 0xFF000000, 0xFF000e00,
};

static GUI_CONST_STORAGE GUI_BITMAP _bmHund = {
  320, /* XSize */
  240, /* YSize */
  320, /* BytesPerLine */
  8, /* BitsPerPixel */
  acHund,  /* Pointer to picture data (indices) */
  &PalHund  /* Pointer to palette */
};

/*********************************************************************
*
*       _acFramed24B
*
* Purpose:
*   Data for framed font used to draw the explanations
*/
static const unsigned char _acFramed24B[] = {
  0x47, 0x55, 0x49, 0x50, 0x18, 0x00, 0x18, 0x00, 0x15, 0x00, 0x0E, 0x00, 0x12, 0x00, 0x5F, 0x00, 0x20, 0x00, 0x7E, 0x00, 0xA0, 0x00, 0x7F, 0x01, 0x92, 0x01, 0x92, 0x01, 0xFA, 0x01, 0xFF, 0x01, 0xC6, 0x02, 0xC7, 0x02, 0xC9, 0x02, 0xC9, 0x02,
  0xD8, 0x02, 0xDD, 0x02, 0x7E, 0x03, 0x7E, 0x03, 0x84, 0x03, 0x8A, 0x03, 0x8C, 0x03, 0x8C, 0x03, 0x8E, 0x03, 0xA1, 0x03, 0xA3, 0x03, 0xCE, 0x03, 0x01, 0x04, 0x0C, 0x04, 0x0E, 0x04, 0x4F, 0x04, 0x51, 0x04, 0x5C, 0x04, 0x5E, 0x04, 0x5F, 0x04,
  0x90, 0x04, 0x91, 0x04, 0x80, 0x1E, 0x85, 0x1E, 0xF2, 0x1E, 0xF3, 0x1E, 0x13, 0x20, 0x15, 0x20, 0x17, 0x20, 0x1E, 0x20, 0x20, 0x20, 0x22, 0x20, 0x26, 0x20, 0x26, 0x20, 0x30, 0x20, 0x30, 0x20, 0x32, 0x20, 0x33, 0x20, 0x39, 0x20, 0x3A, 0x20,
  0x3C, 0x20, 0x3C, 0x20, 0x3E, 0x20, 0x3E, 0x20, 0x44, 0x20, 0x44, 0x20, 0x7F, 0x20, 0x7F, 0x20, 0xA3, 0x20, 0xA4, 0x20, 0xA7, 0x20, 0xA7, 0x20, 0xAC, 0x20, 0xAC, 0x20, 0x05, 0x21, 0x05, 0x21, 0x13, 0x21, 0x13, 0x21, 0x16, 0x21, 0x16, 0x21,
  0x22, 0x21, 0x22, 0x21, 0x26, 0x21, 0x26, 0x21, 0x2E, 0x21, 0x2E, 0x21, 0x5B, 0x21, 0x5E, 0x21, 0x90, 0x21, 0x95, 0x21, 0xA8, 0x21, 0xA8, 0x21, 0x02, 0x22, 0x02, 0x22, 0x06, 0x22, 0x06, 0x22, 0x0F, 0x22, 0x0F, 0x22, 0x11, 0x22, 0x12, 0x22,
  0x15, 0x22, 0x15, 0x22, 0x19, 0x22, 0x1A, 0x22, 0x1E, 0x22, 0x1F, 0x22, 0x29, 0x22, 0x29, 0x22, 0x2B, 0x22, 0x2B, 0x22, 0x48, 0x22, 0x48, 0x22, 0x60, 0x22, 0x61, 0x22, 0x64, 0x22, 0x65, 0x22, 0x02, 0x23, 0x02, 0x23, 0x10, 0x23, 0x10, 0x23,
  0x20, 0x23, 0x21, 0x23, 0x00, 0x25, 0x00, 0x25, 0x02, 0x25, 0x02, 0x25, 0x0C, 0x25, 0x0C, 0x25, 0x10, 0x25, 0x10, 0x25, 0x14, 0x25, 0x14, 0x25, 0x18, 0x25, 0x18, 0x25, 0x1C, 0x25, 0x1C, 0x25, 0x24, 0x25, 0x24, 0x25, 0x2C, 0x25, 0x2C, 0x25,
  0x34, 0x25, 0x34, 0x25, 0x3C, 0x25, 0x3C, 0x25, 0x50, 0x25, 0x6C, 0x25, 0x80, 0x25, 0x80, 0x25, 0x84, 0x25, 0x84, 0x25, 0x88, 0x25, 0x88, 0x25, 0x8C, 0x25, 0x8C, 0x25, 0x90, 0x25, 0x93, 0x25, 0xA0, 0x25, 0xA1, 0x25, 0xAA, 0x25, 0xAC, 0x25,
  0xB2, 0x25, 0xB2, 0x25, 0xBA, 0x25, 0xBA, 0x25, 0xBC, 0x25, 0xBC, 0x25, 0xC4, 0x25, 0xC4, 0x25, 0xCA, 0x25, 0xCB, 0x25, 0xCF, 0x25, 0xCF, 0x25, 0xD8, 0x25, 0xD9, 0x25, 0xE6, 0x25, 0xE6, 0x25, 0x3A, 0x26, 0x3C, 0x26, 0x40, 0x26, 0x40, 0x26,
  0x42, 0x26, 0x42, 0x26, 0x60, 0x26, 0x60, 0x26, 0x63, 0x26, 0x63, 0x26, 0x65, 0x26, 0x66, 0x26, 0x6A, 0x26, 0x6B, 0x26, 0x01, 0xF0, 0x08, 0xF0, 0x10, 0xF0, 0x10, 0xF0, 0x01, 0xFB, 0x02, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0x00, 0x03, 0x00,
  0x00, 0x00, 0x11, 0x00, 0x07, 0x00, 0x00, 0x00, 0xEC, 0x2A, 0x00, 0x00, 0x05, 0x00, 0x12, 0x00, 0x01, 0x00, 0x01, 0x00, 0x08, 0x00, 0x00, 0x00, 0xF2, 0x2A, 0x00, 0x00, 0x09, 0x00, 0x08, 0x00, 0x01, 0x00, 0x01, 0x00, 0x0B, 0x00, 0x00, 0x00,
  0x16, 0x2B, 0x00, 0x00, 0x0C, 0x00, 0x12, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x36, 0x2B, 0x00, 0x00, 0x0B, 0x00, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x7E, 0x2B, 0x00, 0x00, 0x11, 0x00, 0x12, 0x00,
  0x01, 0x00, 0x01, 0x00, 0x13, 0x00, 0x00, 0x00, 0xD2, 0x2B, 0x00, 0x00, 0x0F, 0x00, 0x12, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x00, 0x00, 0x00, 0x3E, 0x2C, 0x00, 0x00, 0x05, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x06, 0x00, 0x00, 0x00,
  0x86, 0x2C, 0x00, 0x00, 0x07, 0x00, 0x17, 0x00, 0x01, 0x00, 0x01, 0x00, 0x08, 0x00, 0x00, 0x00, 0x96, 0x2C, 0x00, 0x00, 0x07, 0x00, 0x17, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x00, 0x00, 0x00, 0xC4, 0x2C, 0x00, 0x00, 0x08, 0x00, 0x0A,
  0x00, 0x00, 0x00, 0x01, 0x00, 0x09, 0x00, 0x00, 0x00, 0xF2, 0x2C, 0x00, 0x00, 0x0B, 0x00, 0x0D, 0x00, 0x01, 0x00, 0x04, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x06, 0x2D, 0x00, 0x00, 0x05, 0x00, 0x09, 0x00, 0x01, 0x00, 0x0E, 0x00, 0x07, 0x00, 0x00,
  0x00, 0x3A, 0x2D, 0x00, 0x00, 0x07, 0x00, 0x05, 0x00, 0x00, 0x00, 0x0A,
  0x00, 0x08, 0x00, 0x00, 0x00, 0x4C, 0x2D, 0x00, 0x00, 0x05, 0x00, 0x05, 0x00, 0x01, 0x00, 0x0E, 0x00, 0x07, 0x00, 0x00, 0x00, 0x56, 0x2D, 0x00, 0x00, 0x07, 0x00, 0x12, 0x00, 0x00, 0x00, 0x01, 0x00, 0x07, 0x00, 0x00, 0x00, 0x60, 0x2D, 0x00,


};

/*********************************************************************
*
*       _aBitmapItem
*/
static const BITMAP_ITEM _aBitmapItem[] = {
  {&_bmBrowser, "Browser" , "Use the browser to explore the www"},
  {&_bmClock,   "Clock"   , "Adjust current time and date"},
  {&_bmDate,    "Date"    , "Use the diary"},
  {&_bmEmail,   "Email"   , "Read an email"},
  {&_bmSystem,  "System"  , "Change system settings"},
  {&_bmRead,    "Read"    , "Read a document"},
  {&_bmWrite,   "Write"   , "Write an email"},
  {&_bmPassword,"Password", "Determine the system password"},
  {&_bmRemote,  "Network" , "Select network"},
};

/*********************************************************************
*
*       Private routines
*
**********************************************************************
*/
/*********************************************************************
*
*       _cbWin
*/
static void _cbWin(WM_MESSAGE * pMsg) {
  int NCode, Id, Sel, ySize;
  WM_HWIN hItem, hDlg;
  GUI_RECT Rect = {0, 0, 0, 0};
  static int yPosToucan = 20;
  static int yAdd       = 1;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);      /* Id of widget */
    NCode = pMsg->Data.v;                 /* Notification code */
    switch (Id) {
    case GUI_ID_ICONVIEW0:
      switch (NCode) {
      case WM_NOTIFICATION_SEL_CHANGED:
        //
        // Change widget text changing the selection 
        //
        Sel   = ICONVIEW_GetSel(pMsg->hWinSrc);
        hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
        TEXT_SetText(hItem, _aBitmapItem[Sel].pExplanation);
        break;
      }
      break;
    }
    break;
  case MSG_MOVE:
    //
    // Move toucan position and invalidate toucan area
    //
    yPosToucan += yAdd;
    if (yPosToucan >= 100) {
      yAdd = -yAdd;
      yPosToucan = 100;
    } else if (yPosToucan <= 20) {
      yAdd = -yAdd;
      yPosToucan = 20;
    }
    Rect.x0 = 146;
    Rect.y0 = yPosToucan - 5;
    Rect.x1 = Rect.x0 + _bmToucan.XSize - 1;
    Rect.y1 = Rect.y0 + _bmToucan.YSize - 1 + 5;
    WM_InvalidateRect(pMsg->hWin, &Rect);
    break;
  case WM_PAINT:
    //
    // Draw background and toucan
    //
    ySize = LCD_GetYSize();
    if (ySize > _bmHund.YSize) {
      GUI_SetBkColor(0xb2c7ca);
      GUI_Clear();
    }
    GUI_DrawBitmap(&_bmHund, 0, (ySize - _bmHund.YSize) / 2);
    GUI_DrawBitmap(&_bmToucan, 146, yPosToucan);
    break;
  }
}

/*********************************************************************
*
*       Public routines
*
**********************************************************************
*/
/*********************************************************************
*
*       GUIDEMO_IconView
*/
void GUIDEMO_IconView(void) {
  WM_CALLBACK * pcbPrev;
  WM_HWIN       hWin, hText0, hText1;
  GUI_FONT      Font;
  int           i, Add, Sel, TimeNextInc, TimeEnd, ySize;

  GUIDEMO_ShowIntro("Icon View Demo",
                    "Demonstrates the use of\n"
                    "the ICONVIEW widget");
  GUIDEMO_HideInfoWin();
  WM_EnableMemdev(WM_HBKWIN);
  pcbPrev = WM_SetCallback(WM_HBKWIN, _cbWin);
  //
  // Create title of sample
  //
  hText0 = TEXT_CreateEx(0, 0, 320, 30, WM_HBKWIN, WM_CF_SHOW, 0, GUI_ID_TEXT0, "STemWin ICONVIEW sample");
  TEXT_SetFont(hText0, &GUI_FontRounded22);
  TEXT_SetTextAlign(hText0, GUI_TA_HCENTER | GUI_TA_VCENTER);
  TEXT_SetTextColor(hText0, 0xFF4040);
  //
  // Create explanation
  //
  hText1 = TEXT_CreateEx(160, 40, 140, 100, WM_HBKWIN, WM_CF_SHOW, 0, GUI_ID_TEXT1, "");
  GUI_SIF_CreateFont(_acFramed24B, &Font, GUI_SIF_TYPE_PROP_FRM);
  TEXT_SetFont(hText1, &Font);
  TEXT_SetTextColor(hText1, GUI_WHITE);
  TEXT_SetWrapMode(hText1, GUI_WRAPMODE_WORD);
  TEXT_SetTextAlign(hText1, GUI_TA_HCENTER | GUI_TA_VCENTER);
  //
  // Create iconview widget
  //
  ySize = LCD_GetYSize();
  hWin = ICONVIEW_CreateEx(10, 30, 125 + 11, ySize - 40, 
                           WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, 
                           ICONVIEW_CF_AUTOSCROLLBAR_V, GUI_ID_ICONVIEW0, 55, 60);
  for (i = 0; i < GUI_COUNTOF(_aBitmapItem); i++) {
    //
    // Add icons to the widget
    //
    ICONVIEW_AddBitmapItem(hWin, _aBitmapItem[i].pBitmap, _aBitmapItem[i].pText);
  }
  ICONVIEW_SetBkColor(hWin, ICONVIEW_CI_SEL, GUI_BLUE | 0xC0000000);
  ICONVIEW_SetFont(hWin, &GUI_Font13B_ASCII);
  WM_SetFocus(hWin);
  Add = 1;
  Sel = 0;
  TimeEnd = GUIDEMO_GetTime() + 9000;
  TimeNextInc = GUIDEMO_GetTime() + 1000;
  do {
    GUI_Delay(50);
    WM_SendMessageNoPara(WM_HBKWIN, MSG_MOVE); // Message for moving the toucan
    if (GUIDEMO_GetTime() >= TimeNextInc) {
      TimeNextInc += 1000;
      if (Sel == 0) {
        Add = 1;
      } else if (Sel == (GUI_COUNTOF(_aBitmapItem) - 1)) {
        Add = -1;
      }
      Sel += Add;
      ICONVIEW_SetSel(hWin, Sel);
    }
  } while ((GUIDEMO_GetTime() < TimeEnd) && (GUIDEMO_CheckCancel() == 0));
  WM_DeleteWindow(hText0);
  WM_DeleteWindow(hText1);
  WM_DeleteWindow(hWin);
  WM_SetCallback(WM_HBKWIN, pcbPrev);
  GUIDEMO_NotifyStartNext();
}

#else

void GUIDEMO_IconView(void) {}

#endif

/*************************** End of file ****************************/
