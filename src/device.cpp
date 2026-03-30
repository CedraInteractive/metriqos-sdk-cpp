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

namespace Teliqos::Internal {

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

std::string getHardwareId() {
#ifdef _WIN32
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
            "SOFTWARE\\Microsoft\\Cryptography", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        char buffer[256] = {};
        DWORD size = sizeof(buffer);
        if (RegQueryValueExA(hKey, "MachineGuid", nullptr, nullptr,
                reinterpret_cast<LPBYTE>(buffer), &size) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            std::string id(buffer);
            if (!id.empty()) return id;
        }
        RegCloseKey(hKey);
    }
#elif defined(__APPLE__)
    FILE* pipe = popen("ioreg -rd1 -c IOPlatformExpertDevice | grep IOPlatformUUID", "r");
    if (pipe) {
        char buffer[256] = {};
        if (fgets(buffer, sizeof(buffer), pipe)) {
            std::string line(buffer);
            auto start = line.find('"', line.find("IOPlatformUUID"));
            if (start != std::string::npos) {
                start = line.find('"', start + 1);
                if (start != std::string::npos) {
                    auto end = line.find('"', start + 1);
                    if (end != std::string::npos) {
                        pclose(pipe);
                        return line.substr(start + 1, end - start - 1);
                    }
                }
            }
        }
        pclose(pipe);
    }
#elif defined(__linux__) && !defined(__ANDROID__)
    FILE* f = fopen("/etc/machine-id", "r");
    if (f) {
        char buffer[64] = {};
        if (fgets(buffer, sizeof(buffer), f)) {
            fclose(f);
            std::string id(buffer);
            while (!id.empty() && (id.back() == '\n' || id.back() == '\r'))
                id.pop_back();
            if (!id.empty()) return id;
        } else {
            fclose(f);
        }
    }
#endif
    return generateUUID();
}

} // namespace Teliqos::Internal
