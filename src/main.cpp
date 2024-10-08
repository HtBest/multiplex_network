#include "graph.hpp"
#include "utils.hpp"
mt19937 rnd(281116);
enum S1 : int
{
    S = 0,
    I = 1,
};
enum S2 : int
{
    U = 0,
    A = 1,
};
bool sample(double p)
{
    return rnd() % 1000000 < p * 1000000;
}
struct Model
{
    struct Node
    {
        S1 state1;
        S2 state2;
    };
    double betau, betaa, mu, lambda, theta;
    vector<Node> nodes;
    Model(int n, double betau, double betaa, double mu, double lambda, double theta, double initIRate) : betau(betau), betaa(betaa), mu(mu), lambda(lambda), theta(theta)
    {
        nodes.resize(n);
        for (int i = 0; i < n; ++i)
        {
            nodes[i].state1 = i < n * initIRate ? S1::I : S1::S;
            nodes[i].state2 = S2::U;
        }
        for (int i = 0; i < n; ++i)
        {
            swap(nodes[i], nodes[rnd() % n]);
        }
    }
};
int main(int argc, char *argv[])
{
    int n = 1e4;
    auto dis = sampleDistribution(n, 3, 10);

    double sum = 0;
    vector<vector<double>> dis2d(dis.size(), vector<double>(dis.size())), dis2d1(n, vector<double>(n)), dis2d2(n, vector<double>(n));
    for (int i = 0; i < dis.size(); ++i)
    {
        for (int j = i; j < dis2d.size(); ++j)
        {
            dis2d[i][j] = dis[i] * dis[j];
            sum += dis2d[i][j];
        }
    }
    for (int i = 0; i < dis.size(); ++i)
    {
        for (int j = i; j < dis2d.size(); ++j)
        {
            dis2d[i][j] /= sum;
        }
    }
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

    // validate(dis2d1, dis2d);
    vector<tuple<int, int, double>> input;

    for (int i = 0; i < dis2d.size(); ++i)
    {
        for (int j = i; j < dis2d.size(); ++j)
        {
            if (i != j)
                dis2d[i][j] += dis2d[j][i], dis2d[j][i] = 0;
            if (dis2d[i][j])
                input.push_back({i, j, dis2d[i][j]});
        }
    }
    auto [g1, g2] = generate(n, 2, input, input);
    n = g1.n;
    Model model(n, 0.1, 0.1, 0.1, 0.1, 0.1, 0.001);
    int T = 100;
    for (int t = 0; t < T; ++t)
    {
        vector<Model::Node> nodes = model.nodes;
        for (int i = 0; i < n; ++i)
        {
            if (nodes[i].state1 == S1::I)
            {
                if (sample(model.mu))
                {
                    model.nodes[i].state1 = S1::S;
                }
            }
            else
            {
                for (int j = g1.head[i]; j; j = g1.edge[j].next)
                {
                    int b = g1.edge[j].b;
                    if (nodes[b].state1 == S1::I && sample(nodes[b].state2 == S2::U ? model.betau : model.betaa))
                    {
                        model.nodes[i].state1 = S1::I;
                        break;
                    }
                }
            }
            if (nodes[i].state2 == S2::A)
            {
                if (sample(model.theta))
                {
                    model.nodes[i].state2 = S2::U;
                }
            }
            else
            {
                for (int j = g2.head[i]; j; j = g2.edge[j].next)
                {
                    int b = g1.edge[j].b;
                    if (nodes[b].state2 == S2::A && sample(model.lambda))
                    {
                        model.nodes[i].state2 = S2::A;
                        break;
                    }
                }
            }
        }
        int cnt = 0;
        for (int i = 0; i < n; ++i)
        {
            if (model.nodes[i].state1 == S1::I)
            {
                cnt++;
            }
        }
        cout << cnt << endl;
    }
    return 0;
}
