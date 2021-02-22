#include "UserInterface.h"

using namespace std;

void UserInterface::PageRangeInput() {
	while (true) {
		cout << "请输入 <起始页> <终止页>: ";
		cin >> unPageStart >> unPageEnd;

		if (unPageStart > unPageEnd) {
			cout << endl << "\t** 顺序有误 **" << endl;
		}
		else {
			break;
		}
	}

	cout << "将从页" << unPageStart << "到页" << unPageEnd << endl;
}

void UserInterface::TagsInput() {
	string tmpInput;
	cout << "输入Tags，以空格间隔多个:" << endl;
	getline(cin, tmpInput);

	stringstream ss(tmpInput);	//reuse
	while (ss >> tmpInput) {
		vstrTags.push_back(tmpInput);
	}

	cout << "共输入" << vstrTags.size() << "个标签" << endl;
}