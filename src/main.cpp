#include <spdlog/spdlog.h>

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "Downloader.h"
#include "NavigationParser.h"
#include "WebParser.h"

using namespace std;

class App {
 public:
  int Main() {
    // user decide tags
    TagsInput();
    spdlog::info("输入了 {} 个tags", vec_strTags.size());
    if (vec_strTags.size() < 1) {
      return -1;
    }

    spdlog::info("正在获取页数...");

    // fetch page amount
    FetchPageAmount();
    if (nTagsPageAmount <= 0) {
      return -2;
    }

    // user decide page range
    PageInput();
    spdlog::info("将下载从{}到{}页，共{}页", nPageLeft, nPageRight,
                 nPageRight - nPageLeft + 1);

    spdlog::info("正在下载链接数据...");

    // get links
    FetchLinks();

    // create path
    Downloader::CreatePath(vec_strTags[0]);

    spdlog::info("开始下载图片...");

    // downloaded
    Downloader::MultiThreadDownloadFiles(vec_strLinks);

    cout << endl;
    spdlog::info("所有图片下载完成!");
    return 0;
  }

  void TagsInput() {
    string tmpInput;
    cout << "输入Tags，以空格间隔多个:" << endl;
    getline(cin, tmpInput);

    stringstream ss(tmpInput);  // reuse
    while (ss >> tmpInput) {
      vec_strTags.push_back(tmpInput);
    }
  }

  void PageInput() {
    while (true) {
      cout << "请输入 <起始页> <终止页>: ";
      cin >> nPageLeft >> nPageRight;
      if (nPageLeft <= 0 || nPageRight <= 0) {
        spdlog::error("输入的页数不能为0或负数");
        continue;
      }
      if (nPageLeft > nPageRight) {
        spdlog::error("输入的页数范围不正确，{}>{}", nPageLeft, nPageRight);
        continue;
      }
      if (nPageLeft > nTagsPageAmount) {
        spdlog::error("起始页超过了页数范围");
        continue;
      }
      if (nPageRight > nTagsPageAmount) {
        spdlog::warn("终止页超过了页数范围，将在最后一页停止");
        nPageRight = nTagsPageAmount;
        // break;
      }
      break;
    }
  }

  void FetchPageAmount() {
    string first_page_url = "https://yande.re/post?tags=";
    for (auto &tag : vec_strTags) {
      first_page_url.append(tag);
      first_page_url.append("+");
    }
    string first_page_source;
    Downloader::DownloadPageToString(first_page_url, first_page_source);
    NavigationParser navigationParser(first_page_source);
    spdlog::info("检查到当前输入Tags有 {} 页结果",
                 navigationParser.nPageAmount);
    if (navigationParser.nPageAmount <= 0) {
      spdlog::error("请检查是否能连接到yande.re和Tags是否输入正确");
    }
    nTagsPageAmount = navigationParser.nPageAmount;
  }

  void FetchLinks() {
    WebParser parser(vec_strTags, nPageLeft, nPageRight, vec_strLinks);
    // it done
  }

 public:
  //[nPageLeft,nPageRight]
  int nPageLeft, nPageRight;
  //
  std::vector<std::string> vec_strTags;
  //
  int nTagsPageAmount;
  //
  std::vector<std::string> vec_strLinks;
};

int main() {
  // Application entrance
  App app;
  return app.Main();
}