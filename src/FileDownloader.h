#ifndef __FILEDOWNLOADER_H__
#define __FILEDOWNLOADER_H__

#include <curl/curl.h>
#include <spdlog/spdlog.h>
#include <stdio.h>

#include <filesystem>
#include <string>

#include "Config.h"
#include "Tool.h"

//文件下载
class FileDownloader
{
public:
  //下载url指定文件到目录下，path末不带斜杠
  //返回文件大小
  static size_t Download(const std::string &path, const std::string &url, const Config &config);

private:
  static std::string __ExtractFilename(CURL *curl_handle, const std::string &url);
};

#endif // __FILEDOWNLOADER_H__