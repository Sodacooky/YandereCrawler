//
// Created by Sodacooky on 2021/12/12.
//

#include "Downloader.h"

#include <fmt/color.h>
#include <fmt/format.h>
#include <chrono>
#include <fstream>

using namespace std;

std::string Downloader::GetPage(const std::string &url, const Config &config)
{
    cpr::Response response;
    if (config.bHttpProxy)
    {
        cpr::Proxies proxies{{"http", config.strProxyAddr}, {"https", config.strProxyAddr}};
        response = Get(cpr::Url{url}, proxies, cpr::ConnectTimeout{32s});
    }
    else
    {
        response = Get(cpr::Url{url}, cpr::ConnectTimeout{32s});
    }

    if (response.status_code == 200)
    {
        return response.text;
    }
    else
    {
        fmt::print(fg(fmt::color::red) | fmt::emphasis::bold, "获取失败 {}\n", url);
        if (response.status_code == 0)
        {
            fmt::print(fg(fmt::color::red) | fmt::emphasis::bold, "CURL错误 {}\n", response.error.message);
        }
        else
        {
            fmt::print(fg(fmt::color::red) | fmt::emphasis::bold, "HTTP代码 {}\n", response.status_code);
        }
        return std::string();  // empty
    }
}

bool Downloader::GetFile(const string &url, const string &finalPath, const Config &config)
{
    // check if file can be create
    ofstream file(finalPath, ios::binary);
    if (!file.is_open())
    {
        fmt::print(fg(fmt::color::red) | fmt::emphasis::bold, "无法创建文件 {}", finalPath);
        return false;
    }
    // start download
    cpr::Response response;
    if (config.bHttpProxy)
    {
        cpr::Proxies proxies{{"http", config.strProxyAddr}, {"https", config.strProxyAddr}};
        response = cpr::Download(file, cpr::Url{url}, proxies, cpr::ConnectTimeout{32s});
    }
    else
    {
        response = cpr::Download(file, cpr::Url{url}, cpr::ConnectTimeout{32s});
    }
    file.close();
    // state check
    if (response.status_code == 200)
    {
        return true;
    }
    else
    {
        fmt::print(fg(fmt::color::red) | fmt::emphasis::bold, "获取失败 {}\n", url);
        if (response.status_code == 0)
        {
            fmt::print(fg(fmt::color::red) | fmt::emphasis::bold, "CURL错误 {}\n", response.error.message);
        }
        else
        {
            fmt::print(fg(fmt::color::red) | fmt::emphasis::bold, "HTTP代码 {}\n", response.status_code);
        }
        return false;
    }
}
