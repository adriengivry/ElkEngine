#pragma once

#include <string>

#include "ElkTools/exportElkTools.h"

namespace ElkTools
{
	namespace Utils
	{
		struct ELK_TOOLS_EXPORT SharedData
		{
			static std::string __CONFIG_FOLDER_PATH;
			static std::string __RESOURCE_MANAGERS_FOLDER_PATH;
		};
	}
}
