#pragma once

// Logging
typedef enum {
	General = 0,
	Rendering,
} LogType;

void TraceLog(LogType logtype, const char* format, ...);

// File managment
bool FileExists(const char* fileName);
bool DirectoryExists(const char* dirPath);
