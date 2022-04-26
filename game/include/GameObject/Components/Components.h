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
#include "TextComponent.h"              // UUID 13
#include "LineComponent.h"              // UUID 14

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
    typedef TextComponent Text;
    typedef LineComponent Line;

    typedef ScriptComponent Scriptable;

} // namespace cmp

#endif // __ALL_COMPONENTS_H__