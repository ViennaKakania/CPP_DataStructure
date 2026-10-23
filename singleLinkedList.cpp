//
// Created by 24394 on 2026/5/4.
//
#include <iostream>
#include <stdexcept>
using std :: cout;
using std :: endl;

// 创建结点类
template<typename T>
struct list_node{
    T val;
    list_node* next;
    explicit list_node(T x, list_node* nxt=nullptr) : val(x), next(nxt) {}
};

template<typename T>
class Linear_list{
private:
    size_t cnt; // 结点数量（与顺序表（动态数组）实现中的size类似）
    list_node<T>* dummy; // 哨兵指针

public:
    explicit Linear_list(): cnt(0), dummy(new list_node<T>(0)){
    }

    ~Linear_list(){ // 析构函数（只有类中有堆区变量时才需要特意重写）
        while(dummy->next){
            list_node<T>* temp = dummy->next;
            dummy->next = temp->next;
            delete temp;
         // temp = nullptr; 因为temp在每一次循环会重新创建，生命周期也随循环结束而结束，因此无需通过置空来防止指针悬挂
        }
        delete dummy;  // 最后还要释放哨兵节点
        dummy = nullptr;
    }

    // 自己实现深拷贝
    Linear_list(const Linear_list& lst): cnt(0), dummy(new list_node<T>(lst.dummy->val)) {
        // this->cnt = lst.cnt;
        if(lst.dummy->next){
            list_node<T>* cur = lst.dummy->next;
            while(cur){
                this->push_back(cur->val);  // 结点要逐个复制
                cur = cur->next;
            }
        }
    };
    Linear_list& operator=(const Linear_list& lst) {
        if(this == &lst) return *this;

        // 接下来这一步，调用operator=的对象要先检查自己的指针属性是否为空，不为空要先置空，才能再进行深拷贝操作

//        if(this->head){
//            delete this->head;
//            this->head = nullptr;
//        }
//        this->head = lst.head;   不能只删除头节点，结点需逐个删除，否则后续的节点会内存泄漏
        // 先清空结点
        while(this->dummy->next){
            list_node<T>* temp = this->dummy->next;
            this->dummy->next = temp->next;
            cnt--;
            delete temp;
        }
        // 深拷贝新链表
        list_node<T>* cur = lst.dummy->next;
        while(cur){
            push_back(cur->val);
            cur = cur->next;
        }
        return *this;  // 增加return实现链式调用
    };

    void push_front(const T& val){ // 头插
        dummy->next = new list_node<T>(val, dummy->next);
        cnt++;
    }

    void pop_front(){
        if(!dummy->next) return;

        list_node<T>* to_delete = dummy->next;
        if(!to_delete->next){
            dummy->next = nullptr;
        }
        else{
            to_delete->next->prior = dummy;
            dummy->next = to_delete->next;
        }
        cnt--;
        delete to_delete;
    }

    void push_back(const T& val){ // 尾插
        if(!dummy->next){
            dummy->next = new list_node<T>(val);
        }
        else{
            list_node<T>* cur = dummy->next;
            while(cur->next){
                cur = cur->next;
            }
            cur->next = new list_node<T>(val);
        }
        cnt++;
    }

    void pop_back(){
        if(!dummy->next) return;

        list_node<T>* cur = dummy->next;
        while(cur->next) cur = cur->next;
        delete cur;
        cnt--;
    }

    void insert(size_t pos, const T& val){ // 在 pos 位置插入
        if(pos > cnt-1) throw std::out_of_range("插入位置超出范围，请使用尾插添加元素");
        if (pos == 0) {
            push_front(val);
            return;
        }

        list_node<T>* pre = dummy->next;
        while(pos > 1){
            pre = pre->next;
            pos--;
        }
        list_node<T>* nxt = pre->next;
        // list_node<T> node{val, nxt};   这是栈对象，函数结束就被销毁
        // pre->next = &node;   指针指向已销毁的对象（悬垂指针）

        // 一定要使用堆分配
        pre->next = new list_node<T>(val, nxt);
        cnt++;
    };

    void erase(size_t pos){ // 删除 pos 位置结点
        if (pos >= cnt) throw std::out_of_range("删除位置无效");

        list_node<T>* to_delete;
        if(pos == 0) {
            to_delete = dummy->next;
            dummy->next = dummy->next->next;
            cnt--;
            delete to_delete;
            return;
        }

        list_node<T>* pre = dummy->next;
        while(pos > 1){
            pre = pre->next;
            pos--;
        }
        to_delete = pre->next;
        pre->next = to_delete->next;
        delete to_delete;
        // to_delete = nullptr;
        cnt--;
    };

    [[nodiscard]] size_t find(const T& val) const{ // 查找并返回位置
        list_node<T>* cur = dummy->next;
        size_t pos = 0;
        while(cur){
            if(cur->val==val){
                return pos;
            }
            pos++;
            cur = cur->next;
        }
        return -1;
    };

    [[nodiscard]] T at(size_t pos) const{ // 按位置访问
        if(pos >= cnt) throw std::out_of_range("访问位置超出范围，无效");
        list_node<T>* cur = dummy->next;
        for(int i=0; i<pos; i++){
            cur = cur->next;
        }
        return cur->val;
    };

    [[nodiscard]] size_t get_size() const{
        return cnt;
    };

    void print() const{
        list_node<T>* cur = dummy->next;
        while(cur){
            cout << cur->val << " ";
            cur = cur->next;
        }
        cout << endl;
    };
};

int main() {
    Linear_list<int> list;
    list.push_front(1);
    list.push_back(2);
    list.insert(1, 99);   // 1, 99, 2
    list.print();         // 输出：1 99 2
    list.erase(0);        // 删除第一个元素
    list.print();         // 输出：99 2

    Linear_list<int> list1 = list;
    Linear_list<int> list2(list);
    list1.print();
    list2.print();


    int num = list.at(2);
    cout << num << endl;
    return 0;
}
