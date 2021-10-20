#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
using namespace std;

int32_t const N = 200000,
        L = static_cast<int32_t>(log2(N)) + 1;

vector<int32_t> parent(N, 0);
vector<int32_t> depth(N, 0);
vector<int32_t> price(N, 0);
vector<vector<pair<int32_t, int32_t>>> lift(N);


int32_t lca(int32_t u, int32_t v) {
    int32_t m = INT32_MAX;

    if (depth[u] != depth[v]) {
        if (depth[u] < depth[v]) {
            swap(u, v);
        }
        int32_t dh = depth[u] - depth[v];
        for (int32_t i = L - 1; i >= 0; --i) {
            if ((1 << i) <= dh) {
                dh -= (1 << i);
                m = min(m, lift[u][i].second);
                u = lift[u][i].first;
            }
        }
    }
    if (u == v) {
        return m;
    }
    for (int32_t i = L - 1; i >= 0; --i) {
        if (lift[u][i].first != lift[v][i].first) {
            m = min(m, lift[u][i].second);
            m = min(m, lift[v][i].second);
            u = lift[u][i].first;
            v = lift[v][i].first;
        }
    }
    return min(min(price[u], m), price[v]);
}

void link(int32_t p, int32_t c, int32_t pr) {
    parent[c] = p;
    depth[c] = depth[p] + 1;
    price[c] = pr;
    lift[c] = vector<pair<int32_t, int32_t>>(L, {0, 0});
    lift[c][0] = {p, pr};
    for (size_t i = 1; i < lift[c].size(); ++i) {
        lift[c][i].first = lift[lift[c][i - 1].first][i - 1].first;
        lift[c][i].second = min(lift[c][i - 1].second, lift[lift[c][i - 1].first][i - 1].second);
    }
}

int main() {
    parent[0] = 0;
    depth[0] = 0;
    price[0] = 0;
    lift[0] = vector<pair<int32_t, int32_t>>(L, {0, INT32_MAX});

    int32_t n, x, y, m;
    cin >> n;

    for (int32_t i = 1; i < n; ++i) {
        cin >> x >> y;
        link(x - 1, i, y);
    }
    cin >> m;
    for (int32_t i = 0; i < m; ++i) {
        cin >> x >> y;
        cout << lca(x - 1, y - 1) << "\n";
    }
}