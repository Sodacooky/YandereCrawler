#include "Config.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

const char *Config::sm_defaultPath = "config.json";

bool Config::IsFileExist()
{
    std::filesystem::directory_entry entry(sm_defaultPath);
    return entry.exists() && entry.is_regular_file();
}

void Config::TryWriteOut()
{
    nlohmann::json json;
    json["default_all_page"]  = isAllPage;
    json["enable_http_proxy"] = isHttpProxy;
    json["thread_amount"]     = threadAmount;
    json["http_proxy_addr"]   = proxyAddr;
    auto dump                 = json.dump(4);

    std::ofstream file(sm_defaultPath);
    file.write(dump.c_str(), dump.size());
    file.close();
}

void Config::TryLoad()
{
    if (!IsFileExist())
    {
        TryWriteOut();
        std::cout << u8"创建了默认配置文件" << std::endl;
    }

    std::ifstream config_file(sm_defaultPath);
    if (!config_file.is_open())
    {
        std::cout << u8"无法打开配置文件，将使用默认配置" << std::endl;
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
    isAllPage                  = config_json["default_all_page"].get<bool>();
    isHttpProxy                = config_json["enable_http_proxy"].get<bool>();
    threadAmount               = config_json["thread_amount"].get<int>();
    proxyAddr                  = config_json["http_proxy_addr"].get<std::string>();
    //虽然可能会因为文件没有写某个参数而引发问题，但这是用户手贱的问题
    //不想写太多if去判断
}
