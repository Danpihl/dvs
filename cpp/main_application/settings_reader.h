#ifndef DVS_CACHE_READER_H_
#define DVS_CACHE_READER_H_

#include <string>
#include "filesystem_included.h"

#include <nlohmann/json.hpp>


class SettingsReader
{
private:
    std::string settings_file_path_;
    nlohmann::json j_;

public:
    SettingsReader() = delete;
    ~SettingsReader()
    {
        createCacheFile();
    }
    SettingsReader(const dvs_filesystem::path& settings_file_path)
    {
        settings_file_path_ = settings_file_path;
        settings_file_path_ += dvs_filesystem::path("cache.json");
        j_["empty"] = "empty";

        if(!dvs_filesystem::exists(settings_file_path_))
        {
            std::cout << "File doesn't exist! Creating..." << std::endl;
            createCacheFile();
        }
        else
        {
            std::cout << "File exists! Reading..." << std::endl;
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
        std::cout << settings_file_path_ << std::endl;

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
        std::ifstream input_file(settings_file_path_);
        input_file >> j_;
    }

    void createCacheFile()
    {
        std::ofstream output_file(settings_file_path_);
        output_file << std::setw(4) << j_ << std::endl;
    }

};


#endif
