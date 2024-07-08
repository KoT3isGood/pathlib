#pragma once
#include "pathlib.h"
#include "vukan/base.h"

void InitWindow(int width, int height, const char* title, void** pWindow);
void CloseWindow(void* window);
bool WindowShouldClose(void* window);
void UpdateWindows(void);
bool IsWindowReady(void* window);
bool IsWindowFullscreen(void* window);
bool IsWindowHidden(void* window);
bool IsWindowMinimized(void* window);
bool IsWindowMaximized(void* window);
bool IsWindowFocused(void* window);
bool IsWindowResized(void* window);
bool IsWindowState(void* window, unsigned int flag);
void SetWindowState(void* window, unsigned int flags);
void ClearWindowState(void* window, unsigned int flags);
void ToggleFullscreen(void* window);
void ToggleBorderlessWindowed(void* window);
void MaximizeWindow(void* window);
void MinimizeWindow(void* window);
void RestoreWindow(void* window);
void SetWindowTitle(void* window, const char *title);
void SetWindowPosition(void* window, int x, int y);
void SetWindowMonitor(void* window, int monitor);
void SetWindowMinSize(void* window, int width, int height);
void SetWindowMaxSize(void* window, int width, int height);
void SetWindowSize(void* window, int width, int height);
int GetWindowSizeX(void* window);
int GetWindowSizeY(void* window);
void SetWindowOpacity(void* window, float opacity);
void SetWindowFocused(void* window);
void *GetWindowHandle(void* window);

void Render();

typedef enum {
  StorageBuffer,
  UniformBuffer,
  Texture,
  Image
} ShaderBindingType;

typedef struct {
  ShaderBindingType type;
  int binding;

} ShaderBinding;

typedef struct {
  ShaderBinding* pShaderBindings;
  int count;
} ShaderLayout;

typedef enum {
	Uniform,
	Storage
} BufferType;
void CreateGPUBuffer(int size, BufferType bufferType, void* mapped, void** buffer);
void* CreateGPUBuffer(int size, BufferType bufferType, void** buffer);
void CreateGPUImage(int x, int y, void* mapped, void** image);
void* CreateGPUImage(int x, int y, void** image);

void LoadShader(unsigned char* vsSPV, unsigned char* fsSPV, ShaderLayout layout, int constantsSize, bool depthEnabled, void** shader);
void LoadShader(unsigned char* csSPV, ShaderLayout layout,int constantsSize, void** shader);
void LoadShader(unsigned char* rgenSPV, unsigned char* rchitSPV,unsigned char* rmissSPV,ShaderLayout layout, int constantsSize,void** shader);
void SetConstants(void* constants, void* shader);
void Record(int x, int y, void* image, void* shader, void* window);
void Draw(int x, int y, void* shader);
void Draw(int x, int y, void* shader, void* window);
void EndRecord(void* shader);

void DestroyGPUBuffer(void* buffer);
void DestroyGPUImage(void* image);
void DestroyShader(void* shader);
