#include "ui/imgui-context.hpp"
#include "imgui.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <platform/log.hpp>

void ImGuiContext::Run(VulkanContext *v, platform::Window *w) {
    IMGUI_CHECKVERSION();
    TE_TRACE("ImGui version: {}", IMGUI_VERSION);
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    static_cast<void>(io);
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGuiStyle &style = ImGui::GetStyle();
    style.ScaleAllSizes(w->main_scale);
    style.FontScaleDpi = w->main_scale;

    ImGui_ImplGlfw_InitForVulkan(w->GetWindowHandle(), true);
    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance = v->GetInstance();
    initInfo.PhysicalDevice = v->GetPhysicalDevice();
    initInfo.Device = v->GetDevice();
    initInfo.QueueFamily = v->GetQueueFamily();
    initInfo.Queue = v->GetQueue();
    initInfo.PipelineCache = nullptr;
    initInfo.DescriptorPool = v->GetDescriptorPool();
    initInfo.MinImageCount = v->GetMinImageCount();
    initInfo.ImageCount = v->wd.ImageCount;
    initInfo.Allocator = nullptr;
    initInfo.PipelineInfoMain.RenderPass = v->wd.RenderPass;
    initInfo.PipelineInfoMain.Subpass = 0;
    initInfo.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    initInfo.CheckVkResultFn = nullptr;
    ImGui_ImplVulkan_Init(&initInfo);

    // Size for imgui window
    float imgui_height = 100.f;
    float imgui_width = 100.f;
    ImVec2 imgui_size(imgui_height, imgui_width);

    TE_TRACE("Imgui sucessfully initialized");
    while (!glfwWindowShouldClose(w->GetWindowHandle())) {
        glfwPollEvents();

        int fb_width;
        int fb_height;
        glfwGetFramebufferSize(w->GetWindowHandle(), &fb_width, &fb_height);
        if (fb_width > 0 && fb_height > 0) {
            v->ResizeSwapChain(fb_width, fb_height);
        }

        if (glfwGetWindowAttrib(w->GetWindowHandle(), GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplVulkan_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_NoResize);
            ImGui::SetNextWindowSize(imgui_size);
            if (ImGui::Button("Exit")) {
                glfwSetWindowShouldClose(w->GetWindowHandle(), GLFW_TRUE);
            }
            ImGui::End();
        }

        ImGui::Render();
        ImDrawData *draw_data = ImGui::GetDrawData();
        const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f ||
                                   draw_data->DisplaySize.y <= 0.0f);
        if (!is_minimized) {
            v->FrameRender(draw_data);
            v->FramePresent();
        }
    }
}

void ImGuiContext::Terminate() {
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
