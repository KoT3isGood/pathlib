#pragma once
typedef enum {
	General = 0,
	Rendering,
} LogType;
void TraceLog(LogType logtype, const char* format, ...);