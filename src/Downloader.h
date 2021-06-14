﻿#ifndef __DOWNLOADER_H__
#define __DOWNLOADER_H__

#include <curl/curl.h>
#include <spdlog/spdlog.h>

#include <chrono>
#include <filesystem>
#include <future>
#include <iostream>
#include <string>
#include <vector>

#include "Tool.h"
/*
  提供网页和图片的下载功能
*/
class Downloader {
 public:
  // 下载网页到字符串
  // 返回是否成功
  static bool DownloadPageToString(const std::string &url,
                                   std::string &src_out);

  // 下载一组图片到文件
  // DownloadPageToFile的封装
  static void MultiDownloadFiles(const std::vector<std::string> &links);

  // 创建文件夹
  static void CreatePath(const std::string &pathname);

 private:
  // 下载一个图片到文件
  // 返回是否成功
  static bool __DownloadPageToFile(const std::string &url);

  // curl write callback
  static size_t __WriteToString(char *buff, size_t block_size,
                                size_t block_amount, void *str_ptr);

  // 设置一些相同的参数
  static void __SetCurlDefaultOpt(CURL *handle);

  // 将链接的最后一段（通常是文件名）提取出来
  static std::string __ExtractFilename(const std::string &src_str);

  //传入一个文件名，返回前32个字符
  static std::string __ExtractShort(const std::string &src_str);
};

#endif  // __DOWNLOADER_H__