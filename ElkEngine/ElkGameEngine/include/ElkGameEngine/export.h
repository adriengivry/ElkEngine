#pragma once

#ifdef ELK_GAMEENGINE_EXPORT_DLL
#define ELK_GAMEENGINE_EXPORT __declspec(dllexport)
#else
#define ELK_GAMEENGINE_EXPORT __declspec(dllimport)
#endif // ELK_GAMEENGINE_EXPORT_DLL