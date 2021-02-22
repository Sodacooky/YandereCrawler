#include "Downloader.h"

using namespace std;

static bool bPathAvaliable = false;
static const std::string strAgent = "User-Agent,Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.182 Safari/537.36 Edg/88.0.705.74";

bool Downloader::DownloadPageToString(const std::string& url, std::string& src_out) {
	//init curl
	CURL* handle = curl_easy_init();
	//set
	__SetCurlDefaultOpt(handle);
	curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, __WriteToString);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, &src_out);
	//do
	auto result = curl_easy_perform(handle);
	bool ret = true;
	if (result != CURLE_OK) {
		string output_msg = "CURL ERROR CODE : " + to_string(result) + "\n";
		cout << output_msg;
		ret = false;
	}
	//clean
	curl_easy_cleanup(handle);
	return ret;
}

bool Downloader::DownloadFile(const std::string& url) {
	//io
	string path = bPathAvaliable ? "./download/" : "./";
	string filename = path + __ExtractFilename(url);
	FILE* file;
	fopen_s(&file, filename.c_str(), "wb");
	if (file == nullptr) {
		cout << " 读写错误！\n";
		return false;
	}
	//init curl
	CURL* download_handle = curl_easy_init();
	//set
	__SetCurlDefaultOpt(download_handle);
	curl_easy_setopt(download_handle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(download_handle, CURLOPT_WRITEFUNCTION, nullptr);
	curl_easy_setopt(download_handle, CURLOPT_WRITEDATA, file);
	//do
	auto result = curl_easy_perform(download_handle);
	//check
	if (result != CURLE_OK) {
		string output_msg_fail = "CURL ERROR CODE : " + to_string(result) + "\n";
		cout << output_msg_fail;
		//clean
		curl_easy_cleanup(download_handle);
		fclose(file);
		return false;
	}
	//output success message
	string output_msg = "已下载 ";
	if (filename.size() > 56) {
		output_msg.append(filename.begin(), filename.begin() + 32);
		output_msg.append("....\n");
	}
	else {
		output_msg.append(filename.begin(), filename.end());
		output_msg.append(" \n");
	}
	cout << output_msg;
	//clean
	curl_easy_cleanup(download_handle);
	fclose(file);
	return true;
}

void Downloader::AsyncDownloadFiles(const std::vector<std::string>& links) {
	std::vector<future<bool>> vfus;
	for (int i = 0; i != links.size(); i++) {
		vfus.push_back(async(DownloadFile, links[i]));
		this_thread::sleep_for(chrono::seconds(2));
	}
	for (int i = 0; i != vfus.size(); i++) {
		//重新下载失败的，并且是单线程进行，确保可靠
		if (vfus[i].valid() && vfus[i].get() == false) {
			if (!DownloadFile(links[i])) {
				string output_msg = "AsyncDownloadFiles()下载 " + links[i] + " 失败\n";
				cout << output_msg;
			}
		}
	}
}

void Downloader::CreatePath() {
	bool succeed = CreateDirectory(L"./download", nullptr);
	if (!succeed) {
		if (GetLastError() != ERROR_ALREADY_EXISTS) {
			cout << "无法创建文件夹，将在程序目录存放图片" << endl;
		}
	}
	bPathAvaliable = true;
}

size_t Downloader::__WriteToString(char* buff, size_t block_size, size_t block_amount, void* str_ptr) {
	auto str = (string*)str_ptr;
	auto size = block_amount * block_size;
	str->append((char*)buff, size);
	return size;
}

void Downloader::__SetCurlDefaultOpt(CURL* handle) {
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(handle, CURLOPT_USERAGENT, strAgent.c_str());
}

void Downloader::__TransSymbol(std::string& str) {
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

std::string Downloader::__ExtractFilename(const std::string& src_str) {
	//从尾部开始找，从第一个斜杠后开始到最后
	auto start = src_str.begin();
	for (auto iter = src_str.end() - 1; iter != src_str.begin(); iter--) {
		if (*iter == '/') {
			start = iter;
			break;
		}
	}
	string tmp(start + 1, src_str.end());
	__TransSymbol(tmp);
	return tmp;
}