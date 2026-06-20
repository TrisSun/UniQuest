// ============================================================
// Guild.h
// 冒险者公会(总控类)的声明。
// Guild 把 Adventurer(用户)和 Quest(委托)组织起来,负责
// 数据的加载/保存、注册登录、发布/接取/完成委托以及菜单交互,
// 是整个系统的核心。
// ============================================================
#ifndef GUILD_H   // 头文件保护,防止被重复包含
#define GUILD_H

#include <string>
#include <vector>          // 用 vector 容器存放所有用户和委托
#include "Adventurer.h"
#include "Quest.h"

// 冒险者公会:整个系统的总控类,管理用户与委托,负责文件读写与菜单交互
class Guild {
public:
    Guild();

    // 程序入口:加载数据并进入主循环
    void run();

private:
    std::vector<Adventurer> users;              // 所有用户
    std::vector<Quest> quests;                  // 所有委托
    std::vector<std::string> jiangsuUniversities; // 江苏省高校白名单(从文件加载)
    int currentUserIndex;          // 当前登录用户在 users 中的下标,-1 表示未登录
    int nextQuestId;               // 下一个委托编号

    // 文件读写
    void loadUsers();
    void saveUsers();
    void loadQuests();
    void saveQuests();
    void loadUniversities();       // 从 universities.txt 加载高校白名单

    // 账号
    void registerUser();
    void login();
    void logout();

    // 委托操作
    void publishQuest();
    void takeQuest();
    void completeQuest();
    void showAllQuests();
    void showMyInfo();

    // 菜单
    void authMenu();  // 未登录时的菜单
    void mainMenu();  // 登录后的菜单

    // 辅助
    int findUser(const std::string& username) const; // 返回下标,找不到返回 -1
    bool isJiangsuUniversity(const std::string& school) const;
};

#endif
