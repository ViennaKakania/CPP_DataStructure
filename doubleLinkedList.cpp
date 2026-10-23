//
// Created by 24394 on 2026/7/30.
//
#include <iostream>
#include <stdexcept>
using std :: cout;
using std :: endl;

template<typename T>
struct list_node{
    T val;
    list_node* prior;
    list_node* next;

    explicit list_node(T x, list_node* pri=nullptr, list_node* nxt=nullptr): val(x), prior(pri), next(nxt){};
};

template<typename T>
class DLinear_list{
private:
    int cnt;
    list_node<T>* dummy;

public:
    explicit DLinear_list(): cnt(0), dummy(new list_node<T>(T{})){}

    ~DLinear_list(){
        while(dummy->next){
            list_node<T>* temp = dummy->next;
            dummy->next = temp->next;
            delete temp;
        }
        delete dummy;
        dummy = nullptr;
    }

    DLinear_list(const DLinear_list& lst): cnt(0), dummy(new list_node<T>(lst.dummy->val)){
        if(lst.dummy->next){
            list_node<T>* cur = lst.dummy->next;
            while(cur){
                this->push_back(cur->val);  // 结点要逐个复制
                cur = cur->next;
            }
        }
    }

    DLinear_list& operator=(const DLinear_list& lst){
        if(this == &lst) return *this;

        while(this->dummy->next){
            list_node<T>* temp = this->dummy->next;
            this->dummy->next = temp->next;
            cnt--;
            delete temp;
        }
        list_node<T>* cur = lst.dummy->next;
        while(cur){
            this->push_back(cur->val);
            cur = cur->next;
        }
        return *this;
    }

    void add_next_node(list_node<T>* p, list_node<T>* to_add){
        if(!p || !to_add) return;

        if(!p->next){
            p->next = to_add;
            to_add->prior = p;
        }
        else{
            p->next->prior = to_add;
            to_add->next = p->next;
            p->next = to_add;
            to_add->prior = p;
        }
        cnt++;
    }

    void delete_prior_node(list_node<T>* p){
        if(!p || p->prior == dummy || p == dummy) return;

        list_node<T>* to_delete = p->prior;
        p->prior = to_delete->prior;
        to_delete->prior->next = p;
        delete to_delete;
        cnt--;
    }

    void push_front(const T& val){
        // 当使用 new 表达式初始化一个变量时，如果左侧的类型与 new 右侧的类型完全一致，那么可以使用 auto 来避免重复书写类型名称
        auto* node = new list_node<T>(val, dummy, dummy->next);
        if(dummy->next) dummy->next->prior = node;  // 判空保护
        dummy->next = node;
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

    void push_back(const T& val){
        list_node<T>* node = new list_node<T>(val);
        if(!dummy->next){
            dummy->next = node;
            node->prior = dummy;
        }
        else{
            list_node<T>* cur = dummy->next;
            while(cur->next) cur = cur->next;
            cur->next = node;
            node->prior = cur;
        }
        cnt++;
    }

    void pop_back(){
        if(!dummy->next) return;

        list_node<T>* cur = dummy->next;
        while(cur->next) cur = cur->next;
        if(cur->prior != dummy) cur->prior->next = nullptr;        // 如果有前驱（即链表不只一个节点)
        else dummy->next = nullptr;      // 只有一个节点
        delete cur;
        cnt--;
    }

    void insert(size_t pos, const T& val){
        if(pos > cnt-1) throw std::out_of_range("插入位置超出范围，请使用尾插添加元素");

        if(pos == 0){
            this->push_front(val);
            return;
        }

        list_node<T>* pre = dummy->next;
        while(pos > 1){
            pre = pre->next;
            pos--;
        }
        list_node<T>* node = new list_node<T>(val, pre, pre->next);
        pre->next->prior = node;
        pre->next = node;
        cnt++;
    }

    void erase(size_t pos){
        if(pos >= cnt) throw std::out_of_range("删除位置超出范围");

        if(pos == 0){
            this->pop_front();
            return;
        }

        if(pos == cnt-1){
            this->pop_back();
            return;
        }

        list_node<T>* pre = dummy->next;
        while(pos > 1){
            pre = pre->next;
            pos--;
        }
        list_node<T>* to_delete = pre->next;
        to_delete->next->prior = pre;
        pre->next = to_delete->next;
        cnt--;
        delete to_delete;
    }

    [[nodiscard]] const T& at(size_t pos) const{
        if(pos >= cnt) throw std::out_of_range("访问位置超出范围，无效");
        list_node<T>* cur = dummy->next;
        for(int i=0; i<pos; i++){
            cur = cur->next;
        }
        return cur->val;
    }

    [[nodiscard]] size_t get_size() const{
        return cnt;
    }

    void print() const{
        list_node<T>* cur = dummy->next;
        while(cur){
            cout << cur->val << " ";
            cur = cur->next;
        }
        cout << endl;
    }
};

int main() {
    DLinear_list<int> list;
    list.push_front(1);
    list.push_back(2);
    list.insert(1, 99);   // 1, 99, 2
    list.print();         // 输出：1 99 2
    list.erase(0);        // 删除第一个元素
    list.print();         // 输出：99 2

    DLinear_list<int> list1 = list;
    DLinear_list<int> list2(list);
    list1.print();
    list2.print();

    cout << list.at(1) << endl;
    cout << list.get_size() << endl;
    return 0;
}