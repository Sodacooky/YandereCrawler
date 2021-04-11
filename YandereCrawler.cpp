#include "YandereCrawler.h"

using namespace std;

int main() {
  // change windows terminal(cmd&powershell) encoding
  system("chcp 65001");
  // Application entrance
  App app;
  return app.Main();
}

int App::Main() {
  // user decide tags and pages range
  UserInterface userinput;
  userinput.TagsInput();
  cout << "正在获取页数，共 " << WebParser::GetMaxPage(userinput.vstrTags)
       << " 页" << endl;
  userinput.PageRangeInput();

  // start task
  Downloader::CreatePath();
  for (unsigned int now_page = userinput.unPageStart;
       now_page <= userinput.unPageEnd; now_page++) {
    cout << "*** 正在下载页 " << now_page << " ***" << endl;
    auto parser = new WebParser(userinput.vstrTags, now_page);
    if (parser->bAvaliable == false) {
      delete parser;
      break;
    }
    Downloader::AsyncDownloadFiles(parser->vstrLinks);
    delete parser;
  }
  cout << endl;
  cout << "\t━━━━━━━━━━━━━━━━━━━\n";
  cout << "\t*** 似乎没有更多了 ***\n";
  cout << "\t━━━━━━━━━━━━━━━━━━━\n";
  cin.ignore(64, '\n');
  cin.get();
  return 0;
}