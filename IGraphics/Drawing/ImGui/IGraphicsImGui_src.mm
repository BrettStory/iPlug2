#if defined IGRAPHICS_IMGUI
#include "imgui_impl_metal.mm"
#include "imgui_impl_osx.mm"

#if defined IGRAPHICS_METAL
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
#endif


#if defined IGRAPHICS_METAL
//  id<MTLCommandBuffer> commandBuffer = [(id) mnvgCommandQueue((NVGcontext*) mGraphics->GetDrawContext()) commandBuffer];
//  id <MTLRenderCommandEncoder> renderEncoder = (id <MTLRenderCommandEncoder>) mRenderEncoder;
//  ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), commandBuffer, renderEncoder);
//  [renderEncoder popDebugGroup];
//  [renderEncoder endEncoding];
//  id<CAMetalDrawable> drawable = [(CAMetalLayer*) mMTLLayer nextDrawable];
//  [commandBuffer presentDrawable:drawable];
//  [commandBuffer commit];
#endif

#endif
