﻿#include "Downloader.h"

using namespace std;

static string sg_strPath = ".";

static const string AGENT =
    "User-Agent,Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 "
    "(KHTML, like Gecko) Chrome/88.0.4324.182 Safari/537.36 Edg/88.0.705.74";

bool Downloader::DownloadPageToString(const std::string &url,
                                      std::string &src_out) {
  // init curl
  CURL *handle = curl_easy_init();
  // set
  __SetCurlDefaultOpt(handle);
  curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, __WriteToString);
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, &src_out);
  // do
  auto result = curl_easy_perform(handle);
  bool ret = true;
  if (result != CURLE_OK) {
    spdlog::critical("CURL 错误代码 : {}", result);
    ret = false;
  }
  // clean
  curl_easy_cleanup(handle);
  return ret;
}

bool Downloader::__DownloadPageToFile(const std::string &url) {
  // 生成文件名
  string path = sg_strPath + "/";
  string filename = __ExtractFilename(url);
  string fullfilename = path + filename;
  //打开文件以供curl使用
  FILE *file;
  file = fopen(fullfilename.c_str(), "wb");
  if (file == nullptr) {
    spdlog::critical("写文件失败");
    return false;
  }
  // init curl
  CURL *download_handle = curl_easy_init();
  // set
  __SetCurlDefaultOpt(download_handle);
  curl_easy_setopt(download_handle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(download_handle, CURLOPT_WRITEFUNCTION, nullptr);
  curl_easy_setopt(download_handle, CURLOPT_WRITEDATA, file);
  // do
  auto result = curl_easy_perform(download_handle);
  // check
  if (result != CURLE_OK) {
    spdlog::critical("CURL 错误代码 : {}", result);
    spdlog::critical("失败 {}", __ExtractShort(filename));
    // clean
    curl_easy_cleanup(download_handle);
    fclose(file);
    return false;
  }

  // clean
  curl_easy_cleanup(download_handle);
  fclose(file);
  return true;
}

void Downloader::MultiDownloadFiles(const std::vector<std::string> &links) {
  const int thread_amount = 4;
  queue<future<bool>> que_fus;

  int now_dispatch_index = 0;
  int done_amount = 0;
  int failed_amount = 0;
  int amount = links.size();

  while (now_dispatch_index < amount) {
    // try push
    if (que_fus.size() < thread_amount) {
      que_fus.push(async(launch::async, __DownloadPageToFile,
                         links[now_dispatch_index]));
      now_dispatch_index++;
    }
    // check empty
    if (que_fus.size() == 0) {
      continue;
    }
    // result check
    if (que_fus.front().wait_for(chrono::seconds(1)) == future_status::ready) {
      // done
      if (que_fus.front().get() == true) {
        done_amount++;
        float progress = done_amount * 1.0f / amount * 100.0f;
        spdlog::info("[{:02.1f}%] {}/{}", progress, done_amount, amount);
      }
      // failed
      else {
        failed_amount++;
      }
      // cleanup
      que_fus.pop();
    }
  }

  spdlog::info("下载结果: {} 完成，{} 失败", done_amount, failed_amount);
}

void Downloader::CreatePath(const std::string &pathname) {
  sg_strPath = pathname;
  filesystem::directory_entry de(pathname);
  if (de.exists() && de.is_directory()) {
    spdlog::warn("文件夹 {} 已存在，使用它", pathname);
    return;
  }
  if (filesystem::create_directories(pathname)) {
    spdlog::info("创建了文件夹 {}", pathname);
  } else {
    spdlog::warn("文件夹 {} 无法创建且不存在，将在程序目录保存文件", pathname);
    sg_strPath = ".";
  }
}

size_t Downloader::__WriteToString(char *buff, size_t block_size,
                                   size_t block_amount, void *str_ptr) {
  auto str = (string *)str_ptr;
  auto size = block_amount * block_size;
  str->append((char *)buff, size);
  return size;
}

void Downloader::__SetCurlDefaultOpt(CURL *handle) {
  curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
  curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
  curl_easy_setopt(handle, CURLOPT_USERAGENT, AGENT.c_str());
  curl_easy_setopt(handle, CURLOPT_TIMEOUT_MS, 320000);
}

void Downloader::__TransSymbol(std::string &str) {
  for (auto iter = str.begin(); iter != str.end();) {
    if (*iter == '%') {
      iter = str.erase(iter, iter + 3);
      iter = str.insert(iter, ' ') + 1;
    } else {
      iter++;
    }
  }
}

std::string Downloader::__ExtractFilename(const std::string &src_str) {
  //从尾部开始找，从第一个斜杠后开始到最后
  auto start = src_str.begin();
  for (auto iter = src_str.end() - 1; iter != src_str.begin(); iter--) {
    if (*iter == '/') {
      start = iter;
      break;
    }
  }
  string tmp(start + 1, src_str.end());
  __TransSymbol(tmp);
  return tmp;
}

std::string Downloader::__ExtractShort(const std::string &src_str) {
  if (src_str.size() <= 48) {
    return string(src_str.begin() + 9, src_str.end());
  } else {
    return string(src_str.begin() + 9, src_str.begin() + 48);
  }
}