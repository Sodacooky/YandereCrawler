#include "WebParser.h"

using namespace std;

void WebParser::__Do() {
  //生成链接表并
  for (int pageNow = m_nPageStart; pageNow <= m_nPageEnd; pageNow++) {
    string url = "https://yande.re/post?page=" + to_string(pageNow);
    url.append("&tags=");
    for (auto &tag : m_rvec_strTags) {
      url.append(tag);
      url.append("+");
    }
    m_vec_strPageLinks.push_back(url);
  }

  //分派任务
  //__GetLinks()返回值是vector<string>
  vector<future<vector<string>>> fus_fetching;
  for (int index = 0; index != m_vec_strPageLinks.size(); index++) {
    fus_fetching.push_back(async(&WebParser::__GetLinks, this, index));
    float progress = (index + 1) * 1.0f / m_vec_strPageLinks.size() * 100.0f;
    spdlog::info("[{:.1f}%] 正在下载第 {} 页链接数据", progress, index + 1);
    this_thread::sleep_for(chrono::milliseconds(500));
  }

  //回收数据
  for (auto &fu : fus_fetching) {
    for (auto &link : fu.get()) {
      m_rvec_strLinks.push_back(link);
    }
  }
  spdlog::info("链接数据下载完成");
}

std::vector<std::string> WebParser::__GetLinks(int pageLinksIndex) {
  string webSource;
  Downloader::DownloadPageToString(m_vec_strPageLinks[pageLinksIndex],
                                   webSource);
  LinkParser parser(webSource);
  return parser.vec_strLinks;
}
