#include "graph.hpp"

pair<vector<pair<int, int>>, vector<tuple<int, int, int>>> DDtoD(int n, vector<tuple<int, int, double>> DD)
{
    double sum = 0, ls = 0;
    for (auto [a, b, v] : DD)
    {
        sum += v / a + v / b;
    }
    sum = n / sum;
    vector<tuple<int, int, int>> res2;
    vector<double> ni(n, 0);
    for (auto [a, b, v] : DD)
    {
        assert(a <= b);
        ni[a] += v * sum;
        ni[b] += v * sum;
        res2.push_back({a, b, v * sum + 0.5});
    }
    int n2 = 0;
    for (int i = 1; i < n; ++i)
    {
        ni[i] = ni[i] / i + 0.5;
        n2 += ni[i];
    }
    vector<pair<int, int>> res1;
    for (int i = 1; i < n; ++i)
    {
        if (ni[i] < 1)
            continue;
        res1.push_back({i, ni[i]});
    }
    return {res1, res2};
}
void generate(int n, vector<tuple<int, int, int>> e, Graph &g, vector<int> deg)
{
    map<pair<int, int>, int> emap;
    map<int, vector<int>> deglist;
    vector<int> remain(n, 0);
    set<pair<int, int>> vis;
    for (int i = 0; i < n; ++i)
    {
        remain[i] = deg[i];
    }
    for (auto [a, b, v] : e)
    {
        emap[{a, b}] = v;
    }
    for (int i = 1; i < g.edge.size(); i += 2)
    {
        int a = g.edge[i].a, b = g.edge[i].b;
        vis.insert({a, b});
        remain[a]--;
        remain[b]--;
        if (deg[a] > deg[b])
            swap(a, b);
        emap[{deg[a], deg[b]}]--;
    }
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < remain[i]; ++j)
        {
            deglist[deg[i]].push_back(i);
        }
    }
    for (auto &list : deglist)
    {
        shuffle(list.second.begin(), list.second.end(), rnd);
    }
    for (auto [key, v] : emap)
    {
        int cnt = 0;
        auto [a, b] = key;
        for (int i = 0; i < v; ++i)
        {
            if (deglist[a].empty() || deglist[b].empty() || (a == b && deglist[a].size() < 2))
            {
                printf("Error %d %d\n", a, b);
                continue;
            }
            int x = deglist[a].back();
            deglist[a].pop_back();
            int y = deglist[b].back();
            deglist[b].pop_back();
            if (vis.count({x, y}) || vis.count({y, x}))
            {
                if (cnt > 10)
                {
                    printf("Failed to add an edge\n");
                    cnt = 0;
                    continue;
                }
                deglist[a].push_back(x);
                deglist[b].push_back(y);
                shuffle(deglist[a].begin(), deglist[a].end(), rnd);
                shuffle(deglist[b].begin(), deglist[b].end(), rnd);
                i--;
                cnt++;
                continue;
            }
            g.addEdge(x, y);
            vis.insert({x, y});
        }
    }
}
pair<Graph, Graph> generate(int n, double d, vector<tuple<int, int, double>> DD1, vector<tuple<int, int, double>> DD2)
{
    auto [d1, e1] = DDtoD(n, DD1);
    auto [d2, e2] = DDtoD(n, DD2);
    int m = d * n / 2, m1 = 0, m2 = 0;
    vector<tuple<int, int, int>> dpair;
    for (auto [d1, n1] : d1)
    {
        for (auto [d2, n2] : d2)
        {
            dpair.push_back({d1, d2, (double)n1 * n2 / n + 0.5});
            // sum += (double)n1 * n2 / n + 0.5;
        }
    }
    for (auto [a, b, v] : e1)
        m1 += v;
    for (auto [a, b, v] : e2)
        m2 += v;

    vector<tuple<int, int, int, int, int>> epair;
    for (auto [a, b, v] : e1)
    {
        for (auto [c, d, w] : e2)
        {
            // Layer 1: a---b Layer 2: c---d
            epair.push_back({a, b, c, d, (double)v * w * m / (2 * m1 * m2) + 0.5});
            // Layer 1: a---b Layer 2: d---c
            epair.push_back({a, b, d, c, (double)v * w * m / (2 * m1 * m2) + 0.5});
        }
    }
    // Graph generation
    vector<pair<int, int>> deg;
    for (auto [a, b, v] : dpair)
    {
        for (int i = 0; i < v; ++i)
            deg.push_back({a, b});
    }
    if (n < deg.size())
        n = deg.size();
    while (deg.size() < n)
    {
        deg.push_back(deg[rnd() % deg.size()]);
    }
    shuffle(deg.begin(), deg.end(), rnd);
    map<pair<int, int>, vector<int>> deglist;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < min(deg[i].first, deg[i].second); ++j)
        {
            deglist[deg[i]].push_back(i);
        }
    }
    for (auto &list : deglist)
    {
        shuffle(list.second.begin(), list.second.end(), rnd);
    }
    set<pair<int, int>> vis;
    Graph g(n);
    for (auto [a, b, c, d, v] : epair)
    {
        int cnt = 0;
        for (int i = 0; i < v; ++i)
        {
            if (deglist[{a, c}].empty() || deglist[{b, d}].empty() || (a == b && c == d && deglist[{a, c}].size() < 2))
            {
                printf("Error\n");
                continue;
            }
            int x = deglist[{a, c}].back();
            deglist[{a, c}].pop_back();
            int y = deglist[{b, d}].back();
            deglist[{b, d}].pop_back();
            if (vis.count({x, y}) || vis.count({y, x}))
            {
                if (cnt > 10)
                {
                    printf("Failed to add an edge\n");
                    cnt = 0;
                    continue;
                }
                deglist[{a, c}].push_back(x);
                deglist[{b, d}].push_back(y);
                shuffle(deglist[{a, c}].begin(), deglist[{a, c}].end(), rnd);
                shuffle(deglist[{b, d}].begin(), deglist[{b, d}].end(), rnd);
                i--;
                cnt++;
                continue;
            }
            g.addEdge(x, y);
            vis.insert({x, y});
            // printf("%d %d\n", x, y);
        }
    }
    Graph g1 = g, g2 = g;
    vector<int> deg1(n), deg2(n);
    printf("%d %d\n", deg.size(), n);
    for (int i = 0; i < deg.size(); ++i)
    {
        deg1[i] = deg[i].first;
        deg2[i] = deg[i].second;
    }
    generate(n, e1, g1, deg1);
    generate(n, e2, g2, deg2);
    // printf("Graph 1\n");
    // g1.print();
    // printf("Graph 2\n");
    // g2.print();
    return {g1, g2};
}