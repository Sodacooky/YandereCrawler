#include "WebParser.h"

using namespace std;

WebParser::WebParser(vector<string>& tags, int page) {
  //生成链接并下载网页
  Downloader::DownloadPageToString(__GenerateURL(tags, page), m_strWebSource);
  //提取整个网页的所有图片链接
  while (true) {
    // get pos
    auto pos = __SeekNextDownloadLinkPos();
    if (pos == -1) break;
    m_nWebSourcePos = pos;
    // get and store link
    auto link = __ExtractLink();
    vstrLinks.push_back(link);
  }
  //如果图片链接1个都没有那么是404网页
  if (vstrLinks.size() > 0) bAvaliable = true;
}

int WebParser::GetMaxPage(vector<string>& tags) {
  //生成链接并下载网页
  string strWebSource;
  Downloader::DownloadPageToString(__GenerateURL(tags, 1), strWebSource);
  //是否有navigation
  auto startPos = strWebSource.find(R"(<div role="navigation")");
  if (startPos == string::npos) return 0;
  //循环找到最后一个 page
  auto endPos = strWebSource.find(R"(<a class="next_page")", startPos);
  string maxPageText = "0";
  while (true) {
    //带空格结尾的Page只在navitation中
    startPos = strWebSource.find("Page ", startPos);
    if (startPos == string::npos) break;
    startPos += 5;  // skip "Page "
    auto pageTextEnd = strWebSource.find("\"", startPos);
    maxPageText.assign(strWebSource.begin() + startPos,
                       strWebSource.begin() + pageTextEnd);
  }
  // string -> int
  stringstream ss(maxPageText);
  int maxPage;
  ss >> maxPage;
  return maxPage;
}

int WebParser::__SeekNextDownloadLinkPos() {
  auto pos =
      m_strWebSource.find(R"(<a class="directlink largeimg")", m_nWebSourcePos);
  if (pos == string::npos) {
    // not found any
    return -1;
  }
  return m_strWebSource.find(R"(href=")", pos) + 6;
}

string WebParser::__ExtractLink() {
  //链接以引号结束
  auto pos = m_strWebSource.find("\"", m_nWebSourcePos);
  return string(m_strWebSource.begin() + m_nWebSourcePos,
                m_strWebSource.begin() + pos);
}

string WebParser::__GenerateURL(const vector<string>& tags, int page) {
  /*	https://yande.re/post?page={page}&tags={tags1}+{tags2}+... */
  string url = "https://yande.re/post?page=" + to_string(page);
  url.append("&tags=");
  for (auto& tag : tags) {
    url.append(tag);
    url.append("+");
  }
  return url;
}