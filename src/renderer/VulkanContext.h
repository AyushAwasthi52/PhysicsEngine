#pragma once

#include <vulkan/vulkan.h>

class Window;

class VulkanContext
{
public:
    bool Init(Window& window);
    void Shutdown();

    void BeginFrame();
    void EndFrame();

private:
    bool CreateInstance();
    bool CreateSurface(Window& window);

private:
    VkInstance m_Instance = VK_NULL_HANDLE;
    VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
};