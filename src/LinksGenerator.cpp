//
// Created by Sodacooky on 2021/12/11.
//

#include "LinksGenerator.h"

#include <curl/curl.h>
#include <iostream>
#include <sstream>

#include "Downloader.h"

std::string LinksGenerator::ConvertTagsInputLineToUrlArg(const std::string &inputTagsLine)
{
    // header
    std::string resultArg;
    resultArg.append("&tags=");
    //将某些字符转换为URL合法形式
    auto curl_handle = curl_easy_init();
    std::vector<std::string> tags;
    std::istringstream ss(inputTagsLine);
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
        resultArg.append(tag).append("+");
    }
    //
    return resultArg;
}

std::string LinksGenerator::GeneratePageUrl(int pageNumber, const std::string &tagsUrlArg)
{
    std::string result;
    result.append("https://yande.re/post?page=").append(std::to_string(pageNumber)).append(tagsUrlArg);
    return result;
}

std::vector<std::string> LinksGenerator::DownloadPageAndExtractPictureUrl(const std::string &pageUrl,
                                                                          const Config &config)
{
    //下载网页
    auto src = Downloader::GetPage(pageUrl, config);
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

std::vector<std::string> LinksGenerator::Generate(int startPage,
                                                  int endPage,
                                                  const std::string &inputTagsLine,
                                                  const Config &config)
{
    std::vector<std::string> downloadingLinks;
    auto urlTagsArg = ConvertTagsInputLineToUrlArg(inputTagsLine);
    //从开始页到终止页生成概览页链接
    for (int now = startPage; now <= endPage; now++)
    {
        auto links = DownloadPageAndExtractPictureUrl(GeneratePageUrl(now, urlTagsArg), config);
        if (links.empty()) break;
        for (auto &link : links)
        {
            downloadingLinks.push_back(link);
        }
        std::printf("[%d] ", now);
    }
    return downloadingLinks;
}
