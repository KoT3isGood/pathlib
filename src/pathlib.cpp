#include "pathlib.h"
#include <stdio.h>
#include <stdarg.h>
#include <filesystem>
#include <string>

void TraceLog(LogType logtype, const char* format, ...)
{

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    const char* fileToOpen;
    switch (logtype) {
      case General: {fileToOpen = "general.log";break;}
      case Rendering: {fileToOpen = "rendering.log";break;}
    }
    FILE* outputFile = fopen(fileToOpen, "a");
    vfprintf(outputFile,format,args);
    fclose(outputFile);
    va_end(args);
}

bool FileExists(const char* fileName) {
  return std::filesystem::exists(std::string(fileName));
};
bool DirectoryExists(const char* dirPath) {
  return std::filesystem::exists(std::string(dirPath));
}

