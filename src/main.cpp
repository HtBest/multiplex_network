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
template <typename T>
struct Node
{
    T rate1, rate2;
};
template <>
struct Node<int>
{
    S1 state1;
    S2 state2;
};
template <typename T>
class Model
{
public:
    double betau, betaa, mu, lambda, delta;
    vector<Node<T>> nodes;
    Model(int n, double betau, double betaa, double mu, double lambda, double delta, double initIRate, double initARate) : betau(betau), betaa(betaa), mu(mu), lambda(lambda), delta(delta)
    {
        nodes.resize(n);
        for (int i = 0; i < n; ++i)
            nodes[i].rate1 = i < n * initIRate ? 1 : 0;
        for (int i = 0; i < n; ++i)
            swap(nodes[i], nodes[rnd() % n]);
        for (int i = 0; i < n; ++i)
            nodes[i].rate2 = i < n * initARate ? 1 : 0;
        for (int i = 0; i < n; ++i)
            swap(nodes[i], nodes[rnd() % n]);
    }
    void next(const Graph &g1, const Graph &g2)
    {
        int n = this->nodes.size();
        vector<Node<T>> nodes = this->nodes;
        for (int i = 0; i < n; ++i)
        {
            if (nodes[i].rate1 == S1::I)
            {
                if (sample(this->mu))
                    this->nodes[i].state1 = S1::S;
            }
            else
            {
                for (int j = g1.head[i]; j; j = g1.edge[j].next)
                {
                    int b = g1.edge[j].b;
                    if (nodes[b].state1 == S1::I && sample(nodes[b].state2 == S2::U ? this->betau : this->betaa))
                    {
                        this->nodes[i].state1 = S1::I;
                        break;
                    }
                }
            }
            if (nodes[i].state2 == S2::A)
            {
                if (sample(this->delta))
                    this->nodes[i].state2 = S2::U;
            }
            else
            {
                for (int j = g2.head[i]; j; j = g2.edge[j].next)
                {
                    int b = g1.edge[j].b;
                    if (nodes[b].state2 == S2::A && sample(this->lambda))
                    {
                        this->nodes[i].state2 = S2::A;
                        break;
                    }
                }
            }
        }
    }
};
template <>
Model<int>::Model(int n, double betau, double betaa, double mu, double lambda, double delta, double initIRate, double initARate) : betau(betau), betaa(betaa), mu(mu), lambda(lambda), delta(delta)
{
    nodes.resize(n);
    for (int i = 0; i < n; ++i)
        nodes[i].state1 = i < n * initIRate ? S1::I : S1::S;
    for (int i = 0; i < n; ++i)
        swap(nodes[i], nodes[rnd() % n]);
    for (int i = 0; i < n; ++i)
        nodes[i].state2 = i < n * initARate ? S2::A : S2::U;
    for (int i = 0; i < n; ++i)
        swap(nodes[i], nodes[rnd() % n]);
}
template <>
void Model<int>::next(const Graph &g1, const Graph &g2)
{
    int n = this->nodes.size();
    vector<Node<int>> nodes = this->nodes;
    for (int i = 0; i < n; ++i)
    {
        if (nodes[i].state1 == S1::I)
        {
            if (sample(this->mu))
                this->nodes[i].state1 = S1::S;
        }
        else
        {
            for (int j = g1.head[i]; j; j = g1.edge[j].next)
            {
                int b = g1.edge[j].b;
                if (nodes[b].state1 == S1::I && sample(nodes[b].state2 == S2::U ? this->betau : this->betaa))
                {
                    this->nodes[i].state1 = S1::I;
                    break;
                }
            }
        }
        if (nodes[i].state2 == S2::A)
        {
            if (sample(this->delta))
                this->nodes[i].state2 = S2::U;
        }
        else
        {
            for (int j = g2.head[i]; j; j = g2.edge[j].next)
            {
                int b = g1.edge[j].b;
                if (nodes[b].state2 == S2::A && sample(this->lambda))
                {
                    this->nodes[i].state2 = S2::A;
                    break;
                }
            }
        }
    }
}
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
    Model<int> model(n, 0.15, 0.1, 0.1, 0.1, 0.1, 0.001, 0.001);
    int T = 100;
    for (int t = 0; t < T; ++t)
    {
        int cnt = 0;
        model.next(g1, g2);
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
