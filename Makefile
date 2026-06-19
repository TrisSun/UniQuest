# 简单 Makefile:在装有 g++ 的环境下,命令行执行 make 即可编译
# 生成可执行文件 guild(Windows 下为 guild.exe)

CXX = g++
CXXFLAGS = -Wall -std=c++11

guild: main.cpp Adventurer.cpp Quest.cpp Guild.cpp
	$(CXX) $(CXXFLAGS) -o guild main.cpp Adventurer.cpp Quest.cpp Guild.cpp

clean:
	rm -f guild guild.exe
