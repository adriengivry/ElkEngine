#include "stdafx.h"

#include "ElkPhysics/Data/Colliders/AColliderData.h"
#include "ElkPhysics/Data/Colliders/SphereColliderData.h"
#include "ElkPhysics/Data/Colliders/BoxColliderData.h"

using namespace ElkPhysics::Data::Colliders;

AColliderData::AColliderData(ColliderType p_type) : type(p_type)
{}