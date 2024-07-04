#include "pathdraw.h"

void InitWindow(int width, int height, const char* title, void** pWindow) {};
void CloseWindow(void* window) {};
bool WindowShouldClose(void* window) {};
bool IsWindowReady(void* window) {};
bool IsWindowFullscreen(void* window) {};
bool IsWindowHidden(void* window) {};
bool IsWindowMinimized(void* window) {};
bool IsWindowMaximized(void* window){};
bool IsWindowFocused(void* window){};
bool IsWindowResized(void* window){};
bool IsWindowState(void* window, unsigned int flag){};
void SetWindowState(void* window, unsigned int flags){};
void ClearWindowState(void* window, unsigned int flags){};
void ToggleFullscreen(void* window){};
void ToggleBorderlessWindowed(void* window){};
void MaximizeWindow(void* window){};
void MinimizeWindow(void* window){};
void RestoreWindow(void* window){};
void SetWindowTitle(void* window, const char *title){};
void SetWindowPosition(void* window, int x, int y){};
void SetWindowMonitor(void* window, int monitor){};
void SetWindowMinSize(void* window, int width, int height){};
void SetWindowMaxSize(void* window, int width, int height){};
void SetWindowSize(void* window, int width, int height){};
void SetWindowOpacity(void* window, float opacity){};
void SetWindowFocused(void* window){};
void *GetWindowHandle(void* window){};

void CreateGPUBuffer(int size,void* mapped, void** buffer){};
void* CreateGPUBuffer(int size, void** buffer){};
void* CreateGPUImage(int x, int y, void** image){};
void CreateGPUImage(int x, int y, void* mapped, void** image){};

void LoadShader(unsigned char* vsSPV, unsigned char* fsSPV, int constantsSize,bool depthEnabled, void** shader){};
void LoadShader(unsigned char* csSPV,int constantsSize, void** shader) {};
void LoadShader(unsigned char* rgenSPV, unsigned char* rchitSPV,unsigned char* rmissSPV, int constantsSize,void** shader) {};
