#pragma once

/* MANAGERS */
#include "ElkGameEngine/Managers/EngineManager.h"
#include "ElkGameEngine/Managers/WindowManager.h"
#include "ElkGameEngine/Managers/SceneManager.h"
#include "ElkGameEngine/Managers/InputManager.h"

/* UTILS */
#include "ElkGameEngine/Utils/SharedContext.h"

/* OBJECTS */
#include "ElkGameEngine/Objects/GameObject.h"
#include "ElkGameEngine/Objects/Components/Behaviours/ACustomComponent.h"
#include "ElkGameEngine/Objects/Components/Behaviours/Physics/ACollider.h"
#include "ElkGameEngine/Objects/Components/Behaviours/Physics/Rigidbody.h"
#include "ElkGameEngine/Objects/Components/Behaviours/ModelRenderer.h"
#include "ElkGameEngine/Objects/Components/Behaviours/Camera.h"
#include "ElkGameEngine/Objects/Components/Behaviours/Light.h"
#include "ElkGameEngine/Objects/Components/Transform.h"
#include "ElkGameEngine/Objects/Components/Behaviours/Physics/BoxCollider.h"

/* TOOLS */
#include <ElkTools/Debug/Log.h>
#include <ElkTools/Debug/Assertion.h>
#include <ElkTools/Utils/Time.h>
#include <ElkTools/Utils/Benchmark.h>
#include <ElkTools/Utils/Profiler.h>
#include <ElkTools/Utils/Date.h>