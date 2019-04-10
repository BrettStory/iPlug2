#if defined IGRAPHICS_IMGUI

#include "IPlugPlatform.h"
#include "IGraphics_select.h"
#include "IGraphicsImGui.h"

ImGuiRenderer::ImGuiRenderer(IGraphics* pGraphics)
: mGraphics(pGraphics)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  ImGui::StyleColorsDark();
  
  // Setup back-end capabilities flags
  //    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
  //    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
  //    io.BackendFlags |= ImGuiBackendFlags_HasMouseHoveredViewport; // We can set io.MouseHoveredViewport correctly (optional, not easy)
  io.BackendPlatformName = "imgui_impl_igraphics";
  
  io.KeyMap[ImGuiKey_Tab] = kVK_TAB;
  io.KeyMap[ImGuiKey_LeftArrow] = kVK_LEFT;
  io.KeyMap[ImGuiKey_RightArrow] = kVK_RIGHT;
  io.KeyMap[ImGuiKey_UpArrow] = kVK_UP;
  io.KeyMap[ImGuiKey_DownArrow] = kVK_DOWN;
  io.KeyMap[ImGuiKey_PageUp] = kVK_PRIOR;
  io.KeyMap[ImGuiKey_PageDown] = kVK_NEXT;
  io.KeyMap[ImGuiKey_Home] = kVK_HOME;
  io.KeyMap[ImGuiKey_End] = kVK_END;
  io.KeyMap[ImGuiKey_Insert] = kVK_INSERT;
  io.KeyMap[ImGuiKey_Delete] = kVK_DELETE;
  io.KeyMap[ImGuiKey_Backspace] = kVK_BACK;
  io.KeyMap[ImGuiKey_Space] = kVK_SPACE;
  io.KeyMap[ImGuiKey_Enter] = kVK_RETURN;
  io.KeyMap[ImGuiKey_Escape] = kVK_ESCAPE;
  io.KeyMap[ImGuiKey_A] = 'A';
  io.KeyMap[ImGuiKey_C] = 'C';
  io.KeyMap[ImGuiKey_V] = 'V';
  io.KeyMap[ImGuiKey_X] = 'X';
  io.KeyMap[ImGuiKey_Y] = 'Y';
  io.KeyMap[ImGuiKey_Z] = 'Z';
  
  //    io.SetClipboardTextFn = [](void*, const char* str) -> void
  //    {
  //    };
  //
  //    io.GetClipboardTextFn = [](void*) -> const char*
  //    {
  //    };
  
  
  ImGui_Impl_Init();
}

ImGuiRenderer::~ImGuiRenderer()
{
  ImGui_Impl_Shutdown();
  ImGui::DestroyContext();
}

static int64_t g_Time = 0;
static int64_t g_TicksPerSecond = 0;

void ImGuiRenderer::BeginFrame()
{  
  ImGui_Impl_NewFrame();
  
  ImGuiIO &io = ImGui::GetIO();
  io.DisplaySize.x = mGraphics->Width() * mGraphics->GetDrawScale();
  io.DisplaySize.y = mGraphics->Height() * mGraphics->GetDrawScale();
  int scale = mGraphics->GetScreenScale();
  io.DisplayFramebufferScale = ImVec2(scale, scale);
  io.DeltaTime = 1.f / mGraphics->FPS();
  
//  if (g_Time == 0.0)
//    g_Time = CFAbsoluteTimeGetCurrent();
//  CFAbsoluteTime current_time = CFAbsoluteTimeGetCurrent();
//  io.DeltaTime = current_time - g_Time;
//  g_Time = current_time;
//
  ImGui::NewFrame();
  
  if(mGraphics->GetIMGUIFunc())
    mGraphics->GetIMGUIFunc()(mGraphics);
  
  ImGui::Render();
}

void ImGuiRenderer::EndFrame()
{  
  ImGui_Impl_RenderDrawData();
}

bool ImGuiRenderer::OnMouseDown(float x, float y, const IMouseMod &mod)
{
  ImGuiIO &io = ImGui::GetIO();
  
  io.MouseDown[0] = mod.L;
  io.MouseDown[1] = mod.R;
  
  return io.WantCaptureMouse;
}

bool ImGuiRenderer::OnMouseUp(float x, float y, const IMouseMod &mod)
{
  ImGuiIO &io = ImGui::GetIO();
  
  io.MouseDown[0] = !mod.L;
  io.MouseDown[1] = !mod.R;
  
  return io.WantCaptureMouse;
}

bool ImGuiRenderer::OnMouseWheel(float x, float y, const IMouseMod &mod, float delta)
{
  ImGuiIO &io = ImGui::GetIO();
  io.MouseWheel += delta;
  
  return io.WantCaptureMouse;
}

void ImGuiRenderer::OnMouseMove(float x, float y, const IMouseMod &mod)
{
  ImGuiIO &io = ImGui::GetIO();
  io.MousePos = ImVec2(x * mGraphics->GetDrawScale(), y * mGraphics->GetDrawScale());
}

bool ImGuiRenderer::OnKeyDown(float x, float y, const IKeyPress &key)
{
  ImGuiIO &io = ImGui::GetIO();
  io.AddInputCharacter(key.Ascii);
  io.KeysDown[key.VK] = 1;
  
  return io.WantCaptureKeyboard;
}

#include "imgui.cpp"
#include "imgui_widgets.cpp"
#include "imgui_draw.cpp"
#include "imgui_demo.cpp"

//#if defined IGRAPHICS_GL
  #if defined IGRAPHICS_GL2
    #include "imgui_impl_opengl2.cpp"
  #elif defined IGRAPHICS_GL3
    #include "imgui_impl_opengl3.cpp"
  #endif
//#endif

#endif //IGRAPHICS_IMGUI
