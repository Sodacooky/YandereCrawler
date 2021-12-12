//
// Created by Sodacooky on 2021/12/12.
//

#ifndef YANDERECRAWLER_DOWNLOADER_H
#define YANDERECRAWLER_DOWNLOADER_H

#include <cpr/cpr.h>
#include <string>

#include "Config.h"

class Downloader
{
  public:
    //下载网页源代码
    static std::string GetPage(const std::string &url, const Config &config);

  public:
    //下载文件到finalPath
    // finalPath是指带文件名及其后缀的路径
    static bool GetFile(const std::string &url, const std::string &finalPath, const Config &config);
};

#endif  // YANDERECRAWLER_DOWNLOADER_H
