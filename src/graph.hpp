#include "core.hpp"

// template <int N>
struct Graph
{
    struct Edge
    {
        int a, b, next, v;
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
    void generate(double d, vector<vector<double>> DD)
    {
        set<pair<int, int>> eset;
        int m = d * n / 2;
        for (int i = 1; i < edge.size(); i += 2)
        {
            if (edge[i].a > edge[i].b)
                eset.insert({edge[i].b, edge[i].a});
            else
                eset.insert({edge[i].a, edge[i].b});
        }
        for (int i = 1; i < n; ++i)
        {
            for (int j = 1; j <= i; ++j)
            {
                if (i != j)
                    DD[i][j] += DD[j][i], DD[j][i] = 0;
                DD[i][j] = int(DD[i][j] * m + 0.5);
            }
        }
        m = 0;
        for (int i = 1; i < n; ++i)
        {
            for (int j = 1; j <= i; ++j)
            {
                m += DD[i][j];
            }
        }

        vector<int> ni(n, 0), degree(n, 0), newdegree(n, 0), od(n, 0);
        for (int i = 1; i < n; ++i)
        {
            for (int j = 1; j <= i; ++j)
            {
                ni[i] += DD[i][j];
                ni[j] += DD[i][j];
            }
        }
        ni[0] = n;
        for (int i = 1; i < n; ++i)
        {
            ni[i] = (double)ni[i] / i + 0.5;
            ni[0] -= ni[i];
        }
        // for (int i = n - 2; i >= 0; --i)
        // {
        //     ni[i] += ni[i + 1];
        // }
        for (int i = 0; i < n; ++i)
        {
            od[i] = i;
            degree[i] = this->degree(i);
        }
        sort(od.begin(), od.end(), [&](int a, int b)
             { return degree[a] < degree[b]; });
        priority_queue<pair<int, int>> candidates;
        for (int i = 0; i < 10; ++i)
            printf("%d %d\n", i, ni[i]);
        for (int i = 0; i < n; ++i)
        {
            printf("%d ", degree[i]);
        }
        for (int i = 0, p = 0; i < n; ++i) // TO be improved
        {
            while (p < n && degree[od[p]] <= i)
            {
                candidates.push({i, od[p]});
                p++;
            }
            if (candidates.size() < ni[i])
            {
                perror("No solution");
                return;
            }
            for (int j = 0; j < ni[i]; ++j)
            {
                int a = candidates.top().second;
                candidates.pop();
                newdegree[a] = i;
            }
        }
        vector<deque<int>> remain(n, deque<int>());
        for (int i = 0; i < n; ++i)
        {
            for (int j = degree[i]; j < newdegree[i]; ++j)
            {
                remain[newdegree[i]].push_back(i);
            }
        }
        for (int i = 0; i < n; ++i)
        {
            shuffle(remain[i].begin(), remain[i].end(), rnd);
        }
        deque<pair<int, int>> reqs;

        for(int i=1;i<edge.size();i+=2)
        {
            int a=edge[i].a,b=edge[i].b;
            if(newdegree[a]>newdegree[b]) swap(a,b);
            DD[newdegree[a]][newdegree[b]]--;
            if(DD[newdegree[a]][newdegree[b]]<0)
            {
                fprintf(stderr,"Overused Dist %d %d %lf\n",a,b,DD[newdegree[a]][newdegree[b]]);
            }
        }
        for (int i = 1; i < n; ++i)
        {
            for (int j = 1; j <= i; ++j)
            {
                for (int k = 0; k < DD[i][j]; ++k)
                {
                    reqs.push_back({i, j});
                }
            }
        }
        shuffle(reqs.begin(), reqs.end(), rnd);
        int cnt = 0;
        for (int k = 0; k < reqs.size(); ++k)
        {
            auto [i, j] = reqs[k];
            if (remain[i].empty() || remain[j].empty() || (i == j && remain[i].size() < 2))
            {
                fprintf(stderr,"Failed to generate edge\n");
                continue;
            }
            int a = remain[i].back();
            remain[i].pop_back();
            int b = remain[j].back();
            remain[j].pop_back();
            if (a == b || eset.count({a, b}))
            {
                remain[i].push_front(a);
                remain[j].push_front(b);
                if (cnt > 10)
                {
                    cnt = 0;
                    fprintf(stderr,"Failed to generate edge\n");
                    continue;
                }
                // shuffle(remain[i].begin(), remain[i].end(), rnd);
                // shuffle(remain[j].begin(), remain[j].end(), rnd);
                k--;
                ++cnt;
                continue;
            }
            addEdge(a, b);
            eset.insert({a, b});
            // printf("Edge %d %d %d %d\n", a, b, i, j);
        }
        printf("%d %d\n", m, edge.size());
    }
};