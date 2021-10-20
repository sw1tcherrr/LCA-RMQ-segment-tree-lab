#include <iostream>
#include <vector>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

using namespace std;

template <typename T>
struct tree {
    using elt = struct {
        T val;
        T upd;
        size_t coord;
    };

    tree(vector<T> const& a, function<T(T const&, T const&)> func, T ntr)
            : bot_sz(get_sz(a.size())),
              neutral(ntr),
              t(2 * bot_sz - 1, {neutral, 0, 0}),
              f(func)
    {
        for (size_t i = 0; i < a.size(); ++i) {
            t[i + bot_sz - 1].val = a[i];
            t[i + bot_sz - 1].coord = i;
        }
        for (int i = bot_sz - 2; i >= 0; --i) {
            t[i].val = f(t[2*i + 1].val, t[2*i + 2].val);
            t[i].coord = t[2 * i + 1].coord;
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
            t[v].coord = get_v(2*v + 1) >= get_v(2*v + 2) ? t[2*v + 1].coord : t[2*v + 2].coord;
        }
    }

    size_t bot_sz;
    elt root() {
        return {get_v(0), 0, get_v(1) >= get_v(2) ? t[1].coord : t[2].coord};
    }
private:
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
        return t[v].val + t[v].upd;
    }

    void push(size_t v) {
        t[2*v + 1].upd += t[v].upd;
        t[2*v + 2].upd += t[v].upd;
        t[v].upd = 0;
        t[v].val = f(get_v(2*v + 1),
                     get_v(2*v + 2));
        t[v].coord = get_v(2*v + 1) >= get_v(2*v + 2) ? t[2*v + 1].coord : t[2*v + 2].coord;
    }
};

struct event {
    int32_t x, l, r, open;
    bool operator<(event const& other) const {
        return x == other.x ? open > other.open : x < other.x;
    }
};

int main() {
    size_t n;
    cin >> n;

    vector<event> events;
    events.reserve(2*n);
    unordered_set<int32_t> s;

    for (size_t i = 0; i < n; ++i) {
        int32_t x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        events.push_back({x1, y1, y2, 1});
        events.push_back({x2, y1, y2, -1});
        s.insert(y1);
        s.insert(y2);
    }

    vector<int32_t> ys;
    ys.assign(s.begin(), s.end());
    sort(ys.begin(), ys.end());
    unordered_map<int32_t , size_t> map;
    for (size_t i = 0; i < ys.size(); ++i) {
        map.insert({ys[i], i});
    }

    sort(events.begin(), events.end());

    tree<int32_t> t(vector<int32_t>(ys.size(), 0), [](int32_t a, int32_t b){return max(a, b);}, 0);


    int32_t cnt = 0, x = 0, y = 0;
    for (event e : events) {
        t.add(0, 0, t.bot_sz, map[e.l], map[e.r] + 1, e.open);
        if (t.root().val > cnt) {
            cnt = t.root().val;
            x = e.x;
            y = ys[t.root().coord];
        }
    }
    cout << cnt << "\n" << x << " " << y << "\n";
}