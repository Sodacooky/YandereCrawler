#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <curl/curl.h>	//from vcpkg

class App {
public:
	int Main();

private:
	/*
		储存的是__WriteSourceCodeToString()写入的网页源码
		下一次curl perform前应该清空它，进行重写
	*/
	std::string m_strNowWebSourceCode;

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

	//判断网页可用性
	bool IsPageExist();

	//从当前m_strNowWebSourceCode源码中提取出tags字符串
	//前提网页得不是404
	std::string __ExtractTags();

	//从当前m_strNowWebSourceCode源码中提取出大图的链接
	//前提网页得不是404
	std::string __ExtractLargeLink();
};