#if defined IGRAPHICS_IMGUI && defined IGRAPHICS_METAL
#include "IGraphicsImGui.h"
#include "IGraphics_select.h"

#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>
#include "imgui_impl_metal.h"

void ImGuiRenderer::Init()
{
  ImGui_ImplMetal_Init(MTLCreateSystemDefaultDevice());
}

void ImGuiRenderer::Destroy()
{
  ImGui_ImplMetal_Shutdown();
}

void ImGuiRenderer::NewFrame()
{
  id<MTLCommandQueue> commandQueue = (__bridge id<MTLCommandQueue>) mnvgCommandQueue((NVGcontext*) mGraphics->GetDrawContext());
  id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
  
  id<CAMetalDrawable> drawable = [(__bridge CAMetalLayer*) mGraphics->GetCALayer() nextDrawable];
  
  MTLRenderPassDescriptor* pDesc = [MTLRenderPassDescriptor renderPassDescriptor];
  pDesc.colorAttachments[0].texture = [drawable texture];
  pDesc.colorAttachments[0].clearColor = MTLClearColorMake(1.f, 0.f, 0.f, 1.f);
  pDesc.colorAttachments[0].loadAction = MTLLoadActionClear; //
  pDesc.colorAttachments[0].storeAction = MTLStoreActionStore;
  
  id <MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:pDesc];
  
  [renderEncoder pushDebugGroup:@"ImGui"];
  
  ImGui_ImplMetal_NewFrame(pDesc);
  
  this->DoFrame();
  
  ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), commandBuffer, renderEncoder);
  [renderEncoder popDebugGroup];
  [renderEncoder endEncoding];
  
  [commandBuffer presentDrawable:drawable];
  [commandBuffer commit];
}

#include "imgui_impl_metal.mm"
#endif
