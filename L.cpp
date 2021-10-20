#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <typename T>
struct tree {
    tree(vector<T> const& a, T ntr)
            : bot_sz(get_sz(a.size())),
              neutral(ntr),
              t(2 * bot_sz - 1, vector<T>(0))
    {
        for (size_t i = 0; i < a.size(); ++i) {
            t[i + bot_sz - 1].push_back(a[i]);
        }

        for (int i = bot_sz - 2; i >= 0; --i) {
            merge(t[2*i + 1].begin(), t[2*i + 1].end(), t[2*i + 2].begin(), t[2*i + 2].end(), back_inserter(t[i]));
        }
    }

    T get(size_t v, size_t l, size_t r, size_t a, size_t b, T x, T y) {
        if (l >= b || r <= a) {
            return neutral;
        } else if (l >= a && r <= b) {
            return upper_bound(t[v].begin(), t[v].end(), y) - lower_bound(t[v].begin(), t[v].end(), x);
        } else {
            return get(2*v + 1, l, (l + r) / 2, a, b, x, y) + get(2*v + 2, (l + r) / 2, r, a, b, x, y);
        }
    }

    size_t bot_sz;
private:
    T neutral;
    vector<vector<T>> t;

    size_t get_sz(size_t n) {
        size_t s = 1;
        while (s < n) {
            s *= 2;
        }
        return s;
    }
};

uint32_t a, b, cur = 0;

uint32_t nextRand17() {
    cur = cur * a + b;
    return cur >> 15;
}

uint32_t nextRand24() {
    cur = cur * a + b;
    return cur >> 8;
}

int main() {
    uint32_t q;
    cin >> q >> a >> b;
    vector<uint32_t> v(1 << 17);
    for (size_t i = 0; i < (1 << 17); ++i) {
        v[i] = nextRand24();
    }

    tree<uint32_t> t(v, 0);

    uint32_t l, r, x, y, sum = 0, ans;
    for (size_t i = 0; i < q; ++i) {
        l = nextRand17();
        r = nextRand17();
        x = nextRand24();
        y = nextRand24();
        if (l > r) swap(l, r);
        if (x > y) swap(x, y);
        ans = t.get(0, 0, t.bot_sz, l, r + 1, x, y);
        b += ans;
        sum += ans;
    }
    cout << sum << "\n";
}