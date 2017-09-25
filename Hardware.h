#ifndef __HARDWARE_H__
#define __HARDWARE_H__



#define DEFAULT_MUSIC_FADE_TIME 2.0f

class Hardware
{
public:

	struct hardware_info {
		char sdl_version[25] = "";
		float ram_gb = 0.f;
		int cpu_count = 0;
		int l1_cachekb = 0;
		int gpu_vendor = 0;
		int gpu_device = 0;
		char gpu_brand[250] = "";
		float vram_mb_budget = 0.f;
		float vram_mb_usage = 0.f;
		float vram_mb_available = 0.f;
		float vram_mb_reserved = 0.f;
	};

public:

	Hardware();
	~Hardware();

	const hardware_info& GetInfo() const;

private:

	mutable hardware_info info;
};

#endif // __MODULEHARDWARE_H__#pragma once
