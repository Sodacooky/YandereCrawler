//
// Created by Sodacooky on 2021/12/11.
//

#include "LinksGenerator.h"

#include <curl/curl.h>
#include <iostream>
#include <sstream>

#include "Downloader.h"

LinksGenerator::LinksGenerator(Config &config, const std::string &tagsLine, int start, int end)
    : m_config(config), m_startPage(start), m_endPage(end)
{
    ConvertTagsLineToArg(tagsLine);
}

void LinksGenerator::ConvertTagsLineToArg(const std::string &tagsLine)
{
    // header
    m_linksTagsArg.append("&tags=");
    //将某些字符转换为URL合法形式
    auto curl_handle = curl_easy_init();
    std::vector<std::string> tags;
    std::istringstream ss(tagsLine);
    std::string unescaped;
    while (ss >> unescaped)
    {
        auto escaped = curl_easy_escape(curl_handle, unescaped.c_str(), 0);
        tags.emplace_back(escaped);
        curl_free(escaped);
    }
    curl_easy_cleanup(curl_handle);
    //装入
    for (auto &tag : tags)
    {
        m_linksTagsArg.append(tag).append("+");
    }
}

std::vector<std::string> LinksGenerator::Start()
{
    std::vector<std::string> downloadingLinks;
    //从开始页到终止页生成概览页链接
    for (int now = m_startPage; now <= m_endPage; now++)
    {
        auto links = ExtractDownloadLinks(GeneratePageLink(now));
        if (links.empty()) break;
        for (auto &link : links)
        {
            downloadingLinks.push_back(link);
        }
        std::printf("[%d] ", now);
    }
    return downloadingLinks;
}

std::string LinksGenerator::GeneratePageLink(int pageNumber)
{
    std::string result;
    result.append("https://yande.re/post?page=").append(std::to_string(pageNumber)).append(m_linksTagsArg);
    return result;
}

std::vector<std::string> LinksGenerator::ExtractDownloadLinks(const std::string &pageLink)
{
    //下载网页
    auto src = Downloader::GetPage(pageLink, m_config);
    //对内容进行提取
    //定位链接位置
    auto GetLinkPosition = [&src](size_t startpos) {
        //<a class="directlink largeimg" href="..."
        const std::string link_header = R"(<a class="directlink largeimg" href=")";
        //找到链接类头
        auto pos = src.find(link_header, startpos);
        if (pos != std::string::npos)
        {
            return pos + link_header.size();
        }
        else
        {
            return pos;  // fail,npos
        }
    };
    auto ExtractLink = [&src](size_t linkPos) {
        //链接以引号结束
        auto linkEndPos = src.find('\"', linkPos);
        //将链接头到引号前拷贝
        return std::string(src.cbegin() + linkPos, src.cbegin() + linkEndPos);
    };
    size_t link_pos = 0;
    std::vector<std::string> links;
    while (true)
    {
        link_pos = GetLinkPosition(link_pos);
        if (link_pos == std::string::npos)
        {
            break;
        }
        auto link = ExtractLink(link_pos);
        links.push_back(link);
    }
    return links;
}