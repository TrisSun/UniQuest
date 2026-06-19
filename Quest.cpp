#include "Quest.h"
#include <sstream>
#include <cstdlib>

Quest::Quest()
    : id(0), title(""), desc(""), publisher(""), taker("-"),
      status(QUEST_WAITING), rewardExp(0), difficulty(1) {}

Quest::Quest(int id,
             const std::string& title,
             const std::string& desc,
             const std::string& publisher,
             int rewardExp,
             int difficulty)
    : id(id), title(title), desc(desc), publisher(publisher), taker("-"),
      status(QUEST_WAITING), rewardExp(rewardExp), difficulty(difficulty) {}

std::string Quest::statusText() const {
    if (status == QUEST_WAITING) return "待接取";
    if (status == QUEST_TAKEN)   return "进行中";
    return "已完成";
}

std::string Quest::toLine() const {
    std::ostringstream oss;
    oss << id << "|" << title << "|" << desc << "|" << publisher << "|"
        << taker << "|" << status << "|" << rewardExp << "|" << difficulty;
    return oss.str();
}

void Quest::fromLine(const std::string& line) {
    std::istringstream iss(line);
    std::string token;

    std::getline(iss, token, '|'); id = std::atoi(token.c_str());
    std::getline(iss, title, '|');
    std::getline(iss, desc, '|');
    std::getline(iss, publisher, '|');
    std::getline(iss, taker, '|');
    std::getline(iss, token, '|'); status = std::atoi(token.c_str());
    std::getline(iss, token, '|'); rewardExp = std::atoi(token.c_str());
    std::getline(iss, token, '|'); difficulty = std::atoi(token.c_str());
}

int Quest::getId() const { return id; }
std::string Quest::getTitle() const { return title; }
std::string Quest::getDesc() const { return desc; }
std::string Quest::getPublisher() const { return publisher; }
std::string Quest::getTaker() const { return taker; }
int Quest::getStatus() const { return status; }
int Quest::getRewardExp() const { return rewardExp; }
int Quest::getDifficulty() const { return difficulty; }

void Quest::setTaker(const std::string& name) { taker = name; }
void Quest::setStatus(int s) { status = s; }
