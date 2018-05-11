#pragma once

#include <stack>

/*

 AGG 2.4 should be modified to avoid bringing carbon headers on mac, which can cause conflicts

 in "agg_mac_pmap.h" ...
 //#include <ApplicationServices/ApplicationServices.h>
 #include <CoreGraphics/CoreGraphics.h>

 */

#include "IGraphicsPathBase.h"
#include "IGraphicsAGG_src.h"

template <class SpanGeneratorType>
class alpha_span_generator : public SpanGeneratorType
{
public:
  
  alpha_span_generator(typename SpanGeneratorType::source_type& source, typename SpanGeneratorType::interpolator_type& interpolator, agg::cover_type a)
  : SpanGeneratorType(source, interpolator), alpha(a) {}
  
  void generate(typename SpanGeneratorType::color_type* span, int x, int y, unsigned len)
  {
    SpanGeneratorType::generate(span, x, y, len);
    
    if (alpha != 255)
    {
      for (unsigned i = 0; i < len; i++, span++)
        span->a = (span->a * alpha + SpanGeneratorType::base_mask) >> SpanGeneratorType::base_shift;
    }
  }
  
private:
  
  agg::cover_type alpha;
};

class AGGBitmap : public APIBitmap
{
public:
  AGGBitmap(agg::pixel_map* pPixMap, int scale) : APIBitmap (pPixMap, pPixMap->width(), pPixMap->height(), scale) {}
  virtual ~AGGBitmap() { delete ((agg::pixel_map*) GetBitmap()); }
};

/** IGraphics draw class using Antigrain Geometry
*   @ingroup DrawClasses*/
class IGraphicsAGG : public IGraphicsPathBase
{
public:
  struct LineInfo
  {
    int mStartChar;
    int mEndChar;
    double mWidth;
    LineInfo() : mWidth(0.0), mStartChar(0), mEndChar(0) {}
  };

#ifdef OS_WIN
  typedef agg::order_bgra PixelOrder;
  //TODO: Map type
#else
  typedef agg::order_argb PixelOrder;
  typedef agg::pixel_map_mac PixelMapType;
#endif
  typedef agg::comp_op_adaptor_rgba<agg::rgba8, PixelOrder> BlenderType;
  typedef agg::comp_op_adaptor_rgba_pre<agg::rgba8, PixelOrder> BlenderTypePre;
  typedef agg::pixfmt_custom_blend_rgba<BlenderType, agg::rendering_buffer> PixfmtType;
  typedef agg::renderer_base <PixfmtType> RenbaseType;
  typedef agg::renderer_scanline_aa_solid<RenbaseType> RendererSolid;
  typedef agg::renderer_scanline_bin_solid<RenbaseType> RendererBin;
  typedef agg::font_engine_freetype_int32 FontEngineType;
  typedef agg::font_cache_manager <FontEngineType> FontManagerType;
  typedef agg::span_interpolator_linear<> InterpolatorType;
  typedef agg::image_accessor_clone<PixfmtType> imgSourceType;
  typedef agg::span_allocator<agg::rgba8> SpanAllocatorType;
  typedef agg::span_image_filter_rgba_bilinear<imgSourceType, InterpolatorType> SpanGeneratorType;
  typedef alpha_span_generator<SpanGeneratorType> SpanAlphaGeneratorType;
  typedef agg::renderer_scanline_aa<RenbaseType, SpanAllocatorType, SpanGeneratorType> BitmapRenderType;
  typedef agg::renderer_scanline_aa<RenbaseType, SpanAllocatorType, alpha_span_generator<SpanGeneratorType> > BitmapAlphaRenderType;

  typedef agg::renderer_base<agg::pixfmt_gray8> maskRenBase;

  // Path Types

  typedef agg::path_storage PathType;
  typedef agg::conv_curve<PathType> CurvedPathType;
  typedef agg::conv_stroke<CurvedPathType> StrokeType;
  typedef agg::conv_dash<CurvedPathType> DashType;
  typedef agg::conv_stroke<DashType> DashStrokeType;
  //typedef agg::conv_transform<PathType> TransformedPathType;
  //typedef agg::conv_transform<StrokeType> TransformedStrokePathType;
  //typedef agg::conv_transform<DashStrokeType> TransformedDashStrokePathType;
  //typedef agg::conv_curve<TransformedPathType> CurvedTransformedPathType;
  typedef agg::rasterizer_scanline_aa<> RasterizerType;
  typedef agg::gradient_lut<agg::color_interpolator<agg::rgba8>, 512> ColorArrayType;

  class Rasterizer
  {
  public:

    RenbaseType& GetBase() { return mRenBase; }

    agg::rgba8 GetPixel(int x, int y) { return mRenBase.pixel(x, y); }

    void ClearWhite() { mRenBase.clear(agg::rgba(1, 1, 1)); }

    void SetOutput(agg::rendering_buffer& renBuf)
    {
      mPixf = PixfmtType(renBuf);
      mRenBase = RenbaseType(mPixf);
      mRenBase.clear(agg::rgba(0, 0, 0, 0));
    }

    template <typename VertexSourceType>
    void Rasterize(VertexSourceType& path, agg::trans_affine transform, const IPattern& pattern,const IBlend* pBlend = nullptr, EFillRule rule = kFillWinding)
    {
      SetPath(path);
      RasterizePattern(transform, pattern, pBlend, rule);
    }

    template <typename RendererType>
    void Rasterize(RendererType& renderer, agg::comp_op_e op)
    {
      agg::scanline_p8 scanline;
      mPixf.comp_op(op);
      agg::render_scanlines(mRasterizer, scanline, renderer);
    }
    
    void BlendFrom(agg::rendering_buffer& renBuf, const IRECT& bounds, int srcX, int srcY, agg::comp_op_e op, agg::cover_type cover)
    {
      mPixf.comp_op(op);
      agg::rect_i r(srcX, srcY, srcX + bounds.W(), srcY + bounds.H());
      mRenBase.blend_from(PixfmtType(renBuf), &r, bounds.L - srcX, bounds.T - srcY, cover);
    }

    template <typename VertexSourceType>
    void SetPath(VertexSourceType& path)
    {
      mRasterizer.reset();
      mRasterizer.add_path(path);
    }

    void RasterizePattern(agg::trans_affine transform, const IPattern& pattern,const IBlend* pBlend = nullptr, EFillRule rule = kFillWinding);

    RenbaseType mRenBase;
    PixfmtType mPixf;
    RasterizerType mRasterizer;
  };

  IGraphicsAGG(IDelegate& dlg, int w, int h, int fps);
  ~IGraphicsAGG();

  void SetDisplayScale(int scale) override;

  void Draw(const IRECT& bounds) override;

  void DrawBitmap(IBitmap& bitmap, const IRECT& dest, int srcX, int srcY, const IBlend* pBlend) override;
  void DrawRotatedMask(IBitmap& base, IBitmap& mask, IBitmap& top, int x, int y, double angle, const IBlend* pBlend) override;

  void PathClear() override { mPath.remove_all(); }
  void PathStart() override { mPath.start_new_path(); }
  void PathClose() override { mPath.close_polygon(); }

  void PathArc(float cx, float cy, float r, float aMin, float aMax) override;

  void PathMoveTo(float x, float y) override;
  void PathLineTo(float x, float y) override;
  void PathCurveTo(float x1, float y1, float x2, float y2, float x3, float y3) override;

  void PathStroke(const IPattern& pattern, float thickness, const IStrokeOptions& options, const IBlend* pBlend) override;
  void PathFill(const IPattern& pattern, const IFillOptions& options, const IBlend* pBlend) override;

  void PathStateSave() override { mState.push(mTransform); }

  void PathStateRestore() override
  {
    mTransform = mState.top();
    mState.pop();
  }

  void PathTransformTranslate(float x, float y) override { mTransform = agg::trans_affine_translation(x, y) * mTransform; }
  void PathTransformScale(float scaleX, float scaleY) override { mTransform = agg::trans_affine_scaling(scaleX, scaleY) * mTransform; }
  void PathTransformRotate(float angle) override { mTransform = agg::trans_affine_rotation(DegToRad(angle)) * mTransform; }

  bool DrawText(const IText& text, const char* str, IRECT& bounds, bool measure = false) override;
  bool MeasureText(const IText& text, const char* str, IRECT& bounds) override;

  IColor GetPoint(int x, int y) override;
  void* GetData() override { return 0; } //TODO
  const char* GetDrawingAPIStr() override { return "AGG"; }

 //  IBitmap CreateIBitmap(const char * cacheName, int w, int h) override;

  void RenderDrawBitmap() override;

private:

  APIBitmap* LoadAPIBitmap(const WDL_String& resourcePath, int scale) override;
  agg::pixel_map* CreateAPIBitmap(int w, int h);
  APIBitmap* ScaleAPIBitmap(const APIBitmap* pBitmap, int s) override;

  void CalculateTextLines(WDL_TypedBuf<LineInfo>* pLines, const IRECT& bounds, const char* str, FontManagerType& manager);

  agg::trans_affine GetRasterTransform() { return agg::trans_affine() / (mTransform * agg::trans_affine_scaling(GetDisplayScale())); }

  template<typename PathType> void DoClip(PathType& path)
  {
    IRECT clip = mClipRECT.Empty() ? GetBounds() : mClipRECT;
    clip.Scale(GetDisplayScale());
    path.clip_box(clip.L, clip.T, clip.R, clip.B);
  }
  
  void ClipRegion(const IRECT& r) override { mClipRECT = r; }
  void ResetClipRegion() override { mClipRECT = IRECT(); }

  IRECT mClipRECT;
  FontEngineType mFontEngine;
  FontManagerType mFontManager;
  agg::rendering_buffer mRenBuf;
  PathType mPath;
  Rasterizer mRasterizer;
  agg::trans_affine mTransform;
  PixelMapType mPixelMap;

  // TODO Oli probably wants this to not be STL but there's nothing in WDL for this...
  std::stack<agg::trans_affine> mState;

  //pipeline to process the vectors glyph paths(curves + contour)
  agg::conv_curve<FontManagerType::path_adaptor_type> mFontCurves;
  agg::conv_contour<agg::conv_curve<FontManagerType::path_adaptor_type> > mFontContour;
};