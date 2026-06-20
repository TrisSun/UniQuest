// ============================================================
// Adventurer.cpp
// Adventurer 类的实现:构造、升级逻辑、与文件文本互转、读取接口。
// ============================================================
#include "Adventurer.h"
#include <sstream>
// 默认构造函数:新对象的等级从 1 开始,经验从 0 开始
Adventurer::Adventurer() : username(""), password(""), school(""), level(1), exp(0) {}
// 带参构造函数:用注册时输入的信息初始化,等级、经验仍为初始值
Adventurer::Adventurer(const std::string& username,
                       const std::string& password,
                       const std::string& school)
    : username(username), password(password), school(school), level(1), exp(0) {}

// 返回"当前等级升到下一级"所需的经验值
int Adventurer::expToNext() const {
    // 升级公式:第 level 级升到下一级需要 level * 100 点经验
    // 等级越高,升级所需经验越多，这很合理！
    return level * 100;
}

// 获得经验值,并在经验足够时自动升级
void Adventurer::gainExp(int amount) {
    if (amount > 0) {
        exp += amount;
    }
    // 经验足够则升级,多出来的经验保留,一次可连升多级
    while (exp >= expToNext()) {
        exp -= expToNext();  // 扣掉本级升级所需经验
        level++;             // 升级啦，很简单！
    }
}

// 把对象打包成文件中的一行文本,字段之间用空格分隔
// 格式:用户名 密码 学校 等级 经验
std::string Adventurer::toLine() const {
    std::ostringstream oss;
    oss << username << " " << password << " " << school << " " << level << " " << exp;
    return oss.str();
}

// 从文件一行还原对象,与 toLine 的格式对应
void Adventurer::fromLine(const std::string& line) {
    std::istringstream iss(line);
    iss >> username >> password >> school >> level >> exp;
}

// 以下为各成员的只读访问函数
std::string Adventurer::getUsername() const { return username; }
std::string Adventurer::getPassword() const { return password; }
std::string Adventurer::getSchool() const { return school; }
int Adventurer::getLevel() const { return level; }
int Adventurer::getExp() const { return exp; }
