#include "core.hpp"

// template <int N>
struct Graph
{
    struct Edge
    {
        int a, b, next;
    };
    vector<Edge> edge;
    vector<int> head;
    int n;
    Graph(int n) : n(n)
    {
        head = vector<int>(n, 0);
        edge = vector<Edge>(1);
    }
    void addEdge(int a, int b)
    {
        edge.push_back({a, b, head[a]});
        head[a] = edge.size() - 1;
        edge.push_back({b, a, head[b]});
        head[b] = edge.size() - 1;
    }
    int degree(int a)
    {
        int e = head[a], cnt = 0;
        while (e)
        {
            cnt++;
            e = edge[e].next;
        }
        return cnt;
    }
    void print()
    {
        printf("---- %d %d ----\n", n, (int)edge.size() / 2);
        for (int i = 1; i < edge.size(); i += 2)
        {
            printf("%d %d\n", edge[i].a, edge[i].b);
        }
        // printDistribution();
    }
    void printDistribution()
    {
        vector<int> deg(n);
        for (int i = 0; i < n; ++i)
        {
            deg[i] = degree(i);
        }
        sort(deg.begin(), deg.end());
        for (int i = 0; i < n; ++i)
        {
            printf("%d%c", deg[i], " \n"[i == n - 1]);
        }
    }
};

pair<vector<pair<int, int>>, vector<tuple<int, int, int>>> DDtoD(int n, vector<tuple<int, int, double>> DD);
void generate(int n, vector<tuple<int, int, int>> e, Graph &g, vector<int> deg);
pair<Graph, Graph> generate(int n, double d, vector<tuple<int, int, double>> DD1, vector<tuple<int, int, double>> DD2);