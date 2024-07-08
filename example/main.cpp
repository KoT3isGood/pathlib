#include "../src/pathlib.h"
#include "../src/pathdraw.h"
int main() {
  void* window = nullptr;
  InitWindow(1280,720,"test", &window);
  while (!WindowShouldClose(window)) {
   UpdateWindows(); 
   Render();
  }
  CloseWindow(window);
	return 0;
}
