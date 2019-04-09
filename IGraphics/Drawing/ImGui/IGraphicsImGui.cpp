#if defined IGRAPHICS_IMGUI

#include "IPlugPlatform.h"

#ifdef OS_WIN
#include "IGraphicsImGui.h"
#include "IGraphicsNanoVG.h"

#include "imgui_impl_win32.h"

#if defined IGRAPHICS_GL2
  #include "imgui_impl_opengl2.h"
#elif defined IGRAPHICS_GL3
  #include "imgui_impl_opengl3.h"
#endif

ImGuiRenderer::ImGuiRenderer(void* pContext, IGraphicsNanoVG* pGraphics)
: mGraphics(pGraphics)
, mMTLLayer(pContext)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  ImGui::StyleColorsDark();
  
  ImGui_ImplWin32_Init(WindowFromDC((HDC) pContext));

#if defined IGRAPHICS_GL2
  ImGui_ImplOpenGL2_Init();
#elif defined IGRAPHICS_GL3
  ImGui_ImplOpenGL3_Init();
#endif
}

ImGuiRenderer::~ImGuiRenderer()
{
#ifdef OS_WIN
  ImGui_ImplWin32_Shutdown();
#endif
  
#ifdef IGRAPHICS_GL2
  ImGui_ImplOpenGL2_Shutdown();
#elif defined IGRAPHICS_GL3
  ImGui_ImplOpenGL3_Shutdown();
#endif
  
  ImGui::DestroyContext();
}

void ImGuiRenderer::BeginFrame()
{
#if defined IGRAPHICS_GL2
  ImGui_ImplOpenGL2_NewFrame();
#elif defined IGRAPHICS_GL2
  ImGui_ImplOpenGL3_NewFrame();
#endif

  ImGui_ImplWin32_NewFrame();
  
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags = 0; // disable kb
  io.DisplaySize.x = mGraphics->WindowWidth();
  io.DisplaySize.y = mGraphics->WindowHeight();
  int scale = mGraphics->GetScreenScale();
  io.DisplayFramebufferScale = ImVec2(scale, scale);
  io.DeltaTime = 1.f / mGraphics->FPS();
  ImGui::NewFrame();
  
  if(mGraphics->GetIMGUIFunc())
    mGraphics->GetIMGUIFunc()(mGraphics);
  
  ImGui::Render();
}

void ImGuiRenderer::EndFrame()
{
#if defined IGRAPHICS_GL2
  ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
#elif defined IGRAPHICS_GL2
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
}

#include "imgui_impl_win32.cpp"

#endif // OS_WIN

#include "imgui.cpp"
#include "imgui_widgets.cpp"
#include "imgui_draw.cpp"
#include "imgui_demo.cpp"

#if defined IGRAPHICS_GL
#if defined IGRAPHICS_GL2
#include "imgui_impl_opengl2.cpp"
#elif defined IGRAPHICS_GL3
#include "imgui_impl_opengl3.cpp"
#endif
#endif

#endif //IGRAPHICS_IMGUI
