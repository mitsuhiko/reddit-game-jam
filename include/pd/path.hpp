#ifndef _INC_PD_PATH_HPP_
#define _INC_PD_PATH_HPP_

#include <pd/pd.hpp>

namespace pd {

    namespace posixpath {
        std::string join(const std::string &a, const std::string &b);
        std::string join_with_dir(const std::string &a, const std::string &b);
        std::string dirname(const std::string &path);
    }

    namespace ntpath {
        std::string join(const std::string &a, const std::string &b);
        std::string join_with_dir(const std::string &a, const std::string &b);
        std::string dirname(const std::string &path);
    }

    namespace path {
#   if PD_PLATFORM == PD_PLATFORM_OSX
        using namespace posixpath;
#   elif PD_PLATFORM == PD_PLATFORM_WINDOWS
        using namespace ntpath;
#   endif
        const std::string &get_resource_path();
        void set_cwd(const std::string &path);
        std::string get_cwd();
        bool exists(const std::string &path);
        bool is_file(const std::string &path);
        bool is_dir(const std::string &path);
    }
}

#endif
