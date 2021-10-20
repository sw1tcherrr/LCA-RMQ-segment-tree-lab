#include <iostream>
#include <vector>

using namespace std;

int32_t const N = 100000;
int32_t const LOG = 19;

int32_t depth[N];
int32_t parent[N];
int32_t lift[N][LOG];
vector<int32_t> edges[N];
int32_t root;
int32_t T = 0;
int32_t L[N];
int32_t R[N];

void dfs(int32_t v, int32_t par) {
    parent[v] = par;
    depth[v] = depth[par] + 1;
    lift[v][0] = par;
    for (int32_t i = 1; i < LOG; ++i) {
        lift[v][i] = lift[lift[v][i - 1]][i - 1];
    }

    L[v] = T++;
    for (int32_t u : edges[v]) {
        if (u != par) {
            dfs(u, v);
        }
    }
    R[v] = T;
}

bool is_ancestor(int32_t anc, int32_t v) {
    return L[anc] <= L[v] && L[v] < R[anc];
}

int32_t lca_(int32_t v, int32_t u) {
    if (is_ancestor(u, v)) {
        return u;
    }
    for (int32_t i = LOG - 1; i >= 0; --i) {
        if (!is_ancestor(lift[u][i], v)) {
            u = lift[u][i];
        }
    }
    return parent[u];
}

int32_t lca(int32_t u, int32_t v) {
    int32_t a = lca_(root, u);
    int32_t b = lca_(root, v);
    if (a == b) {
        return lca_(u, v);
    } else if (depth[a] > depth[b]) {
        return a;
    } else {
        return b;
    }
}

int main() {
    string op;
    int32_t n, m, a, b;
    cin >> n;
    while (n) {
        root = 0;
        T = 0;
        depth[0] = 0;
        for (auto& i : edges) {
            i.clear();
        }

        for (int32_t i = 1; i < n; ++i) {
            cin >> a >> b; --a; --b;
            edges[a].push_back(b);
            edges[b].push_back(a);
        }
        dfs(0, 0);

        cin >> m;
        while (m--) {
            cin >> op;
            if (op == "!") {
                cin >> a;
                root = a - 1;
            } else if (op == "?") {
                cin >> a >> b;
                cout << lca(a - 1, b - 1) + 1 << endl;
            }
        }
        cin >> n;
    }
}
