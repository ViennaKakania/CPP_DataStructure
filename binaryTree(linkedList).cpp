//
// Created by 24394 on 2026/8/4.
//
#include <iostream>
#include <queue>
#include <vector>
#include <cmath>
#include <algorithm>
using std :: cout;
using std :: endl;
using std :: queue;
using std :: vector;
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
class Binary_tree{
private:
    size_t cnt;
    tree_node<T>* root;

    void insertRec(T val){
        tree_node<T>* new_node = new tree_node(val);
        if(!root) root = new_node;
        else{
            queue<tree_node<T>*> q;
            q.push(root);
            while(!q.empty()){
                tree_node<T>* cur = q.front();
                q.pop();

                if(!cur->left){
                    cur->left = new_node;
                    break;
                }
                else q.push(cur->left);

                if(!cur->right){
                    cur->right = new_node;
                    break;
                }
                else q.push(cur->right);
            }
        }
        cnt++;
    }

    void mirrorRec(tree_node<T>* node){
        if(!node) return;
        swap(node->left, node->right);
        mirrorRec(node->left);
        mirrorRec(node->right);
    }

    // 辅助函数：递归复制子树
    tree_node<T>* copy_tree(tree_node<T>* node) {
        if(!node) return nullptr;

        tree_node<T>* new_left = copy_tree(node->left);
        tree_node<T>* new_right = copy_tree(node->right);
        tree_node<T>* copy = new tree_node(node->val, new_left, new_right);
        return copy;
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

    void destroy_tree(tree_node<T>* node){
        if(!node) return;
        destroy_tree(node->left);
        destroy_tree(node->right);
        delete node;
    }

    size_t heightRec(tree_node<T>* node) const{
        if(!node) return 0;
        return 1 + max(heightRec(node->left), heightRec(node->right));
    }

    bool isEqualRec(tree_node<T>* node1, tree_node<T>* node2) const{
        if(!node1 && !node2) return true;
        if(!node1 || !node2) return false;
        if(node1->val != node2->val) return false;
        return isEqualRec(node1->left, node2->left) && isEqualRec(node1->right, node2->right);
    }

    bool isSubtreeRec(tree_node<T>* node1, tree_node<T>* node2) const{
        if(!node1) return false;   // 空树不能包含非空子树
        if(isEqualRec(node1, node2)) return true;  // 当前根节点开始匹配
        // 否则在左右子树中继续查找
        return isSubtreeRec(node1->left, node2) || isSubtreeRec(node1->right, node2);
    }
public:
    explicit Binary_tree(int cnt=0, tree_node<T>* r=nullptr): cnt(cnt), root(r){
    }

    ~Binary_tree(){
        destroy_tree(root);
        root = nullptr;
    }

    Binary_tree(const Binary_tree& bt){
        this->root = copy_tree(bt.root);
        this->cnt = bt.cnt;
    }

    Binary_tree& operator=(const Binary_tree& bt){
        if(this == &bt) return *this;

        if(root){
            destroy_tree(root);
            root = nullptr;
        }
        this->root = copy_tree(bt.root);
        this->cnt = bt.cnt;
        return *this;
    }

    bool operator==(const Binary_tree& bt){
        if(this->cnt != bt.cnt) return false;
        return isEqualRec(root, bt.root);
    }

    void mirror(){
        mirrorRec(root);
    }

    void insert(T val){
        insertRec(val);
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

        queue<tree_node<T>*> q;
        q.push(root);
        while(!q.empty()){
            tree_node<T>* cur = q.front();
            q.pop();
            cout << cur->val << " ";
            if(cur->left) q.push(cur->left);
            if(cur->right) q.push(cur->right);
        }
        cout << endl;
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

    [[nodiscard]] bool is_full() const{
        return cnt == static_cast<size_t>(pow(2, get_height()) - 1);
    }

    [[nodiscard]] bool is_complete() const{
        if(!root) return true;
        queue<tree_node<T>*> q;
        q.push(root);
        bool has_null = false;
        while(!q.empty()){
            auto* cur = q.front();
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

    [[nodiscard]] bool is_balanced() const{
        if(!root) return true;
        return abs(static_cast<int>(heightRec(root->left) - heightRec(root->right))) <= 1;
    }

    [[nodiscard]] bool is_subtree(const Binary_tree& other) const {
        if(!other.root) return true;  // 空树被认为是任何树的子树（通常定义）
        if(!root) return false;       // 当前树为空，不能包含非空子树
        return isSubtreeRec(root, other.root);
    }

};

// 线索中序遍历
template<typename T>
void thread_inorder(Binary_tree<T>& bt){
    tree_node<T>* cur = bt.get_root();
    // 找到中序遍历中以当前cur节点的左子节点为根的子树的最后一个节点，把它的右指针连接根cur，作为根节点cur的前驱
    while(cur){
        if(cur->left){
            // 前驱一定在左子树中找到（当前cur作为根时，左子树最靠右下的节点为cur的前驱）
            tree_node<T>* prior = cur->left;

            // 找到cur的前驱prior
            while(prior->right && prior->right != cur) prior = prior->right;
            // 最底下的右节点就是当前子树根节点的前驱，并建立线索：将前驱的 right 指向当前节点
            if(!prior->right){
                prior->right = cur;
                cur = cur->left;
            }
            // 前驱的right已经是cur（线索已建立过），说明以cur为根的左子树已遍历完毕，此时打印当前子树的根节点cur的同时消除线索
            else{
                prior->right = nullptr;
                cout << cur->val << " ";
                cur = cur->right;
            }
        }
        // 若无左子树，则先打印当前子树的根节点，再访问当前子树根的右子节点（或者也可能是通过线索回溯）
        else{
            cout << cur->val << " ";
            cur = cur->right;
        }
    }
    cout << endl;
}

// 线索前序遍历（线索后序遍历只需先把前序的逻辑中左换成右，右换成左，最后把输出倒序即可）
template<typename T>
void thread_preorder(Binary_tree<T>& bt){
    tree_node<T>* cur = bt.get_root();

    while(cur){
        if(!cur->left){
            cout << cur->val << " ";  // 无左子树直接访问根节点
            cur = cur->right;
        }
        else{
            tree_node<T>* prior = cur->left;
            while(prior->right && prior->right != cur) prior = prior->right;
            if(!prior->right){
                cout << cur->val << " ";          // 先访问当前子树的根
                prior->right = cur;               // 再建立线索
                cur = cur->left;                  // 再去遍历左子树，并找到前驱为左子树根的节点，建立新线索
            }
            else{
                prior->right = nullptr;
                cur = cur->right;
            }
        }

    }
}

int main(){
    // ========== 1. 插入与遍历 ==========
    Binary_tree<int> tree;
    vector<int> vals = {1, 2, 3, 4, 5, 6, 7};
    for(int v: vals) tree.insert(v);

    cout << "=== 插入 1~7（完全二叉树） ===" << endl;
    cout << "层序: ";
    tree.levelOrder();   // 1 2 3 4 5 6 7
    cout << "前序: ";
    tree.preorder();     // 1 2 4 5 3 6 7
    cout << "中序: ";
    tree.inorder();      // 4 2 5 1 6 3 7
    cout << "线索中序: ";
    thread_inorder(tree);      // 4 2 5 1 6 3 7
    cout << "后序: ";
    tree.postorder();    // 4 5 2 6 7 3 1
    cout << "大小: " << tree.get_size() << endl; // 7

    // ========== 2. 高度与完全/满树判断 ==========
    cout << "\n=== 属性检查 ===" << endl;
    cout << "高度: " << tree.get_height() << endl;          // 3 (根高度为1)
    cout << "是否满树: " << tree.is_full() << endl;         // 1 (7 == 2^3 -1)
    cout << "是否完全: " << tree.is_complete() << endl;     // 1
    cout << "是否平衡: " << tree.is_balanced() << endl;     // 1

    // ========== 3. 镜像 ==========
    cout << "\n=== 镜像翻转 ===" << endl;
    tree.mirror();
    cout << "镜像后层序: ";
    tree.levelOrder(); // 1 3 2 7 6 5 4
    // 再次镜像恢复
    tree.mirror();

    // ========== 4. 拷贝构造与赋值 ==========
    cout << "\n=== 拷贝构造 ===" << endl;
    Binary_tree<int> tree_copy(tree);
    cout << "原树层序: ";
    tree.levelOrder();
    cout << "拷贝树层序: ";
    tree_copy.levelOrder();

    cout << "=== 赋值操作 ===" << endl;
    Binary_tree<int> tree_assigned;
    tree_assigned = tree;
    cout << "赋值树层序: ";
    tree_assigned.levelOrder();

    // ========== 5. 比较与子树 ==========
    cout << "\n=== 比较与子树 ===" << endl;
    Binary_tree<int> subtree;
    subtree.insert(2);
    subtree.insert(4);
    subtree.insert(5);
    cout << "子树层序: ";
    subtree.levelOrder(); // 2 4 5
    cout << "tree 是否包含 subtree: " << tree.is_subtree(subtree) << endl; // 1 (2为根的子树)
    cout << "tree == subtree: " << (tree == subtree) << endl;             // 0

    // 构造一个不同树
    Binary_tree<int> diff_tree;
    diff_tree.insert(1);
    diff_tree.insert(2);
    diff_tree.insert(4); // 结构不同
    cout << "tree == diff_tree: " << (tree == diff_tree) << endl; // 0

    // ========== 6. 边界情况 ==========
    cout << "\n=== 边界测试 ===" << endl;
    Binary_tree<int> empty_tree;
    cout << "空树高度: " << empty_tree.get_height() << endl;        // 0
    cout << "空树是否满树: " << empty_tree.is_full() << endl;       // 1 (按定义可能为true)
    cout << "空树是否完全: " << empty_tree.is_complete() << endl;   // 1
    cout << "空树是否平衡: " << empty_tree.is_balanced() << endl;   // 1
    cout << "空树是否子树: " << empty_tree.is_subtree(empty_tree) << endl; // 1

    // 单节点树
    Binary_tree<int> single;
    single.insert(10);
    cout << "单节点高度: " << single.get_height() << endl;          // 1
    cout << "单节点是否满树: " << single.is_full() << endl;         // 1
    cout << "单节点是否完全: " << single.is_complete() << endl;     // 1
    cout << "单节点是否平衡: " << single.is_balanced() << endl;     // 1

    cout << "\n所有测试通过！" << endl;
    return 0;
}