// ============================================================
// Quest.h
// 委托(任务)类的声明。
// 一个 Quest 对象代表公告栏上的一条委托,记录它的内容、
// 发布者/接取者以及当前所处的状态。
// ============================================================
#pragma once   // 头文件保护,防止被重复包含

#include <string>

// 委托状态用整数常量表示,便于在文件中存储和判断
// 0 待接取, 1 进行中, 2 已完成
const int QUEST_WAITING = 0;
const int QUEST_TAKEN   = 1;
const int QUEST_DONE    = 2;

// 委托(任务)类
class Quest {
public:
    Quest();                            // 默认构造:用于从文件读入前先创建空对象
    Quest(int id,                       // 带参构造:发布新委托时使用
          const std::string& title,
          const std::string& desc,
          const std::string& publisher,
          int rewardExp,
          int difficulty);

    // 与文件一行文本互相转换(竖线 | 分隔)
    std::string toLine() const;            // 打包成一行文本
    void fromLine(const std::string& line);// 从一行文本还原

    std::string statusText() const; // 把状态数字转成中文描述(待接取/进行中/已完成)

    // 一组只读访问函数(getter)
    int getId() const;
    std::string getTitle() const;
    std::string getDesc() const;
    std::string getPublisher() const;
    std::string getTaker() const;
    int getStatus() const;
    int getRewardExp() const;
    int getDifficulty() const;

    // 一组修改函数(setter):接取委托、改变委托状态时使用
    void setTaker(const std::string& name);
    void setStatus(int s);

private:
    int id;                // 委托编号
    std::string title;     // 标题
    std::string desc;      // 描述
    std::string publisher; // 发布者用户名
    std::string taker;     // 接取者用户名(无则为 "-")
    int status;            // 状态
    int rewardExp;         // 完成奖励经验
    int difficulty;        // 难度(1-5)
};
