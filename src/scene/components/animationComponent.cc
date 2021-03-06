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

#include "console/consoleTypes.h"
#include "animationComponent.h"
#include "graphics/core.h"
#include "scene/object.h"
#include "scene/components/meshComponent.h"
#include "scene/scene.h"
#include "game/gameProcess.h"

// Script bindings.
#include "animationComponent_Binding.h"

// Debug Profiling.
#include "debug/profiler.h"

// bgfx/bx
#include <bgfx/bgfx.h>
#include <bx/fpumath.h>

// Assimp - Asset Import Library
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/types.h>

namespace Scene
{
   IMPLEMENT_CONOBJECT(AnimationComponent);

   AnimationComponent::AnimationComponent()
   {
      mAnimationIndex   = 0;
      mAnimationTime    = 0.0f;
      mSpeed            = 1.0f;
      mTargetName       = StringTable->EmptyString;
   }

   void AnimationComponent::initPersistFields()
   {
      Parent::initPersistFields();

      addGroup("AnimationComponent");
         addField("Speed", TypeF32, Offset(mSpeed, AnimationComponent), "");
         addField("Target", TypeString, Offset(mTargetName, AnimationComponent), "");
         addProtectedField("MeshAsset", TypeAssetId, Offset(mMeshAssetId, AnimationComponent), &setMeshField, &defaultProtectedGetFn, "The asset Id of the mesh to animate."); 
         addProtectedField("AnimationIndex", TypeS32, Offset(mAnimationIndex, AnimationComponent), &setAnimationIndexField, &defaultProtectedGetFn, "The index of animation.");
      endGroup("AnimationComponent");
   }

   void AnimationComponent::onAddToScene()
   {  
      // Load Target
      if ( mTargetName != StringTable->EmptyString )
         mTarget = dynamic_cast<MeshComponent*>(mOwnerObject->findComponent(mTargetName));

      if (mMeshAsset->isLoaded())
      {
         mOwnerObject->setProcessTick(true);
         if (mOwnerObject->isClientObject())
            ClientProcessList::get()->addObject(mOwnerObject);

         setProcessTicks(true);
      }
   }

   void AnimationComponent::onRemoveFromScene()
   {  
      //setProcessTicks(false);
   }

   void AnimationComponent::setMesh( const char* pImageAssetId )
   {
      // Sanity!
      AssertFatal( pImageAssetId != NULL, "Cannot use a NULL asset Id." );

      // Fetch the asset Id.
      mMeshAssetId = StringTable->insert(pImageAssetId);
      mMeshAsset.setAssetId(mMeshAssetId);

      if ( mMeshAsset.isNull() )
         Con::errorf("[AnimationComponent] Failed to load mesh asset.");
   }

   void AnimationComponent::setAnimationIndex(U32 index)
   {
      mAnimationIndex = index;
   }

   Vector<StringTableEntry> AnimationComponent::getAnimationNames()
   {
      return mMeshAsset->getAnimationNames();
   }

   void AnimationComponent::interpolateMove( F32 delta )
   {  
      // Unused at the moment.
   }

   void AnimationComponent::processMove(const Move* move)
   {  
      if ( !mTarget.isNull() )
      {
         mTarget->mTransformCount = mMeshAsset->getAnimatedTransforms(mAnimationIndex, mAnimationTime, mTarget->mTransformTable[1]) + 1;
         mTarget->refreshTransforms();
      }
   }

   void AnimationComponent::advanceMove( F32 timeDelta )
   {  
      mAnimationTime += (timeDelta * mSpeed);
   }

   void AnimationComponent::interpolateTick(F32 delta)
   {

   }

   void AnimationComponent::processTick()
   {

   }

   void AnimationComponent::advanceTime(F32 timeDelta)
   {
      mAnimationTime += (timeDelta * mSpeed);
   }
}
