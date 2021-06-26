#ifndef __CONFIGREADER_H__
#define __CONFIGREADER_H__

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

#include "Config.h"

//从json文件中读取设置
//如果json文件不存在那么生成一份默认配置
class ConfigReader
{
public:
  //默认路径的设置文件是否存在
  static bool IsExist();
  //从默认路径读取设置文件
  static Config Read();
  //将默认设置文件写出
  static void WriteDefault();
};

#endif // __CONFIGREADER_H__