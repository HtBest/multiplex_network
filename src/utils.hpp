#include "core.hpp"

vector<double> sampleDistribution(int n, int minv, int maxv)
{
    assert(maxv < 1 << 15);
    std::normal_distribution<> dis(0.5, 0.2);
    vector<double> p(n, 0);
    int M = maxv * maxv;
    for (int i = 0; i < M; ++i)
    {
        int d = dis(rnd) * (maxv - minv) + minv;
        if (d < minv || d >= maxv)
        {
            i--;
            continue;
        }
        p[d]++;
    }
    for (auto &x : p)
    {
        x /= M;
    }
    return p;
}

// template <int N>
// array<array<double, N>, N> sampleDistribution2d()
// {
//     assert(N < 1 << 15);
//     std::normal_distribution<> dis(0.5, 0.2);
//     array<array<double, N>, N> p(N, 0);
//     for (int i = 0; i < N * N; ++i)
//     {
//         int d = dis(rnd) * N;
//         if (d < 0 || d >= N)
//         {
//             i--;
//             continue;
//         }
//         p[d]++;
//     }
//     for (auto &x : p)
//     {
//         x /= m;
//     }
//     return p;
// }