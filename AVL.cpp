//
// Created by 24394 on 2026/8/16.
// 平衡二叉树
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
struct AVL_node{
    T val;
    AVL_node* left;
    AVL_node* right;
    int height;  // 利用高度计算平衡因子（-1，0，1）

    explicit AVL_node(T data, AVL_node<T>* l=nullptr, AVL_node<T>* r=nullptr, int h=1): val(data), left(l), right(r), height(h){
    }
};

// 相比一般的二叉查找树，只有插入和删除逻辑不同，平衡二叉树的插入删除依赖左旋和右旋两个操作
template<typename T>
class AVL{
private:
    size_t cnt;
    AVL_node<T>* root;

    void destroy_tree(AVL_node<T>* node){
        if(!node) return;
        destroy_tree(node->left);
        destroy_tree(node->right);
        delete node;
    }

    // 辅助函数：递归复制子树
    AVL_node<T>* copy_tree(AVL_node<T>* node) {
        if(!node) return nullptr;

        AVL_node<T>* new_left = copy_tree(node->left);
        AVL_node<T>* new_right = copy_tree(node->right);
        AVL_node<T>* copy = new AVL_node(node->val, new_left, new_right, node->height);
        return copy;
    }

    bool isEqualRec(AVL_node<T>* node1, AVL_node<T>* node2) const{
        if(!node1 && !node2) return true;
        if(!node1 || !node2) return false;
        if(node1->val != node2->val || node1->height != node2->height) return false;
        return isEqualRec(node1->left, node2->left) && isEqualRec(node1->right, node2->right);
    }

    void updateHeight(AVL_node<T>* node){
        if(node){
            int lh = node->left? node->left->height: 0;
            int rh = node->right? node->right->height: 0;
            node->height = 1 + max(lh, rh);
        }
    }

    int balanceCal(AVL_node<T>* node){
        if(node){
            int lh = node->left? node->left->height: 0;
            int rh = node->right? node->right->height: 0;
            return lh - rh;
        }
        return 0;
    }

    void rotate_left(AVL_node<T>* rot_node){   // 该节点作为根的子树的变化：该节点的右子节点变成新的根，自己变成左子节点
        AVL_node<T>* cur = root;
        AVL_node<T>* parent = nullptr;   // 旋转节点的父节点，之后要当新根的父节点
        while(cur){
            if(rot_node->val == cur->val) break;
            parent = cur;
            if(rot_node->val < cur->val) cur = cur->left;
            else if(rot_node->val > cur->val) cur = cur->right;
        }
        if(!cur) return;

        AVL_node<T>* new_root = rot_node->right;
        if(!new_root) return;
        rot_node->right = new_root->left;
        new_root->left = rot_node;

        // 更新高度（必须先更新 rot_node，再更新 new_root）
        updateHeight(rot_node);
        updateHeight(new_root);

        if(!parent) root = new_root;  // 当旋转节点为根节点root时，要更新该属性
        else if(parent->left == rot_node) parent->left = new_root;
        else parent->right = new_root;
    }

    void rotate_right(AVL_node<T>* rot_node){  // 该节点作为根的子树的变化：该节点的左子节点变成新的根，自己变成右子节点
        AVL_node<T>* cur = root;
        AVL_node<T>* parent = nullptr;   // 旋转节点的父节点，之后要当新根的父节点
        while(cur){
            if(rot_node->val == cur->val) break;
            parent = cur;
            if(rot_node->val < cur->val) cur = cur->left;
            else if(rot_node->val > cur->val) cur = cur->right;
        }
        if(!cur) return;

        AVL_node<T>* new_root = rot_node->left;
        if(!new_root) return;
        rot_node->left = new_root->right;
        new_root->right = rot_node;

        // 更新高度（必须先更新 rot_node，再更新 new_root）
        updateHeight(rot_node);
        updateHeight(new_root);

        if(!parent) root = new_root;  // 当旋转节点为根节点root时，要更新该属性
        else if(parent->left == rot_node) parent->left = new_root;
        else parent->right = new_root;
    }

    void rebalance1(vector<AVL_node<T>*> v){  // 传入的是插入操作经历的路径，在这条路径上找最靠下的非平衡节点
        if(v.empty()) return;

        for(int i=v.size()-1; i>=0; --i){  // 不能用size_t，否则系统默认i可能取到负数从而产生冲突（实际上传入的path一定至少有一个元素root）
            AVL_node<T>* node = v[i];
            updateHeight(node);
            int balance = balanceCal(node);
            // 左子树高，左重则右旋
            if(balance > 1){
                AVL_node<T>* l_child = node->left;
                int l_balance = balanceCal(l_child);  // 当前节点node左子树的平衡因子
                if(l_balance < 0) rotate_left(l_child);  // 左子树右重先左旋
                rotate_right(node);
                break;
            }
            // 右子树高，右重则左旋
            else if(balance < -1){
                AVL_node<T>* r_child = node->right;
                int r_balance = balanceCal(r_child);  // 当前节点node右子树的平衡因子
                if(r_balance > 0) rotate_right(r_child);  // 右子树左重先右旋
                rotate_left(node);
                break;
            }
            else continue;
        }
    }

    void rebalance2(vector<AVL_node<T>*> v){  // 传入的是删除操作经历的路径，在这条路径上不断使不平衡的位置平衡
        if (v.empty()) return;

        for(int i=v.size()-1; i>=0; --i){  // 不能用size_t，否则系统默认i可能取到负数从而产生冲突（实际上传入的path一定至少有一个元素root）
            AVL_node<T>* node = v[i];
            updateHeight(node);
            int balance = balanceCal(node);
            bool rotated = false;
            // 左子树高，左重则右旋
            if(balance > 1){
                AVL_node<T>* l_child = node->left;
                int l_balance = balanceCal(l_child);  // 当前节点node左子树的平衡因子
                if(l_balance < 0) rotate_left(l_child);  // 左子树右重先左旋
                rotate_right(node);
                rotated = true;
            }
            // 右子树高，右重则左旋
            else if(balance < -1){
                AVL_node<T>* r_child = node->right;
                int r_balance = balanceCal(r_child);  // 当前节点node右子树的平衡因子
                if(r_balance > 0) rotate_right(r_child);  // 右子树左重先右旋
                rotate_left(node);
                rotated = true;
            }

            // 决定是否继续向上
            if(rotated) continue;     // 删除场景：旋转后子树高度可能降低，必须继续向上检查
            else{                     // 未旋转：根据平衡因子判断高度是否变化
                if(balance == 0) continue;   // 高度降低（原来可能是±1），继续向上
                else break;                  // balance == ±1，高度未变，可以停止
            }
        }
    }

    void inorderRec(AVL_node<T>* node){
        if(!node) return;

        inorderRec(node->left);
        cout << node->val << " ";
        inorderRec(node->right);
    }

    void preorderRec(AVL_node<T>* node){
        if(!node) return;

        cout << node->val << " ";
        preorderRec(node->left);
        preorderRec(node->right);
    }

    void postorderRec(AVL_node<T>* node){
        if(!node) return;

        postorderRec(node->left);
        postorderRec(node->right);
        cout << node->val << " ";
    }

    size_t heightRec(AVL_node<T>* node) const{
        if(!node) return 0;
        return 1 + max(heightRec(node->left), heightRec(node->right));
    }

    bool isSubtreeRec(AVL_node<T>* node1, AVL_node<T>* node2) const{
        if(!node1) return false;   // 空树不能包含非空子树
        if(isEqualRec(node1, node2)) return true;  // 当前根节点开始匹配
        // 否则在左右子树中继续查找
        return isSubtreeRec(node1->left, node2) || isSubtreeRec(node1->right, node2);
    }


public:
    explicit AVL(int cnt=0, AVL_node<T>* r=nullptr): cnt(cnt), root(r){
    }

    ~AVL(){
        destroy_tree(root);
        root = nullptr;
    }

    AVL(const AVL& avl){
        this->root = copy_tree(avl.root);
        this->cnt = avl.cnt;
    }

    AVL& operator=(const AVL& avl){
        if(this == &avl) return *this;

        if(root){
            destroy_tree(root);
            root = nullptr;
        }
        this->root = copy_tree(avl.root);
        this->cnt = avl.cnt;
        return *this;
    }

    bool operator==(const AVL& avl) const{
        if(this->cnt != avl.cnt) return false;
        return isEqualRec(root, avl.root);
    }

    void insert(T val){
        if(!root){
            root = new AVL_node<T>(val);
            cnt++;
            return;
        }

        vector<AVL_node<T>*> path;  // 查找插入位置时记录路径
        AVL_node<T>* cur = root;
        AVL_node<T>* parent = nullptr;
        while(cur){
            path.emplace_back(cur);
            parent = cur;
            if(val < cur->val) cur = cur->left;
            else if(val > cur->val) cur = cur->right;
            else return;                                // 存在该值则不插入
        }
        AVL_node<T>* new_node = new AVL_node<T>(val);
        if(val < parent->val) parent->left = new_node;
        else if(val > parent->val) parent->right = new_node;
        cnt++;

        // 自底向上调整
        rebalance1(path);
    }

    void remove(T val){
        if(!root) return;

        vector<AVL_node<T>*> path;  // 查找删除位置时记录路径
        AVL_node<T>* to_remove = root;
        AVL_node<T>* parent = nullptr;   // 删除节点的父节点
        while(to_remove){
            if(val == to_remove->val) break;
            path.emplace_back(to_remove);
            parent = to_remove;
            if(val < to_remove->val) to_remove = to_remove->left;
            else if(val > to_remove->val) to_remove = to_remove->right;
        }

        // 未找到
        if(!to_remove) return;

        // 情况1 & 2：目标节点至多有一个子节点
        if(!to_remove->left || !to_remove->right){
            AVL_node<T>* child = to_remove->left? to_remove->left: to_remove->right;
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
        }
        // 情况3：目标节点有两个子节点 (先寻找后继（右子树中的最小节点）及其父节点，完成空缺的填充，再把用于填补的节点删除)
        else{
            path.emplace_back(to_remove);

            AVL_node<T>* succ_parent = to_remove;
            AVL_node<T>* succ = to_remove->right;
            while(succ->left){
                path.emplace_back(succ);
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

        // 自底向上调整
        rebalance2(path);
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

        Queue<AVL_node<T>*> q(cnt);
        q.push(root);
        while(!q.empty()){
            AVL_node<T>* cur = q.get_front();
            q.pop();
            cout << cur->val << " ";
            if(cur->left) q.push(cur->left);
            if(cur->right) q.push(cur->right);
        }
        cout << endl;
    }

    [[nodiscard]] AVL_node<T>* find(T key) const{
        AVL_node<T>* cur = root;
        while(cur && key != cur->val){
            if(key < cur->val) cur = cur->left;
            else cur = cur->right;
        }
        return cur;
    }

    [[nodiscard]] AVL_node<T>* get_root() const{
        return root;
    }

    [[nodiscard]] size_t get_size() const{
        return cnt;
    }

    [[nodiscard]] size_t get_height() const{
        return heightRec(root);
    }

    [[nodiscard]] bool is_subtree(const AVL& other) const {
        if(!other.root) return true;  // 空树被认为是任何树的子树（通常定义）
        if(!root) return false;       // 当前树为空，不能包含非空子树
        return isSubtreeRec(root, other.root);
    }

    [[nodiscard]] bool is_full() const{
        return cnt == static_cast<size_t>(pow(2, get_height()) - 1);
    }

    [[nodiscard]] bool is_complete() const{
        if(!root) return true;
        Queue<AVL_node<T>*> q(cnt);
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

int main(){
    AVL<int> tree1;
    cout << "初始树大小: " << tree1.get_size() << endl;  // 0
    cout << "初始树高度: " << tree1.get_height() << endl;  // 0
    cout << "是否为空树: " << bool_str(tree1.get_root() == nullptr) << endl;  // true

    int values[] = {5, 3, 7, 2, 4, 6, 8};
    for(int v: values) tree1.insert(v);
    cout << "大小: " << tree1.get_size() << endl;  // 7
    cout << "高度: " << tree1.get_height() << endl;  // 3

    // 遍历输出
    cout << "前序遍历: "; tree1.preorder();
    cout << "中序遍历: "; tree1.inorder();
    cout << "后序遍历: "; tree1.postorder();
    cout << "层序遍历: "; tree1.levelOrder();

    // 判断树性质
    cout << "是否为满二叉树: " << bool_str(tree1.is_full()) << endl;  // true
    cout << "是否为完全二叉树: " << bool_str(tree1.is_complete()) << endl;  // true

    // 测试插入重复值（不应增加节点）
    tree1.insert(5);
    cout << "插入重复值5后，大小: " << tree1.get_size() << endl;  // 7
    cout << "层序遍历（应无变化）: "; tree1.levelOrder();

    // 拷贝构造（测试拷贝）
    AVL<int> tree2(tree1);
    cout << "拷贝构造 tree2 (tree1的拷贝):" << endl;
    cout << "tree2 中序遍历: "; tree2.inorder();

    // 测试相等性
    cout << "tree1 == tree2? " << bool_str(tree1 == tree2) << endl;  // false
    AVL<int> tree3;
    tree3.insert(5);
    tree3.insert(3);
    tree3.insert(7);
    tree3.insert(2);
    tree3.insert(4);
    tree3.insert(6);
    tree3.insert(8);
    cout << "tree2 == tree3? " << bool_str(tree2 == tree3) << endl;  // true

    // 测试子树
    AVL<int> subtree;
    subtree.insert(3);
    subtree.insert(2);
    subtree.insert(4);
    cout << "tree2 是否包含子树 {3,2,4} : " << bool_str(tree2.is_subtree(subtree)) << endl; // true
    AVL<int> not_sub;
    not_sub.insert(10);
    cout << "tree2 是否包含子树 {10} : " << bool_str(tree2.is_subtree(not_sub)) << endl; // false
    cout << endl;

    // 测试赋值运算符
    AVL<int> tree4;
    tree4 = tree2;
    cout << "tree4 (赋值自tree2) 中序遍历: "; tree4.inorder();
    cout << "tree4 == tree2? " << bool_str(tree4 == tree2) << endl;  // true

    // 测试删除
    tree2.remove(3);   // 删除带两个子节点的节点
    tree2.remove(7);
    cout << "tree2 删除3后中序: "; tree2.inorder();   // 应输出 2 4 5 6 8
    cout << "大小: " << tree2.get_size() << endl; // 5
}