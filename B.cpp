#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    vector<int> a(n);
    cin >> a[0];
    for (int i = 0; i < n - 1; ++i) {
        a[i + 1] = (23 * a[i] + 21563) % 16714589;
    }

    vector<vector<int>> st(static_cast<int>(log2(n)) + 1, vector<int>(n));
    st[0] = a;
    for (size_t k = 1; k < st.size(); ++k) {
        for (int i = 0; i < n; ++i) {
            st[k][i] = min(st[k - 1][i], i + (1 << (k - 1)) < n ? st[k - 1][i + (1 << (k - 1))] : INT32_MAX);
        }
    }


    int u, v, ans = 0;
    for (int i = 0; i < m; ++i) {
        if (i == 0) {
            cin >> u >> v;
        } else {
            u = ((17 * u + 751 + ans + 2 * i) % n) + 1;
            v = ((13 * v + 593 + ans + 5 * i) % n) + 1;
        }

        int l = min(u, v) - 1,
        r = max(u, v) - 1,
        k = static_cast<int>(log2(r - l + 1));

        ans = min(st[k][l], st[k][r - (1<<k) + 1]);
    }

    cout << u << " " << v << " " << ans << endl;
}