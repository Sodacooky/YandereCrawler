#include "Downloader.h"

#include <curl/curl.h>
#include <spdlog/spdlog.h>

std::vector<char> Downlaoder::Download(const std::string &url, const Config &config)
{
    const char *cstrAgent =
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) "
        "Chrome/94.0.4606.81 Safari/537.36 Edg/94.0.992.50";

    std::vector<char> result;

    CURL *pCurl = curl_easy_init();
    curl_easy_setopt(pCurl, CURLOPT_TIMEOUT_MS, 24000);
    curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(pCurl, CURLOPT_USERAGENT, cstrAgent);
    curl_easy_setopt(pCurl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, __CurlWriteFunc);
    curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &result);
    // curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 0);
    // curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, 0);
    if (config.bHttpProxy)
    {
        curl_easy_setopt(pCurl, CURLOPT_PROXY, config.strProxyAddr.c_str());
    }

    auto retCode = curl_easy_perform(pCurl);
    curl_easy_cleanup(pCurl);

    if (retCode != CURLE_OK)
    {
        spdlog::error(u8"{} 获取失败!\n{}", url, retCode);
        return std::vector<char>();
    }
    else
    {
        return result;
    }
}

size_t Downlaoder::__CurlWriteFunc(char *ptrBuffer, size_t size, size_t nmemb, void *ptrVecChar)
{
    auto vec = (std::vector<char> *)ptrVecChar;
    for (int i = 0; i != nmemb; i++)
    {
        vec->push_back(ptrBuffer[i]);
    }
    return nmemb;
}
