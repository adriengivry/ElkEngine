#pragma once

#include <map>
#include <unordered_map>
#include <memory>
#include <iostream>

#include "ElkGameEngine/export.h"
#include "ElkGameEngine/Objects/AObject.h"
#include "ElkGameEngine/Objects/Components/AComponent.h"
#include "ElkGameEngine/Utils/SharedContext.h"
#include "ElkGameEngine/Objects/Components/Transform.h"

namespace ElkGameEngine
{
	namespace Objects
	{
		/*
		 * A GameObject is an Object that can be placed in the Scene. It contains differents components
		 */
		class ELK_GAMEENGINE_EXPORT GameObject : public AObject
		{
		public:
			explicit GameObject(ElkGameEngine::Utils::SharedContext& p_sharedContext);
			explicit GameObject(ElkGameEngine::Utils::SharedContext& p_sharedContext, GameObject& p_parent, const std::string& p_name = "GameObject");
			virtual ~GameObject() = default;

			void Setup();

			std::shared_ptr<Objects::Components::Transform> transform;

			template<typename Class, typename...Args>
			std::shared_ptr<Class> AddComponent(Args... args);
			void AddChild(GameObject& p_child);

			template<typename Class>
			std::shared_ptr<Class> GetComponent();
			std::unordered_map<size_t, std::shared_ptr<Objects::Components::AComponent>>& GetComponents();
			std::vector <std::shared_ptr<GameObject>>& GetChilds();
			GameObject& GetParent() const;
			bool HasParent() const;

			void SetParent(GameObject& p_parent);

			Utils::SharedContext& m_sharedContext;
			std::unordered_map<size_t, std::shared_ptr<Objects::Components::AComponent>> m_components;
			GameObject* m_parent;
			std::vector<std::shared_ptr<GameObject>> m_childs;
		};

		template <typename Class, typename ... Args>
		std::shared_ptr<Class> GameObject::AddComponent(Args... args)
		{
			auto newComp = std::make_shared<Class>(std::forward<Args>(args)...);
			m_components[typeid(*newComp).hash_code()] = newComp;
			m_components[typeid(*newComp).hash_code()]->SetOwner(*this);
			return newComp;
		}

		template <typename Class>
		std::shared_ptr<Class> GameObject::GetComponent()
		{
			std::shared_ptr<Class> result(nullptr);

			if (m_components.count(typeid(Class).hash_code()) > 0)
				result = std::dynamic_pointer_cast<Class>(m_components[typeid(Class).hash_code()]);

			return result;
		}
	}
}
