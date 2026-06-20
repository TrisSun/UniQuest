// ============================================================
// Adventurer.h
// 冒险者(用户)类的声明。
// 一个 Adventurer 对象代表一个注册用户,保存其账号信息以及
// 在公会中的成长数据(等级、经验)。
// ============================================================
#ifndef ADVENTURER_H   // 头文件保护,防止被重复包含
#define ADVENTURER_H

#include <string>

// 冒险者(用户)类:记录账号信息、等级与经验
class Adventurer {
public:
    Adventurer();                                  // 默认构造:等级为 1、经验为 0
    Adventurer(const std::string& username,        // 带参构造:注册新用户时使用
               const std::string& password,
               const std::string& school);

    // 获得经验值,并在达到阈值时自动升级(可连升多级)
    void gainExp(int amount);
    // 当前等级升到下一级所需的经验值
    int expToNext() const;

    // 与文件一行文本互相转换(空格分隔)
    std::string toLine() const;
    void fromLine(const std::string& line);

    // 下面是一组只读访问函数(getter),供外部获取私有成员的值
    std::string getUsername() const;
    std::string getPassword() const;
    std::string getSchool() const;
    int getLevel() const;
    int getExp() const;

private:
    std::string username; // 用户名
    std::string password; // 密码
    std::string school;   // 所在学校
    int level;            // 等级
    int exp;              // 当前经验
};

#endif
