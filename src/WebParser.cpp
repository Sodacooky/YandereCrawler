#include "WebParser.h"

using namespace std;

void WebParser::__Do() {
  //生成链接表并
  for (int pageNow = m_nPageStart; pageNow <= m_nPageEnd; pageNow++) {
    string url = "https://yande.re/post?page=" + to_string(pageNow);
    url.append("&tags=");
    for (auto& tag : m_rvec_strTags) {
      url.append(tag);
      url.append("+");
    }
    m_vec_strPageLinks.push_back(url);
  }

  //分派任务
  //__GetLinks()返回值是vector<string>
  vector<future<vector<string>>> fus_fetching;
  for (int index = 0; index != m_vec_strPageLinks.size(); index++) {
    fus_fetching.push_back(async(__GetLinks, this, index));
    this_thread::sleep_for(chrono::milliseconds(1000));
  }

  //回收数据
  int fetch_page = m_nPageStart;
  for (auto& fu : fus_fetching) {
    for (auto& link : fu.get()) {
      m_rvec_strLinks.push_back(link);
    }
    float progress = (fetch_page - m_nPageStart + 1) * 1.0f /
                     (m_nPageEnd - m_nPageStart + 1) * 100.0f;
    spdlog::info("[{:.1f}%]第 {} 页链接数据已下载", progress, fetch_page);
    fetch_page++;
  }
}

std::vector<std::string> WebParser::__GetLinks(int pageLinksIndex) {
  string webSource;
  Downloader::DownloadPageToString(m_vec_strPageLinks[pageLinksIndex],
                                   webSource);
  LinkParser parser(webSource);
  return parser.vec_strLinks;
}
