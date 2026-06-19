#ifndef QUEST_H
#define QUEST_H

#include <string>

// 委托状态:0 待接取, 1 进行中, 2 已完成
const int QUEST_WAITING = 0;
const int QUEST_TAKEN   = 1;
const int QUEST_DONE    = 2;

// 委托(任务)类
class Quest {
public:
    Quest();
    Quest(int id,
          const std::string& title,
          const std::string& desc,
          const std::string& publisher,
          int rewardExp,
          int difficulty);

    // 与文件一行文本互相转换(竖线 | 分隔)
    std::string toLine() const;
    void fromLine(const std::string& line);

    std::string statusText() const; // 状态中文描述

    int getId() const;
    std::string getTitle() const;
    std::string getDesc() const;
    std::string getPublisher() const;
    std::string getTaker() const;
    int getStatus() const;
    int getRewardExp() const;
    int getDifficulty() const;

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

#endif
