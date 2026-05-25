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
    
    if (!CreateLogicalDevice())
        return false;

    return true;
}

void VulkanContext::Shutdown()
{
    if (m_Device)
    {
        vkDestroyDevice(m_Device, nullptr);
    }

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

    VkPhysicalDevice bestDevice = VK_NULL_HANDLE;

    for (const auto& device : devices)
    {
        QueueFamilyIndices indices =
            FindQueueFamilies(device);

        if (!indices.IsComplete())
            continue;

        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(device, &props);

        std::cout << "Found GPU: "
                  << props.deviceName
                  << std::endl;

        // Prefer dedicated GPUs
        if (props.deviceType
            == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            bestDevice = device;

            // IMPORTANT
            m_QueueIndices = indices;

            std::cout << "Selected Dedicated GPU: "
                      << props.deviceName
                      << std::endl;

            break;
        }

        // fallback if no dedicated GPU found
        if (bestDevice == VK_NULL_HANDLE)
        {
            bestDevice = device;

            // IMPORTANT
            m_QueueIndices = indices;
        }
    }

    if (bestDevice == VK_NULL_HANDLE)
    {
        std::cerr << "No suitable GPU found!"
                  << std::endl;

        return false;
    }

    m_PhysicalDevice = bestDevice;

    return true;
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

bool VulkanContext::CreateLogicalDevice()
{
    float queuePriority = 1.0f;

    std::vector<VkDeviceQueueCreateInfo> queueInfos;

    std::vector<uint32_t> uniqueQueues =
    {
        m_QueueIndices.graphicsFamily.value(),
        m_QueueIndices.presentFamily.value()
    };

    // remove duplicate if same queue family
    std::sort(uniqueQueues.begin(), uniqueQueues.end());

    uniqueQueues.erase(
        std::unique(uniqueQueues.begin(),
                    uniqueQueues.end()),
        uniqueQueues.end()
    );

    for (uint32_t queueFamily : uniqueQueues)
    {
        VkDeviceQueueCreateInfo queueInfo{};
        queueInfo.sType =
            VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

        queueInfo.queueFamilyIndex = queueFamily;
        queueInfo.queueCount = 1;
        queueInfo.pQueuePriorities = &queuePriority;

        queueInfos.push_back(queueInfo);
    }

    VkPhysicalDeviceFeatures features{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType =
        VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount =
        static_cast<uint32_t>(queueInfos.size());

    createInfo.pQueueCreateInfos =
        queueInfos.data();

    createInfo.pEnabledFeatures = &features;

    if (vkCreateDevice(
            m_PhysicalDevice,
            &createInfo,
            nullptr,
            &m_Device)
        != VK_SUCCESS)
    {
        std::cerr << "Failed to create logical device!"
                  << std::endl;

        return false;
    }

    vkGetDeviceQueue(
        m_Device,
        m_QueueIndices.graphicsFamily.value(),
        0,
        &m_GraphicsQueue
    );

    vkGetDeviceQueue(
        m_Device,
        m_QueueIndices.presentFamily.value(),
        0,
        &m_PresentQueue
    );

    std::cout << "Logical Device Created!"
              << std::endl;

    return true;
}