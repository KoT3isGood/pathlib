#include "pathdraw.h"

#include "GLFW/glfw3.h"


uint32_t numWindows=0;
void InitWindow(int width, int height, const char* title, void** pWindow) {
  if (!numWindows) {
    if(!glfwInit()) {
      return; 
    }
    vukan::InitVulkan();
  };
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  *pWindow = (void*)glfwCreateWindow(width, height, title, NULL, NULL);
  if (!*pWindow) {
      if (!numWindows) {
          glfwTerminate();
      }
  }
  vukan::CreateSurface(*pWindow);
  numWindows += 1;
};
void CloseWindow(void* window) {
  glfwDestroyWindow((GLFWwindow*)window);
  numWindows -= 1;
  vukan::DestroySurface(window);
  numWindows += 1;
  if (!numWindows) {
      glfwTerminate();
      vukan::DeinitVulkan();
  }
};
bool WindowShouldClose(void* window) {
    
    return glfwWindowShouldClose((GLFWwindow*)window);
};
void UpdateWindows(void) {
  glfwPollEvents();
}
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
void SetWindowTitle(void* window, const char *title){
    glfwSetWindowTitle((GLFWwindow*)window, title);
};
void SetWindowPosition(void* window, int x, int y){};
void SetWindowMonitor(void* window, int monitor){};
void SetWindowMinSize(void* window, int width, int height){};
void SetWindowMaxSize(void* window, int width, int height){};
void SetWindowSize(void* window, int width, int height){};
int GetWindowSizeX(void* window) {
    int height;
    int width;
    glfwGetWindowSize((GLFWwindow*)window, &width,&height);
    return width;
};
int GetWindowSizeY(void* window) {
    int height;
    int width;
    glfwGetWindowSize((GLFWwindow*)window, &width, &height);
    return height;
};
void SetWindowOpacity(void* window, float opacity){};
void SetWindowFocused(void* window){};


void *GetWindowHandle(void* window){}
void Render()
{
    vukan::Render();
};
void* GetWindowImage(void* window){
  
};


void CreateGPUBuffer(int size, BufferType bufferType, void* mapped, void** buffer)
{
   
}

void* CreateGPUBuffer(int size, BufferType bufferType, void** buffer)
{
    return nullptr;
}

void CreateGPUImage(int x, int y, void* mapped, void** image)
{
}

void* CreateGPUImage(int x, int y, void** image)
{
    return nullptr;
}

void LoadShader(unsigned char* vsSPV, unsigned char* fsSPV, ShaderLayout layout, int constantsSize, bool depthEnabled, void** shader) {};
void LoadShader(unsigned char* csSPV, ShaderLayout layout,int constantsSize, void** shader) {};
void LoadShader(unsigned char* rgenSPV, unsigned char* rchitSPV,unsigned char* rmissSPV,ShaderLayout layout, int constantsSize,void** shader){};
void SetConstants(void* constants, void* shader){};
void Record(int x, int y, void* image, void* shader, void* window){};
void Draw(int x, int y, void* shader){};
void EndRecord(void* shader){};

void DestroyGPUBuffer(void* buffer){};
void DestroyGPUImage(void* image){};
void DestroyShader(void* shader){};
