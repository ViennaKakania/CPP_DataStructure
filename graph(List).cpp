//
// Created by 24394 on 2026/8/10.
//
#include <iostream>
#include "C:\Users\24394\Desktop\DataStructure_Algorithm\Queue\circleQueue(sequenceList).cpp"
#include "C:\Users\24394\Desktop\DataStructure_Algorithm\LinearList\sequenceList(dynamicArray).cpp"
#include "C:\Users\24394\Desktop\DataStructure_Algorithm\Stack\stack(sequenceList).cpp"
#include <functional>
#include <climits>
using std::cout;
using std::endl;
using std::function;

struct Edge{
    int to;       // 目标顶点在 vertices 中的索引
    int weight;   // 边的权值

    // 默认构造函数（Linear_list 需要）
    Edge(): to(-1), weight(0){}

    // 带参构造函数
    Edge(int t, int w): to(t), weight(w){}
};

template<typename T>
class Vertex{
private:
    T id;
    Linear_list<Edge> edges;  // 存边（相当于存了目标索引 + 权值）

public:
    // 新增默认构造函数（new T[cap]要求T有默认构造函数，但Vertex类没有提供，导致无法分配Vertex<T>数组）
    Vertex(): id(T()), edges(0){
    }

    explicit Vertex(const T& id): id(id), edges(0){
    }

    ~Vertex()= default;

    Vertex(const Vertex<T>& vtx): id(vtx.id), edges(vtx.edges){
    }

    Vertex& operator=(const Vertex<T>& vtx)= default;

    [[nodiscard]] const T& get_id() const{
        return id;
    }

    [[nodiscard]] const Linear_list<Edge>& get_edges() const{  // 获取从某个顶点发出的所有边
        return edges;
    }

    void build_edge(int to_idx, int weight){
        Edge new_edge(to_idx, weight);
        edges.push_back(new_edge);
    }

    bool neighbor(int idx){
        for(int i=0; i<edges.get_size(); ++i){
            if(edges.at(i).to == idx) return true;
        }
        return false;
    }
};

template<typename T>
class Graph{
private:
    Linear_list<Vertex<T>> vertices;
    bool directed;

    [[nodiscard]] int find_index(T id) const{
        for(size_t i=0; i<vertices.get_size(); ++i){
            if(vertices.at(i).get_id() == id) return static_cast<int>(i);
        }
        return -1;
    }

public:
    explicit Graph(bool is_directed=false): vertices(0), directed(is_directed){
    }

    ~Graph()= default;

    void add_vertex(const T& id){
        if(this->find_index(id) != -1){
            cout << "顶点已存在" << endl;
            return;
        }
        vertices.push_back(Vertex<T>(id));
    }

    void add_edge(const T& from, const T& to, int weight=1){
        int u = find_index(from);
        int v = find_index(to);
        if(u < 0 || v < 0){
            cout << "有顶点不存在" << endl;
            return;
        }
        vertices.at(u).build_edge(v, weight);
        if(!directed){
            vertices.at(v).build_edge(u, weight);
        }
    }

    [[nodiscard]] bool adjacent(const T& from, const T& to) const{  // 判断两个顶点之间是否有边
        int x = find_index(from);
        int y = find_index(to);
        if(x == -1 || y == -1){
            cout << "顶点不存在" << endl;
            return false;
        }

        const auto& nbr = vertices.at(x).get_edges();
        for (int i=0; i<nbr.get_size(); ++i){
            if(nbr.at(i).to == y) return true;
        }
        return false;
    }

    [[nodiscard]] const auto& get_vertices() const{
        return vertices;
    }

    // 注意传入的参数是id
    void get_neighbor(const T& vtx){
        int idx = find_index(vtx);
        if(find_index(vtx) == -1){
            cout << "顶点不存在" << endl;
            return;
        }

        const Linear_list<Edge>& nbr = vertices.at(idx).get_edges();
        cout << vtx << "的邻顶点：";
        for(int i=0; i<nbr.get_size(); ++i) cout << vertices.at(nbr.at(i).to).get_id() << " ";
        cout << endl;
    }

    // 邻接表的打印是依据顶点，依次打印每个顶点及其相连的边
    void printList() const{
        for(int i=0; i<vertices.get_size(); ++i){
            const Vertex<T>& vex = vertices.at(i);
            cout << "" << vex.get_id() << ": ";
            const Linear_list<Edge>& nbr = vex.get_edges();
            for(size_t j=0; j<nbr.get_size(); ++j){
                int idx = nbr.at(j).to;
                cout << vertices.at(idx).get_id() << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    void DFS(const T& start_id){
        int start_idx = find_index(start_id);
        if(start_idx == -1){
            cout << "顶点不存在" << endl;
            return;
        }

        size_t n = vertices.get_size();
        int* visited = new int[n];
        for(size_t i=0; i<n; ++i) visited[i] = 0;

        cout << "从" << start_id << "开始的深度优先搜索: ";
        function<void(int)> dfsRec = [&](int idx){        // 使用function包装是为了实现递归，而用[&]是因为要读写lambda函数外的visited数组
            cout << vertices.at(idx).get_id() << " ";
            visited[idx] = 1;
            const Linear_list<Edge>& nbr = vertices.at(idx).get_edges();
            for(size_t j=0; j<nbr.get_size(); ++j){
                if(visited[nbr.at(j).to] == 0){
                    int nxt_idx = nbr.at(j).to;
                    dfsRec(nxt_idx);
                }
            }
        };
        dfsRec(start_idx);
        delete []visited;
        cout << endl;
    }

    void BFS(const T& start_id){
        int start_idx = find_index(start_id);
        if(start_idx == -1){
            cout << "顶点不存在" << endl;
            return;
        }

        size_t n = vertices.get_size();
        int* visited = new int[n];
        for(size_t i=0; i<n; ++i) visited[i] = 0;

        cout << "从" << start_id << "开始的广度优先搜索: ";
        Queue<int> q(static_cast<int>(n));
        q.push(start_idx);
        visited[start_idx] = 1;
        while(!q.empty()){
            int cur_idx = q.get_front();
            cout << vertices.at(cur_idx).get_id() << " ";
            q.pop();

            const Linear_list<Edge>& nbr = vertices.at(cur_idx).get_edges();
            for(size_t i=0; i<nbr.get_size(); ++i){
                int nxt_idx = nbr.at(i).to;
                if(visited[nxt_idx] == 0) {
                    visited[nxt_idx] = 1;
                    q.push(nxt_idx);
                }
            }
        }
        cout << endl;
        delete []visited;
    }
};

// 求顶点u到其它顶点的最短路径（无向图）
// 只适用于非加权图
template<typename T>
int* BFS_min_distance(const Graph<T>& g, int start){   // 顶点参数可考虑用id而不是vertices中的索引，但是需要把find_index改成public
    int n = g.get_vertices().get_size();
    int* d = new int[n];
    int* visited = new int[n];
    for(int i=0; i<n; ++i){
        d[i] = -1;
        visited[i] = 0;
    }
    d[start] = 0;

    Queue<int> q(n);
    q.push(start);
    while(!q.empty()){
        int cur_idx = q.get_front();
        q.pop();

        const Linear_list<Edge>& nbr = g.get_vertices().at(cur_idx).get_edges();
        for(size_t i=0; i<nbr.get_size(); ++i){
            int nxt_idx = nbr.at(i).to;
            if(visited[nxt_idx] == 0){
                visited[nxt_idx] = 1;
                d[nxt_idx] = d[cur_idx] + 1;
                q.push(nxt_idx);
            }
        }
    }
    delete[] visited;
    return d;
}

// 求顶点start到其它顶点的最短路径
// 可适用于加权图（不包含负权）
template<typename T>
int* Dijkstra_min_distance(const Graph<T>& g, int start){
    int n = g.get_vertices().get_size();
    int* final = new int[n];  // 记录各顶点是否已经找到最短路径
    int* d = new int[n];  // 记录到每个顶点的最短路径长度
    int* pre = new int[n]; // 记录每个顶点最短路径上的上一个经过的顶点

    for(int i=0; i<n; ++i){
        final[i] = 0;
        d[i] = INT_MAX;
        pre[i] = -1;
    }
    final[start] = 1;
    d[start] = 0;

    const Linear_list<Edge>& nbr = g.get_vertices().at(start).get_edges();  // 对start的邻顶点做处理
    for(int i=0; i<nbr.get_size(); ++i){
        d[nbr.at(i).to] = nbr.at(i).weight;
        pre[nbr.at(i).to] = 0;
    }

    for(int cnt=0; cnt<n-1; ++cnt){
        // 先找到未访问的顶点中距离最小的顶点（具体来说应该说是选择已到达过且未标记访问的顶点中d最小的）
        int from_idx = -1;
        int min_d = INT_MAX;
        for(int i=0; i<n; ++i){
            if(final[i] == 0 && d[i] < min_d){
                from_idx = i;
                min_d = d[i];
            }
        }
        if(from_idx == -1) break;

        final[from_idx] = 1;  // 某个顶点标记已访问，是在选中它作为当前起始顶点，在离开它去往下一个顶点时标记

        // 遍历顶点from的所有出边（即遍历矩阵第from行），从而实现依次到顶点to的路径缩短
        const Linear_list<Edge>& cur_nbr = g.get_vertices().at(from_idx).get_edges();
        for(int i=0; i<cur_nbr.get_size(); ++i){
            int to_idx = cur_nbr.at(i).to;
            int weight = cur_nbr.at(i).weight;
            if(final[to_idx] == 0 && d[from_idx] + weight < d[to_idx]){
                d[to_idx] = d[from_idx] + weight;
                pre[to_idx] = from_idx;  // 记录前驱
            }
        }
    }

    delete[] final;
    delete[] pre;

    return d;
}

template<typename T>
int** Floyd_min_distance(const Graph<T>& g){
    int n = g.get_vertices().get_size();
    // 初始化d矩阵
    int** d = new int*[n];
    for(int i=0; i<n; ++i){
        d[i] = new int[n];
        for(int j=0; j<n; ++j){
            d[i][j] = i == j? 0: INT_MAX;
        }
    }

    for(int from_idx=0; from_idx<n; ++from_idx){  // 遍历各顶点，记录与之相连的顶点之间的距离
        const auto& nbr = g.get_vertices().at(from_idx).get_edges();
        for(int i=0; i<nbr.get_size(); ++i){
            int to_idx = nbr.at(i).to;
            d[from_idx][to_idx] = nbr.at(i).weight;
        }
    }

    // k枚举中转点，以k为中转点，更新i到j的最短路径
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

template<typename T>
auto topological_sort(const Graph<T>& g){
    int n = g.get_vertices().get_size();
    // 计算入度
    std::vector<int> indeg(n);  // 建议用 vector，或者 new int[n] 手动管理
    for(int i=0; i<n; ++i) {
        const auto& nbr = g.get_vertices().at(i).get_edges();
        for(int j=0; j<nbr.get_size(); ++j){
            indeg[nbr.at(j).to]++;
        }
    }

    Stack<int> stk(n);
    for(int i=0; i<n; ++i){
        if(indeg[i] == 0){
            stk.push(i);
        }
    }

    Linear_list<int> sorted(n);
    int cnt = 0;
    while(!stk.empty()){
        int top_idx = stk.get_top();
        sorted.push_back(g.get_vertices().at(top_idx).get_id());
        stk.pop();
        cnt++;

        const auto& nbr = g.get_vertices().at(top_idx).get_edges();
        for(int i=0; i<nbr.get_size(); ++i){
            int to_idx = nbr.at(i).to;
            if(--indeg[to_idx] == 0){
                stk.push(to_idx);
            }
        }
    }

    if(cnt < n) return Linear_list<T>(0);
    return sorted;
}

template<typename T>
Linear_list<T> dfs_reverse_topological(const Graph<T>& g){
    int n = g.get_vertices().get_size();
    int* visited = new int[n]();  // 初始化为0
    Stack<int> stk(n);

    function<void(int)> dfs = [&](int idx){
        visited[idx] = 1;
        const auto& nbr = g.get_vertices.at(idx).get_edges();
        for(int i=0; i<nbr.get_size(); ++i){
            int nxt_idx = nbr.at(i).to;
            if(!visited[nxt_idx]) dfs(nxt_idx);
        }
        stk.push(idx); // 递归结束后再将当前顶点入栈（后序）
    };

    // 对所有未访问顶点执行 DFS
    for(int i=0; i<n; ++i){
        if(!visited[i]) dfs(i);
    }

    // 从栈中弹出，就是逆拓扑顺序（终点先弹）
    Linear_list<T> sorted(0);
    while(!stk.empty()){
        int idx = stk.get_top();
        stk.pop();
        sorted.push_back(g.get_vertices().at(idx).get_id());
    }

    delete[] visited;
    return sorted;
}


int main(){
    // 整数 ID 的无向图
    Graph<int> g1(true);  // 无向图

    // 添加顶点 0~4
    g1.add_vertex(0);
    g1.add_vertex(1);
    g1.add_vertex(2);
    g1.add_vertex(3);
    g1.add_vertex(4);

    // 添加边（无向）
    g1.add_edge(0, 1);
    g1.add_edge(0, 4);
    g1.add_edge(1, 2);
    g1.add_edge(1, 3);
    g1.add_edge(1, 4);
    g1.add_edge(2, 3);
    g1.add_edge(3, 4);

    auto sorted = topological_sort(g1);
    for(int i=0; i<5; ++i) cout << sorted.at(i) << " ";
    cout << endl;

    int* arr = BFS_min_distance(g1, 0);
    for(int i=0; i<5; ++i) cout << arr[i] << " ";
    cout << endl;
    delete[] arr;

    cout << "邻接表:" << endl;
    g1.printList();
    g1.BFS(0);
    g1.DFS(0);

    cout << g1.adjacent(2, 4) << endl;
    g1.get_neighbor(0);

    // 测试重复顶点和无效边
    g1.add_vertex(0);           // “顶点已存在”
    g1.add_edge(0, 9);          // “有顶点不存在”
    cout << endl;

    // 字符 ID 的有向图
    Graph<char> g2(true);  // 有向图

    // 添加顶点 'A' ~ 'C'
    g2.add_vertex('A');
    g2.add_vertex('B');
    g2.add_vertex('C');

    // 添加有向边 A->B, B->C, C->A
    g2.add_edge('A', 'B');
    g2.add_edge('B', 'C');
    g2.add_edge('C', 'A');

    cout << "邻接表（有向）:" << endl;
    g2.printList();
    g2.BFS('A');
    g2.BFS('X'); // 测试从不存在顶点出发
    g2.DFS('A');
    g2.DFS('X'); // 测试从不存在顶点出发

    // 复制构造和赋值
    Graph<int> g3 = g1;          // 调用拷贝构造
    cout << "g3：" << endl;
    g3.printList();

    Graph<int> g4;
    g4 = g1;                     // 调用赋值运算符
    cout << "g4：" << endl;
    g4.printList();
}