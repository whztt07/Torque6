//-----------------------------------------------------------------------------
// Copyright (c) 2015 Andrew Mac
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------


#ifndef SKY_LIGHT_COMPONENT_H
#define SKY_LIGHT_COMPONENT_H

#ifndef _CONSOLEINTERNAL_H_
#include "console/consoleInternal.h"
#endif

#ifndef _RENDERING_H_
#include "rendering/rendering.h"
#endif

#ifndef BGFX_H_HEADER_GUARD
#include <bgfx/bgfx.h>
#endif

#ifndef _BASE_COMPONENT_H_
#include "scene/components/baseComponent.h"
#endif

#ifndef _RENDER_CAMERA_H
#include "rendering/renderCamera.h"
#endif

#ifndef _CUBEMAP_PROCESSOR_H_
#include "lighting/cubemapProcessor.h"
#endif

#ifndef _SCENE_CORE_H_
#include "scene/scene.h"
#endif

namespace Scene
{
   class SkyLightFilter;

   // SkyLight
   class SkyLightComponent : public BaseComponent, public Rendering::RenderHook, public Scene::ScenePreprocessor
   {
      private:
         typedef BaseComponent Parent;

      protected:
         U32                        mState;
         Graphics::Shader*          mDiffuseShader;
         Graphics::Shader*          mSpecularShader;

         Rendering::RenderCamera*   mSkyLightCamera;
         SkyLightFilter*            mSkyLightCameraFilter;
         bgfx::TextureHandle        mSkyLightCubemap;
         bgfx::FrameBufferHandle    mSkyLightCubemapBuffers[6];
         U8                         mSkyLightCaptureSide;

         Lighting::CubemapProcessor*   mCubemapProcessor;
         bgfx::TextureHandle           mBRDFTexture;
         bgfx::TextureHandle           mIrradianceCubemap;
         bgfx::TextureHandle           mRadianceCubemap;

         void startSkyLightCapture();
         void captureSkyLight();
         void endSkyLightCapture();

         void initBuffers();
         void destroyBuffers();

      public:
         SkyLightComponent();
         ~SkyLightComponent();

         // BaseComponent
         void onAddToScene();
         void onRemoveFromScene();
         void refresh();

         // Rendering::RenderHook
         void preRender(Rendering::RenderCamera* camera);
         void render(Rendering::RenderCamera* camera);
         void postRender(Rendering::RenderCamera* camera);
         void resize();

         // Scene::ScenePreprocessor
         void preprocess();

         static void initPersistFields();

         DECLARE_CONOBJECT(SkyLightComponent);
   };

   class SkyLightFilter : public Rendering::RenderFilter
   {
      public:
         SkyLightFilter();
         void execute();
   };
}

#endif // SKY_LIGHT_COMPONENT_H