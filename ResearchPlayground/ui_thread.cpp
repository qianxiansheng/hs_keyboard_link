#include "ui_thread.h"

#include "util/myqueue.h"
#include "util/thread_pool.h"

bool alert_flag = false;
char alert_message[256] = { 0 };
char label_message[256] = { 0 };

BlockingQueue<UIEvent>* queueUI;

extern ThreadPool* pool;


static bool UIThreadEventHandler()
{
	UIEvent e;
	queueUI->take(e);
	printf("event type: %d %s\n", e.eventType, e.message.c_str());
	switch (e.eventType)
	{
	case UI_EVENT_TYPE_ALERT_MESSAGE:
		alert_flag = true;
		strcpy(alert_message, e.message.c_str());
		break;

	case UI_EVENT_TYPE_LABEL_MESSAGE:
		strcpy(label_message, e.message.c_str());
		break;

	case UI_EVENT_TYPE_EXIT:
		return false;
	}

	return true;
}

static void UIThread()
{
	while (UIThreadEventHandler())
	{
	}
}

void UIThreadPutEvent(UIEventTypeEnum eventType)
{
	UIEvent e;
	e.eventType = eventType;
	queueUI->put(e);
}
void UIThreadPutEvent(UIEvent&& e)
{
	queueUI->put(e);
}
void UIThreadPutEvent(UIEvent& e)
{
	queueUI->put(e);
}

void UIThreadPutEvent_AlertMessage(std::string message)
{
	UIEvent e;
	e.eventType = UI_EVENT_TYPE_ALERT_MESSAGE;
	e.message = message;
	queueUI->put(e);
}
void UIThreadPutEvent_LabelMessage(std::string message)
{
	UIEvent e;
	e.eventType = UI_EVENT_TYPE_LABEL_MESSAGE;
	e.message = message;
	queueUI->put(e);
}
void UIThreadPutEvent_Exit()
{
	UIEvent e;
	e.eventType = UI_EVENT_TYPE_EXIT;
	queueUI->put(e);
}

void InitUIThread()
{
	// UI thread
	queueUI = new BlockingQueue<UIEvent>(32);
	pool->enqueue(UIThread);
}

void DestroyUIThread()
{
	UIThreadPutEvent_Exit();
}

