#include <iostream>
#include <vector>
#include <functional>
using namespace std;

template <typename T>
struct tree {
    tree(vector<T> const& a, function<T(T const&, T const&)> func, T ntr)
            : bot_sz(get_sz(a.size())),
            neutral(ntr),
            t(2 * bot_sz - 1, {neutral, 0, false}),
            f(func)
    {
        for (size_t i = 0; i < a.size(); ++i) {
            t[i + bot_sz - 1].val = a[i];
        }
        for (int i = bot_sz - 2; i >= 0; --i) {
            t[i].val = f(t[2*i + 1].val, t[2*i + 2].val);
        }
    }

    void set(size_t v, size_t l, size_t r, size_t a, size_t b, T x) {
        if (l >= b || r <= a) {
            return;
        } else if (l >= a && r <= b) {
            t[v].upd = x;
            t[v].set = true;
        } else {
            push(v);
            set(2*v + 1, l, (l + r) / 2, a, b, x);
            set(2*v + 2, (l + r) / 2, r, a, b, x);
            t[v].val = f(get_v(2*v + 1),
                         get_v(2*v + 2));
        }
    }

    void add(size_t v, size_t l, size_t r, size_t a, size_t b, T x) {
        if (l >= b || r <= a) {
            return;
        } else if (l >= a && r <= b) {
            t[v].upd += x;
        } else {
            push(v);
            add(2*v + 1, l, (l + r) / 2, a, b, x);
            add(2*v + 2, (l + r) / 2, r, a, b, x);
            t[v].val = f(get_v(2*v + 1),
                         get_v(2*v + 2));
        }
    }

    T get(size_t v, size_t l, size_t r, size_t a, size_t b) {
        if (l >= b || r <= a) {
            return neutral;
        } else if (l >= a && r <= b) {
            return get_v(v);
        } else {
            push(v);
            return f(get(2*v + 1, l, (l + r) / 2, a, b),
                     get(2*v + 2, (l + r) / 2, r, a, b));
        }
    }

    size_t bot_sz;
private:
    using elt = struct {
        T val;
        T upd;
        bool set;
    };

    T neutral;
    vector<elt> t;
    function<T(T, T)> f;

    size_t get_sz(size_t n) {
        size_t s = 1;
        while (s < n) {
            s *= 2;
        }
        return s;
    }

    T get_v(size_t v) {
        if (t[v].set) {
            return t[v].upd;
        } else {
            return t[v].val + t[v].upd;
        }
    }

    void push(size_t v) {
        if (2*v + 2 < t.size()) {
            if (t[v].set) {
                t[2*v + 1].upd = t[v].upd;
                t[2*v + 2].upd = t[v].upd;
                t[2*v + 1].set = true;
                t[2*v + 2].set = true;
            } else {
                t[2*v + 1].upd += t[v].upd;
                t[2*v + 2].upd += t[v].upd;
            }
        }
        t[v].set = false;
        t[v].upd = 0;
        t[v].val = f(get_v(2*v + 1),
                     get_v(2*v + 2));
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

    tree<int64_t> t(a, [](int64_t a, int64_t b){return min(a, b);}, INT64_MAX);

    string op;
    size_t l, r;
    int64_t x;

    while (cin >> op) {
        if (op == "min") {
            cin >> l >> r;
            cout << t.get(0, 0, t.bot_sz, l - 1, r) << "\n";
        } else if (op == "set") {
            cin >> l >> r >> x;
            t.set(0, 0, t.bot_sz, l - 1, r, x);
        } else if (op == "add") {
            cin >> l >> r >> x;
            t.add(0, 0, t.bot_sz, l - 1, r, x);
        }
    }
}