#ifndef __KL_PERSISTENCE_H__
#define __KL_PERSISTENCE_H__

#include "pch.h"

void AssignmentConfigWrite(const char* filename);
void AssignmentConfigRead(const char* filename);

void MacroConfigWrite(const char* filename);
void MacroConfigRead(const char* filename);

#endif