#ifndef DVS_CACHE_READER_H_
#define DVS_CACHE_READER_H_

#include <string>
#include "filesystem_include.h"
#include <iomanip>

#include <nlohmann/json.hpp>


class CacheReader
{
private:
    std::string cache_file_path_;
    nlohmann::json j_;

public:
    CacheReader() = delete;
    ~CacheReader()
    {
        createCacheFile();
    }
    CacheReader(const dvs_filesystem::path& cache_file_path)
    {
        cache_file_path_ = cache_file_path;
        cache_file_path_ += dvs_filesystem::path("cache.json");
        j_["empty"] = "empty";

        if(!dvs_filesystem::exists(cache_file_path_))
        {
            createCacheFile();
        }
        else
        {
            try
            {
                readCacheFile();
            }
            catch(const std::exception& e)
            {
                std::cout << "Exception caught during reading cache file, creating new..." << std::endl;
                createCacheFile();
            }
        }
    }

    bool hasKey(const std::string& key)
    {
        return j_.count(key);
    }

    template <typename T>
    T readCache(const std::string& key)
    {
        return j_[key];
    }

    template <typename T>
    void writeToCache(const std::string& key, const T& val)
    {
        j_[key] = val;
        createCacheFile();
    }

    void readCacheFile()
    {
        std::ifstream input_file(cache_file_path_);
        input_file >> j_;
    }

    void createCacheFile()
    {
        std::ofstream output_file(cache_file_path_);
        output_file << std::setw(4) << j_ << std::endl;
    }

};


#endif
