#include "PageDownloader.h"

std::string PageDownloader::Download(const std::string &url, const Config &config)
{
  std::string src_out;
  // init curl
  CURL *handle = curl_easy_init();
  // set
  curl_easy_setopt(handle, CURLOPT_TIMEOUT_MS, 640000);
  curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt(handle, CURLOPT_USERAGENT, CURL_AGENT.c_str());
  curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, __WriteToString);
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, &src_out);
  if (config.EnableHttpProxy())
  {
    curl_easy_setopt(handle, CURLOPT_PROXY, config.ProxyAddress().c_str());
  }
  // do
  auto result = curl_easy_perform(handle);
  if (result != CURLE_OK)
  {
    spdlog::critical("CURL 错误代码 : {}", result);
    return "null";
  }
  // clean
  curl_easy_cleanup(handle);
  return src_out;
}

size_t PageDownloader::__WriteToString(char *buff,
                                       size_t block_size,
                                       size_t block_amount,
                                       void *str_ptr)
{
  auto str = (std::string *)str_ptr;
  auto size = block_amount * block_size;
  str->append((char *)buff, size);
  return size;
}