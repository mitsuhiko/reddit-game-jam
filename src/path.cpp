#include <pd/path.hpp>
#include <errno.h>
#if PD_PLATFORM == PD_PLATFORM_OSX
#   include <unistd.h>
#   include <CoreFoundation/CoreFoundation.h>
#   include <sys/stat.h>
#elif PD_PLATFORM == PD_PLATFORM_WINDOWS
#   pragma comment(lib, "shlwapi")
#   include "shlobj.h"
#   include "shlwapi.h"
#   include <sys/types.h>
#   include <sys/stat.h>
#   define stat _stat
#endif


std::string pd::posixpath::join(const std::string &a, const std::string &b)
{
    if (!b.empty() && b[0] == '/')
        return b;
    std::string rv = a;
    if (!rv.empty() && rv[rv.size() - 1] != '/')
        rv += "/";
    return rv + b;
}

std::string pd::posixpath::join_with_dir(const std::string &a, const std::string &b)
{
    return join(dirname(a), b);
}

std::string pd::posixpath::dirname(const std::string &path)
{
    size_t pos = path.find_last_of('/');
    if (pos == std::string::npos)
        return ".";
    return path.substr(0, pos);
}

std::string pd::ntpath::join(const std::string &a, const std::string &b)
{
    if (!b.empty() && (b[0] == '/' || b[0] == '\\'))
        return b;
    std::string rv = a;
    if (!rv.empty() && rv[rv.size() - 1] != '/' && rv[rv.size() - 1] != '\\')
        rv += "/";
    return rv + b;
}

std::string pd::ntpath::join_with_dir(const std::string &a, const std::string &b)
{
    return join(dirname(a), b);
}

std::string pd::ntpath::dirname(const std::string &path)
{
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos)
        return ".";
    return path.substr(0, pos);
}

const std::string &pd::path::get_resource_path()
{
    static std::string path_cache;
    if (!path_cache.empty())
        return path_cache;
#if PD_PLATFORM == PD_PLATFORM_OSX
    char *path = new char[4096];
    CFBundleRef main_bundle = CFBundleGetMainBundle();
    CFURLRef main_bundle_url = CFBundleCopyBundleURL(main_bundle);
    CFStringRef cf_stringref = CFURLCopyFileSystemPath(main_bundle_url,
                                                       kCFURLPOSIXPathStyle);
    CFStringGetCString(cf_stringref, path, 4000, kCFStringEncodingASCII);
    CFRelease(main_bundle_url);
    CFRelease(cf_stringref);
    strcat(path, "/Contents/Resources");
    path_cache = path;
    delete[] path;
#elif PD_PLATFORM == PD_PLATFORM_WINDOWS
    char *pathptr, *path = new char[MAX_PATH + 20];
    DWORD length = GetModuleFileNameA(0, path, MAX_PATH);
    while (1) {
        if ((pathptr = strrchr(path, '\\')) == 0) {
            pd::critical_error("IO Error", "Could not find resource folder");
            return path_cache; /* not reached */
        }
        strcpy(pathptr, "\\resources");
        if (PathIsDirectoryA(path))
            break;
        *pathptr = '\0';
    }
    path_cache = path;
    delete[] path;
#else
#   error "Unsupported platform in get_resource_path"
#endif
    return path_cache;
}

void pd::path::set_cwd(const std::string &path)
{
#if PD_PLATFORM == PD_PLATFORM_WINDOWS
    SetCurrentDirectoryA(path.c_str());
#else
    ::chdir(path.c_str());
#endif
}

std::string pd::path::get_cwd()
{
#if PD_PLATFORM == PD_PLATFORM_WINDOWS
    char path[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, path);
    return std::string(path);
#else
    char buf[4096];
    return std::string(getcwd(buf, sizeof(buf)));
#endif
}

bool pd::path::exists(const std::string &path)
{
    struct stat buf;
    if (stat(path.c_str(), &buf) && (errno == ENOENT || errno == ENOTDIR))
        return false;
    return true;
}

bool pd::path::is_file(const std::string &path)
{
    struct stat buf;
    if (stat(path.c_str(), &buf) && (errno == ENOENT || errno == ENOTDIR))
        return false;
    return (buf.st_mode & S_IFREG) != 0;
}

bool pd::path::is_dir(const std::string &path)
{
    struct stat buf;
    if (stat(path.c_str(), &buf) && (errno == ENOENT || errno == ENOTDIR))
        return false;
    return (buf.st_mode & S_IFDIR) != 0;
}
