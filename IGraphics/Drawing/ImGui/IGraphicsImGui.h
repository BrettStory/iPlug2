#pragma once

#if defined IGRAPHICS_IMGUI
#include "imgui.h"

#if defined IGRAPHICS_GL2
#include "imgui_impl_opengl2.h"
#define ImGui_Impl_Init() ImGui_ImplOpenGL2_Init()
#define ImGui_Impl_Shutdown() ImGui_ImplOpenGL2_Shutdown()
#define ImGui_Impl_RenderDrawData() ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData())
#define ImGui_Impl_NewFrame() ImGui_ImplOpenGL2_NewFrame()
#elif defined IGRAPHICS_GL3
#include "imgui_impl_opengl3.h"
#define ImGui_Impl_Init() ImGui_ImplOpenGL3_Init()
#define ImGui_Impl_Shutdown() ImGui_ImplOpenGL3_Shutdown()
#define ImGui_Impl_RenderDrawData() ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData())
#define ImGui_Impl_NewFrame() ImGui_ImplOpenGL3_NewFrame()
#elif defined IGRAPHICS_METAL && defined __OBJC__
#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>
#define ImGui_Impl_Init() ImGui_ImplMetal_Init(MTLCreateSystemDefaultDevice())
#define ImGui_Impl_Shutdown() ImGui_ImplMetal_Shutdown()
#define ImGui_Impl_NewFrame() ImGui_ImplMetal_NewFrame(pDesc)
#define ImGui_Impl_RenderDrawData() ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), commandBuffer, renderEncoder);
#include "imgui_impl_metal.h"
#endif

#include "IGraphicsStructs.h"
//#include "IGraphicsConstants.h"

class IGraphics;

class ImGuiRenderer
{
public:
  ImGuiRenderer(IGraphics* pGraphics);
  ~ImGuiRenderer();
  
  void BeginFrame();
  void EndFrame();
  bool OnMouseDown(float x, float y, const IMouseMod &mod);
  bool OnMouseUp(float x, float y, const IMouseMod &mod);
  bool OnMouseWheel(float x, float y, const IMouseMod &mod, float delta);
  void OnMouseMove(float x, float y, const IMouseMod &mod);
  bool OnKeyDown(float x, float y, const IKeyPress &key);
  
private:
  IGraphics* mGraphics;
};
#endif
