// ============================================================
// Guild.cpp
// 冒险者公会(总控类)的实现。
// 包含:数据文件读写、注册/登录、发布/接取/完成委托、信息展示
// 以及两层菜单(未登录菜单 authMenu、登录后主菜单 mainMenu)。
// ============================================================
#include "Guild.h"
#include <iostream>
#include <fstream>    // 文件读写
#include <sstream>
#include <cstdlib>    // atoi、exit

using namespace std;

// 内置的江苏省高校名单,只有名单内学校的学生才能注册
static const char* JIANGSU_UNIVERSITIES[] = {
    "南京大学", "东南大学", "南京航空航天大学", "南京理工大学",
    "河海大学", "南京师范大学", "苏州大学", "江南大学",
    "中国矿业大学", "南京农业大学", "江苏大学", "扬州大学",
    "南京邮电大学", "南京工业大学", "中国药科大学"
};
static const int JIANGSU_COUNT = 15;

// 读取一整行输入(用于可能含空格的内容)
static string readLine(const string& prompt) {
    cout << prompt;
    string line;
    getline(cin, line);
    return line;
}

// 读取一个整数(带简单的非法输入处理)
static int readInt(const string& prompt) {
    cout << prompt;
    string line;
    getline(cin, line);
    return atoi(line.c_str());
}

// 构造函数:初始未登录(下标 -1),委托编号从 1 开始计
Guild::Guild() : currentUserIndex(-1), nextQuestId(1) {}

// 程序主流程:先读入文件数据,再不断显示菜单处理用户操作
void Guild::run() {
    loadUsers();
    loadQuests();

    cout << "============================================" << endl;
    cout << "        欢迎来到江苏冒险者公会系统" << endl;
    cout << "============================================" << endl;

    // 根据是否登录显示不同菜单:未登录显示注册/登录,已登录显示功能菜单
    while (true) {
        if (currentUserIndex == -1) {
            authMenu();
        } else {
            mainMenu();
        }
    }
}

// ---------------- 文件读写 ----------------

// 从 users.txt 逐行读入用户:每读一行就还原成一个 Adventurer 加入 users
void Guild::loadUsers() {
    users.clear();
    ifstream fin("users.txt");
    if (!fin) return; // 文件不存在则当作没有用户
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;   // 跳过空行
        Adventurer a;
        a.fromLine(line);
        users.push_back(a);
    }
    fin.close();
}

// 把内存中所有用户写回 users.txt(每个用户一行,覆盖原文件)
void Guild::saveUsers() {
    ofstream fout("users.txt");
    for (size_t i = 0; i < users.size(); ++i) {
        fout << users[i].toLine() << endl;
    }
    fout.close();
}

// 从 quests.txt 逐行读入委托,并算出下一个可用的委托编号
void Guild::loadQuests() {
    quests.clear();
    nextQuestId = 1;
    ifstream fin("quests.txt");
    if (!fin) return;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;   // 跳过空行
        Quest q;
        q.fromLine(line);
        quests.push_back(q);
        // nextQuestId 始终比已有最大编号大 1,保证新编号不重复
        if (q.getId() >= nextQuestId) {
            nextQuestId = q.getId() + 1;
        }
    }
    fin.close();
}

// 把内存中所有委托写回 quests.txt(每条委托一行,覆盖原文件)
void Guild::saveQuests() {
    ofstream fout("quests.txt");
    for (size_t i = 0; i < quests.size(); ++i) {
        fout << quests[i].toLine() << endl;
    }
    fout.close();
}

// ---------------- 辅助函数 ----------------

// 按用户名在 users 中查找,找到返回其下标,找不到返回 -1(线性查找)
int Guild::findUser(const string& username) const {
    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i].getUsername() == username) return (int)i;
    }
    return -1;
}

// 校验学校是否在内置的江苏高校名单内,用于限制只有江苏学生能注册
bool Guild::isJiangsuUniversity(const string& school) const {
    for (int i = 0; i < JIANGSU_COUNT; ++i) {
        if (school == JIANGSU_UNIVERSITIES[i]) return true;
    }
    return false;
}

// ---------------- 账号 ----------------

// 注册新用户:依次校验用户名是否为空、是否重复、学校是否为江苏高校、密码是否为空
void Guild::registerUser() {
    cout << "\n----- 注册新冒险者 -----" << endl;
    string username = readLine("请输入用户名(不含空格): ");
    if (username.empty()) {
        cout << "用户名不能为空!" << endl;
        return;
    }
    if (findUser(username) != -1) {   // 用户名必须唯一
        cout << "该用户名已存在,请换一个!" << endl;
        return;
    }

    string school = readLine("请输入你的学校(必须是江苏省高校): ");
    if (!isJiangsuUniversity(school)) {   // 限制:只有江苏高校学生可注册
        cout << "抱歉,本公会只对江苏省高校学生开放,注册失败!" << endl;
        cout << "(支持的学校示例:南京大学、东南大学、南京航空航天大学 等)" << endl;
        return;
    }

    string password = readLine("请输入密码(不含空格): ");
    if (password.empty()) {
        cout << "密码不能为空!" << endl;
        return;
    }

    // 校验全部通过,创建新用户加入内存并立即写回文件保存
    users.push_back(Adventurer(username, password, school));
    saveUsers();
    cout << "注册成功!欢迎你," << username << "!现在可以登录了。" << endl;
}

// 登录:查找用户名并比对密码,成功后记录当前用户下标
void Guild::login() {
    cout << "\n----- 登录 -----" << endl;
    string username = readLine("用户名: ");
    string password = readLine("密码: ");

    int idx = findUser(username);
    if (idx == -1) {
        cout << "用户不存在!" << endl;
        return;
    }
    if (users[idx].getPassword() != password) {
        cout << "密码错误!" << endl;
        return;
    }
    currentUserIndex = idx;   // 记住当前登录的是哪个用户
    cout << "登录成功!欢迎回来," << username << "。" << endl;
}

// 退出登录:把当前用户下标重置为 -1,回到未登录状态
void Guild::logout() {
    currentUserIndex = -1;
    cout << "已退出登录。" << endl;
}

// ---------------- 委托操作 ----------------

// 发布委托:由当前登录用户填写信息,生成新委托并保存
void Guild::publishQuest() {
    cout << "\n----- 发布委托 -----" << endl;
    string title = readLine("委托标题(不要用 | 符号): ");
    if (title.empty()) {
        cout << "标题不能为空!" << endl;
        return;
    }
    string desc = readLine("委托描述: ");
    int reward = readInt("完成奖励经验值(建议 50-300): ");
    if (reward <= 0) reward = 50;            // 非法奖励给个默认值
    int difficulty = readInt("难度等级(1-5): ");
    if (difficulty < 1) difficulty = 1;      // 把难度限制在 1-5 之间
    if (difficulty > 5) difficulty = 5;

    // 发布者就是当前登录用户;用 nextQuestId 作为新委托编号
    string publisher = users[currentUserIndex].getUsername();
    Quest q(nextQuestId, title, desc, publisher, reward, difficulty);
    quests.push_back(q);
    nextQuestId++;          // 编号自增,供下一条委托使用
    saveQuests();
    cout << "委托发布成功!编号为 " << q.getId() << "。" << endl;
}

// 接取委托:输入编号后,校验状态与发布者,合法则把委托标记为"进行中"
void Guild::takeQuest() {
    cout << "\n----- 接取委托 -----" << endl;
    showAllQuests();        // 先展示公告栏,方便用户挑选编号
    int id = readInt("请输入要接取的委托编号(输入 0 返回): ");
    if (id == 0) return;

    string me = users[currentUserIndex].getUsername();
    for (size_t i = 0; i < quests.size(); ++i) {
        if (quests[i].getId() == id) {        // 找到对应编号的委托
            if (quests[i].getStatus() != QUEST_WAITING) {
                cout << "该委托已被接取或已完成,无法接取!" << endl;
                return;
            }
            if (quests[i].getPublisher() == me) {
                cout << "不能接取自己发布的委托!" << endl;
                return;
            }
            // 校验通过:记录接取者并把状态推进到"进行中"
            quests[i].setTaker(me);
            quests[i].setStatus(QUEST_TAKEN);
            saveQuests();
            cout << "接取成功!请努力完成委托:" << quests[i].getTitle() << endl;
            return;
        }
    }
    cout << "找不到该编号的委托!" << endl;
}

// 提交完成委托:先列出冒险者当前进行中的委托,选定后标记完成并发放经验
void Guild::completeQuest() {
    cout << "\n----- 提交完成委托 -----" << endl;
    string me = users[currentUserIndex].getUsername();

    // 先列出"我接取且进行中"的委托,方便用户选择
    cout << "你正在进行的委托:" << endl;
    bool hasAny = false;
    for (size_t i = 0; i < quests.size(); ++i) {
        if (quests[i].getTaker() == me && quests[i].getStatus() == QUEST_TAKEN) {
            cout << "  [" << quests[i].getId() << "] " << quests[i].getTitle()
                 << " (奖励经验 " << quests[i].getRewardExp() << ")" << endl;
            hasAny = true;
        }
    }
    if (!hasAny) {
        cout << "  (暂无进行中的委托)" << endl;
        return;
    }

    int id = readInt("请输入要提交完成的委托编号(输入 0 返回): ");
    if (id == 0) return;

    for (size_t i = 0; i < quests.size(); ++i) {
        if (quests[i].getId() == id) {
            // 只有"我接取且进行中"的委托才允许提交完成
            if (quests[i].getTaker() != me || quests[i].getStatus() != QUEST_TAKEN) {
                cout << "这不是你正在进行的委托,无法提交!" << endl;
                return;
            }
            quests[i].setStatus(QUEST_DONE);   // 委托状态推进到"已完成"
            // 记录升级前后的等级,用于判断是否升级
            int oldLevel = users[currentUserIndex].getLevel();
            users[currentUserIndex].gainExp(quests[i].getRewardExp());
            int newLevel = users[currentUserIndex].getLevel();
            saveQuests();   // 委托和用户数据都发生了变化,分别保存
            saveUsers();
            cout << "委托完成!获得经验 " << quests[i].getRewardExp() << "。" << endl;
            if (newLevel > oldLevel) {
                cout << "恭喜升级!当前等级:Lv." << newLevel << endl;
            } else {
                cout << "当前等级:Lv." << newLevel
                     << ",经验 " << users[currentUserIndex].getExp()
                     << "/" << users[currentUserIndex].expToNext() << endl;
            }
            return;
        }
    }
    cout << "找不到该编号的委托!" << endl;
}

// 显示公告栏:把所有委托的概要信息列成一张表
void Guild::showAllQuests() {
    cout << "\n===== 委托公告栏 =====" << endl;
    if (quests.empty()) {
        cout << "(目前还没有任何委托)" << endl;
        return;
    }
    cout << "编号  状态     难度  奖励   标题 (发布者)" << endl;
    cout << "--------------------------------------------" << endl;
    for (size_t i = 0; i < quests.size(); ++i) {
        Quest& q = quests[i];
        cout << q.getId() << "    "
             << q.statusText() << "   "
             << q.getDifficulty() << "星   "
             << q.getRewardExp() << "    "
             << q.getTitle() << " (" << q.getPublisher() << ")" << endl;
    }
}

// 显示当前用户的个人信息:基本资料 + 自己发布的委托 + 自己接取的委托
void Guild::showMyInfo() {
    Adventurer& me = users[currentUserIndex];
    cout << "\n===== 我的信息 =====" << endl;
    cout << "用户名:" << me.getUsername() << endl;
    cout << "学校:" << me.getSchool() << endl;
    cout << "等级:Lv." << me.getLevel() << endl;
    cout << "经验:" << me.getExp() << "/" << me.expToNext() << endl;

    cout << "\n我发布的委托:" << endl;
    bool hasPub = false;
    for (size_t i = 0; i < quests.size(); ++i) {
        if (quests[i].getPublisher() == me.getUsername()) {
            cout << "  [" << quests[i].getId() << "] " << quests[i].getTitle()
                 << " - " << quests[i].statusText() << endl;
            hasPub = true;
        }
    }
    if (!hasPub) cout << "  (无)" << endl;

    cout << "我接取的委托:" << endl;
    bool hasTake = false;
    for (size_t i = 0; i < quests.size(); ++i) {
        if (quests[i].getTaker() == me.getUsername()) {
            cout << "  [" << quests[i].getId() << "] " << quests[i].getTitle()
                 << " - " << quests[i].statusText() << endl;
            hasTake = true;
        }
    }
    if (!hasTake) cout << "  (无)" << endl;
}

// ---------------- 菜单 ----------------

// 未登录时的菜单:只提供注册、登录、退出系统三个选项
void Guild::authMenu() {
    cout << "\n========== 请先登录 ==========" << endl;
    cout << "  1. 注册" << endl;
    cout << "  2. 登录" << endl;
    cout << "  0. 退出系统" << endl;
    int choice = readInt("请选择: ");
    switch (choice) {   // 根据输入分发到对应功能
        case 1: registerUser(); break;
        case 2: login(); break;
        case 0:
            cout << "感谢使用,再见!" << endl;
            exit(0);    // 直接结束整个程序
        default:
            cout << "无效选项,请重新输入!" << endl;
    }
}

// 登录后的主菜单:提供浏览/发布/接取/完成委托等功能
void Guild::mainMenu() {
    cout << "\n========== 主菜单 (当前用户:"
         << users[currentUserIndex].getUsername() << ") ==========" << endl;
    cout << "  1. 浏览所有委托" << endl;
    cout << "  2. 发布委托" << endl;
    cout << "  3. 接取委托" << endl;
    cout << "  4. 提交完成委托" << endl;
    cout << "  5. 查看我的信息" << endl;
    cout << "  6. 退出登录" << endl;
    cout << "  0. 退出系统" << endl;
    int choice = readInt("请选择: ");
    switch (choice) {   // 根据输入分发到对应功能
        case 1: showAllQuests(); break;
        case 2: publishQuest(); break;
        case 3: takeQuest(); break;
        case 4: completeQuest(); break;
        case 5: showMyInfo(); break;
        case 6: logout(); break;
        case 0:
            cout << "感谢使用,再见!" << endl;
            exit(0);    // 直接结束整个程序
        default:
            cout << "无效选项,请重新输入!" << endl;
    }
}
