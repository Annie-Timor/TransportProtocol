#ifndef _UI_SETTING_H_
#define _UI_SETTING_H_

extern int com_port;
extern int com_open_flag;

void UI_ScanComPort(void);
void UI_OpenSerialPort(void);
void UI_SelectFile(int control);
void Schedule(int cur_position, int total_size);
void SendStartSignal(void);

#endif