//
// Created by 24394 on 2026/8/7.
//
#include <iostream>

class UnionFind{
private:
    int size;      // 元素个数
    int* parent;   // 父节点数组
    int* rank;     // 秩（树的高度）

public:
    // 初始化 n 个元素（编号 0 ~ n-1）
    explicit UnionFind(int n): size(n){
        parent = new int[n];  // 每个节点的父节点指向-1
        rank = new int[n]{0};  // 初始秩为 0
        for(int i=0; i<n; ++i) {
            parent[i] = -1;      // 每个节点都是根
        }
    }

    ~UnionFind() {
        delete[] parent;
        delete[] rank;
        parent = nullptr;
        rank = nullptr;
    }

    // 查找根节点（带路径压缩，递归版本）
    int find(int x){
        if(parent[x] == -1) return x;
        return parent[x] = find(parent[x]);   // 递归压缩
    }

    // 查找根节点（带路径压缩，迭代版本，防止递归过深）
    int find_iter(int x){
        int root = x;
        while(parent[root] != -1) root = parent[root];
        // 二次遍历，将路径上的节点直接挂到根
        while(parent[x] != -1){
            int next = parent[x];
            parent[x] = root;
            x = next;
        }
        return root;
    }

    // 合并两个元素所在的集合
    void unite(int x, int y){
        int rootX = find(x);
        int rootY = find(y);
        if (rootX == rootY) return;

        // 按秩合并：将秩小的树接到秩大的树上
        if(rank[rootX] < rank[rootY]){
            parent[rootX] = rootY;
        }
        else if(rank[rootX] > rank[rootY]){
            parent[rootY] = rootX;
        }
        else{
            // 秩相等，任意合并，并增加新根的秩
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }

    // 判断两个元素是否连通
    bool connected(int x, int y){
        return find(x) == find(y);
    }

    [[nodiscard]] int get_size() const{
        return size;
    }
};


int main() {
    UnionFind uf(10);
    uf.unite(0, 1);
    uf.unite(2, 3);
    uf.unite(1, 3);
    std::cout << std::boolalpha << uf.connected(0, 3) << std::endl;  // true
    return 0;
}