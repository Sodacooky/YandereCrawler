#include "WebParser.h"

using namespace std;

WebParser::WebParser(vector<string>& tags, int page) {
  //生成链接并下载网页
  string url = "https://yande.re/post?page=" + to_string(page);
  url.append("&tags=");
  for (auto& tag : tags) {
    url.append(tag);
    url.append("+");
  }
  string webSource;
  Downloader::DownloadPageToString(url, webSource);
  //获取链接
  LinkParser linkParser(webSource);
  //填充
  vec_strLinks.swap(linkParser.vec_strLinks);
}
