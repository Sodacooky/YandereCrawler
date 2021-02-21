#include "YandereCrawler.h"

using namespace std;

int main() {
	App app;
	return app.Main();
}

int App::Main() {
	curl_global_init(CURL_GLOBAL_ALL);
	CURL* handle = curl_easy_init();

	curl_easy_setopt(handle, CURLOPT_URL, "https://yande.re/post/show/742047");
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, __WriteSourceCodeToString);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, &m_strNowWebSourceCode);
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);

	curl_easy_perform(handle);
	curl_easy_cleanup(handle);
	curl_global_cleanup();

	if (m_strNowWebSourceCode.find("hakurei_reimu") != string::npos) {
		cout << "Match!" << endl;
	}

	return 0;
}

void App::__TagsInputInterface() {
	cout << "输入tags，以空格间隔，回车结束输入：" << endl;

	string tmp;
	getline(cin, tmp);

	stringstream ss(tmp);
	while (ss >> tmp) {
		m_vec_strTags.push_back(tmp);
	}

	cout << "你输入了 " << m_vec_strTags.size() << " 个标签" << endl;
}

void App::__IdRangeInputInterface() {
	while (true) {
		cout << "输入起始的图片id: ";
		cin >> m_unId;
		cout << endl << "输入结束的图片id: ";
		cin >> m_unEndId;

		if (m_unId > m_unEndId) {
			cout << "*** 输入的id不正确，起始id应小于结束id ***" << endl;
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

bool App::IsPageExist() {
	if (m_strNowWebSourceCode.find("Not Found") == string::npos) {
		return true;
	}
	return false;
}

std::string App::__ExtractTags() {
	//从HTML的meta中提取

	//固定的开头
	const string left_keyword = R"(<meta property="og:description" content=")";
	//tags信息以另一边引号结束

	auto left_keyword_pos = m_strNowWebSourceCode.find(left_keyword);
	auto tags_startpos = left_keyword_pos + left_keyword.size();

	auto tags_endpos = m_strNowWebSourceCode.find("\"", tags_startpos);

	return std::string(m_strNowWebSourceCode.begin() + tags_startpos, m_strNowWebSourceCode.begin() + tags_endpos);
}

std::string App::__ExtractLargeLink() {
	//原图链接都以此开头
	const string left_keyword = R"(https://files.yande.re/image/)";
	auto startpos = m_strNowWebSourceCode.find(left_keyword);

	//链接后都是引号
	auto endpos = m_strNowWebSourceCode.find("\"", startpos);

	return std::string(m_strNowWebSourceCode.begin() + startpos, m_strNowWebSourceCode.begin() + endpos);
}