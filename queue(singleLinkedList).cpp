//
// Created by 24394 on 2026/8/2.
//
#include <iostream>
#include <stdexcept>
using std::cout;
using std::endl;

template<typename T>
struct list_node{
    T val;
    list_node* next;
    explicit list_node(T x, list_node* nxt=nullptr) : val(x), next(nxt) {}
};

template<typename T>
class Queue{
private:
    size_t cnt;
    list_node<T>* frontend;
    list_node<T>* backend;

public:
    Queue(): cnt(0), frontend(nullptr), backend(nullptr){
    }

    ~Queue(){
        while(this->frontend){
            list_node<T>* temp = frontend;
            frontend = frontend->next;
            delete temp;
        }
        frontend = nullptr;
        backend = nullptr;
    }

    Queue(const Queue& q): cnt(0), frontend(nullptr), backend(nullptr) {
        if (q.empty()) return;

        list_node<T>* cur = q.frontend;
        for(int i=0; i<q.cnt; ++i){
            this->push(cur->val);
            cur = cur->next;
        }
    }

    Queue& operator=(const Queue& q){
        if(this == &q) return *this;

        while(frontend){
            list_node<T>* temp = frontend;
            frontend = frontend->next;
            cnt--;
            delete temp;
        }
        frontend = nullptr;
        backend = nullptr;

        list_node<T>* cur = q.frontend;
        for(int i=0; i<q.cnt; ++i){
            this->push(cur->val);
            cur = cur->next;
        }
        return *this;
    }

    void push(const T& val){  // 尾插（链表尾端作为队尾）
        if(!backend){
            frontend = backend = new list_node(val);
            cnt++;
            return;
        }

        backend->next = new list_node(val);
        backend = backend->next;
        cnt++;
    }

    void pop(){  // 头删（链表首段作为队首）
        if(!frontend){
            cout << "队列已空，无法弹出" << endl;
            return;
        }
        list_node<T>* to_delete = frontend;
        frontend = frontend->next;
        if(!frontend) backend = nullptr;
        cnt--;
        delete to_delete;
    }

    [[nodiscard]] bool empty() const{
        return backend == nullptr;
    }

    [[nodiscard]] T get_front() const{
        if(!frontend) throw std::runtime_error("队列为空");

        return frontend->val;
    }

    [[nodiscard]] T get_back() const{
        if(!backend) throw std::runtime_error("队列为空");

        return backend->val;
    }

    [[nodiscard]] size_t get_size() const{
        return cnt;
    }
};

int main(){
    Queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    cout << "队首: " << q.get_front() << endl;  // 1
    cout << "队尾: " << q.get_back() << endl;  // 3
    q.pop();
    cout << "队首: " << q.get_front() << endl;  // 2
    cout << "大小: " << q.get_size() << endl;       // 2

    Queue<int> q2(q);            // 拷贝构造
    cout << "q2队首: " << q2.get_front() << endl;  // 2
    cout << "q2队尾: " << q2.get_back() << endl;  // 3
    q2.pop();
    cout << "s2大小: " << q2.get_size() << endl;       // 1
    cout << "s大小: " << q.get_size() << endl;         // 2（不受影响）

    Queue<int> q3;
    q3 = q;                       // 赋值
    cout << "q3队首: " << q3.get_front() << endl;  // 2
    q3.pop();
    cout << "q3大小: " << q3.get_size() << endl;  // 1

    q3.pop();
    cout << "q3大小: " << q3.get_size() << endl;
    cout << "q3队首: " << q3.get_front() << endl;
    cout << "q3队尾: " << q3.get_back() << endl;
}