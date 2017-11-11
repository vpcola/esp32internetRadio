/*
 * ui.h
 *
 *  Created on: 01.04.2017
 *      Author: michaelboeckling
 */

#ifndef _INCLUDE_UI_H_
#define _INCLUDE_UI_H_

#define UI_QUEUE_LENGTH 3 
#define UI_MESSAGE_LENGTH 100

typedef enum { 
    UI_NONE, 
    UI_CONNECTING, 
    UI_CONNECTED,
    UI_STATION_UPDATE 
} ui_event_t;

typedef struct {
    ui_event_t event;
    char data[UI_MESSAGE_LENGTH]; // Data to pass, ex. Station Name
} ui_queue_message_t;

// Queue and event without payload data
void ui_queue_event(ui_event_t evt); 
// Queue message with payload string data
void ui_queue_event_message(ui_event_t evt, const char * message);
int ui_init();

#endif /* _INCLUDE_UI_H_ */
