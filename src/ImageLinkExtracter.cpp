#include "ImageLinkExtracter.h"

#include <curl/curl.h>
#include "Downloader.h"

size_t ImageLinkExtracter::__GetLinkPosition(size_t startpos, const std::string &src)
{
    //<a class="directlink largeimg" href="..."
    std::string link_header = R"(<a class="directlink largeimg" href=")";
    //找到链接类头
    auto pos = src.find(link_header, startpos);
    if (pos != std::string::npos)
    {
        return pos + link_header.size();
    }
    else
    {
        return pos;
    }
}

std::string ImageLinkExtracter::__ExtractLink(size_t link_start_pos, const std::string &src)
{
    //链接以引号结束
    auto link_end_pos = src.find("\"", link_start_pos);
    //将链接头到引号前拷贝
    return std::string(src.cbegin() + link_start_pos, src.cbegin() + link_end_pos);
}

std::string ImageLinkExtracter::__ExtractFilename(const std::string &url)
{
    auto startpos = url.rfind('/') + 1;  //假定其不会出错
    std::string filename(url.begin() + startpos, url.end());

    // unescape
    CURL *handle   = curl_easy_init();
    auto filename_ = curl_unescape(filename.c_str(), filename.size());
    filename.clear();
    filename.assign(filename_);
    curl_free(filename_);
    curl_easy_cleanup(handle);

    return filename;
}

std::map<std::string, std::string> ImageLinkExtracter::Extract(const std::string &search_page_url,
                                                               const Config &config)
{
    auto src_byte_stream = Downlaoder::Download(search_page_url, config);
    std::string src(src_byte_stream.data(), src_byte_stream.size());
    std::map<std::string, std::string> links;

    size_t link_pos = 0;
    while (true)
    {
        link_pos = __GetLinkPosition(link_pos, src);
        if (link_pos == std::string::npos)
        {
            break;
        }

        auto link     = __ExtractLink(link_pos, src);
        auto filename = __ExtractFilename(link);

        links.insert({filename, link});
    }

    return links;
}
