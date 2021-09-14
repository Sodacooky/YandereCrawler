#include "PageLinkGenerator.h"

#include <curl/curl.h>
#include <list>
#include <sstream>

const std::string PageLinkGenerator::sm_strHeader = "https://yande.re/post?page=";

std::string PageLinkGenerator::Generate(int page)
{
    return sm_strHeader + std::to_string(page) + m_strTagsPart;
}

PageLinkGenerator::PageLinkGenerator(const std::string &tags_line)
{
    std::list<std::string> escaped_tags;

    auto curl_handle = curl_easy_init();
    std::istringstream ss(tags_line);
    std::string tmp;
    while (ss >> tmp)
    {
        auto escaped = curl_easy_escape(curl_handle, tmp.c_str(), 0);
        tmp.assign(escaped);
        curl_free(escaped);
        escaped_tags.push_back(tmp);
    }
    curl_easy_cleanup(curl_handle);

    m_strTagsPart.assign("&tags=");
    for (auto &tag : escaped_tags)
    {
        m_strTagsPart.append(tag).append("+");
    }
}
