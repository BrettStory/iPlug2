#pragma once

#include "IPlugPlatform.h"

#if defined IGRAPHICS_IMGUI

#include "imgui.h"

class IGraphicsNanoVG;

class ImGuiRenderer
{
public:
  ImGuiRenderer(void* pContext, IGraphicsNanoVG* pGraphics);
  ~ImGuiRenderer();
  void BeginFrame();
  void EndFrame();

private:
  IGraphicsNanoVG* mGraphics;
  void* mMTLLayer;
  void* mRenderEncoder;
};
#endif
