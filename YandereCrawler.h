#pragma once

#include <curl/curl.h>  //from vcpkg
#include <stdlib.h>

#include <iostream>
#include <string>
#include <vector>

#include "Downloader.h"
#include "UserInterface.h"
#include "WebParser.h"

class App {
 public:
  int Main();
};