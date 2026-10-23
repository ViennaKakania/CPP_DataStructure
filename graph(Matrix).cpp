//
// Created by 24394 on 2026/8/10.
//
#include <iostream>
#include "C:\Users\24394\Desktop\DataStructure_Algorithm\Queue\circleQueue(sequenceList).cpp"
#include <functional>
#include <climits>
using std::cout;
using std::endl;
using std::function;

class Graph{
private:
    int vexnum;      // 顶点数
    int** matrix;    // 邻接矩阵，matrix[i][j] 表示边权（0表示无边）
    bool directed;   // 是否为有向图

public:
// 构造函数：初始化 n 个顶点的图，默认无向图
    explicit Graph(int n, bool directed=false): vexnum(n), directed(directed){
        // 分配二维数组
        matrix = new int*[vexnum];
        for(int i=0; i<vexnum; ++i){
            matrix[i] = new int[vexnum];
            // 初始化为0（无边）
            for(int j=0; j<vexnum; ++j){
                matrix[i][j] = 0;
            }
        }
    }

    ~Graph() {
        for(int i=0; i<vexnum; ++i){
            delete[] matrix[i];
        }
        delete[] matrix;
    }

    void add_edge(int u, int v, int w=1){
        if(u < 0 || u >= vexnum || v < 0 || v >= vexnum){
            cout << "Invalid vertex index!" << endl;
            return;
        }
        matrix[u][v] = w;
        if(!directed){
            matrix[v][u] = w;   // 无向图对称
        }
    }

    void remove_edge(int u, int v){
        if(u < 0 || u >= vexnum || v < 0 || v >= vexnum) return;
        matrix[u][v] = 0;
        if(!directed){
            matrix[v][u] = 0;
        }
    }

    void DFS(int start) const{
        if(start < 0 || start >= vexnum){
            cout << "无效起始顶点" << endl;
            return;
        }

        int* visited = new int[vexnum];
        for(int i=0; i<vexnum; ++i) visited[i] = 0;

        cout << "从顶点" << start << "开始的深度优先搜索: ";
        function<void(int)> dfsRec = [&](int i){
            cout << i << " ";
            visited[i] = 1;
            for(int j=0; j<vexnum; ++j){
                if(matrix[i][j] != 0 && visited[j] == 0){
                    dfsRec(j);
                }
            }
        };
        dfsRec(start);
        delete []visited;
        cout << endl;
    }

    void BFS(int start) const{
        if(start < 0 || start >= vexnum){
            cout << "无效起始顶点" << endl;
            return;
        }

        int* visited = new int[vexnum];
        for(int i=0; i<vexnum; ++i) visited[i] = 0;

        Queue<int> q(vexnum);
        visited[start] = 1;
        q.push(start);
        cout << "从顶点" << start << "开始的广度优先搜索: ";

        while(!q.empty()){
            int u = q.get_front();
            cout << u << " ";
            q.pop();
            for(int v=0; v<vexnum; ++v){
                if(matrix[u][v] != 0 && visited[v] == 0){
                    q.push(v);
                    visited[v] = 1;
                }
            }
        }
        cout << endl;
        delete []visited;
    }

    bool adjacent(int x, int y){
        return matrix[x][y] > 0;
    }

    [[nodiscard]] int get_vexnum() const{
        return vexnum;
    }

    [[nodiscard]] int** get_matrix() const{
        return matrix;
    }

    void get_neighbor(int x){
        cout << "x的邻顶点：";
        for(int i=0; i<vexnum; ++i){
            if(matrix[x][i] > 0) cout << i << " ";
        }
        cout << endl;
    }

    // 邻接矩阵打印的图是一个矩阵
    void printMatrix() const{
        cout << "邻接矩阵:" << endl;
        for(int i=0; i<vexnum; ++i){
            for(int j=0; j<vexnum; ++j){
                cout << matrix[i][j] << " ";
            }
            cout << endl;
        }
    }
};

// 求顶点u到其它顶点的最短路径（无向图）
// 只适用于非加权图
int* BFS_min_distance(const Graph& g, int start){
    int n = g.get_vexnum();
    int* d = new int[n];
    int* visited = new int[n];
    for(int i=0; i<n; ++i){
        d[i] = -1;
        visited[i] = 0;
    }
    d[start] = 0;

    int** matrix = g.get_matrix();
    Queue<int> q(n);
    q.push(start);
    while(!q.empty()){
        int cur = q.get_front();
        q.pop();

        for(int i=0; i<n; ++i){
            if(matrix[i][cur] > 0 && visited[i] == 0){
                visited[i] = 1;
                d[i] = d[cur] + 1;
                q.push(i);
            }
        }
    }
    delete[] visited;
    return d;
}

// 求顶点start到其它顶点的最短路径
// 可适用于加权图（不包含负权）
int* Dijkstra_min_distance(const Graph& g, int start){
    int n = g.get_vexnum();
    int* final = new int[n];  // 记录各顶点是否已经找到最短路径
    int* d = new int[n];  // 记录到每个顶点的最短路径长度
    int* pre = new int[n]; // 记录每个顶点最短路径上的上一个经过的顶点

    for(int i=0; i<n; ++i) final[i] = 0;
    final[start] = 1;

    for(int i=0; i<n; ++i){
        if(g.get_matrix()[start][i] > 0){
            d[i] = g.get_matrix()[start][i];
            pre[i] = start;
        }
        else{
            d[i] = INT_MAX;
            pre[i] = -1;
        }
    }
    d[start] = 0;
    pre[start] = -1;

    for(int cnt=0; cnt<n-1; ++cnt){
        // 先找到未访问的顶点中距离最小的顶点
        int from = -1;
        int min_d = INT_MAX;
        for(int i=0; i<n; ++i){
            if(final[i] == 0 && d[i] < min_d){
                from = i;
                min_d = d[i];
            }
        }
        if(from == -1) break;

        final[from] = 1;  // 某个顶点标记已访问，是在选中它作为当前起始顶点，在离开它去往下一个顶点时标记

        // 遍历顶点from的所有出边（即遍历矩阵第from行），从而实现依次到顶点to的路径缩短
        for(int to=0; to<n; ++to){
            int weight = g.get_matrix()[from][to];
            if(weight > 0 && final[to] == 0 && d[from] + weight < d[to]){
                d[to] = d[from] != INT_MAX? (d[from] + weight): INT_MAX;
                pre[to] = from;  // 记录前驱
            }
        }
    }

    delete[] final;
    delete[] pre;

    return d;
}

int** Floyd_min_distance(const Graph& g){
    int n = g.get_vexnum();
    // 动态分配距离矩阵d，并初始化
    int** d = new int*[n];
    for(int i=0; i<n; ++i){
        d[i] = new int[n];
        for(int j=0; j<n; ++j){
            if(i == j) d[i][j] = 0;
            else if(g.get_matrix()[i][j] != 0) d[i][j] = g.get_matrix()[i][j];
            else d[i][j] = INT_MAX;
        }
    }

    for(int k=0; k<n; ++k){
        for(int i=0; i<n; ++i){
            if(d[i][k] == INT_MAX) continue;
            for(int j=0; j<n; ++j){
                if(d[k][j] == INT_MAX) continue;

                if(d[i][k] + d[k][j] < d[i][j]){
                    d[i][j] = d[i][k] + d[k][j];
                }
            }
        }
    }

    return d;
}

int main(){
    Graph g(5);

    // 添加边
    g.add_edge(0, 1);
    g.add_edge(0, 4);
    g.add_edge(1, 2);
    g.add_edge(1, 3);
    g.add_edge(1, 4);
    g.add_edge(2, 3);
    g.add_edge(3, 4);

    int* arr = BFS_min_distance(g, 0);
    for(int i=0; i<5; ++i) cout << arr[i] << " ";
    cout << endl;
    delete[] arr;

    g.printMatrix();

    cout << g.adjacent(1, 4) << endl;
    g.get_neighbor(0);

    g.BFS(0);
    g.DFS(0);

    // 删除边并再次打印
    g.remove_edge(1, 0);
    cout << "\nAfter removing edge (1,4):" << endl;
    g.printMatrix();
}