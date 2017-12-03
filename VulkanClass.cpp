#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <cstring>
#include <algorithm>

#include "VulkanClass.h"

VKAPI_ATTR VkBool32 VKAPI_CALL HelloTriangleApplication::debugCallback(
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT objType,
    uint64_t obj,
    size_t location,
    int32_t code,
    const char* layerPrefix,
    const char* msg,
    void* userData)
{
  std::cerr << "validation layer: " << msg << std::endl;
  return VK_FALSE;
}

void HelloTriangleApplication::run()
{
  initWindow();
  initVulkan();
  mainLoop();
  cleanup();
}

void HelloTriangleApplication::mainLoop()
{
  while ( !glfwWindowShouldClose(m_pWindow) )
  {
    glfwPollEvents();
    drawFrame();
  }

  vkDeviceWaitIdle(m_device);
}

void HelloTriangleApplication::drawFrame()
{
  vkQueueWaitIdle(m_presentQueue);

  uint32_t imageIndex;
  vkAcquireNextImageKHR(m_device, m_swapChain, std::numeric_limits<uint64_t>::max(),
                        m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

  VkSubmitInfo submitInfo = {};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = {m_imageAvailableSemaphore};
  VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &m_rgCommandBuffers[imageIndex];

  VkSemaphore signalSemaphores[] = {m_renderFinishedSemaphore};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  if (vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
    throw std::runtime_error("failed to submit draw command buffer!");
  }

  VkPresentInfoKHR presentInfo = {};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;

  VkSwapchainKHR swapChains[] = {m_swapChain};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &imageIndex;
  presentInfo.pResults = nullptr; // Optional

  vkQueuePresentKHR(m_presentQueue, &presentInfo);
}

void HelloTriangleApplication::cleanup()
{
  vkDestroySemaphore(m_device, m_renderFinishedSemaphore, nullptr);
  vkDestroySemaphore(m_device, m_imageAvailableSemaphore, nullptr);

  vkDestroyCommandPool(m_device, m_commandPool, nullptr);

  for (size_t i = 0; i < m_rgSwapChainFramebuffers.size(); i++) {
    vkDestroyFramebuffer(m_device, m_rgSwapChainFramebuffers[i], nullptr);
  }

  vkDestroyRenderPass(m_device, m_renderPass, nullptr);
  vkDestroyPipeline(m_device, m_graphicsPipeline, nullptr);
  vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);

  for (size_t imageViewID = 0; imageViewID < m_rgSwapChainImageViews.size(); ++imageViewID) {
    vkDestroyImageView(m_device, m_rgSwapChainImageViews[imageViewID], nullptr);
  }

  vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);
  vkDestroyDevice(m_device, nullptr);
  DestroyDebugReportCallbackEXT(m_VkInstance, m_callback, nullptr);
  vkDestroySurfaceKHR(m_VkInstance, m_surface, nullptr);

  vkDestroyInstance(m_VkInstance, nullptr);

  glfwDestroyWindow(m_pWindow);
  glfwTerminate();
}
