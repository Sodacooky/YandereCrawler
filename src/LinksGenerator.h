#ifndef YANDERECRAWLER_LINKSGENERATOR_H
#define YANDERECRAWLER_LINKSGENERATOR_H

#include <string>
#include <vector>
#include "Config.h"

/*  通过给定的tags，访问网站提取出原图链接，并放入队列中
 * */
class LinksGenerator
{
  public:
    //开始下载网页，提取链接，返回所有图片的下载链接
    static std::vector<std::string> Generate(int startPage,
                                             int endPage,
                                             const std::string &inputTagsLine,
                                             const Config &config);

  private:
    //在构造函数中，将tagsLine这一控制台输入转换为转以后的m_linksTagsArg这一URL参数
    static std::string ConvertTagsInputLineToUrlArg(const std::string &inputTagsLine);
    // use tags and page number to generate page url
    static std::string GeneratePageUrl(int pageNumber, const std::string &tagsUrlArg);
    // use page link to get all picture downloading link
    static std::vector<std::string> DownloadPageAndExtractPictureUrl(const std::string &pageUrl, const Config &config);
};

#endif  // YANDERECRAWLER_LINKSGENERATOR_H
