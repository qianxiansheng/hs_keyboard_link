#ifndef __KL_UI_THREAD_H__
#define __KL_UI_THREAD_H__

#include <iostream>

enum UIEventTypeEnum
{
	UI_EVENT_TYPE_ALERT_MESSAGE = 0x30,
	UI_EVENT_TYPE_LABEL_MESSAGE = 0x31,
	UI_EVENT_TYPE_EXIT = 0xF0,
};

typedef struct
{
	uint32_t eventType;

	std::string message;
} UIEvent;

void UIThreadPutEvent(UIEventTypeEnum eventType);
void UIThreadPutEvent(UIEvent&& e);
void UIThreadPutEvent(UIEvent& e);

void UIThreadPutEvent_AlertMessage(std::string message);
void UIThreadPutEvent_LabelMessage(std::string message);
void UIThreadPutEvent_Exit();

void InitUIThread();

void DestroyUIThread();

#define showLabel UIThreadPutEvent_LabelMessage
#define showAlert UIThreadPutEvent_AlertMessage
#define showAlert_(msg) \
	UIThreadPutEvent_LabelMessage(msg);\
	UIThreadPutEvent_AlertMessage(msg);

#endif