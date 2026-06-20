// ============================================================
// Adventurer.cpp
// Adventurer 类的实现:构造、升级逻辑、与文件文本互转、读取接口。
// ============================================================
#include "Adventurer.h"
#include <sstream>
// 默认构造函数:新对象的等级从 1 开始,经验从 0 开始,初始称号为"新人",未转职
Adventurer::Adventurer()
    : username(""), password(""), school(""), level(1), exp(0),
      noviceTitle("新人"), job("") {}
// 带参构造函数:用注册时输入的信息初始化,等级、经验仍为初始值
Adventurer::Adventurer(const std::string& username,
                       const std::string& password,
                       const std::string& school)
    : username(username), password(password), school(school), level(1), exp(0),
      noviceTitle("新人"), job("") {}

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
// 格式:用户名 密码 学校 等级 经验 初始称号 职业
// 职业为空时用占位符 "-" 写入,避免空字段破坏按空格切分的解析
std::string Adventurer::toLine() const {
    std::ostringstream oss;
    oss << username << " " << password << " " << school << " "
        << level << " " << exp << " "
        << noviceTitle << " " << (job.empty() ? "-" : job);
    return oss.str();
}

// 从文件一行还原对象,与 toLine 的格式对应。
// 为兼容旧存档(没有称号/职业两列),读取不到时给出默认值。
void Adventurer::fromLine(const std::string& line) {
    std::istringstream iss(line);
    iss >> username >> password >> school >> level >> exp;
    if (!(iss >> noviceTitle)) noviceTitle = "新人"; // 旧存档缺这一列
    if (!(iss >> job)) job = "";                      // 旧存档缺这一列
    if (job == "-") job = "";                          // 占位符还原为空
}

// 以下为各成员的只读访问函数
std::string Adventurer::getUsername() const { return username; }
std::string Adventurer::getPassword() const { return password; }
std::string Adventurer::getSchool() const { return school; }
int Adventurer::getLevel() const { return level; }
int Adventurer::getExp() const { return exp; }

// 称号 / 职业相关
std::string Adventurer::getNoviceTitle() const { return noviceTitle; }
void Adventurer::setNoviceTitle(const std::string& t) { noviceTitle = t; }
std::string Adventurer::getJob() const { return job; }
void Adventurer::setJob(const std::string& j) { job = j; }

// 计算对外展示的称号:
// - 已转职:直接显示职业名(如 学者 / 贤者 / 游侠 ...)
// - 未转职:按等级给出  1~5 级 新人/菜鸟(自选) → 6~19 级 冒险者 → 20 级及以上 勇者
std::string Adventurer::getTitle() const {
    if (!job.empty()) return job;
    if (level >= 20) return "勇者";
    if (level >= 6) return "冒险者";
    return noviceTitle;
}
