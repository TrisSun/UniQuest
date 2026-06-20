// ============================================================
// Quest.cpp
// Quest 类的实现:构造、状态文字、与文件文本互转、读取/修改接口。
// ============================================================
#include "Quest.h"
#include <sstream>
#include <cstdlib>   // atoi

// 默认构造:新委托无接取者(用 "-" 占位),状态为"待接取"
Quest::Quest()
    : id(0), title(""), desc(""), publisher(""), taker("-"),
      status(QUEST_WAITING), rewardExp(0), difficulty(1) {}

// 带参构造函数:发布委托时填入编号、标题、描述等;接取者初始为空,状态为"待接取"
Quest::Quest(int id,
             const std::string& title,
             const std::string& desc,
             const std::string& publisher,
             int rewardExp,
             int difficulty)
    : id(id), title(title), desc(desc), publisher(publisher), taker("-"),
      status(QUEST_WAITING), rewardExp(rewardExp), difficulty(difficulty) {}

std::string Quest::statusText() const {
    if (status == QUEST_WAITING) return "待接取";
    if (status == QUEST_TAKEN)   return "进行中";
    return "已完成";
}

// 打包成文件一行,用竖线 | 分隔(标题、描述可能含空格,故不用空格)
std::string Quest::toLine() const {
    std::ostringstream oss;
    oss << id << "|" << title << "|" << desc << "|" << publisher << "|"
        << taker << "|" << status << "|" << rewardExp << "|" << difficulty;
    return oss.str();
}

// 从文件中的一行文本还原委托对象
// 用 getline(..., '|') 按竖线逐段读取,数字字段再用 atoi 转成整数
void Quest::fromLine(const std::string& line) {
    std::istringstream iss(line);
    std::string token;   // 临时存放需要转成数字的字段

    std::getline(iss, token, '|'); id = std::atoi(token.c_str());
    std::getline(iss, title, '|');
    std::getline(iss, desc, '|');
    std::getline(iss, publisher, '|');
    std::getline(iss, taker, '|');
    std::getline(iss, token, '|'); status = std::atoi(token.c_str());
    std::getline(iss, token, '|'); rewardExp = std::atoi(token.c_str());
    std::getline(iss, token, '|'); difficulty = std::atoi(token.c_str());
}

// 以下为各成员的只读访问函数
int Quest::getId() const { return id; }
std::string Quest::getTitle() const { return title; }
std::string Quest::getDesc() const { return desc; }
std::string Quest::getPublisher() const { return publisher; }
std::string Quest::getTaker() const { return taker; }
int Quest::getStatus() const { return status; }
int Quest::getRewardExp() const { return rewardExp; }
int Quest::getDifficulty() const { return difficulty; }

// 以下为修改函数
void Quest::setTaker(const std::string& name) { taker = name; }  // 设置接取者
void Quest::setStatus(int s) { status = s; }                     // 设置委托状态
