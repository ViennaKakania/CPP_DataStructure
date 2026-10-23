//
// Created by 24394 on 2026/8/1.
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
class Stack{
private:
    size_t cnt;
    list_node<T>* top;

public:
    explicit Stack(): cnt(0), top(nullptr){
    }

    ~Stack(){
        while(this->top){
            list_node<T> *temp = top;
            top = top->next;
            delete temp;
        }
    }

    Stack(const Stack& stk): cnt(0), top(nullptr){
        if(stk.empty()) return;

        T values[stk.cnt];
        list_node<T>* cur = stk.top;  int i = 0;
        while(cur){
            values[i] = cur->val;
            cur = cur->next;
            ++i;
        }

        i -= 1;
        for(i; i>=0; --i){
            this->push(values[i]);
        }
    };

    Stack& operator=(const Stack& stk) {
        if(this == &stk) return *this;

        while(this->top){
            list_node<T>* temp = top;
            top = top->next;
            cnt--;
            delete temp;
        }

        T values[stk.cnt];
        list_node<T>* cur = stk.top;
        for(int i=0; i<stk.cnt; ++i){
            values[i] = cur->val;
            cur = cur->next;
        }
        for(int i=stk.cnt-1; i>=0; --i){
            this->push(values[i]);
        }
        return *this;
    }

    void push(const T& val){ // 保留头插（把链表前端作为栈顶）
        top = new list_node<T>(val, top);
        cnt++;
    }

    void pop(){  // 保留头删（把链表前端作为栈顶）
        if(!top){
            cout << "栈已空，无法弹出" << endl;
            return;
        }
        list_node<T>* to_delete = top;
        top = top->next;
        --cnt;
        delete to_delete;
    }

    [[nodiscard]] bool empty() const{
        return top == nullptr;
    }

    [[nodiscard]] T get_top() const{
        if(!top) throw std::runtime_error("栈为空");

        return top->val;
    }

    [[nodiscard]] size_t get_size() const{
        return cnt;
    }
};

int main(){
    try{
        Stack<int> s;
        cout << "栈顶: " << s.get_top() << endl;
        s.push(1);
        s.push(2);
        s.push(3);
        cout << "栈顶: " << s.get_top() << endl;  // 3
        s.pop();
        cout << "栈顶: " << s.get_top() << endl;  // 2
        cout << "大小: " << s.get_size() << endl;       // 2

        Stack<int> s2(s);            // 拷贝构造
        cout << "s2栈顶: " << s2.get_top() << endl;  // 2
        s2.pop();
        cout << "s2大小: " << s2.get_size() << endl;       // 1
        cout << "s大小: " << s.get_size() << endl;         // 2（不受影响）

        Stack<int> s3;
        s3 = s;                       // 赋值
        cout << "s3栈顶: " << s3.get_top() << endl;  // 2
        s3.pop();
        cout << "s3大小: " << s3.get_size() << endl;  // 1
    }
    catch (const std::exception &e){
        std::cerr << e.what() << endl;
    }
}