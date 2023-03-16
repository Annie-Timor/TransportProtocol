/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: QUIT */
#define  PANEL_SERIAL_PORT                2       /* control type: ring, callback function: (none) */
#define  PANEL_SCAN_PORT                  3       /* control type: command, callback function: SCAN_PORT_CALLBACK */
#define  PANEL_BAUD_RATE                  4       /* control type: ring, callback function: (none) */
#define  PANEL_OPEN_PORT                  5       /* control type: command, callback function: OPEN_PORT_CALLBACK */
#define  PANEL_TEXTBOX                    6       /* control type: textBox, callback function: (none) */
#define  PANEL_OUT_FILE                   7       /* control type: string, callback function: (none) */
#define  PANEL_IN_FILE                    8       /* control type: string, callback function: (none) */
#define  PANEL_START_RUN                  9       /* control type: command, callback function: START_RUN_CALLBACK */
#define  PANEL_SELECT_OUT_FILE            10      /* control type: command, callback function: SELECT_OUT_FILE_CALLBACK */
#define  PANEL_SELECT_IN_FILE             11      /* control type: command, callback function: SELECT_IN_FILE_CALLBACK */
#define  PANEL_CLEAR_TEXT                 12      /* control type: command, callback function: CLEAR_TEXT_CALLBACK */
#define  PANEL_SELECT_FOLD                13      /* control type: radioButton, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK CLEAR_TEXT_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OPEN_PORT_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QUIT(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SCAN_PORT_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SELECT_IN_FILE_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SELECT_OUT_FILE_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK START_RUN_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
