//
// Created by 24394 on 2026/8/1.
//
#include <iostream>
#include <stdexcept>
using std::cout;
using std::endl;

template<typename T>
class Stack{
private:
    size_t size;
    size_t capacity;
    T* data;
    int top;  // 把数组尾部作为栈顶

    void reserve(){
        size_t new_cap = capacity==0? 4: (2 * capacity);
        T* new_data = new T[new_cap];
        for(size_t i=0; i<size; ++i){
            new_data[i] = *(data+i);
        }
        delete []data;
        this->data = new_data;
        this->capacity = new_cap;
    }

public:
    explicit Stack(size_t cap): size(0), capacity(cap), data(nullptr), top(-1){
        if(cap > 0) data = new T[cap];
    }

    ~Stack(){
        delete []data;
        data = nullptr;
    }

    Stack(const Stack& stk){
        this->size = stk.size;
        this->capacity = stk.capacity;
        this->data = new T[capacity];
        for(size_t i=0; i<size; ++i){
            this->data[i] = stk.data[i];
        }
        this->top = stk.top;
    }

    Stack& operator=(const Stack& stk){
        if(this == &stk) return *this;

        if(this->data){
            delete []data;
            data = nullptr;
        }
        this->size = stk.size;
        this->capacity = stk.capacity;
        this->data = new T(capacity);
        for(size_t i=0; i<size; ++i){
            this->data[i] = stk.data[i];
        }
        this->top = stk.top;
        return *this;
    }

    void push(const T& val){  // 尾插
        if(size == capacity) reserve();

        this->top += 1;
        data[top] = val;
        size++;
    }

    void pop(){  // 尾删
        if(top == -1) {
            cout << "栈已空" << endl;
            return;
        }
        top -= 1;
        size--;
    }

    [[nodiscard]] bool empty() const{
        return size == 0;
    }

    [[nodiscard]] T get_top() const{
        if(top == -1) throw std::runtime_error("栈为空");

        return data[top];
    }

    [[nodiscard]] size_t get_size() const{
        return size;
    }
};

//int main(){
//    Stack<int> s(0);
//    s.push(1);
//    s.push(2);
//    s.push(3);
//    cout << "栈顶: " << s.get_top() << endl;  // 3
//    s.pop();
//    cout << "栈顶: " << s.get_top() << endl;  // 2
//    cout << "大小: " << s.get_size() << endl;       // 2
//
//    Stack<int> s2(s);            // 拷贝构造
//    cout << "s2栈顶: " << s2.get_top() << endl;  // 2
//    s2.pop();
//    cout << "s2大小: " << s2.get_size() << endl;       // 1
//    cout << "s大小: " << s.get_size() << endl;         // 2（不受影响）
//
//    Stack<int> s3(0);
//    s3 = s;                       // 赋值
//    cout << "s3栈顶: " << s3.get_top() << endl;  // 2
//    s3.pop();
//    cout << "s3大小: " << s3.get_size() << endl;  // 1
//}