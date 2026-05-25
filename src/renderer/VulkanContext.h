#pragma once

#include <vulkan/vulkan.h>

#include <optional>
#include <vector>
#include <algorithm>

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool IsComplete() const
    {
        return graphicsFamily.has_value()
            && presentFamily.has_value();
    }
};

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
    bool PickPhysicalDevice();
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
    bool CreateLogicalDevice();

private:
    VkInstance m_Instance = VK_NULL_HANDLE;
    VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
    VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
    VkDevice m_Device = VK_NULL_HANDLE;

    VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
    VkQueue m_PresentQueue  = VK_NULL_HANDLE;

    QueueFamilyIndices m_QueueIndices;
};