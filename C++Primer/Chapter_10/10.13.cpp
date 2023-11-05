// 标准库定义了名为 partition 的算法，它接受一个谓词，对容器内容进行划分，使得谓词为true 的值会排在容器的前半部分，
// 而使得谓词为 false 的值会排在后半部分。算法返回一个迭代器，指向最后一个使谓词为 true 的元素之后的位置。
// 编写函数，接受一个 string，返回一个 bool 值，指出 string 是否有5个或更多字符。使用此函数划分 words。打印出长度大于等于5的元素。

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

//将words去重
std::vector<std::string> &elimDups(std::vector<std::string> &words) {
    sort(words.begin(), words.end());   //对words进行排序，让相同元素相邻
    auto end_unique = unique(words.begin(), words.end());   // 使用unique对元素进行去重，返回最后一个不重复元素的迭代器
    words.erase(end_unique, words.end());   // 将重复的元素删除
    return words;   // 返回words
}

bool greater_than_5(const std::string& s) {
    return s.size() >= 5;
}

int main() {

    std::vector<std::string> words = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "a", "aa", "aaa", "aaaaa"};

    auto it = partition(words.begin(), words.end(), greater_than_5); // 对words内容进行划分，让长度大于5的排在前面，长度小于5的排在后面，返回第一个长度小于5的迭代器

    //打印排好序的words
    for(auto word : words) {
        std::cout << word << " ";
    }
    std::cout << std::endl;

    words.erase(it, words.end()); // 将长度不足5的元素删除

    for(auto word : words) {
        std::cout << word << " ";
    }
    std::cout << std::endl;



    return 0;
}
