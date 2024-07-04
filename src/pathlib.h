#pragma once

// Logging
typedef enum {
	General = 0,
	Rendering,
} LogType;
typedef void(*TraceLogCallback)(LogType logtype, const char* text, ...);
void TraceLog(LogType logtype, const char* text, ...);
void SetTraceLogCallback(TraceLogCallback callback);

// File system
bool FileExists(const char* fileName);
bool DirectoryExists(const char* dirPath);
bool IsFileExtension(const char* fileName, const char* ext);
int GetFileLenght(const char* fileName);
const char *GetFileExtension(const char* fileName);
const char *GetFileName(const char *filePath);
const char *GetFileNameWithoutExt(const char *filePath);
const char *GetDirectoryPath(const char *filePath);
const char *GetPrevDirectoryPath(const char *dirPath);
const char *GetWorkingDirectory(void);
const char *GetApplicationDirectory(void);
bool ChangeDirectory(const char *dir);
long GetFileModTime(const char *fileName);

// File managment
unsigned char *LoadFileData(const char *fileName);
void UnloadFileData(unsigned char *data);
