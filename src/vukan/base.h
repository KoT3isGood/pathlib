#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"
#include "../pathlib.h"
namespace vukan {
	void InitVulkan();
	void Render();
	void DeinitVulkan();
	void CreateSurface(void* window);
	void DestroySurface(void* window);
	void CreateBuffer(int size, VkBufferUsageFlags bufferFlags,void* mapped, void** buffer);
	void CreateImage(int x, int y, void* mapped, void** image);
}
