#include "pathlib.h"
#include <stdio.h>
#include <stdarg.h>
#include <filesystem>
#include <string>
#include <istream>
#include <fstream>


bool shouldResetLog = true;
TraceLogCallback traceLogImpl = nullptr;

void SetTraceLogCallback(TraceLogCallback callback) {
  traceLogImpl = callback;
};


void TraceLog(unsigned int logtype, const char* text, ...)
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
    /*if (shouldResetLog) {
      FILE* fptr = fopen("general.log", "wb");
	    fprintf(fptr, "");
	    fclose(fptr);
      fptr = fopen("rendering.log", "wb");
	    fprintf(fptr, "");
	    fclose(fptr);
      shouldResetLog = false;
    }*/
    //FILE* outputFile = fopen(fileToOpen, "a");
    //vfprintf(outputFile,text,args);
    //fclose(outputFile); 
    va_end(args);
}

bool FileExists(const char* fileName) {
  return std::filesystem::exists(fileName);
};
bool DirectoryExists(const char* dirPath) {
  return std::filesystem::exists(dirPath);
}
bool IsFileExtension(const char* fileName, const char* ext) {
 return !strcmp(GetFileExtension(fileName),ext);
};
int GetFileLenght(const char* fileName) {
  return std::filesystem::file_size(std::string(fileName));
};

const char *GetFileExtension(const char* fileName) {
  static std::string outstr;
  outstr = std::filesystem::path(fileName).extension().string();
  return outstr.c_str(); 
};

const char *GetFileName(const char *filePath) {
  static std::string outstr;
  outstr = std::filesystem::path(filePath).filename().string();
  return outstr.c_str();
};
const char *GetFileNameWithoutExt(const char *filePath) {
  static std::string outstr;
  outstr = std::filesystem::path(filePath).stem().string();
  return outstr.c_str();
};
const char *GetDirectoryPath(const char *filePath) {
  static std::string outstr;
  outstr = std::filesystem::path(filePath).parent_path().string();
  return outstr.c_str();
};

const char *GetPrevDirectoryPath(const char *dirPath) {
  static std::string outstr;
  outstr = std::filesystem::path(dirPath).parent_path().string();
  return outstr.c_str();
};
const char *GetWorkingDirectory(void) {
  static std::string outstr;
  outstr = std::filesystem::current_path().string().c_str();
  return outstr.c_str();
};
const char *GetApplicationDirectory(void) {return "NOT IMPLEMENTED";};


bool ChangeDirectory(const char *dir) { 
  if (DirectoryExists(dir)) {
    std::filesystem::current_path(dir);
    return true;
  } 
  return false;
};
long GetFileModTime(const char *fileName) {

};

unsigned char *LoadFileData(const char *fileName) {

  std::ifstream file(fileName, std::ios::ate | std::ios::binary);

  if(!file.is_open()) {
    return nullptr;
  }

	size_t fileSize = (size_t)file.tellg();
	unsigned char* data = (unsigned char*)malloc(fileSize);
  
	file.seekg(0);
	file.read((char*)data, fileSize);

	file.close();

  return data;
};

void UnloadFileData(unsigned char *data) {
  free(data);
};
