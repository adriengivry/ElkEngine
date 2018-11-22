#pragma once

#include <ElkAPI/IDataStructure.h>

namespace ElkRendering
{
	namespace Data
	{
		struct Color : ElkAPI::IDataStructure
		{
			uint8_t red;
			uint8_t green;
			uint8_t blue;
			uint8_t alpha;

			// Color values between 0 and 255
			explicit Color(const uint8_t p_red = 255,
			      const uint8_t p_green = 255,
			      const uint8_t p_blue = 255,
			      const uint8_t p_alpha = 255)
				: red(p_red), green(p_green), blue(p_blue), alpha(p_alpha)
			{
			}

			// Color values between 0 and 1
			explicit Color(const glm::vec3& p_color)
			{
				SetColor(p_color);
				alpha = 255;
			}

			// Color values between 0 and 1
			explicit Color(const glm::vec4& p_color)
			{
				SetColor(p_color);
			}

			virtual ~Color() = default;

			void Setup() override
			{
			}

			// Color values between 0 and 1
			void SetColor(const glm::vec3& p_color)
			{
				red = static_cast<uint8_t>((p_color.r > 0 && p_color.r < 1) ? p_color.r * 255 : 255);
				green = static_cast<uint8_t>((p_color.g > 0 && p_color.g < 1) ? p_color.g * 255 : 255);
				blue = static_cast<uint8_t>((p_color.b > 0 && p_color.b < 1) ? p_color.b * 255 : 255);
			}

			// Color values between 0 and 1
			void SetColor(const glm::vec4& p_color)
			{
				red = static_cast<uint8_t>((p_color.r > 0 && p_color.r < 1) ? p_color.r * 255 : 255);
				green = static_cast<uint8_t>((p_color.g > 0 && p_color.g < 1) ? p_color.g * 255 : 255);
				blue = static_cast<uint8_t>((p_color.b > 0 && p_color.b < 1) ? p_color.b * 255 : 255);
				alpha = static_cast<uint8_t>((p_color.a > 0 && p_color.a < 1) ? p_color.a * 255 : 255);
			}

			// Color values between 0 and 1
			glm::vec3 GetColorAsVec3() const
			{
				return glm::vec3(red / 255, green / 255, blue / 255);
			}

			// Color values between 0 and 1
			glm::vec4 GetColorAsVec4() const
			{
				return glm::vec4(red / 255, green / 255, blue / 255, alpha / 255);
			}
		};
	}
}
