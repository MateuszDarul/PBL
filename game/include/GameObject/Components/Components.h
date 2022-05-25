#ifndef __ALL_COMPONENTS_H__
#define __ALL_COMPONENTS_H__

#include "Component.h"
#include "NameComponent.h"              // UUID 1
#include "ListComponent.h"              // UUID 2
#include "ModelComponent.h"             // UUID 3
#include "ModelInstancesComponent.h"    // UUID 4
#include "ShaderComponent.h"            // UUID 5
#include "TransformComponent.h"         // UUID 6
#include "CameraComponent.h"            // UUID 7
#include "PointLightComponent.h"        // UUID 8
#include "SpotLightComponent.h"         // UUID 9
#include "FrustumCullingComponent.h"    // UUID 10

#include "SphereCollider.h"             // UUID 11
#include "BoxCollider.h"                // UUID 12
#include "TextComponent.h"              // UUID 13
#include "LineComponent.h"              // UUID 14
#include "ParticleComponent.h"          // UUID 15
#include "RefractiveGlassComponent.h"   // UUID 16

#include "ModelGuiComponent.h"          // UUID 50

#include "ScriptComponent.h"            // UUID 100

namespace cmp
{
    typedef NameComponent Name;
    typedef ListComponent List;
    typedef ModelComponent Model;
    typedef ModelInstancesComponent ModelInst;
    typedef ModelGuiComponent ModelGui;
    typedef ShaderComponent Shader;
    typedef TransformComponent Transform;
    typedef CameraComponent Camera;
    typedef PointLightComponent PointLight;
    typedef SpotLightComponent SpotLight;
    typedef FrustumCullingComponent FrustumCulling;
    typedef ParticleComponent Particles;
    typedef RefractiveGlassComponent Refract;

    typedef SphereCollider SphereCol;
    typedef BoxCollider BoxCol;
    typedef CameraComponent Camera;
    typedef TextComponent Text;
    typedef LineComponent Line;

    typedef ScriptComponent Scriptable;

} // namespace cmp

#endif // __ALL_COMPONENTS_H__