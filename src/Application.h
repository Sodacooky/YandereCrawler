#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <map>
#include <string>
#include <vector>
#include "Config.h"

//应用程序
class Application
{
  public:
    Application();

  public:
    // main
    void Main();

  private:
    //用户交互
    void __PromptInput();

    //分配下载线程
    //全部下载完成后返回
    void __DispatchDownloadThred(const std::map<std::string, std::string> &links);

    //下载线程
    void __DownloadThreadFunc(const std::string &link, const std::string &filename);

    //创建文件夹
    // dir_name不以分隔符结尾
    static bool __CreateDirectory(const std::string &dir_name);

    //将二进制数据保存
    // dir_name不以分隔符结尾
    static void __SaveToFile(const std::string &dirname,
                             const std::string &filename,
                             const std::vector<char> &data);

  private:
    //配置文件
    Config m_config;
    //输入的Tags行
    std::string m_strTagsLine;
    //页码范围
    int m_nStart, m_nEnd;
    //保存文件的目录
    std::string m_strDirectory;
};

#endif  // __APPLICATION_H__