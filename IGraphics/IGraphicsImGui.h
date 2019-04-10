#pragma once

#if defined IGRAPHICS_IMGUI
#include "imgui.h"
#include "IGraphicsStructs.h"

class IGraphics;

class ImGuiRenderer
{
public:
  ImGuiRenderer(IGraphics* pGraphics);
  ~ImGuiRenderer();
  
  /** Initialise ImGui backend ( differing implementations for GL/METAL (see .mm) */
  void Init();
  
  /** Destroy ImGui backend ( differing implementations for GL/METAL (see .mm) */
  void Destroy();
  
  /** Per Frame ImGui backend-stuff ( differing implementations for GL/METAL (see .mm) */
  void NewFrame();
  
  /** Frame processing that is the same across platforms */
  void DoFrame();
  
  bool OnMouseDown(float x, float y, const IMouseMod &mod);
  bool OnMouseUp(float x, float y, const IMouseMod &mod);
  bool OnMouseWheel(float x, float y, const IMouseMod &mod, float delta);
  void OnMouseMove(float x, float y, const IMouseMod &mod);
  bool OnKeyDown(float x, float y, const IKeyPress &key);
  
private:
  IGraphics* mGraphics;
};
#endif
