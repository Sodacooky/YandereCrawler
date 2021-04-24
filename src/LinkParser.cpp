#include "LinkParser.h"

using namespace std;

void LinkParser::__Do() {
  //定位原图链接
  auto ___FindDownloadLinkPos = [this](int begin_pos) -> int {
    //找到链接类头
    auto pos =
        m_rstrWebSource.find(R"(<a class="directlink largeimg")", begin_pos);
    if (pos == string::npos) return -1;
    //从链接类头移动到链接位置
    return m_rstrWebSource.find(R"(href=")", pos) + 6;
  };

  //提取出原图
  auto ___ExtractDownloadLink = [this](int link_start_pos) -> string {
    //链接以引号结束
    auto endPos = m_rstrWebSource.find("\"", link_start_pos);
    //将链接头到引号前拷贝
    return string(m_rstrWebSource.begin() + link_start_pos,
                  m_rstrWebSource.begin() + endPos);
  };

  //当前查找位置
  int nowFindingPos = 0;

  //提取整个网页的所有图片链接
  while (true) {
    // 找到链接位置
    auto link_pos = ___FindDownloadLinkPos(nowFindingPos);
    if (link_pos == -1) break;  // "eof"

    // 提取出链接
    auto link = ___ExtractDownloadLink(link_pos);
    vec_strLinks.push_back(link);

    // 当前查找位置移动到链接后
    nowFindingPos = link_pos + link.size() + 1;
  }
}
