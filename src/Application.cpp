#include "Application.h"

#include <curl/curl.h>
#include <fmt/format.h>
#include <teemo/teemo.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <thread>

#include "Config.h"
#include "Downloader.h"
#include "LinksGenerator.h"

std::mutex Application::sm_mutexProgressBar;

int main()
{
    system("chcp 65001");

    Application app;
    app.Main();
    return 0;
}

Application::Application() : m_startPage(1), m_endPage(1), m_config(), m_directoryName("."), m_tagsLine() {}

void Application::Main()
{
    m_config.TryLoad();
    PromptInput();

    m_directoryName = TryCreateDirectory(m_tagsLine) ? m_tagsLine : ".";

    std::cout << u8"正在获取链接..." << std::endl;
    LinksGenerator linksGenerator(m_config, m_tagsLine, m_startPage, m_endPage);
    auto links = linksGenerator.Start();

    std::cout << u8"正在下载图片..." << std::endl;
    for (int index = 0; index != links.size(); index++)
    {
        auto filename = ExtractFilename(links[index]);
        std::string trimmed(filename.begin(), filename.size() > 64 ? filename.begin() + 64 : filename.end());
        if (trimmed.size() >= 60)
        {
            trimmed.append("...");
        }
        std::cout << fmt::format("({}/{}) {}", index + 1, links.size(), trimmed) << std::endl;
        Downloader::GetFileMultiThread(links[index], m_directoryName + "/" + filename, m_config);
    }

    std::cout << u8"下载结束" << std::endl;
}

void Application::PromptInput()
{
    std::cout << u8"请以空格间隔输入Tags，如 miko hakurei_reimu :" << std::endl;
    std::getline(std::cin, m_tagsLine);
    std::cout << fmt::format(u8"输入了 {}", m_tagsLine) << std::endl;
    if (m_config.bAllPage)
    {
        m_startPage = 1;
        m_endPage   = 9999;
        return;
    }

    std::cout << u8"请输入 <起始页> <终止页>，如 1 999:" << std::endl;
    std::cin >> m_startPage >> m_endPage;
    std::cout << std::endl;
    if (m_startPage > m_endPage)
    {
        std::cout << u8"起始页大于终止页" << std::endl;
        exit(-1);
    }
    std::cout << fmt::format(u8"指定下载了 {} 页", m_endPage - m_startPage + 1) << std::endl;
}

bool Application::TryCreateDirectory(const std::string &dir_name)
{
    if (std::filesystem::create_directory(dir_name))
    {
        std::cout << fmt::format(u8"创建了目录 {}", dir_name) << std::endl;
        return true;
    }
    else
    {
        std::filesystem::directory_entry entry(dir_name);
        if (entry.exists())
        {
            if (entry.is_directory())
            {
                std::cout << fmt::format(u8"** 目录 {} 已存在，将重用 **", dir_name) << std::endl;
                return true;
            }
            else
            {
                std::cout << fmt::format(u8"** {} 文件已存在，无法创建目录 **", dir_name) << std::endl;
                return false;
            }
        }
        else
        {
            std::cout << fmt::format(u8"** 无法创建目录 {} **", dir_name) << std::endl;
            return false;
        }
    }
}

std::string Application::ExtractFilename(const std::string &link)
{
    auto startPos = link.rfind('/') + 1;  //假定其不会出错
    std::string filename(link.begin() + startPos, link.end());

    // unescape
    CURL *handle   = curl_easy_init();
    auto filename_ = curl_unescape(filename.c_str(), filename.size());
    filename.clear();
    filename.assign(filename_);
    curl_free(filename_);
    curl_easy_cleanup(handle);

    //剔除windows非法字符
    std::string illegalSymbol = R"(\/:*"<>|)";
    while (filename.find_first_of(illegalSymbol) != std::string::npos)
    {
        filename.erase(filename.begin() + filename.find_first_of(illegalSymbol));
    }

    return filename;
}