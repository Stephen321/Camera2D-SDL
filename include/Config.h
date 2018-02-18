#ifndef CONFIG_H
#define CONFIG_H

#ifdef CAMERA2D_EXPORTS
#define CAMERA2D_API __declspec(dllexport)
#else
#define CAMERA2D_API __declspec(dllimport)
#endif

#endif