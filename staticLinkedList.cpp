//
// Created by 24394 on 2026/7/31.
// 用数组的方式实现链表
#include <iostream>
#include <stdexcept>
using std :: cout;
using std :: endl;

#define max_size 100
template<typename T>
struct list_node{
    T data;
    int next; // 游标：下一个元素的下标索引
};

template<typename T>
class Linear_list{
private:
    size_t size;
    int dummy = 0; // 0号位不存数据，相当于哨兵
    list_node<T> nodes[max_size];  // 元素为节点的动态数组
    int avail;  // 备用链表，指向当前下标索引最小的空闲节点

    // 从备用链表中取出一个节点返回其下标
    int malloc_node(){
        if(avail == -1){
            cout << "静态链表已满，无法分配！" << endl;
            return -1; // 溢出
        }
        int p = avail;          // 取走备用链表头
        avail = nodes[avail].next; // 备用链表头后移
        return p;
    }

    // 将指定下标的节点回收到备用链表（头插法回收）
    void free_node(int index){
        nodes[index].next = avail; // 当前节点指向旧的备用头
        avail = index;             // 更新备用头为当前节点
    }

public:
    Linear_list(): size(0), avail(1){  // 备用链表的头指针指向第一个空闲节点
        nodes[dummy].next = -1;
        for(int i=1; i<max_size-1; ++i){
            nodes[i].next = i+1;
        }
        nodes[max_size - 1].next = -1;  // 最后一个节点指向空
    }

    void insert(size_t pos, T val){
        if(pos == 0 || pos > size+1) throw std::out_of_range("插入位置超出范围");

        int temp = malloc_node();
        if(temp == -1) throw std::runtime_error("静态链表已满");

        int pre = dummy;
        while(pos > 1){
            pre = nodes[pre].next;
            pos--;
        }
        nodes[temp].data = val;
        nodes[temp].next = nodes[pre].next;
        nodes[pre].next = nodes[temp];
        size++;
    }

    void erase(size_t pos){
        if(pos == 0 || pos > size) throw std::out_of_range("删除位置超出范围");

        int pre = dummy;  // 找要删除元素的上一个元素
        while(pos > 1){
            pre = nodes[pre].next;
            pos--;
        }
        int to_delete = nodes[pre].next;
        nodes[pre].next = nodes[to_delete].next;
        free_node(to_delete);
        size--;
    }

    void display() const{
        if(size == 0){
            cout << "链表为空" << endl;
            return;
        }
        int p = nodes[dummy].next;
        cout << "链表数据: ";
        while(p != -1){
            cout << nodes[p].data << " -> ";
            p = nodes[p].next;
        }
        cout << "nullptr" << endl;
    }

    int find(const T& val) const{  // 查找元素关于链表的下标位置
        int p = nodes[dummy].next;
        int pos = 1;  // pos是元素的下标顺序
        while(p != -1){  // 依据游标，得以按照链表元素下标顺序查找
            if(nodes[p].data == val) return pos;
            p = nodes[p].next;
            pos++;
        }
        return -1;
    }
};

