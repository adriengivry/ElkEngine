#pragma once

#include "ElkGameEngine/export.h"
#include "ElkGameEngine/Utils/SharedContext.h"
#include "ElkGameEngine/Objects/AObject.h"
#include "ElkGameEngine/Objects/GameObject.h"

namespace ElkGameEngine
{
	namespace Objects
	{
		/*
		 * Class that contain a root gameObject which can have some childs
		 */
		class ELK_GAMEENGINE_EXPORT Scene : public AObject
		{
		public:
			explicit Scene(ElkGameEngine::Utils::SharedContext& p_sharedContext);
			~Scene() = default;

			ElkGameEngine::Objects::GameObject& GetRoot() const;

		private:
			Utils::SharedContext&								m_sharedContext;
			std::shared_ptr<ElkGameEngine::Objects::GameObject> m_root;
		};
	}
}
