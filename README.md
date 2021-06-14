# 简介

C++ yande.re 下载器

输入tags，界定页的范围，然后在yande.re中下载符合输入的tags的图片。

仅保证Windows平台。

虽然可在Linux下编译通过，但经我测试yande.re无法在Linux下通过CURL(通过proxychains)正常访问，也许需要全局____。

# 安装与编译

依赖curl、spdlog。

如果你使用vcpkg并设置好了环境变量那么你可以直接使用CMake编译或使用VisualStudio。

否则你需要自己修改CMake。

# 为什么是C++？

懂的都懂。

# 关于滥用

yande.re原则上不允许此类程序，~~我并不会为此程序的滥用负责~~， 但是管理员态度不太好，上传原图说是low-res打回这都是他们的小问题了（正好是个zh_cn管理员），手动上传的用户被消磨了积极性那么爬虫用户也离这不远了。

换句话说，下爆。