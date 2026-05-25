#include "VulkanContext.h"
#include "platform/Window.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <iostream>

bool VulkanContext::Init(Window& window)
{
    if (!CreateInstance())
        return false;

    if (!CreateSurface(window))
        return false;

    if (!PickPhysicalDevice())
        return false;

    return true;
}

void VulkanContext::Shutdown()
{
    if (m_Surface)
    {
        vkDestroySurfaceKHR(
            m_Instance,
            m_Surface,
            nullptr
        );
    }

    if (m_Instance)
    {
        vkDestroyInstance(m_Instance, nullptr);
    }
}

bool VulkanContext::CreateInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "PhysicsEngine2D";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "CustomEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t extensionCount = 0;

    const char* const* extensions =
        SDL_Vulkan_GetInstanceExtensions(
            &extensionCount
        );
    
    createInfo.enabledExtensionCount = extensionCount;
    createInfo.ppEnabledExtensionNames = extensions;

    if (vkCreateInstance(&createInfo, nullptr, &m_Instance)
        != VK_SUCCESS)
    {
        std::cerr << "Failed to create Vulkan instance!"
                  << std::endl;

        return false;
    }

    std::cout << "Vulkan Instance Created!"
              << std::endl;

    return true;
}

bool VulkanContext::CreateSurface(Window& window)
{
    if (!SDL_Vulkan_CreateSurface(
            window.GetSDLWindow(),
            m_Instance,
            nullptr,
            &m_Surface))
    {
        std::cerr << "Failed to create Vulkan surface: "
                  << SDL_GetError()
                  << std::endl;

        return false;
    }

    std::cout << "Vulkan Surface Created!"
              << std::endl;

    return true;
}

void VulkanContext::BeginFrame()
{
}

void VulkanContext::EndFrame()
{
}

bool VulkanContext::PickPhysicalDevice()
{
    uint32_t deviceCount = 0;

    vkEnumeratePhysicalDevices(
        m_Instance,
        &deviceCount,
        nullptr
    );

    if (deviceCount == 0)
    {
        std::cerr << "No Vulkan GPUs found!"
                  << std::endl;

        return false;
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);

    vkEnumeratePhysicalDevices(
        m_Instance,
        &deviceCount,
        devices.data()
    );

    for (const auto& device : devices)
    {
        QueueFamilyIndices indices =
            FindQueueFamilies(device);

        if (indices.IsComplete())
        {
            m_PhysicalDevice = device;

            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(device, &props);

            std::cout << "Selected GPU: "
                      << props.deviceName
                      << std::endl;

            return true;
        }
    }

    std::cerr << "No suitable GPU found!"
              << std::endl;

    return false;
}

QueueFamilyIndices VulkanContext::FindQueueFamilies(
    VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(
        device,
        &queueFamilyCount,
        nullptr
    );

    std::vector<VkQueueFamilyProperties> families(
        queueFamilyCount
    );

    vkGetPhysicalDeviceQueueFamilyProperties(
        device,
        &queueFamilyCount,
        families.data()
    );

    int i = 0;

    for (const auto& family : families)
    {
        if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;

        vkGetPhysicalDeviceSurfaceSupportKHR(
            device,
            i,
            m_Surface,
            &presentSupport
        );

        if (presentSupport)
        {
            indices.presentFamily = i;
        }

        if (indices.IsComplete())
            break;

        i++;
    }

    return indices;
}

