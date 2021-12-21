//
// Created by Sodacooky on 2021/12/12.
//

#include "Downloader.h"

#include <cpr/cpr.h>
#include <fmt/color.h>
#include <fmt/format.h>
#include <teemo/teemo.h>
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

bool Downloader::GetFileMultiThread(const string &url, const string &finalPath, const Config &config)
{
    int64_t total = 114514, downloaded = 0, speed = 0;
    //制造一个空行
    auto FlushLine = []() {
        for (int i = 0; i != 128; i++)
        {
            printf("\b \b");
        }
    };
    // update bar
    auto UpdateProgBar = [&]() {
        FlushLine();
        //进度条
        float percent = (downloaded * 100.0f) / (total * 1.0f);
        printf("\r[");  // go back to line start point
        for (int i = 0; i < 50; i++)
        {
            if (i * 2 <= percent)
            {
                printf("=");
            }
            else
            {
                printf(" ");
            }
        }
        printf("] %5.1f %%, %d KB/s", percent, speed / 1024);
    };
    // teemo::ProgressFunctor
    auto ProjCallback = [&](int64_t total_new, int64_t downloaded_new) {
        total      = total_new;
        downloaded = downloaded_new;
        UpdateProgBar();
    };
    // teemo::RealtimeSpeedFunctor
    auto SpeedCallback = [&](int64_t speed_new) {
        speed = speed_new;
        UpdateProgBar();
    };

    teemo::Teemo downloader;
    downloader.setThreadNum(config.nThreadAmount);
    downloader.setRedirectedUrlCheckEnabled(true);
    if (config.bHttpProxy)
    {
        downloader.setProxy(config.strProxyAddr);
    }
    auto future_result = downloader.start(url, finalPath, teemo::ResultFunctor(), ProjCallback, SpeedCallback);
    auto result        = future_result.get();

    FlushLine();
    if (result != teemo::SUCCESSED)
    {
        printf(u8"失败 %d\n", result);
        return false;
    }

    return true;
}
