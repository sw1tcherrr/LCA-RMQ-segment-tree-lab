#include <iostream>
#include <vector>
#include <functional>
using namespace std;

struct tree {
    tree(vector<int64_t> const& a, function<int64_t(int64_t, int64_t)> func, int64_t n)
            : bot_sz(get_sz(a.size())), neutral(n), t(2 * bot_sz - 1, neutral), f(func) {
        for (size_t i = 0; i < a.size(); ++i) {
            t[i + bot_sz - 1] = a[i];
        }
        for (int i = bot_sz - 2; i >= 0; --i) {
            t[i] = f(t[2*i + 1], t[2*i + 2]);
        }
    }

    void set(size_t i, int64_t x) {
        size_t v = i + bot_sz - 1;
        t[v] = x;
        while (v != 0) {
            v = (v - 1) / 2;
            t[v] = f(t[2*v + 1], t[2*v + 2]);
        }
    }

    int64_t get(size_t v, size_t l, size_t r, size_t a, size_t b) {
        if (l >= b || r <= a) {
            return neutral;
        } else if (l >= a && r <= b) {
            return t[v];
        } else {
            return f(get(2*v + 1, l, (l + r) / 2, a, b),
                     get(2*v + 2, (l + r) / 2, r, a, b));
        }
    }

    size_t bot_sz;
private:
    int64_t neutral;
    vector<int64_t> t;
    function<int64_t(int64_t, int64_t)> f;

    size_t get_sz(size_t n) {
        size_t s = 1;
        while (s < n) {
            s *= 2;
        }
        return s;
    }
};

int main() {
    ios_base::sync_with_stdio(0);

    int n;
    cin >> n;
    vector<int64_t> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    tree t(a, [](int64_t a, int64_t b){return a + b;}, 0);

    string op;
    size_t l, r, i;
    int64_t x;

    while (cin >> op) {
        if (op == "sum") {
            cin >> l >> r;
            cout << t.get(0, 0, t.bot_sz, l - 1, r) << "\n";
        } else if (op == "set") {
            cin >> i >> x;
            t.set(i - 1, x);
        }
    }
}