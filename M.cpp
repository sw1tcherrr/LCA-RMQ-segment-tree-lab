#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

struct vertex {
    int32_t v = 0, d = 0;
    bool operator<(vertex other) const {
        return d < other.d;
    }
};

vector<vector<int32_t>> children;
vector<int32_t> pos;
vector<vector<vertex>> st;
int32_t p = 0;

void tour(int32_t v, int32_t d) {
    pos[v] = p++;
    st[0].push_back({v, d});
    for (auto u : children[v]) {
        tour(u, d + 1);
        st[0].push_back({v, d});
        p++;
    }
}

int main() {
    int32_t n, m;
    cin >> n >> m;

    children.resize(n, vector<int32_t>(0));
    pos.resize(n);
    st.emplace_back(0);

    for (int32_t i = 1; i <= n - 1; ++i) {
        int32_t v;
        cin >> v;
        children[v].push_back(i);
    }

    tour(0, 0);
    size_t sz = st[0].size();
    st.resize(static_cast<int>(log2(sz)) + 1, vector<vertex>(sz));
    for (size_t k = 1; k < st.size(); ++k) {
        for (size_t i = 0; i < sz; ++i) {
            st[k][i] = min(st[k - 1][i], i + (1 << (k - 1)) < st[0].size() ? st[k - 1][i + (1 << (k - 1))] : vertex({0, INT32_MAX}));
        }
    }

    int64_t sum = 0, ans = 0, x, y, z;
    int32_t a1, a2;
    for (int32_t i = 0; i < m; ++i) {
        if (i == 0) {
            cin >> a1 >> a2;
            cin >> x >> y >> z;
        } else {
            a1 = static_cast<int>((x * a1 + y * a2 + z) % n);
            a2 = static_cast<int>((x * a2 + y * a1 + z) % n);
        }

        int32_t l = min(pos[static_cast<int>(a1 + ans) % n], pos[a2]),
                r = max(pos[static_cast<int>(a1 + ans) % n], pos[a2]),
                k = static_cast<int>(log2(r - l + 1));

        ans = min(st[k][l], st[k][r - (1<<k) + 1]).v;
        sum += ans;
    }

    cout << sum << "\n";
}