#include "Config.h"

#include <spdlog/spdlog.h>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

const char *Config::sm_cstrDefaultPath = "config.json";

bool Config::__IsFileExist()
{
    std::filesystem::directory_entry entry(sm_cstrDefaultPath);
    return entry.exists() && entry.is_regular_file();
}

void Config::WriteOut()
{
    nlohmann::json json;
    json["default_all_page"]  = bAllPage;
    json["enable_http_proxy"] = bHttpProxy;
    json["thread_amount"]     = nThreadAmount;
    json["http_proxy_addr"]   = strProxyAddr;
    auto dump                 = json.dump(4);

    std::ofstream file(sm_cstrDefaultPath);
    file.write(dump.c_str(), dump.size());
    file.close();
}

void Config::TryLoad()
{
    if (!__IsFileExist())
    {
        WriteOut();
        spdlog::warn(u8"创建了默认配置文件");
    }

    std::ifstream config_file(sm_cstrDefaultPath);
    if (!config_file.is_open())
    {
        spdlog::error(u8"无法打开配置文件，将使用默认配置");
        return;
    }

    std::string config_json_dump;
    while (config_file.peek() != EOF)
    {
        std::string tmpLine;
        std::getline(config_file, tmpLine);
        config_json_dump.append(tmpLine).append("\n");
    }
    config_json_dump.push_back('\0');

    nlohmann::json config_json = nlohmann::json::parse(config_json_dump);
    bAllPage                   = config_json["default_all_page"].get<bool>();
    bHttpProxy                 = config_json["enable_http_proxy"].get<bool>();
    nThreadAmount              = config_json["thread_amount"].get<int>();
    strProxyAddr               = config_json["http_proxy_addr"].get<std::string>();
    //虽然可能会因为文件没有写某个参数而引发问题，但这是用户手贱的问题
    //不想写太多if去判断
}
