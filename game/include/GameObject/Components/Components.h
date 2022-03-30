#ifndef __ALL_COMPONENTS_H__
#define __ALL_COMPONENTS_H__

#include "Component.h"
#include "NameComponent.h"              // UUID 1
#include "ListComponent.h"              // UUID 2
#include "ModelComponent.h"             // UUID 3
#include "ModelInstancesComponent.h"    // UUID 4
#include "ShaderComponent.h"            // UUID 5
#include "TransformComponent.h"         // UUID 6

namespace cmp
{

    typedef NameComponent Name;
    typedef ListComponent List;
    typedef ModelComponent Model;
    typedef ModelInstancesComponent ModelInst;
    typedef ShaderComponent Shader;
    typedef TransformComponent Transform;

} // namespace cmp

#endif // __ALL_COMPONENTS_H__