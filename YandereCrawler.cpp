#include "YandereCrawler.h"

using namespace std;

int main() {
	App app;
	return app.Main();
}

int App::Main() {
	__TagsInputInterface();
	__IdRangeInputInterface();

	curl_global_init(CURL_GLOBAL_ALL);

	const int THREAD_AMOUNT = 1024;
	future<void> fu[THREAD_AMOUNT];
	//初次
	for (int i=0; m_unId <= m_unEndId && i < THREAD_AMOUNT; m_unId++, i++) {
		fu[i] = async(&App::__Process, this, m_unId);
		this_thread::sleep_for(chrono::milliseconds(200));
	}
	//loop
	while (m_unId <= m_unEndId) {
		//未完成则等待
		fu[m_unId % THREAD_AMOUNT].wait();
		//分配新的任务
		fu[m_unId % THREAD_AMOUNT] = async(&App::__Process, this, m_unId);
		//
		m_unId++;
		this_thread::sleep_for(chrono::milliseconds(125));
	}

	curl_global_cleanup();
	cout << "Done." << endl;
	cin.get();
	return 0;
}

void App::__Process(unsigned int pic_id) {
	string src;

	CURL* handle = curl_easy_init();
	curl_easy_setopt(handle, CURLOPT_URL, __MakeURL(pic_id).c_str());
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, __WriteSourceCodeToString);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, &src);
	curl_easy_setopt(handle, CURLOPT_USERAGENT, R"(User-Agent,Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.182 Safari/537.36 Edg/88.0.705.74)");
	auto ret = curl_easy_perform(handle);
	curl_easy_cleanup(handle);

	if (ret != CURLE_OK) {
		printf("[%d]\tCURL错误\n", pic_id);
		return;
	}

	if (!__IsPageExist(src)) {
		printf("[%d]\t404 Not Found\n", pic_id);

		return;
	}

	if (!__IsMatch(__ExtractTags(src))) {
		printf("[%d]\t不匹配\n", pic_id);

		return;
	}

	__DownloadPicture(__ExtractLargeLink(src));
	printf("[%d]\t下载完成\n", pic_id);
}

void App::__TagsInputInterface() {
	cout << "输入tags，以空格间隔，回车结束输入：" << endl;

	string tmp;
	getline(cin, tmp);

	stringstream ss(tmp);
	while (ss >> tmp) {
		m_vec_strTags.push_back(tmp);
	}

	cout << endl << "你输入了 " << m_vec_strTags.size() << " 个标签" << endl;
}

void App::__IdRangeInputInterface() {
	while (true) {
		cout << endl << "输入起始的图片id: ";
		cin >> m_unId;

		cout << endl << "输入结束的图片id: ";
		cin >> m_unEndId;

		if (m_unId > m_unEndId) {
			cout << "***  输入的id不正确，起始id应小于结束id  ***" << endl;
		}
		else {
			break;
		}
	}
}

size_t App::__WriteSourceCodeToString(void* buff, size_t block_size, size_t block_amount, void* string_ptr) {
	auto pString = (string*)string_ptr;
	pString->append((char*)buff, block_amount * block_size);
	return block_amount * block_size;
}

void App::__DownloadPicture(const std::string& link) {
	FILE* file;
	fopen_s(&file, __ExtractFilename(link).c_str(), "wb");
	if (file == nullptr) {
		cout << " 读写错误！" << endl;
		return;
	}

	//new curl
	CURL* download_handle = curl_easy_init();
	//set
	curl_easy_setopt(download_handle, CURLOPT_URL, link.c_str());
	curl_easy_setopt(download_handle, CURLOPT_WRITEFUNCTION, nullptr);
	curl_easy_setopt(download_handle, CURLOPT_WRITEDATA, file);
	curl_easy_setopt(download_handle, CURLOPT_SSL_VERIFYHOST, 0);
	curl_easy_setopt(download_handle, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(download_handle, CURLOPT_USERAGENT, R"(User-Agent,Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.182 Safari/537.36 Edg/88.0.705.74)");

	//do
	auto ret = curl_easy_perform(download_handle);

	//check
	//if (ret == CURLE_OK) {
	//	cout << " 下载完成！" << endl;
	//}
	//else {
	//	cout << " 失败！" << endl;
	//}

	//clean
	curl_easy_cleanup(download_handle);
	fclose(file);
}

bool App::__IsPageExist(const std::string& src) {
	if (src.find("Not Found") == string::npos) {
		return true;
	}
	return false;
}

bool App::__IsMatch(const std::string& str_tags) {
	for (auto& tag : m_vec_strTags) {
		if (str_tags.find(tag) == string::npos) {
			return false;
		}
	}

	return true;
}

std::string App::__MakeURL(unsigned int id) {
	const string header = "https://yande.re/post/show/";
	return header + to_string(id);
}

std::string App::__ExtractTags(const std::string& src) {
	//从HTML的meta中提取

	//固定的开头
	const string left_keyword = R"(<meta property="og:description" content=")";
	//tags信息以另一边引号结束

	auto left_keyword_pos = src.find(left_keyword);
	auto tags_startpos = left_keyword_pos + left_keyword.size();

	auto tags_endpos = src.find("\"", tags_startpos);

	return std::string(src.begin() + tags_startpos, src.begin() + tags_endpos);
}

std::string App::__ExtractLargeLink(const std::string& src) {
	//原图链接都以此开头
	const string left_keyword = R"(https://files.yande.re/image/)";
	auto startpos = src.find(left_keyword);

	//链接后都是引号
	auto endpos = src.find("\"", startpos);

	return std::string(src.begin() + startpos, src.begin() + endpos);
}

std::string App::__ExtractFilename(const std::string& link) {
	//从尾部开始找，从第一个斜杠后开始到最后
	auto start = link.begin();
	for (auto iter = link.end() - 1; iter != link.begin(); iter--) {
		if (*iter == '/') {
			start = iter;
			break;
		}
	}

	string tmp(start + 1, link.end());
	__TransSymbol(tmp);

	return tmp;
}

void App::__TransSymbol(std::string& str) {
	for (auto iter = str.begin(); iter != str.end();) {
		if (*iter == '%') {
			iter = str.erase(iter, iter + 3);
			iter = str.insert(iter, ' ') + 1;
		}
		else {
			iter++;
		}
	}
}