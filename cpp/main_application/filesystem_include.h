#ifndef DVS_FILESYSTEM_INCLUDE_H_
#define DVS_FILESYSTEM_INCLUDE_H_


#ifdef PLATFORM_LINUX_M
#include <experimental/filesystem>
namespace dvs_filesystem = std::experimental::filesystem;
#endif

#ifdef PLATFORM_APPLE_M
#include <filesystem>
namespace dvs_filesystem = std::filesystem;
#endif



#endif
