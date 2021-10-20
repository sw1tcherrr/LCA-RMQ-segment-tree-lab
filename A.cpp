#include <iostream>
#include <vector>
using namespace std;

uint32_t cur = 0, a, b;

uint32_t next_rand() {
    cur = cur * a + b;
    return cur >> 8;
}

int main() {
    int m, q, n = 1<<24;
    cin >> m >> q >> a >> b;
    vector<int32_t> v(n, 0);

    for (int i = 0; i < m; ++i) {
        int add = next_rand();
        int l = next_rand();
        int r = next_rand();
        if (l > r) swap(l, r);

        v[l] += add;
        v[r + 1] -= add;
    }

    uint32_t res = 0;

    for (int j = 0; j < 2; ++j) {
        for (int i = 1; i < n; ++i) {
            v[i] += v[i - 1];
        }
    }

    for (int i = 0; i < q; ++i) {
        int l = next_rand();
        int r = next_rand();
        if (l > r) swap(l, r);

        res += v[r] - (l ? v[l - 1] : 0);
    }
    cout << res << endl;
}