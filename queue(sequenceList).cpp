//
// Created by 24394 on 2026/8/2.
//
#include <iostream>
#include <stdexcept>
using std :: cout;
using std :: endl;

template<typename T>
class Queue{
private:
    size_t size;
    size_t capacity;
    T* data;
    int frontend = 0;
    int backend;

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
    explicit Queue(size_t cap): size(0), capacity(cap), data(nullptr), frontend(0), backend(-1){
        if(cap > 0) data = new T[cap];
    }

    ~Queue(){
        delete []data;
        data = nullptr;
    }

    Queue(const Queue& q){
        this->size = q.size;
        this->capacity = q.capacity;
        this->data = new T[capacity];
        for(size_t i=0; i<size; ++i){
            this->data[i] = *(q.data+i);
        }
        this->frontend = q.frontend;
        this->backend = q.backend;
    }

    Queue& operator=(const Queue& q){
        if(this == &q) return *this;

        if(this->data){
            delete []data;
            data = nullptr;
        }
        this->size = q.size;
        this->capacity = q.capacity;
        this->data = new T(capacity);
        for(size_t i=0; i<size; ++i){
            this->data[i] = q.data[i];
        }
        this->backend = q.backend;
        return *this;
    }

    void push(T val){  // 尾插
        if(size == capacity) reserve();

        backend += 1;
        data[backend] = val;
        size++;
    }

    void pop(){  // 头删
        if(backend == -1){
            cout << "队列已空" << endl;
            return;
        }
        for(size_t i=1; i<size; ++i){
            data[i-1] = data[i];
        }
        backend -= 1;
        size--;
    }

    [[nodiscard]] bool empty() const{
        return size == 0;
    }

    [[nodiscard]] T get_front() const{
        if(backend == -1) throw std::runtime_error("队列为空");

        return data[frontend];
    }

    [[nodiscard]] T get_back() const{
        if(backend == -1) throw std::runtime_error("队列为空");

        return data[backend];
    }

    [[nodiscard]] size_t get_size() const{
        return size;
    }
};

int main(){
    Queue<int> q(0);
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

    Queue<int> q3(0);
    q3 = q;                       // 赋值
    cout << "q3队首: " << q3.get_front() << endl;  // 2
    q3.pop();
    cout << "q3大小: " << q3.get_size() << endl;  // 1

    q3.pop();
    cout << "q3大小: " << q3.get_size() << endl;
    cout << "q3队首: " << q3.get_front() << endl;
    cout << "q3队尾: " << q3.get_back() << endl;
}