#pragma once

// Logging
typedef enum {
	General = 0,
	Rendering,
} LogType;
typedef void(*TraceLogCallback)(LogType logtype, const char* text, ...);
void TraceLog(LogType logtype, const char* text, ...);
void SetTraceLogCallback(TraceLogCallback callback);

// FileSystem
bool FileExists(const char* fileName);
bool DirectoryExists(const char* dirPath);
bool IsFileExtension(const char* filename, const char* ext);
int GetFileLenght(const char* fileName);
