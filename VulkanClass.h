#ifndef VULKAN_CLASS_H
#define VULKAN_CLASS_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

std::vector<char> readFile(const std::string& filename);

struct QueueFamilyIndices
{
  int graphicsFamily = -1;
  int presentFamily = -1;

  bool isComplete() {
    return graphicsFamily >= 0 && presentFamily >= 0;
  }
};

struct SwapChainSupportDetails
{
  VkSurfaceCapabilitiesKHR         capabilities;
  std::vector<VkSurfaceFormatKHR>  formats;
  std::vector<VkPresentModeKHR>    presentModes;
};

class HelloTriangleApplication {
public:

  void run();

private:

  void initWindow();
  void initVulkan();
  void mainLoop();
  void cleanup();

  void drawFrame();

  void createInstance();
  void setupDebugCallback();

  void pickPhysicalDevice();
  void createLogicalDevice();

  void createSurface();
  void createSwapChain();

  void createImageViews();
  void createGraphicsPipeline();

  void createRenderPass();

  void createFramebuffers();
  void createCommandPool();
  void createCommandBuffers();

  void createSemaphores();

  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
  QueueFamilyIndices findQueueFamilies( VkPhysicalDevice device );

  bool isDeviceSuitable(VkPhysicalDevice device);

  VkShaderModule createShaderModule(const std::vector<char>& code);

  static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
      VkDebugReportFlagsEXT flags,
      VkDebugReportObjectTypeEXT objType,
      uint64_t obj,
      size_t location,
      int32_t code,
      const char* layerPrefix,
      const char* msg,
      void* userData);

private:

    GLFWwindow*                    m_pWindow;
    VkInstance                     m_VkInstance;
    VkDebugReportCallbackEXT       m_callback;
    VkSurfaceKHR                   m_surface;

    VkPhysicalDevice               m_physicalDevice = VK_NULL_HANDLE;
    VkDevice                       m_device;
    VkQueue                        m_graphicsQueue;
    VkQueue                        m_presentQueue;

    VkSwapchainKHR                 m_swapChain;
    std::vector<VkImage>           m_rgSwapChainImages;
    std::vector<VkImageView>       m_rgSwapChainImageViews;
    VkFormat                       m_swapChainImageFormat;
    VkExtent2D                     m_swapChainExtent;

    VkRenderPass                   m_renderPass;
    VkPipelineLayout               m_pipelineLayout;

    VkPipeline                     m_graphicsPipeline;

    std::vector<VkFramebuffer>     m_rgSwapChainFramebuffers;

    VkCommandPool                  m_commandPool;
    std::vector<VkCommandBuffer>   m_rgCommandBuffers;

    VkSemaphore                    m_imageAvailableSemaphore;
    VkSemaphore                    m_renderFinishedSemaphore;
};

inline VkResult CreateDebugReportCallbackEXT(VkInstance instance,
                                             const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
                                             const VkAllocationCallbacks* pAllocator,
                                             VkDebugReportCallbackEXT* pCallback)
{
  auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance,"vkCreateDebugReportCallbackEXT");
  if (func != nullptr) {
      return func(instance, pCreateInfo, pAllocator, pCallback);
  } else {
      return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

inline void DestroyDebugReportCallbackEXT(VkInstance instance,
                                          VkDebugReportCallbackEXT callback,
                                          const VkAllocationCallbacks* pAllocator)
{
  auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
  if (func != nullptr) {
    func(instance, callback, pAllocator);
  }
}

#endif // VULKAN_CLASS_H
