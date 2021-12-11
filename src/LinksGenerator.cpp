//
// Created by Sodacooky on 2021/12/11.
//

#include "LinksGenerator.h"

#include <curl/curl.h>
#include <fmt/format.h>
#include <iostream>
#include <sstream>

LinksGenerator::LinksGenerator(Config &config, const std::string &tagsLine, int start, int end)
    : m_config(config), m_startPage(start), m_endPage(end)
{
    ConvertTagsLineToArg(tagsLine);
}

void LinksGenerator::ConvertTagsLineToArg(const std::string &tagsLine)
{  // header
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
        for (auto &link : ExtractDownloadLinks(GeneratePageLink(now)))
        {
            downloadingLinks.push_back(link);
        }
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
    auto src = DownloadPage(pageLink);
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

std::string LinksGenerator::DownloadPage(const std::string &pageLink) const
{
    std::vector<char> bytes;

    CURL *pCurl = curl_easy_init();
    curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(pCurl, CURLOPT_URL, pageLink.c_str());
    curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, DownloadCallback);
    curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &bytes);
    if (m_config.bHttpProxy)
    {
        curl_easy_setopt(pCurl, CURLOPT_PROXY, m_config.strProxyAddr.c_str());
    }

    auto retCode = curl_easy_perform(pCurl);
    curl_easy_cleanup(pCurl);

    if (retCode != CURLE_OK)
    {
        std::cout << fmt::format(u8"下载失败于 {}", pageLink) << std::endl;
        return std::string();
    }
    else
    {
        // vector内部数组直接转成字符串
        return std::string(bytes.data(), bytes.size());
    }
}

size_t LinksGenerator::DownloadCallback(char *ptrBuffer, size_t size, size_t nmemb, void *ptrVecChar)
{
    auto vec = (std::vector<char> *)ptrVecChar;
    for (int i = 0; i != nmemb; i++)
    {
        vec->push_back(ptrBuffer[i]);
    }
    return nmemb;
}
