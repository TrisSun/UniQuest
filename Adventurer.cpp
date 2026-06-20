// Adventurer.cpp - Adventurer 类的实现
#include "Adventurer.h"
#include <sstream>

Adventurer::Adventurer() : username(""), password(""), school(""), level(1), exp(0) {}

Adventurer::Adventurer(const std::string& username,
                       const std::string& password,
                       const std::string& school)
    : username(username), password(password), school(school), level(1), exp(0) {}

int Adventurer::expToNext() const {
    // 升级公式:第 level 级升到下一级需要 level * 100 点经验
    return level * 100;
}

void Adventurer::gainExp(int amount) {
    if (amount > 0) {
        exp += amount;
    }
    // 经验足够则升级,多出来的经验保留,一次可连升多级
    while (exp >= expToNext()) {
        exp -= expToNext();
        level++;
    }
}

// 打包成文件一行,格式:用户名 密码 学校 等级 经验(空格分隔)
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

std::string Adventurer::getUsername() const { return username; }
std::string Adventurer::getPassword() const { return password; }
std::string Adventurer::getSchool() const { return school; }
int Adventurer::getLevel() const { return level; }
int Adventurer::getExp() const { return exp; }
