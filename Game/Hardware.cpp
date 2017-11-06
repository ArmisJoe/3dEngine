#include "Globals.h"
#include "Hardware.h"
#include "gpudetect/DeviceId.h"
#include "SDL/include/SDL.h"

Hardware::Hardware()
{
	SDL_version version;
	SDL_GetVersion(&version);

	sprintf_s(info.sdl_version, 25, "%i.%i.%i", version.major, version.minor, version.patch);
	info.cpu_count = SDL_GetCPUCount();
	info.l1_cachekb = SDL_GetCPUCacheLineSize();
	info.ram_gb = (float)SDL_GetSystemRAM() / (1024.f);

	uint vendor, device_id;
	std::wstring brand;
	unsigned __int64 video_mem_budget;
	unsigned __int64 video_mem_usage;
	unsigned __int64 video_mem_available;
	unsigned __int64 video_mem_reserved;

	if (getGraphicsDeviceInfo(&vendor, &device_id, &brand, &video_mem_budget, &video_mem_usage, &video_mem_available, &video_mem_reserved))
	{
		info.gpu_vendor = vendor;
		info.gpu_device = device_id;
		sprintf_s(info.gpu_brand, 250, "%S", brand.c_str());
		info.vram_mb_budget = float(video_mem_budget) / 1073741824.0f;
		info.vram_mb_usage = float(video_mem_usage) / (1024.f * 1024.f * 1024.f);
		info.vram_mb_available = float(video_mem_available) / (1024.f * 1024.f * 1024.f);
		info.vram_mb_reserved = float(video_mem_reserved) / (1024.f * 1024.f * 1024.f);
	}
}

// Destructor
Hardware::~Hardware()
{}

const Hardware::hardware_info & Hardware::GetInfo() const
{
	unsigned __int64 video_mem_budget;
	unsigned __int64 video_mem_usage;
	unsigned __int64 video_mem_available;
	unsigned __int64 video_mem_reserved;

	if (getGraphicsDeviceInfo(nullptr, nullptr, nullptr, &video_mem_budget, &video_mem_usage, &video_mem_available, &video_mem_reserved))
	{
		info.vram_mb_budget = float(video_mem_budget) / (1024.f * 1024.f);
		info.vram_mb_usage = float(video_mem_usage) / (1024.f * 1024.f);
		info.vram_mb_available = float(video_mem_available) / (1024.f * 1024.f);
		info.vram_mb_reserved = float(video_mem_reserved) / (1024.f * 1024.f);
	}

	return info;
}
