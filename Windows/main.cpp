#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h> 
#include <vulkan/vulkan_android.h>

#include <stdio.h>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <vector>

HINSTANCE instanceHandle;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const bool enableValidationLayers = true;

//  function for creating debugUtilsMessenger
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

//  destroy debugUtilsMessenger
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

class HelloTriangleApplication {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:

    WNDCLASSEX wc = {};
    HWND hWnd;

    //  Vulkan instance
    VkInstance instance;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT debugMessenger;

    bool checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    std::vector<const char*> getRequiredExtensions() {

         std::vector<const char*> extensions;

        extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
        extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);


        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    void initWindow() {

        
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = DefWindowProc; // assign default window procedure
        wc.hInstance = instanceHandle;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.lpszClassName = L"VulkanWindowClass";

        RegisterClassEx(&wc);

        RECT rect = { 0, 0, 800, 600 };
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

        hWnd = CreateWindowEx(
            0,
            wc.lpszClassName,
            L"Vulkan Window",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            rect.right - rect.left,
            rect.bottom - rect.top,
            nullptr,
            nullptr,
            instanceHandle,
            nullptr
        );

        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);
    }

    void initVulkan() {
        createInstance();
        setupDebugMessenger();
        pickPhysicalDevice();
    }   

    void createInstance() {
        //  check if validation layers are available
        if (enableValidationLayers && !checkValidationLayerSupport()) {
            printf("validation layers requested, but not available!\n");
            PostQuitMessage(1);
        }

        //  get a list of available extensions and print
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        printf("available extensions:\n");
        for (const auto& extension : extensions) {
            printf("\t %s \n", extension.extensionName);
        }

        //  vulkan app info
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        //  vulkan instance create info
        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        //  set create info validation layers if requested
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();

            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
        } else {
            createInfo.enabledLayerCount = 0;

            createInfo.pNext = nullptr;
        }


        //  get required extensions and enable them
        auto requiredExtensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();

        VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
        
        if (result == VK_ERROR_EXTENSION_NOT_PRESENT) {
            printf("VULKAN EXTENSION NOT PRESENT");
            PostQuitMessage(1);
        } else if (result != VK_SUCCESS) {
            printf("VULKAN CREATION FAILED...\n");
            PostQuitMessage(1);
        } else {
            printf("VULKAN CREATED OK!\n");
            
        }
    }

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }


    void setupDebugMessenger() {

        if (!enableValidationLayers) return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        populateDebugMessengerCreateInfo(createInfo);

        if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug messenger!");
        }

    }

    void pickPhysicalDevice() {

        uint32_t deviceCount = 0;
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        if (deviceCount == 0) {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        for (const auto& device : devices) {
            if (isDeviceSuitable(device)) {
                physicalDevice = device;
                break;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to find a suitable GPU!");
        }

        printf("Suitable GPU found!\n");
    }

    void mainLoop() {
        MSG msg;
        bool shouldQuit = false;
        while (!shouldQuit) {
            while (GetMessage( &msg, NULL, 0, 0 )) {

                TranslateMessage(&msg);
                

                switch (msg.message)
                {
                    case WM_WINDOWPOSCHANGED:
                        printf("NEW SIZE: %u, %u\n", LOWORD(msg.lParam), HIWORD(msg.lParam));
                        break;
                    case WM_KEYDOWN:
                        printf("KEYDOWN: \n");
                        break;
                    case WM_CLOSE:
                        shouldQuit = true;
                        break;
                    default:
                        break;
                }

                if (msg.message == WM_QUIT)
                shouldQuit = true;
                     //PostQuitMessage(0);
                    

            
                DispatchMessage(&msg);
                
            }

            //if (msg.message == WM_SIZE)
            //        printf("NEW SIZE: %u, %u\n", LOWORD(msg.lParam), HIWORD(msg.lParam));

            

            // Render frame
            // ...

            // Process input
            // ...
        }
    }
     
    void cleanup() {

        printf("cleanup\n");

        if (enableValidationLayers) {
            //DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        }

        vkDestroyInstance(instance, nullptr);

        DestroyWindow(hWnd);
        UnregisterClass(wc.lpszClassName, instanceHandle);

    }

    bool isDeviceSuitable(VkPhysicalDevice device) {
        return true;
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {

        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            //  message is important enough to show
            printf("validation layer: %s\n", pCallbackData->pMessage);
        }
        
        // return true to abort the current API call
        return VK_FALSE;
    }
};

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) { 
int main() {

    //instanceHandle = hInstance;
    instanceHandle = GetModuleHandle(nullptr);

    HelloTriangleApplication app;

    try {
        app.run();
    } catch (const std::exception& e) {
        printf("CRITICAL-ERROR: %s\n", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
    

    // Cleanup
