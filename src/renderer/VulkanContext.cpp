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
    
    if (!CreateSwapchain())
        return false;
    
    if (!CreateImageViews())
        return false;

    return true;
}

void VulkanContext::Shutdown()
{
    for (auto imageView : m_SwapchainImageViews)
    {
        vkDestroyImageView(
            m_Device,
            imageView,
            nullptr
        );
    }
    if (m_Swapchain)
    {
        vkDestroySwapchainKHR(
            m_Device,
            m_Swapchain,
            nullptr
        );
    }
    if (m_Device)
    {
        vkDestroyDevice(
            m_Device,
            nullptr
        );
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
        vkDestroyInstance(
            m_Instance,
            nullptr
        );
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

    const std::vector<const char*> deviceExtensions =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

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

    createInfo.enabledExtensionCount =
        static_cast<uint32_t>(
            deviceExtensions.size());

    createInfo.ppEnabledExtensionNames =
        deviceExtensions.data();

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

SwapchainSupportDetails
VulkanContext::QuerySwapchainSupport(
    VkPhysicalDevice device)
{
    SwapchainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        device,
        m_Surface,
        &details.capabilities
    );

    uint32_t formatCount;

    vkGetPhysicalDeviceSurfaceFormatsKHR(
        device,
        m_Surface,
        &formatCount,
        nullptr
    );

    if (formatCount != 0)
    {
        details.formats.resize(formatCount);

        vkGetPhysicalDeviceSurfaceFormatsKHR(
            device,
            m_Surface,
            &formatCount,
            details.formats.data()
        );
    }

    uint32_t presentModeCount;

    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device,
        m_Surface,
        &presentModeCount,
        nullptr
    );

    if (presentModeCount != 0)
    {
        details.presentModes.resize(
            presentModeCount
        );

        vkGetPhysicalDeviceSurfacePresentModesKHR(
            device,
            m_Surface,
            &presentModeCount,
            details.presentModes.data()
        );
    }

    return details;
}

VkSurfaceFormatKHR
VulkanContext::ChooseSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR>& formats)
{
    for (const auto& format : formats)
    {
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB &&
            format.colorSpace
            == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return format;
        }
    }

    return formats[0];
}

VkPresentModeKHR
VulkanContext::ChoosePresentMode(
    const std::vector<VkPresentModeKHR>& modes)
{
    for (const auto& mode : modes)
    {
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return mode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanContext::ChooseExtent(
    const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width
        != UINT32_MAX)
    {
        return capabilities.currentExtent;
    }

    VkExtent2D extent =
    {
        1280,
        720
    };

    extent.width =
        std::clamp(
            extent.width,
            capabilities.minImageExtent.width,
            capabilities.maxImageExtent.width
        );

    extent.height =
        std::clamp(
            extent.height,
            capabilities.minImageExtent.height,
            capabilities.maxImageExtent.height
        );

    return extent;
}

bool VulkanContext::CreateSwapchain()
{
    SwapchainSupportDetails support =
        QuerySwapchainSupport(
            m_PhysicalDevice
        );

    VkSurfaceFormatKHR surfaceFormat =
        ChooseSurfaceFormat(support.formats);

    VkPresentModeKHR presentMode =
        ChoosePresentMode(support.presentModes);

    VkExtent2D extent =
        ChooseExtent(support.capabilities);

    uint32_t imageCount =
        support.capabilities.minImageCount + 1;

    if (support.capabilities.maxImageCount > 0 &&
        imageCount >
        support.capabilities.maxImageCount)
    {
        imageCount =
            support.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType =
        VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

    createInfo.surface = m_Surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace =
        surfaceFormat.colorSpace;

    createInfo.imageExtent = extent;

    createInfo.imageArrayLayers = 1;

    createInfo.imageUsage =
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilyIndices[] =
    {
        m_QueueIndices.graphicsFamily.value(),
        m_QueueIndices.presentFamily.value()
    };

    if (m_QueueIndices.graphicsFamily
        != m_QueueIndices.presentFamily)
    {
        createInfo.imageSharingMode =
            VK_SHARING_MODE_CONCURRENT;

        createInfo.queueFamilyIndexCount = 2;

        createInfo.pQueueFamilyIndices =
            queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode =
            VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform =
        support.capabilities.currentTransform;

    createInfo.compositeAlpha =
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = presentMode;

    createInfo.clipped = VK_TRUE;

    if (vkCreateSwapchainKHR(
            m_Device,
            &createInfo,
            nullptr,
            &m_Swapchain)
        != VK_SUCCESS)
    {
        std::cerr << "Failed to create swapchain!"
                  << std::endl;

        return false;
    }

    vkGetSwapchainImagesKHR(
        m_Device,
        m_Swapchain,
        &imageCount,
        nullptr
    );

    m_SwapchainImages.resize(imageCount);

    vkGetSwapchainImagesKHR(
        m_Device,
        m_Swapchain,
        &imageCount,
        m_SwapchainImages.data()
    );

    m_SwapchainImageFormat =
        surfaceFormat.format;

    m_SwapchainExtent = extent;

    std::cout << "Swapchain Created!"
              << std::endl;

    return true;
}

bool VulkanContext::CreateImageViews()
{
    m_SwapchainImageViews.resize(
        m_SwapchainImages.size()
    );

    for (size_t i = 0; i < m_SwapchainImages.size(); i++)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType =
            VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

        createInfo.image =
            m_SwapchainImages[i];

        createInfo.viewType =
            VK_IMAGE_VIEW_TYPE_2D;

        createInfo.format =
            m_SwapchainImageFormat;

        createInfo.components.r =
            VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.components.g =
            VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.components.b =
            VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.components.a =
            VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask =
            VK_IMAGE_ASPECT_COLOR_BIT;

        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;

        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(
                m_Device,
                &createInfo,
                nullptr,
                &m_SwapchainImageViews[i])
            != VK_SUCCESS)
        {
            std::cerr
                << "Failed to create image view!"
                << std::endl;

            return false;
        }
    }

    std::cout << "Image Views Created!"
              << std::endl;

    return true;
}