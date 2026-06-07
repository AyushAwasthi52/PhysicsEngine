#pragma once

#include <vulkan/vulkan.h>

#include <optional>
#include <vector>
#include <algorithm>

struct SwapchainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;

    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};
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
    bool CreateSwapchain();
    SwapchainSupportDetails QuerySwapchainSupport(
        VkPhysicalDevice device);
    VkSurfaceFormatKHR ChooseSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& formats);
    VkPresentModeKHR ChoosePresentMode(
        const std::vector<VkPresentModeKHR>& modes);
    VkExtent2D ChooseExtent(
        const VkSurfaceCapabilitiesKHR& capabilities);

private:
    VkInstance m_Instance = VK_NULL_HANDLE;
    VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
    VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
    VkDevice m_Device = VK_NULL_HANDLE;

    VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
    VkQueue m_PresentQueue  = VK_NULL_HANDLE;

    QueueFamilyIndices m_QueueIndices;

private:
    VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;
    std::vector<VkImage> m_SwapchainImages;
    VkFormat m_SwapchainImageFormat;
    VkExtent2D m_SwapchainExtent;

private:
    std::vector<VkImageView> m_SwapchainImageViews;

public:
    bool CreateImageViews();

private:
    bool CreateRenderPass();

private:
    VkRenderPass m_RenderPass = VK_NULL_HANDLE;
};