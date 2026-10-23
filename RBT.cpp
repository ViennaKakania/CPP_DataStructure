//
// Created by 24394 on 2026/8/17.
// 红黑树
#include <iostream>
#include "../../Queue/circleQueue(sequenceList).cpp"
#include <vector>
#include <algorithm>
#include <cmath>
using std :: cout;
using std :: endl;
using std :: swap;
using std :: max;
using std :: abs;
using std :: vector;

template<typename T>
struct RB_node{
    T val;
    int color;  // 红：1，黑：0
    RB_node* parent;
    RB_node* left;
    RB_node* right;

    explicit RB_node(T data,  int c=1, RB_node<T>* p=nullptr, RB_node<T>* l=nullptr, RB_node<T>* r=nullptr): val(data), color(c),
    parent(p), left(l), right(r){
    }
};

template<typename T>
class RBT{
private:
    size_t cnt;
    RB_node<T>* root;

    void destroy_tree(RB_node<T>* node){
        if(!node) return;
        destroy_tree(node->left);
        destroy_tree(node->right);
        delete node;
    }

    // 辅助函数：递归复制子树
    RB_node<T>* copy_tree(RB_node<T>* node, RB_node<T>* parent = nullptr) {
        if(!node) return nullptr;

        RB_node<T>* new_left = copy_tree(node->left);
        RB_node<T>* new_right = copy_tree(node->right);
        RB_node<T>* copy = new RB_node(node->val, node->color, parent, new_left, new_right);
        return copy;
    }

    bool isEqualRec(RB_node<T>* node1, RB_node<T>* node2) const{
        if(!node1 && !node2) return true;
        if(!node1 || !node2) return false;
        if(node1->val != node2->val || node1->color != node2->color) return false;
        return isEqualRec(node1->left, node2->left) && isEqualRec(node1->right, node2->right);
    }

    void rotate_left(RB_node<T>* rot_node){   // 该节点作为根的子树的变化：该节点的右子节点变成新的根，自己变成左子节点
        RB_node<T>* new_root = rot_node->right;
        if(!new_root) return;
        rot_node->right = new_root->left;
        if(new_root->left) new_root->left->parent = rot_node;
        new_root->parent = rot_node->parent;
        new_root->left = rot_node;
        rot_node->parent = new_root;

        if(!new_root->parent) root = new_root;  // 当旋转节点为根节点root时，要更新该属性
        else if(new_root->parent->left == rot_node) new_root->parent->left = new_root;
        else new_root->parent->right = new_root;
    }

    void rotate_right(RB_node<T>* rot_node){  // 该节点作为根的子树的变化：该节点的左子节点变成新的根，自己变成右子节点
        RB_node<T>* new_root = rot_node->left;
        if(!new_root) return;
        rot_node->left = new_root->right;
        if(new_root->right) new_root->right->parent = rot_node;
        new_root->parent = rot_node->parent;
        new_root->right= rot_node;
        rot_node->parent = new_root;

        if(!new_root->parent) root = new_root;  // 当旋转节点为根节点root时，要更新该属性
        else if(new_root->parent->left == rot_node) new_root->parent->left = new_root;
        else new_root->parent->right = new_root;
    }

    // 插入修复（相当于AVL的rebalance1；grandparent相当于AVL树中的最靠下非平衡节点）
    void fix_insert(RB_node<T>* current){
        // 当父节点存在且为红色时，需要修复（因为不能连续有红节点，而插入的节点默认先以红节点插入）
        while(current->parent && current->parent->color == 1){
            RB_node<T>* parent = current->parent;
            RB_node<T>* grandparent = parent->parent;

            // 父节点是祖父的左孩子
            if(parent == grandparent->left){
                RB_node<T>* uncle = grandparent->right;

                // 情况1：叔叔为红色————只需变色
                if(uncle && uncle->color == 1){
                    parent->color = 0;
                    uncle->color = 0;
                    grandparent->color = 1;
                    current = grandparent;  // 向上移动，继续检查
                }
                // 情况2 & 3：叔叔为黑色 → 需要旋转+变色
                else{
                    if(current == parent->right){
                        current = parent;
                        rotate_left(current);     // 相当于AVL树的最小不平衡子树的根节点的左子树右重先左旋
                        parent = grandparent;
                        grandparent = parent->parent;
                    }
                    parent->color = 0;
                    grandparent->color = 1;
                    rotate_right(grandparent);   // 相当于AVL树的最小不平衡子树左重右旋
                    current = parent;
                }
            }
            // 父节点是祖父的右孩子
            else{
                RB_node<T>* uncle = grandparent->left;

                // 情况1：叔叔为红色————只需变色
                if(uncle && uncle->color == 1){
                    parent->color = 0;
                    uncle->color = 0;
                    grandparent->color = 1;
                    current = grandparent;  // 向上移动，继续检查
                }
                // 情况2 & 3：叔叔为黑色 → 需要旋转+变色
                else{
                    if(current == parent->left){
                        current = parent;
                        rotate_right(current);    // 相当于AVL树的最小不平衡子树的根节点的右子树左重先右旋
                        parent = grandparent;
                        grandparent = parent->parent;
                    }
                    parent->color = 0;
                    grandparent->color = 1;
                    rotate_left(grandparent);     // 相当于AVL树的最小不平衡子树右重左旋
                    current = parent;
                }
            }
        }
        // 确保根节点始终为黑色
        root->color = 0;
    }

    void inorderRec(RB_node<T>* node){
        if(!node) return;

        inorderRec(node->left);
        cout << node->val << " ";
        inorderRec(node->right);
    }

    void preorderRec(RB_node<T>* node){
        if(!node) return;

        cout << node->val << " ";
        preorderRec(node->left);
        preorderRec(node->right);
    }

    void postorderRec(RB_node<T>* node){
        if(!node) return;

        postorderRec(node->left);
        postorderRec(node->right);
        cout << node->val << " ";
    }

    size_t heightRec(RB_node<T>* node) const{
        if(!node) return 0;
        return 1 + max(heightRec(node->left), heightRec(node->right));
    }

    bool isSubtreeRec(RB_node<T>* node1, RB_node<T>* node2) const{
        if(!node1) return false;   // 空树不能包含非空子树
        if(isEqualRec(node1, node2)) return true;  // 当前根节点开始匹配
        // 否则在左右子树中继续查找
        return isSubtreeRec(node1->left, node2) || isSubtreeRec(node1->right, node2);
    }


public:
    explicit RBT(int cnt=0, RB_node<T>* r=nullptr): cnt(cnt), root(r){
    }

    ~RBT(){
        destroy_tree(root);
        root = nullptr;
    }

    RBT(const RBT& rbt){
        this->root = copy_tree(rbt.root);
        this->cnt = rbt.cnt;
    }

    RBT& operator=(const RBT& rbt){
        if(this == &rbt) return *this;

        if(root){
            destroy_tree(root);
            root = nullptr;
        }
        this->root = copy_tree(rbt.root);
        this->cnt = rbt.cnt;
        return *this;
    }

    bool operator==(const RBT& rbt) const{
        if(this->cnt != rbt.cnt) return false;
        return isEqualRec(root, rbt.root);
    }

    void insert(T val){
        if(!root){
            root = new RB_node<T>(val);
            cnt++;
            return;
        }

        RB_node<T>* cur = root;
        RB_node<T>* parent = nullptr;
        while(cur){
            parent = cur;
            if(val < cur->val) cur = cur->left;
            else if(val > cur->val) cur = cur->right;
            else return;                                // 存在该值则不插入
        }
        RB_node<T>* new_node = new RB_node<T>(val);
        if(val < parent->val) parent->left = new_node;
        else if(val > parent->val) parent->right = new_node;
        new_node->parent = parent;
        cnt++;

        fix_insert(new_node);
    }

    void remove(T val){
        if(!root) return;

        RB_node<T>* to_remove = root;
        RB_node<T>* parent = nullptr;   // 删除节点的父节点
        while(to_remove){
            if(val == to_remove->val) break;
            parent = to_remove;
            if(val < to_remove->val) to_remove = to_remove->left;
            else if(val > to_remove->val) to_remove = to_remove->right;
        }

        // 未找到
        if(!to_remove) return;

        // 情况1 & 2：目标节点至多有一个子节点
        if(!to_remove->left || !to_remove->right){
            RB_node<T>* child = to_remove->left? to_remove->left: to_remove->right;
            if(!parent){
                delete root;
                root = child;
            }
            else{
                if(to_remove->val < parent->val) parent->left = child;
                else parent->right = child;
                delete to_remove;
            }
            cnt--;
            return;
        }
    }

    void inorder(){
        inorderRec(root);
        cout << endl;
    }

    void preorder(){
        preorderRec(root);
        cout << endl;
    }

    void postorder(){
        postorderRec(root);
        cout << endl;
    }

    void levelOrder(){
        if(!root) return;

        Queue<RB_node<T>*> q(cnt);
        q.push(root);
        while(!q.empty()){
            RB_node<T>* cur = q.get_front();
            q.pop();
            cout << cur->val << " ";
            if(cur->left) q.push(cur->left);
            if(cur->right) q.push(cur->right);
        }
        cout << endl;
    }

    [[nodiscard]] RB_node<T>* find(T key) const{
        RB_node<T>* cur = root;
        while(cur && key != cur->val){
            if(key < cur->val) cur = cur->left;
            else cur = cur->right;
        }
        return cur;
    }

    [[nodiscard]] RB_node<T>* get_root() const{
        return root;
    }

    [[nodiscard]] size_t get_size() const{
        return cnt;
    }

    [[nodiscard]] size_t get_height() const{
        return heightRec(root);
    }

    [[nodiscard]] bool is_subtree(const RBT& other) const {
        if(!other.root) return true;  // 空树被认为是任何树的子树（通常定义）
        if(!root) return false;       // 当前树为空，不能包含非空子树
        return isSubtreeRec(root, other.root);
    }

    [[nodiscard]] bool is_full() const{
        return cnt == static_cast<size_t>(pow(2, get_height()) - 1);
    }

    [[nodiscard]] bool is_complete() const{
        if(!root) return true;
        Queue<RB_node<T>*> q(cnt);
        q.push(root);
        bool has_null = false;
        while(!q.empty()){
            auto* cur = q.get_front();
            q.pop();
            if(!cur) has_null = true;
            else{
                if(has_null) return false; // 前面已出现空，后面不能再有非空
                q.push(cur->left);
                q.push(cur->right);
            }
        }
        return true;
    }
};

const char* bool_str(bool b){
    return b? "true": "false";
}