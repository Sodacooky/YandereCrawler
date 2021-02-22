#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

/*
		用户交互集合
		提供"界面"，然后从这里获取输入的数据
*/
class UserInterface {
public:
	//用户输入页范围
	void PageRangeInput();

	//用户输入tags
	void TagsInput();

public:
	//page
	unsigned int unPageStart = 1;
	unsigned int unPageEnd   = 1;

	//tags
	std::vector<std::string> vstrTags;
};
