#ifndef __MULTIFILEDOWNLOADER_H__
#define __MULTIFILEDOWNLOADER_H__

#include <spdlog/spdlog.h>

#include <chrono>
#include <future>
#include <list>
#include <memory>
#include <string>

#include "Config.h"
#include "FileDownloader.h"

//多线程（可为单）文件下载器
class MultiFileDownloader
{
public:
  static void Download(const std::list<std::string> &list, const std::string &path, const Config &config);
};

#endif // __MULTIFILEDOWNLOADER_H__