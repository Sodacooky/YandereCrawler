#include "YandereCrawler.h"

using namespace std;

int main() {
	App app;
	return app.Main();
}

int App::Main() {
	UserInterface userinput;
	userinput.TagsInput();
	userinput.PageRangeInput();
	Downloader::CreatePath();
	for (unsigned int now_page = userinput.unPageStart; now_page <= userinput.unPageEnd; now_page++) {
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
	cin.get();
	return 0;
}