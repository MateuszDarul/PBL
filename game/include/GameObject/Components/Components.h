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
#include "FrustumCullingComponent.h"    // UUID 10

#include "SphereCollider.h"             // UUID 11
#include "BoxCollider.h"                // UUID 12

#include "ScriptComponent.h"            // UUID 100

namespace cmp
{
    typedef NameComponent Name;
    typedef ListComponent List;
    typedef ModelComponent Model;
    typedef ModelInstancesComponent ModelInst;
    typedef ShaderComponent Shader;
    typedef TransformComponent Transform;
    typedef CameraComponent Camera;
    typedef PointLightComponent PointLight;
    typedef FrustumCullingComponent FrustumCulling;

    typedef SphereCollider SphereCol;
    typedef BoxCollider BoxCol;

    typedef ScriptComponent Scriptable;

} // namespace cmp

#endif // __ALL_COMPONENTS_H__