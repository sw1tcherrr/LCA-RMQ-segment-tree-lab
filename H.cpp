#include <iostream>
#include <vector>
#include <functional>
using namespace std;

template <typename T>
struct tree {
    tree(vector<T> const& a, function<T(T const&, T const&)> func, T ntr)
            : bot_sz(get_sz(a.size())),
              neutral(ntr),
              t(2 * bot_sz - 1, neutral),
              f(func)
    {
        for (size_t i = 0; i < a.size(); ++i) {
            t[i + bot_sz - 1] = a[i];
        }
        for (int i = bot_sz - 2; i >= 0; --i) {
            t[i] = f(t[2*i + 1], t[2*i + 2]);
        }
    }

    void set(T x) {
        size_t v = x + bot_sz - 1;
        t[v] = x;
        while (v != 0) {
            v = (v - 1) / 2;
            t[v] = f(t[2*v + 1], t[2*v + 2]);
        }
    }

    T get(size_t v, T x) {
        if (t[v] >= x) {
            if (2 * v + 2 < t.size()) {
                T res = neutral;
                if (t[2 * v + 1] >= x) {
                    res = get(2 * v + 1, x);
                } else if (t[2 * v + 2] >= x) {
                    res = get(2 * v + 2, x);
                }
                t[v] = f(t[2*v + 1], t[2*v + 2]);
                return res;
            }
            T tmp = t[v];
            t[v] = neutral;
            return tmp;
        }
        return get(0, 0);
    }

private:
    size_t bot_sz;
    T neutral;
    vector<T> t;
    function<T(T, T)> f;

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

    size_t n;
    int m;
    cin >> n >> m;
    vector<int32_t> a(n);
    for (size_t i = 0; i < n; ++i) {
        a[i] = i;
    }
    tree<int32_t> t(a, [](int a, int b){return max(a, b);}, INT32_MIN);

    string op;
    int32_t x;

    while (m--) {
        cin >> op;
        if (op == "enter") {
            cin >> x;
            cout << t.get(0, x - 1) + 1 << "\n";
        } else if (op == "exit") {
            cin >> x;
            t.set(x - 1);
        }
    }
}