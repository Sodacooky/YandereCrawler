#include "Application.h"

#include <spdlog/spdlog.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <queue>
#include <thread>

#include "Config.h"
#include "Downloader.h"
#include "ImageLinkExtracter.h"
#include "PageLinkGenerator.h"

int main()
{
    Application app;
    app.Main();
    return 0;
}

void Application::Main()
{
    m_config.TryLoad();
    __PromptInput();

    PageLinkGenerator link_generator(m_strTagsLine);
    m_strDirectory = __CreateDirectory(m_strTagsLine) ? m_strTagsLine : ".";

    for (int page = m_nStart; page <= m_nEnd; page++)
    {
        auto links = ImageLinkExtracter::Extract(link_generator.Generate(page), m_config);
        spdlog::info(u8"开始下载第 {} 页...", page);
        __DispatchDownloadThred(links);
        std::cout << std::endl;
    }

    spdlog::info(u8"下载结束");
}

void Application::__PromptInput()
{
    spdlog::info(u8"请以空格间隔输入Tags，如 miko hakurei_reimu :");
    std::getline(std::cin, m_strTagsLine);
    spdlog::info(u8"输入了 {}", m_strTagsLine);

    if (m_config.bAllPage)
    {
        m_nStart = 1;
        m_nEnd   = 9999;
        return;
    }

    spdlog::info(u8"请输入 <起始页> <终止页>，如 1 999:");
    std::cin >> m_nStart >> m_nEnd;
    std::cout << std::endl;

    if (m_nStart > m_nEnd)
    {
        spdlog::critical(u8"起始页大于终止页");
        exit(-1);
    }

    spdlog::info(u8"指定下载了 {} 页", m_nEnd - m_nStart + 1);
}

void Application::__DispatchDownloadThred(const std::map<std::string, std::string> &links)
{
    std::queue<std::future<void>> que_future;

    auto iter_pair = links.begin();
    while (iter_pair != links.end())
    {
        if (que_future.size() < m_config.nThreadAmount)
        {
            auto future = std::async(std::launch::async, &Application::__DownloadThreadFunc, this,
                                     std::ref(iter_pair->second), std::ref(iter_pair->first));
            que_future.push(std::move(future));
            iter_pair++;
        }
        else
        {
            auto status = que_future.front().wait_for(std::chrono::seconds(1));
            if (status == std::future_status::ready)
            {
                que_future.pop();
            }
        }
    }

    //等待所有任务完成
    while (que_future.size() != 0)
    {
        auto status = que_future.front().wait_for(std::chrono::seconds(1));
        if (status == std::future_status::ready)
        {
            que_future.pop();
        }
    }
}

void Application::__DownloadThreadFunc(const std::string &link, const std::string &filename)
{
    auto data = Downlaoder::Download(link, m_config);
    __SaveToFile(m_strDirectory, filename, data);
    std::cout << u8"■ ";
}

bool Application::__CreateDirectory(const std::string &dir_name)
{
    if (std::filesystem::create_directory(dir_name))
    {
        spdlog::info(u8"创建了目录 {}", dir_name);
        return true;
    }
    else
    {
        std::filesystem::directory_entry entry(dir_name);
        if (entry.exists())
        {
            if (entry.is_directory())
            {
                spdlog::warn(u8"目录 {} 已存在，将重用", dir_name);
                return true;
            }
            else
            {
                spdlog::warn(u8"{} 文件已存在，无法创建目录", dir_name);
                return false;
            }
        }
        else
        {
            spdlog::warn(u8"无法创建目录 {}", dir_name);
            return false;
        }
    }
}

void Application::__SaveToFile(const std::string &dirname,
                               const std::string &filename,
                               const std::vector<char> &data)
{
    auto fullpath = dirname + "/" + filename;
    std::ofstream file(fullpath, std::ios::binary);
    file.write(data.data(), data.size());
    file.close();
}

Application::Application()
    : m_nStart(1), m_nEnd(1), m_config(), m_strDirectory("."), m_strTagsLine()
{}
