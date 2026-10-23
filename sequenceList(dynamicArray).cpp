//
// Created by 24394 on 2026/5/4.
// 所谓动态数组，就是可以不断调整容量从而调整数组长度的数组
#include <iostream>
#include <stdexcept>

using std::cout;
using std::endl;


template<typename T>
class Linear_list{
private:
    size_t size; // 当前元素个数
    size_t capacity; // 总容量
    T* data;  // 指向动态数组的指针

    void reserve(){ // 内部扩容函数（当容量不足时调用）
        size_t new_cap = capacity==0? 4: capacity * 2;
        T* new_data = new T[new_cap];
        for(size_t i=0; i<size; i++){
            new_data[i] = data[i];
        }
        delete []data;
        data = new_data;
        capacity = new_cap;
    }

public:
    explicit Linear_list(size_t cap): size(0), capacity(cap), data(nullptr){ // 构造函数
        if(cap > 0) data = new T[cap];
    }

    ~Linear_list(){ // 析构函数（只有类中有堆区变量时才需要特意重写）
        delete []data;
        data = nullptr;
    };

    Linear_list(const Linear_list& lst): size(lst.size), capacity(lst.capacity), data(nullptr){ // 手动实现深拷贝
//        if(this->data){   拷贝构造只有创建对象时调用，不用像赋值运算符一样先置空
//            delete []data;
//            this->data = nullptr;
//        }

//        this->size = lst.size;
//        this->capacity = lst.capacity;
//        this->data = nullptr;
        if(lst.data){
            this->data = new T[capacity];
            for(size_t i = 0; i < size; ++i){
                this->data[i] = lst.data[i];
            }
        }
    }

    Linear_list& operator=(const Linear_list& lst){
        if(this == &lst) return *this;

        this->size = lst.size;
        this->capacity = lst.capacity;

        // 接下来这一步，调用operator=的对象要先检查自己的指针属性是否为空，不为空要先置空，才能再进行深拷贝操作
        if(this->data){
            delete[] this->data;
            this->data = nullptr;
        }

        // 当lst不为空表时，才操作当前对象的data等于lst的data
        if(lst.data){
            this->data = new T[capacity];
            for(size_t i=0; i<size; ++i) {
                data[i] = lst.data[i];
            }
        }
        return *this;  // 增加return实现链式调用
    }

    void push_back(const T& val){  // 尾插
        if(size==capacity) reserve();
        data[size] = val;
        size++;
    }

    void insert(size_t pos, const T& val){ // 在 pos 位置插入
        if(pos > size) throw std::out_of_range("插入位置超出范围，请使用尾插添加元素");
        if(size == capacity) reserve();
        for(size_t i=size; i>pos; i--){
            data[i] = data[i-1];
        }
        data[pos] = val;
        size++;
    }

    void erase(size_t pos){ // 删除 pos 位置元素
        if(pos >= size) throw std::out_of_range("删除位置超出范围，无效");
        for(size_t i=pos; i<size-1; i++){
            data[i] = data[i+1];
        }
        size--;
    }

    [[nodiscard]] int find(const T& val) const{  // 返回第一个匹配位置，未找到返回 -1
        for(int i=0; i<size; i++){
            if(data[i] == val) return i;
        }
        return -1;
    }

    T& at(size_t pos){ // 随机访问
        if(pos >= size) throw std::out_of_range("访问位置超出范围，无效");
        return data[pos];
    }

    const T& at(size_t pos) const{ // 随机访问
        if(pos >= size) throw std::out_of_range("访问位置超出范围，无效");
        return data[pos];
    }

    [[nodiscard]] size_t get_size() const{
        return size;
    }

    [[nodiscard]] bool empty() const{
        return size == 0;
    }

    void print() const{ // 遍历打印
        for(size_t i=0; i<size; i++){
            cout << data[i] << " ";
        }
        cout << endl;
    }

};

//int main(){
//    Linear_list<int> list(0);
//    list.push_back(1);
//    list.push_back(2);
//    list.insert(1, 99);   // 1, 99, 2
//    list.print();         // 输出：1 99 2
//    list.erase(0);        // 删除第一个元素
//    list.print();         // 输出：99 2
//    std::cout << list.find(99) << std::endl; // 输出 0
//
//    Linear_list<int> list1 = list;
//    Linear_list<int> list2(list);
//    list1.print();
//    list2.print();
//    return 0;
//}