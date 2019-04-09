#include "IPlugPlatform.h"

#if defined IGRAPHICS_IMGUI

#include "IGraphicsImGui.h"
#include "IGraphicsNanoVG.h"

#if defined IGRAPHICS_GL2
  #include "imgui_impl_opengl2.h"
#elif defined IGRAPHICS_GL3
  #include "imgui_impl_opengl3.h"
#elif defined IGRAPHICS_METAL
  #import <Metal/Metal.h>
  #import <QuartzCore/QuartzCore.h>
  #include "imgui_impl_metal.h"
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
  
#if defined IGRAPHICS_GL2
  ImGui_ImplOpenGL2_Init();
#elif defined IGRAPHICS_METAL
  ImGui_ImplMetal_Init(id<MTLDevice> (mnvgDevice((NVGcontext*) mGraphics->GetDrawContext())));
#endif
}

ImGuiRenderer::~ImGuiRenderer()
{
#ifdef IGRAPHICS_GL2
  ImGui_ImplOpenGL2_Shutdown();
#elif defined IGRAPHICS_METAL
  ImGui_ImplMetal_Shutdown();
#endif
  
  ImGui::DestroyContext();
}

void ImGuiRenderer::BeginFrame()
{
#if defined IGRAPHICS_GL2
  ImGui_ImplOpenGL2_NewFrame();
#elif defined IGRAPHICS_METAL
//  id<MTLCommandBuffer> commandBuffer = [(id) mnvgCommandQueue((NVGcontext*) mGraphics->GetDrawContext()) commandBuffer];
//
//  id<CAMetalDrawable> drawable = [(CAMetalLayer*) mMTLLayer nextDrawable];
//
//  MTLRenderPassDescriptor* pDesc = [MTLRenderPassDescriptor renderPassDescriptor];
//  pDesc.colorAttachments[0].texture = [drawable texture];
//  pDesc.colorAttachments[0].clearColor = MTLClearColorMake(1.f, 1.f, 1.f, 1.f);
//  pDesc.colorAttachments[0].loadAction = MTLLoadActionLoad; //
//  pDesc.colorAttachments[0].storeAction = MTLStoreActionStore;
//
//  id <MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:pDesc];
//  mRenderEncoder = (void*) renderEncoder;
//
//  [renderEncoder pushDebugGroup:@"ImGui"];
//  ImGui_ImplMetal_NewFrame(pDesc);
#endif

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
#elif defined IGRAPHICS_METAL
//  id<MTLCommandBuffer> commandBuffer = [(id) mnvgCommandQueue((NVGcontext*) mGraphics->GetDrawContext()) commandBuffer];
//  id <MTLRenderCommandEncoder> renderEncoder = (id <MTLRenderCommandEncoder>) mRenderEncoder;
//  ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), commandBuffer, renderEncoder);
//  [renderEncoder popDebugGroup];
//  [renderEncoder endEncoding];
//  id<CAMetalDrawable> drawable = [(CAMetalLayer*) mMTLLayer nextDrawable];
//  [commandBuffer presentDrawable:drawable];
//  [commandBuffer commit];
#endif
}

#endif
