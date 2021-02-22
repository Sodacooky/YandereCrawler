#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>	//from vcpkg

#include "Downloader.h"
#include "WebParser.h"
#include "UserInterface.h"

class App {
public:
	int Main();
};