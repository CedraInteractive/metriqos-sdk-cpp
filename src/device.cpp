#include "internal.h"
#include <string>
#include <unordered_map>

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#elif defined(__APPLE__) || defined(__linux__)
#  include <sys/utsname.h>
#endif

#ifdef __APPLE__
#  include <sys/sysctl.h>
#endif

#ifdef __linux__
#  include <unistd.h>
#  include <fstream>
#endif

namespace Metriqos::Internal {

std::unordered_map<std::string, std::string> collectDeviceInfo() {
    std::unordered_map<std::string, std::string> info;

#ifdef _WIN32
    info["os"] = "Windows";

    SYSTEM_INFO si;
    GetSystemInfo(&si);
    info["cpu_cores"] = std::to_string(si.dwNumberOfProcessors);

    MEMORYSTATUSEX mem;
    mem.dwLength = sizeof(mem);
    if (GlobalMemoryStatusEx(&mem)) {
        info["ram_mb"] = std::to_string(mem.ullTotalPhys / (1024 * 1024));
    }

#elif defined(__APPLE__) || defined(__linux__)
    struct utsname uts;
    if (uname(&uts) == 0) {
        info["os"] = uts.sysname;
        info["os_version"] = uts.release;
        info["arch"] = uts.machine;
    }

#  ifdef __APPLE__
    int ncpu = 0;
    size_t len = sizeof(ncpu);
    if (sysctlbyname("hw.ncpu", &ncpu, &len, nullptr, 0) == 0) {
        info["cpu_cores"] = std::to_string(ncpu);
    }
    int64_t memsize = 0;
    len = sizeof(memsize);
    if (sysctlbyname("hw.memsize", &memsize, &len, nullptr, 0) == 0) {
        info["ram_mb"] = std::to_string(memsize / (1024 * 1024));
    }
#  endif

#  ifdef __linux__
    long ncpu = sysconf(_SC_NPROCESSORS_ONLN);
    if (ncpu > 0) {
        info["cpu_cores"] = std::to_string(ncpu);
    }
    long pages = sysconf(_SC_PHYS_PAGES);
    long pageSize = sysconf(_SC_PAGE_SIZE);
    if (pages > 0 && pageSize > 0) {
        info["ram_mb"] = std::to_string((static_cast<int64_t>(pages) * pageSize) / (1024 * 1024));
    }
#  endif
#endif

    return info;
}

} // namespace Metriqos::Internal
