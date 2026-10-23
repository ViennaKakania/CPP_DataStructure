// Recurse递归（简写Rec）
// Created by 24394 on 2026/8/15.
// 二叉查找树（binary search tree） ，取消了镜像操作
#include <iostream>
#include "../../Queue/circleQueue(sequenceList).cpp"
#include <algorithm>
#include <cmath>
using std :: cout;
using std :: endl;
using std :: swap;
using std :: max;
using std :: abs;

template<typename T>
struct tree_node{
    T val;
    tree_node* left;
    tree_node* right;

    explicit tree_node(T data, tree_node<T>* l=nullptr, tree_node<T>* r=nullptr): val(data), left(l), right(r){}
};

template<typename T>
class BST{
private:
    size_t cnt;
    tree_node<T>* root;

    void destroy_tree(tree_node<T>* node){
        if(!node) return;
        destroy_tree(node->left);
        destroy_tree(node->right);
        delete node;
    }

    // 辅助函数：递归复制子树
    tree_node<T>* copy_tree(tree_node<T>* node) {
        if(!node) return nullptr;

        tree_node<T>* new_left = copy_tree(node->left);
        tree_node<T>* new_right = copy_tree(node->right);
        tree_node<T>* copy = new tree_node(node->val, new_left, new_right);
        return copy;
    }

    bool isEqualRec(tree_node<T>* node1, tree_node<T>* node2) const{
        if(!node1 && !node2) return true;
        if(!node1 || !node2) return false;
        if(node1->val != node2->val) return false;
        return isEqualRec(node1->left, node2->left) && isEqualRec(node1->right, node2->right);
    }

    tree_node<T>* insertRec(tree_node<T>* node, T val){   // node是插入位置，表示在node下选左右插入值为val的节点
        if(!node){
            tree_node<T>* new_node = new tree_node(val);
            cnt++;
            return new_node;
        }

        if(val < node->val) node->left = insertRec(node->left, val);
        else if(val > node->val) node->right = insertRec(node->right, val);
        // 相等则不插入

        return node;
    }

    tree_node<T>* removeRec(tree_node<T>* node, T val, bool& deleted){
        if(!node) return nullptr;

        if(val < node->val){
            node->left = removeRec(node->left, val, deleted);
            return node;
        }
        else if(val > node->val){
            node->right = removeRec(node->right, val, deleted);
            return node;
        }
        else{
            // 找到要删除的节点 node
            deleted = true;   // 标记已删除（最终会减少 cnt）

            // 情况1 & 2：处理无子或只有一个子
            if(!node->left){
                tree_node<T>* right_child = node->right;
                delete node;
                return right_child;
            }
            else if(!node->right){
                tree_node<T>* left_child = node->left;
                delete node;
                return left_child;
            }

            // 情况3：有两个子节点，找后继（successor），填充删除该节点的空缺（右子树的最小值/左子树的最大值）
            tree_node<T>* succ = node->right;
            while(succ->left) succ = succ->left;
            T succ_val = succ->val;

            // 用后继值覆盖当前节点
            node->val = succ_val;
            // 递归删除右子树中的后继节点（后继一定没有左子，会进入情况1/2）
            node->right = removeRec(node->right, succ_val, deleted);
            return node;
        }
    }

    void inorderRec(tree_node<T>* node){
        if(!node) return;

        inorderRec(node->left);
        cout << node->val << " ";
        inorderRec(node->right);
    }

    void preorderRec(tree_node<T>* node){
        if(!node) return;

        cout << node->val << " ";
        preorderRec(node->left);
        preorderRec(node->right);
    }

    void postorderRec(tree_node<T>* node){
        if(!node) return;

        postorderRec(node->left);
        postorderRec(node->right);
        cout << node->val << " ";
    }

    size_t heightRec(tree_node<T>* node) const{
        if(!node) return 0;
        return 1 + max(heightRec(node->left), heightRec(node->right));
    }

    bool isBalancedRec(tree_node<T>* node) const{
        if(!node) return true;
        int diff = heightRec(node->left) - heightRec(node->right);
        return abs(diff) <= 1 && isBalancedRec(node->left) && isBalancedRec(node->right);
    }

    bool isSubtreeRec(tree_node<T>* node1, tree_node<T>* node2) const{
        if(!node1) return false;   // 空树不能包含非空子树
        if(isEqualRec(node1, node2)) return true;  // 当前根节点开始匹配
        // 否则在左右子树中继续查找
        return isSubtreeRec(node1->left, node2) || isSubtreeRec(node1->right, node2);
    }


public:
    explicit BST(int cnt=0, tree_node<T>* r=nullptr): cnt(cnt), root(r){
    }

    ~BST(){
        destroy_tree(root);
        root = nullptr;
    }

    BST(const BST& bt){
        this->root = copy_tree(bt.root);
        this->cnt = bt.cnt;
    }

    BST& operator=(const BST& bt){
        if(this == &bt) return *this;

        if(root){
            destroy_tree(root);
            root = nullptr;
        }
        this->root = copy_tree(bt.root);
        this->cnt = bt.cnt;
        return *this;
    }

    bool operator==(const BST& bt) const{
        if(this->cnt != bt.cnt) return false;
        return isEqualRec(root, bt.root);
    }

    void insert(T val){
        this->root = insertRec(root, val);
    }

    void remove(T val){
        bool deleted = false;
        root = removeRec(root, val, deleted);
        if(deleted) --cnt;
    }

    void insert2(T val){
        if(!root){
            root = new tree_node<T>(val);
            cnt++;
            return;
        }

        tree_node<T>* cur = root;
        tree_node<T>* parent = nullptr;
        while(cur){
            parent = cur;
            if(val < cur->val) cur = cur->left;
            else if(val > cur->val) cur = cur->right;
            else return;                                // 存在该值则不插入
        }
        tree_node<T>* new_node = new tree_node<T>(val);
        if(val < parent->val) parent->left = new_node;
        else if(val > parent->val) parent->right = new_node;
        cnt++;
    }

    void remove2(T val){
        if(!root) return;

        tree_node<T>* to_remove = root;
        tree_node<T>* parent = nullptr;   // 删除节点的父节点
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
            tree_node<T>* child = to_remove->left? to_remove->left: to_remove->right;
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

        // 情况3：目标节点有两个子节点 (先寻找后继（右子树中的最小节点）及其父节点，完成空缺的填充，再把用于填补的节点删除)
        tree_node<T>* succ_parent = to_remove;
        tree_node<T>* succ = to_remove->right;
        while(succ->left){
            succ_parent = succ;
            succ = succ->left;
        } // succ指向 因作为被删除节点位置的填补 而实际删除的节点
        to_remove->val = succ->val;  // 并没有delete掉该节点，只是改变val作形式上删除

        // 之后再把右子树的最小节点删除
        if(succ == to_remove->right) succ_parent->right = succ->right;// 说明被删除节点的右子树只有右子节点，最小的就是第一个右子节点（该节点再往下就只有右子树），让第一个右子节点无法访问到即可
        else succ_parent->left = succ->right;  // 正常情况，被删除节点是右子树的最小左子节点（该节点再往下就只有右子树），把左指针指向该节点的右子树即可
        delete succ;
        cnt--;
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

        Queue<tree_node<T>*> q(cnt);
        q.push(root);
        while(!q.empty()){
            tree_node<T>* cur = q.get_front();
            q.pop();
            cout << cur->val << " ";
            if(cur->left) q.push(cur->left);
            if(cur->right) q.push(cur->right);
        }
        cout << endl;
    }

    [[nodiscard]] tree_node<T>* find(T key) const{
        tree_node<T>* cur = root;
        while(cur && key != cur->val){
            if(key < cur->val) cur = cur->left;
            else cur = cur->right;
        }
        return cur;
    }

    [[nodiscard]] tree_node<T>* get_root() const{
        return root;
    }

    [[nodiscard]] size_t get_size() const{
        return cnt;
    }

    [[nodiscard]] size_t get_height() const{
        return heightRec(root);
    }

    [[nodiscard]] bool is_balanced() const{
        if(!root) return true;
        return isBalancedRec(root);
    }

    [[nodiscard]] bool is_subtree(const BST& other) const {
        if(!other.root) return true;  // 空树被认为是任何树的子树（通常定义）
        if(!root) return false;       // 当前树为空，不能包含非空子树
        return isSubtreeRec(root, other.root);
    }

    [[nodiscard]] bool is_full() const{
        return cnt == static_cast<size_t>(pow(2, get_height()) - 1);
    }

    [[nodiscard]] bool is_complete() const{
        if(!root) return true;
        Queue<tree_node<T>*> q(cnt);
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

//int main(){
//    BST<int> tree1;
//    cout << "初始树大小: " << tree1.get_size() << endl;  // 0
//    cout << "初始树高度: " << tree1.get_height() << endl;  // 0
//    cout << "是否为空树: " << bool_str(tree1.get_root() == nullptr) << endl;  // true
//
//    int values[] = {5, 3, 7, 2, 4, 6, 8};
//    for(int v: values) tree1.insert(v);
//    cout << "大小: " << tree1.get_size() << endl;  // 7
//    cout << "高度: " << tree1.get_height() << endl;  // 3
//
//    // 遍历输出
//    cout << "前序遍历: "; tree1.preorder();
//    cout << "中序遍历: "; tree1.inorder();
//    cout << "后序遍历: "; tree1.postorder();
//    cout << "层序遍历: "; tree1.levelOrder();
//
//    // 判断树性质
//    cout << "是否为满二叉树: " << bool_str(tree1.is_full()) << endl;  // true
//    cout << "是否为完全二叉树: " << bool_str(tree1.is_complete()) << endl;  // true
//    cout << "是否平衡: " << bool_str(tree1.is_balanced()) << endl;  // true
//
//    // 测试插入重复值（不应增加节点）
//    tree1.insert(5);
//    cout << "插入重复值5后，大小: " << tree1.get_size() << endl;  // 7
//    cout << "层序遍历（应无变化）: "; tree1.levelOrder();
//
//    // 拷贝构造（测试拷贝）
//    BST<int> tree2(tree1);
//    cout << "拷贝构造 tree2 (tree1的拷贝):" << endl;
//    cout << "tree2 中序遍历: "; tree2.inorder();
//
//    // 测试相等性
//    cout << "tree1 == tree2? " << bool_str(tree1 == tree2) << endl;  // false
//    BST<int> tree3;
//    tree3.insert(5);
//    tree3.insert(3);
//    tree3.insert(7);
//    tree3.insert(2);
//    tree3.insert(4);
//    tree3.insert(6);
//    tree3.insert(8);
//    cout << "tree2 == tree3? " << bool_str(tree2 == tree3) << endl;  // true
//
//    // 测试子树
//    BST<int> subtree;
//    subtree.insert(3);
//    subtree.insert(2);
//    subtree.insert(4);
//    cout << "tree2 是否包含子树 {3,2,4} : " << bool_str(tree2.is_subtree(subtree)) << endl; // true
//    BST<int> not_sub;
//    not_sub.insert(10);
//    cout << "tree2 是否包含子树 {10} : " << bool_str(tree2.is_subtree(not_sub)) << endl; // false
//    cout << endl;
//
//    // 测试赋值运算符
//    BST<int> tree4;
//    tree4 = tree2;
//    cout << "tree4 (赋值自tree2) 中序遍历: "; tree4.inorder();
//    cout << "tree4 == tree2? " << bool_str(tree4 == tree2) << endl;  // true

//    // 测试删除
//    tree2.remove(3);   // 删除带两个子节点的节点
//    tree2.remove2(7);
//    cout << "tree2 删除3后中序: "; tree2.inorder();   // 应输出 2 4 5 6 8
//    cout << "大小: " << tree2.get_size() << endl; // 5
//}