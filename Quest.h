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
    std::string toLine() const;            // 对象转成文件一行
    void fromLine(const std::string& line);// 从文件一行还原对象

    std::string statusText() const; // 把状态数字转成中文描述(待接取0/进行中1/已完成2)

    // 一组只读访问函数(getter)
    int getId() const;//获取编号
    std::string getTitle() const;//获取标题
    std::string getDesc() const;//获取描述
    std::string getPublisher() const;//获取发布者
    std::string getTaker() const;//获取接取者
    int getStatus() const;//获取委托的状态(0待接取/1进行中/2已完成)
    int getRewardExp() const;//获取奖励经验
    int getDifficulty() const;//获取难度

    // 一组修改函数(setter):接取委托、改变委托状态时使用
    void setTaker(const std::string& name);//设置接取者
    void setStatus(int s);//设置委托的状态(0待接取/1进行中/2已完成)

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
