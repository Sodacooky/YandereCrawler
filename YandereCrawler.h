#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
#include <chrono>
#include <future>
#include <curl/curl.h>	//from vcpkg

class App {
public:
	int Main();

private:
	//用户输入的一组tags，必须所有tag都能在源码中找到才算是命中
	std::vector<std::string> m_vec_strTags;

	//当前网页的Id，逐渐递增
	unsigned int m_unId = 1;

	//目标停止网页Id
	unsigned int m_unEndId = 0;

private:
	//请用户输入tags
	void __TagsInputInterface();

	//请用户设置起始查找ID和结束ID
	void __IdRangeInputInterface();

	//curl写网页到字符串m_strNowWebSourceCode的callback
	static size_t __WriteSourceCodeToString(void* buff, size_t block_size, size_t block_amount, void* string_ptr);

	//curl下载图片
	void __DownloadPicture(const std::string& link);

	//给ID，分析这个页面，如果命中则下载
	void __Process(unsigned int pic_id);

	//用ID生成yande.re show链接
	std::string __MakeURL(unsigned int id);

	//判断网页可用性
	bool __IsPageExist(const std::string& src);

	//判断是否tags全部满足
	bool __IsMatch(const std::string& str_tags);

	//从当前m_strNowWebSourceCode源码中提取出tags字符串
	//前提网页得不是404
	std::string __ExtractTags(const std::string& src);

	//从当前m_strNowWebSourceCode源码中提取出大图的链接
	//前提网页得不是404
	std::string __ExtractLargeLink(const std::string& src);

	//用__ExtractLargeLink()的或其他的链接，提取出文件名
	std::string __ExtractFilename(const std::string& link);

	//将文件名中的%20去掉
	void __TransSymbol(std::string& str);
};