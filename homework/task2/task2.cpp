#include <iostream>
#include "../task1/Vector/vector.h"
#include <deque>
#include <random>

// 定义一个结构体，用来表示柱子的索引和高度
struct Bar {
    int index; // 柱子的索引
    int height; // 柱子的高度
};

// 定义一个队列类，用来存储柱子
class Queue {
private:
    std::deque<Bar> data; 
public:
    // 判断队列是否为空
    bool empty() {
        return data.empty();
    }

    // 返回队列的大小
    int size() {
        return data.size();
    }

    // 在队尾插入一个柱子
    void push(Bar bar) {
        data.push_back(bar);
    }

    // 在队首删除一个柱子，并返回它
    Bar pop() {
        Bar front = data.front();
        data.pop_front();
        return front;
    }

    // 返回队首的柱子，但不删除它
    Bar peek() {
        return data.front();
    }
};
// 定义一个函数，用来计算矩形的最大面积
int maxArea(std::vector<int>& heights) {
    int max_area = 0; 
    Queue stack; 
    for (int i = 0; i < heights.size(); i++) { 
        while (!stack.empty() && heights[i] < stack.peek().height) { 
            Bar top = stack.pop(); 
            int width = stack.empty() ? i : i - stack.peek().index - 1; 
            int area = top.height * width; 
            max_area = std::max(max_area, area); 
        }
        stack.push(Bar{ i, heights[i] }); 
    }
    while (!stack.empty()) { 
        Bar top = stack.pop(); 
        int width = stack.empty() ? heights.size() : heights.size() - stack.peek().index - 1; 
        int area = top.height * width; 
        max_area = std::max(max_area, area); 
    }
    return max_area; // 返回最大面积
}
// 生成随机数
int random(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> dis(min, max); 
    return dis(gen); // 返回一个在[min, max]范围内的随机数
}

// 打印柱子数组和最大面积
void print(std::vector<int>& heights) {
    std::cout << "高度: [";
    for (int i = 0; i < heights.size(); i++) {
        std::cout << heights[i];
        if (i != heights.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]\n";
    std::cout << "最大面积为: " << maxArea(heights) << "\n\n";
}

// 测试数据的个数
const int p = 10;

// 柱子数组长度的最小值和最大值
const int MIN_L = 1;
const int MAX_L = 105;
// 柱子高度的最小值和最大值
const int MIN_H = 0;
const int MAX_H = 104;

//生成测试数据并调用打印函数
int main() {
    for (int i = 0; i < p; i++) { // 循环生成10组测试数据
        int length = random(MIN_L, MAX_L); // 随机生成柱子数组长度
        std::vector<int> heights(length); // 创建一个长度为length的柱子数组
        for (int j = 0; j < length; j++) { // 遍历柱子数组
            heights[j] = random(MIN_H, MAX_H); // 随机生成柱子高度
        }
        print(heights);
    }
    return 0; 
}