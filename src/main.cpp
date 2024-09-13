#include "graph.hpp"
#include "utils.hpp"
mt19937 rnd(281116);
bool validate(vector<vector<double>> a, vector<vector<double>> b)
{
    assert(a.size() == b.size());
    assert(a[0].size() == b[0].size());
    for (int i = a.size() - 1; i >= 0; --i)
    {
        for (int j = a[i].size() - 1; j >= 0; --j)
        {
            if (i < a.size() - 1)
                a[i][j] += a[i + 1][j], b[i][j] += b[i + 1][j];
            if (j < a[i].size() - 1)
                a[i][j] += a[i][j + 1], b[i][j] += b[i][j + 1];
            if (i < a.size() - 1 && j < a[i].size() - 1)
                a[i][j] -= a[i + 1][j + 1], b[i][j] -= b[i + 1][j + 1];
            if (a[i][j] + 1e-6 < b[i][j])
                return false;
        }
    }
    printf("Validation passed\n");
    return true;
}
int main(int argc, char *argv[])
{
    constexpr int n = 1000;
    auto dis = sampleDistribution(n, 3, 10);
    for (auto x : dis)
    {
        cout << x << " ";
    }
    cout << endl;

    double sum = 0;
    vector<vector<double>> dis2d(n, vector<double>(n)), dis2d1(n, vector<double>(n)), dis2d2(n, vector<double>(n));
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            dis2d[i][j] = dis[i] * dis[j];
            if (i == j)
                dis2d[i][j] *= 2;
            sum += dis2d[i][j];
        }
    }
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            dis2d[i][j] /= sum;
        }
    }
    Graph g(n);
    g.generate(2, dis2d);
    Graph g1 = g, g2 = g;
    dis2d1 = dis2d;
    for (int i = 0, cnt = 0; i < n * n & cnt < 1e7; ++i, ++cnt)
    {
        int a = rnd() % n, b = rnd() % n, c = rnd() % n, d = rnd() % n;
        if (a > b)
            swap(a, b);
        if (c > d)
            swap(c, d);
        if (dis2d1[a][c] < 1.0 / (n * n))
        {
            i--;
            continue;
        }
        dis2d1[a][c] -= 1 / (n * n);
        dis2d1[b][d] += 1 / (n * n);
    }
    validate(dis2d1, dis2d);
    g1.generate(5, dis2d1);
    g2.generate(5, dis2d1);
    return 0;
}
