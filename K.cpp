#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
using namespace std;

int32_t const N = 500000,
L = static_cast<int32_t>(log2(N)) + 1;

vector<int32_t> parent(N, 0);
vector<int32_t> depth(N, 0);
vector<vector<int32_t>> lift(N);


int32_t lca(int32_t u, int32_t v) {
    if (depth[u] != depth[v]) {
        if (depth[u] < depth[v]) {
            swap(u, v);
        }
        int32_t dh = depth[u] - depth[v];
        for (int32_t i = L - 1; i >= 0; --i) {
            if ((1 << i) <= dh) {
                dh -= (1 << i);
                u = lift[u][i];
            }
        }
    }
    if (u == v) {
        return u;
    }
    for (int32_t i = L - 1; i >= 0; --i) {
        if (lift[u][i] != lift[v][i]) {
            u = lift[u][i];
            v = lift[v][i];
        }
    }
    return parent[u];
}

void link(int32_t p, int32_t c) {
    parent[c] = p;
    depth[c] = depth[p] + 1;
    lift[c] = vector<int32_t>(L, 0);
    lift[c][0] = p;
    for (size_t i = 1; i < lift[c].size(); ++i) {
        lift[c][i] = lift[lift[c][i - 1]][i - 1];
    }
}

int main() {
    parent[0] = 0;
    depth[0] = 0;
    lift[0] = vector<int32_t>(L, 0);

    string op;
    int32_t k, a, b;
    cin >> k;
    while (k--) {
        cin >> op >> a >> b;
        if (op == "ADD") {
            link(a - 1, b - 1);
        } else if (op == "GET") {
            cout << lca(a - 1, b - 1) + 1 << "\n";
        }
    }
}