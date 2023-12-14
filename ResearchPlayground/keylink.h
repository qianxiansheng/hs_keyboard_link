#ifndef __KL_KEYLINK_H__
#define __KL_KEYLINK_H__

#include "pch.h"

float dpiScale();
void UpdateWindowDockNodeWidth(const char* windowName, float width);
void UpdateWindowDockNodeHeight(const char* windowName, float height);

void EnableKeyHook(bool enable);
void HideApplication();
void RestoreApplication();

#endif __KL_KEYLINK_H__