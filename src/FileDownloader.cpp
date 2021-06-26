#include "FileDownloader.h"

std::string FileDownloader::__ExtractFilename(CURL *curl_handle,
                                              const std::string &url)
{
  //从尾部开始找，从第一个斜杠后开始到最后
  auto pos = url.rfind('/');
  if (pos == std::string::npos)
  {
    throw "找不到斜杠";
  }

  //转义
  std::string escaped_str(url.cbegin() + pos + 1, url.cend());
  int raw_str_length = 0;
  char *raw_tmp = curl_easy_unescape(curl_handle, escaped_str.c_str(), 0, &raw_str_length);
  std::string raw_str(raw_tmp, raw_str_length);
  curl_free(raw_tmp);

  //剔除非法字符
  std::string illegal_charset = "\\/:*?\"<>|";
  for (auto iter = raw_str.begin(); iter != raw_str.end();)
  {
    if (illegal_charset.find(*iter) != std::string::npos)
    {
      iter = raw_str.erase(iter);
    }
    else
    {
      iter++;
    }
  }

  return raw_str;
}

size_t FileDownloader::Download(const std::string &path,
                                const std::string &url,
                                const Config &config)
{
  // init curl
  CURL *pCurl = curl_easy_init();
  // 生成文件名
  std::string filename = __ExtractFilename(pCurl, url);
  std::string fullpath = path + "/" + filename;
  //打开文件以供curl使用
  FILE *file;
  file = fopen(fullpath.c_str(), "wb");
  if (file == nullptr)
  {
    spdlog::critical("{} 写文件失败", fullpath);
    return false;
  }
  // set curl
  curl_easy_setopt(pCurl, CURLOPT_TIMEOUT_MS, 640000);
  curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt(pCurl, CURLOPT_USERAGENT, CURL_AGENT.c_str());
  curl_easy_setopt(pCurl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, nullptr);
  curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, file);
  if (config.EnableHttpProxy())
  {
    curl_easy_setopt(pCurl, CURLOPT_PROXY, config.ProxyAddress().c_str());
  }
  // do
  auto result = curl_easy_perform(pCurl);
  // clean
  curl_easy_cleanup(pCurl);
  fclose(file);
  // check
  if (result != CURLE_OK)
  {
    spdlog::critical("CURL 错误代码 : {}", result);
    spdlog::critical("失败 {}", filename);
    return 0;
  }
  //
  if (filename.size() < 32)
  {
    spdlog::info("  已下载 {}", std::string(filename.begin(), filename.end()));
  }
  else
  {
    spdlog::info("  已下载 {}", std::string(filename.begin(), filename.begin() + 32));
  }
  return std::filesystem::file_size(fullpath);
}
