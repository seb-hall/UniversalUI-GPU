#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vulkan/vulkan.h>

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




class HelloTriangleApplication {
public:
    VOID run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:

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

    void initWindow() {

        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = DefWindowProc; // assign default window procedure
        wc.hInstance = instanceHandle;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.lpszClassName = L"VulkanWindowClass";

        RegisterClassEx(&wc);

        RECT rect = { 0, 0, 800, 600 };
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

        HWND hWnd = CreateWindowEx(
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

        if (enableValidationLayers && !checkValidationLayerSupport()) {
            printf("validation layers requested, but not available!\n");
            PostQuitMessage(1);
        }

        // Vulkan instance creation
        VkInstance instance;

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
        
        if (result != VK_SUCCESS) {
            printf("VULKAN CREATION FAILED...\n");
            PostQuitMessage(1);
        } else {
            printf("VULKAN CREATED OK!\n");
            printf("available extensions:\n");
            for (const auto& extension : extensions) {
                printf("\t %s \n", extension.extensionName);
            }
        }

    }   

    void mainLoop() {
        MSG msg;
        while (true)
        {
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);

                if (msg.message == WM_QUIT)
                     PostQuitMessage(0);

            }

            switch (msg.message)
                {
                    case WM_CLOSE:
                        PostQuitMessage(0);
                        break;
                    default:
                        break;
                }

            // Render frame
            // ...

            // Process input
            // ...
        }
    }
     
    void cleanup() {

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
    //vkDestroyInstance(instance, nullptr);

    //DestroyWindow(hWnd);
    //UnregisterClass(wc.lpszClassName, hInstance);
