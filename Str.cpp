//
// Created by 24394 on 2026/8/2.
//
#include <iostream>
#include <stdexcept>
#include <cstring>  // c风格字符
#include <algorithm>
using std::cout;
using std::endl;
using std::strlen;
using std::strcpy;
using std::min;

class Str{
private:
    size_t size;
    size_t capacity;
    char* data;

    void reserve(size_t new_cap){
        if(new_cap < size) throw std::length_error("reserve: 新容量太小");

        char* new_data = new char[new_cap+1];
        for(size_t i=0; i<size; ++i){
            new_data[i] = data[i];
        }
        new_data[size] = '\0';
        delete []data;
        this->data = new_data;
        this->capacity = new_cap;
    }

public:
    explicit Str(): size(0), capacity(0), data(new char[1]{'\0'}){
    }

    ~Str(){
        delete []data;
        data = nullptr;
    }

    // 从C字符串构造
    explicit Str(const char* c) {
        if(!c){
            data = new char[1]{'\0'};
            size = 0;
            capacity = 0;
        }
        else{
            size = strlen(c);
            capacity = size;
            data = new char[capacity+1];
            strcpy(data, c);
        }
    }

    Str(const Str& s): size(s.size), capacity(s.capacity){
        data = new char[capacity+1];
        strcpy(data, s.data);
    }

    Str& operator=(const Str& s){
        if(this == &s) return *this;

        this->clear();
        this->size = s.size;
        this->capacity = s.capacity;
        if(!s.empty()){
            this->data = new char[capacity+1];
            for(size_t i=0; i<size; ++i) {
                data[i] = s.data[i];
            }
        }
        return *this;
    }

    // 访问（保证运用此字符串时，写函数调用函数时参数类型既可以是常量，也可以不是）
    char& operator[](size_t pos){
        if(pos >= size) throw std::out_of_range("访问位置超出范围");
        return data[pos];
    }

    [[nodiscard]] const char& at(size_t pos) const{
        if(pos >= size) throw std::out_of_range("访问位置超出范围");
        return data[pos];
    }

    Str& operator+=(const char& c){
        this->push_back(c);
        return *this;
    }

    friend bool operator==(const Str& lhs, const Str& rhs){
        if(lhs.size != rhs.size) return false;
        for(size_t i=0; i<lhs.size; ++i){
            if(lhs.data[i] != rhs.data[i]) return false;
        }
        return true;
    }

    void push_back(const char& c){
        if(size == capacity){
            size_t new_cap = capacity+1;
            reserve(new_cap);
        }
        data[size] = c;
        data[++size] = '\0';
    }

    void pop_back(){
        if(this->empty()) return;
        data[--size] = '\0';
    }

    void insert(size_t pos, const Str& s){
        if(s.size == 0) return;
        if(pos > size) throw std::out_of_range("插入位置超出范围");

        // 累加自身时先拷贝一份新副本
        if(this == &s){
            const Str& copy(s);
            insert(pos, copy);
            return;
        }

        if(size + s.size > capacity) reserve(this->size + s.size);
        for(size_t k=size; k>pos; --k){
            data[k+s.size-1] = data[k-1];
        }
        for(size_t i=pos, j=0; j<s.size; ++i, ++j){
            this->data[i] = s.data[j];
        }
        this->size += s.size;
    }

    void erase(size_t pos, size_t n){
        if(n == 0) return;
        if(pos > size) throw std::out_of_range("删除位置超出范围");

        size_t end = pos + n;
        if(end > size) end = size;  // 最多删除范围到size-1
        for(size_t i=pos, j=end; j<=size; ++i, ++j){
            data[i] = data[j];
        }
        this->size -= min(n, size-pos);  // 实际删除的个数可能达不到n个
    }

    void clear(){
        delete []data;
        this->data = nullptr;
        this->size = 0;
    }

    size_t find(char c){  // 字符第一次出现的位置
        for(size_t i=0; i<size; ++i){
            if(data[i] == c) return i;
        }
        return -1;
    }

    [[nodiscard]] Str substr(size_t pos, size_t n) const{
        if(n == 0 || pos > size) throw std::out_of_range("位置超出范围");

        size_t len = min(n, size-pos);
        Str sub_s;
        sub_s.reserve(len);                     // 分配 len+1 字节，此时 capacity=len
        for(size_t i=0; i<len; ++i){
            sub_s.data[i] = data[pos+i];
        }
        sub_s[len] = '\0';
        sub_s.size = len;
        return sub_s;
    }

    [[nodiscard]] bool empty() const{
        return size == 0;
    }

    [[nodiscard]] size_t get_size() const{
        return size;
    }
};

int main(){
    Str s;
    s.push_back('h');
    s.push_back('e');
    s.push_back('l');
    s.push_back('l');
    s.push_back('o');
    s.push_back('w');
    s.push_back('o');
    s.push_back('r');
    s.push_back('l');
    s.push_back('d');

    Str t;
    t.push_back('l');
    t.push_back('l');
    t.push_back('o');
    cout << s[9] << endl;
}