#include "Guild.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

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

Guild::Guild() : currentUserIndex(-1), nextQuestId(1) {}

void Guild::run() {
    loadUsers();
    loadQuests();

    cout << "============================================" << endl;
    cout << "        欢迎来到江苏冒险者工会系统" << endl;
    cout << "============================================" << endl;

    while (true) {
        if (currentUserIndex == -1) {
            authMenu();
        } else {
            mainMenu();
        }
    }
}

// ---------------- 文件读写 ----------------

void Guild::loadUsers() {
    users.clear();
    ifstream fin("users.txt");
    if (!fin) return; // 文件不存在则当作没有用户
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        Adventurer a;
        a.fromLine(line);
        users.push_back(a);
    }
    fin.close();
}

void Guild::saveUsers() {
    ofstream fout("users.txt");
    for (size_t i = 0; i < users.size(); ++i) {
        fout << users[i].toLine() << endl;
    }
    fout.close();
}

void Guild::loadQuests() {
    quests.clear();
    nextQuestId = 1;
    ifstream fin("quests.txt");
    if (!fin) return;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        Quest q;
        q.fromLine(line);
        quests.push_back(q);
        if (q.getId() >= nextQuestId) {
            nextQuestId = q.getId() + 1; // 保证新编号不重复
        }
    }
    fin.close();
}

void Guild::saveQuests() {
    ofstream fout("quests.txt");
    for (size_t i = 0; i < quests.size(); ++i) {
        fout << quests[i].toLine() << endl;
    }
    fout.close();
}

// ---------------- 辅助函数 ----------------

int Guild::findUser(const string& username) const {
    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i].getUsername() == username) return (int)i;
    }
    return -1;
}

bool Guild::isJiangsuUniversity(const string& school) const {
    for (int i = 0; i < JIANGSU_COUNT; ++i) {
        if (school == JIANGSU_UNIVERSITIES[i]) return true;
    }
    return false;
}

// ---------------- 账号 ----------------

void Guild::registerUser() {
    cout << "\n----- 注册新冒险者 -----" << endl;
    string username = readLine("请输入用户名(不含空格): ");
    if (username.empty()) {
        cout << "用户名不能为空!" << endl;
        return;
    }
    if (findUser(username) != -1) {
        cout << "该用户名已存在,请换一个!" << endl;
        return;
    }

    string school = readLine("请输入你的学校(必须是江苏省高校): ");
    if (!isJiangsuUniversity(school)) {
        cout << "抱歉,本工会只对江苏省高校学生开放,注册失败!" << endl;
        cout << "(支持的学校示例:南京大学、东南大学、南京航空航天大学 等)" << endl;
        return;
    }

    string password = readLine("请输入密码(不含空格): ");
    if (password.empty()) {
        cout << "密码不能为空!" << endl;
        return;
    }

    users.push_back(Adventurer(username, password, school));
    saveUsers();
    cout << "注册成功!欢迎你," << username << "!现在可以登录了。" << endl;
}

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
    currentUserIndex = idx;
    cout << "登录成功!欢迎回来," << username << "。" << endl;
}

void Guild::logout() {
    currentUserIndex = -1;
    cout << "已退出登录。" << endl;
}

// ---------------- 委托操作 ----------------

void Guild::publishQuest() {
    cout << "\n----- 发布委托 -----" << endl;
    string title = readLine("委托标题(不要用 | 符号): ");
    if (title.empty()) {
        cout << "标题不能为空!" << endl;
        return;
    }
    string desc = readLine("委托描述: ");
    int reward = readInt("完成奖励经验值(建议 50-300): ");
    if (reward <= 0) reward = 50;
    int difficulty = readInt("难度等级(1-5): ");
    if (difficulty < 1) difficulty = 1;
    if (difficulty > 5) difficulty = 5;

    string publisher = users[currentUserIndex].getUsername();
    Quest q(nextQuestId, title, desc, publisher, reward, difficulty);
    quests.push_back(q);
    nextQuestId++;
    saveQuests();
    cout << "委托发布成功!编号为 " << q.getId() << "。" << endl;
}

void Guild::takeQuest() {
    cout << "\n----- 接取委托 -----" << endl;
    showAllQuests();
    int id = readInt("请输入要接取的委托编号(输入 0 返回): ");
    if (id == 0) return;

    string me = users[currentUserIndex].getUsername();
    for (size_t i = 0; i < quests.size(); ++i) {
        if (quests[i].getId() == id) {
            if (quests[i].getStatus() != QUEST_WAITING) {
                cout << "该委托已被接取或已完成,无法接取!" << endl;
                return;
            }
            if (quests[i].getPublisher() == me) {
                cout << "不能接取自己发布的委托!" << endl;
                return;
            }
            quests[i].setTaker(me);
            quests[i].setStatus(QUEST_TAKEN);
            saveQuests();
            cout << "接取成功!请努力完成委托:" << quests[i].getTitle() << endl;
            return;
        }
    }
    cout << "找不到该编号的委托!" << endl;
}

void Guild::completeQuest() {
    cout << "\n----- 提交完成委托 -----" << endl;
    string me = users[currentUserIndex].getUsername();

    // 列出我正在进行的委托
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
            if (quests[i].getTaker() != me || quests[i].getStatus() != QUEST_TAKEN) {
                cout << "这不是你正在进行的委托,无法提交!" << endl;
                return;
            }
            quests[i].setStatus(QUEST_DONE);
            int oldLevel = users[currentUserIndex].getLevel();
            users[currentUserIndex].gainExp(quests[i].getRewardExp());
            int newLevel = users[currentUserIndex].getLevel();
            saveQuests();
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

void Guild::authMenu() {
    cout << "\n========== 请先登录 ==========" << endl;
    cout << "  1. 注册" << endl;
    cout << "  2. 登录" << endl;
    cout << "  0. 退出系统" << endl;
    int choice = readInt("请选择: ");
    switch (choice) {
        case 1: registerUser(); break;
        case 2: login(); break;
        case 0:
            cout << "感谢使用,再见!" << endl;
            exit(0);
        default:
            cout << "无效选项,请重新输入!" << endl;
    }
}

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
    switch (choice) {
        case 1: showAllQuests(); break;
        case 2: publishQuest(); break;
        case 3: takeQuest(); break;
        case 4: completeQuest(); break;
        case 5: showMyInfo(); break;
        case 6: logout(); break;
        case 0:
            cout << "感谢使用,再见!" << endl;
            exit(0);
        default:
            cout << "无效选项,请重新输入!" << endl;
    }
}
