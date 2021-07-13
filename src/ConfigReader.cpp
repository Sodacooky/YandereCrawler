#include "ConfigReader.h"
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

const std::string CONFIG_FILENAME = "config.json";

bool ConfigReader::IsExist()
{
	std::filesystem::directory_entry entry(CONFIG_FILENAME);
	return entry.exists() && entry.is_regular_file();
}

Config ConfigReader::Read()
{
	std::filesystem::directory_entry config_file_info(CONFIG_FILENAME);
	if (!config_file_info.exists() || !config_file_info.is_regular_file())
	{
		throw "config.json不存在，这是个意外，程序应该自己创建一份新的文件到目录下";
	}

	std::ifstream config_file(CONFIG_FILENAME);
	if (config_file.is_open() == false)
	{
		throw "无法打开config.json";
	}

	unsigned int file_size = config_file_info.file_size();
	char *buff = new char[file_size + 1];
	memset(buff, '\0', file_size + 1);
	config_file.read(buff, file_size);
	config_file.close();

	nlohmann::json config = nlohmann::json::parse(buff);

	delete[] buff;

	Config tmp_config;

	if (config.contains("default_all_page"))
	{
		tmp_config.DefaultDownloadAllPage(config["default_all_page"].get<bool>());
	}
	if (config.contains("enable_http_proxy"))
	{
		tmp_config.EnableHttpProxy(config["enable_http_proxy"].get<bool>());
	}
	if (config.contains("thread_amount"))
	{
		tmp_config.DownloadThreadAmount(config["thread_amount"].get<int>());
	}
	if (config.contains("http_proxy_addr"))
	{
		tmp_config.ProxyAddress(config["http_proxy_addr"].get<std::string>());
	}

	return tmp_config;
}

void ConfigReader::WriteDefault()
{
	nlohmann::json config;
	Config default_config;
	config["default_all_page"] = default_config.DefaultDownloadAllPage();
	config["enable_http_proxy"] = default_config.EnableHttpProxy();
	config["thread_amount"] = default_config.DownloadThreadAmount();
	config["http_proxy_addr"] = default_config.ProxyAddress();

	std::ofstream config_file(CONFIG_FILENAME);
	if (config_file.is_open() == false)
	{
		throw "无法创建config.json文件";
	}

	auto content = config.dump(4);
	config_file.write(content.c_str(), content.size());
	config_file.close();
}
