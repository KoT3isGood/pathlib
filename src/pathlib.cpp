#include "pathlib.h"
#include <stdio.h>
#include <stdarg.h>
#include <filesystem>
#include <string>


bool shouldResetLog = true;
TraceLogCallback traceLogImpl = nullptr;

void SetTraceLogCallback(TraceLogCallback callback) {
  traceLogImpl = callback;
};


void TraceLog(LogType logtype, const char* text, ...)
{
      
    va_list args;
    va_start(args, text);
    // Custom Logging
    if (traceLogImpl) {
      traceLogImpl(logtype,text,args);
      va_end(args);
      return;
    }

    // Console
    vprintf(text, args);
    
    // Files
    const char* fileToOpen;
    switch (logtype) {
      case General: {fileToOpen = "general.log";break;}
      case Rendering: {fileToOpen = "rendering.log";break;}
    }
    if (shouldResetLog) {
      FILE* fptr = fopen("general.log", "wb");
	    fprintf(fptr, "");
	    fclose(fptr);
      fptr = fopen("rendering.log", "wb");
	    fprintf(fptr, "");
	    fclose(fptr);
      shouldResetLog = false;
    }
    FILE* outputFile = fopen(fileToOpen, "a");
    vfprintf(outputFile,text,args);
    fclose(outputFile); 
    va_end(args);
}

bool FileExists(const char* fileName) {
  return std::filesystem::exists(fileName);
};
bool DirectoryExists(const char* dirPath) {
  return std::filesystem::exists(dirPath);
}
bool IsFileExtension(const char* filename, const char* ext) {
 return !strcmp(std::filesystem::path(filename).extension().string().c_str(),ext);
};
int GetFileLenght(const char* fileName) {
  return std::filesystem::file_size(std::string(fileName));
};
