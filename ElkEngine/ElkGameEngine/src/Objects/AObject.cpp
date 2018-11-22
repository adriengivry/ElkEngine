#include "stdafxGameEngine.h"

#include "ElkGameEngine/Objects/AObject.h"

using namespace ElkGameEngine::Objects;

uint64_t AObject::ID_INCREMENT = 0;

AObject::AObject()
{
	SetName("AObject");

	m_instanceID = ID_INCREMENT++;
}

uint64_t AObject::GetInstanceID() const
{
	return m_instanceID;
}

std::string& AObject::GetName()
{
	return m_name;
}

void AObject::SetName(const std::string& p_name)
{
	m_name = p_name;
}
