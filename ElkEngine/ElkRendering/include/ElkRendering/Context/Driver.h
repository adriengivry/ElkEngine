#pragma once

#include <ElkAPI\IContext.h>

#include "ElkRendering/exportRendering.h"

namespace ElkRendering
{
	namespace Context
	{
		/*
		* The Driver context handle the OpenGL Context. Actually it uses GLEW Libarary
		*/
		class ELK_RENDERING_EXPORT Driver : public ElkAPI::IContext
		{
		public:
			Driver();
			virtual ~Driver() = default;
			void Setup() override;
			void Close() override {}
			bool IsActive() override;

			void DisplayDriverInfo();

			void ClearScreen();

		private:
			bool m_active;
		};
	}
}
