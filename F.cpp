#include <iostream>
#include <vector>
#include <functional>
using namespace std;

uint32_t mod;

template <typename T>
struct matrix {
    matrix() = default;
    matrix(T val) : matrix() {
        m.fill(val);
    }
    matrix(array<T, 4> v) : m(move(v)) {}

    T const& operator[](size_t idx) const {
        return m[idx];
    }

    T& operator[](size_t idx) {
        return m[idx];
    }

    friend matrix operator*(matrix const& a, matrix const& b) {
        matrix c;
        c[0] = ((a[0] * b[0]) % mod + (a[1] * b[2]) % mod) % mod;
        c[1] = ((a[0] * b[1]) % mod + (a[1] * b[3]) % mod) % mod;
        c[2] = ((a[2] * b[0]) % mod + (a[3] * b[2]) % mod) % mod;
        c[3] = ((a[2] * b[1]) % mod + (a[3] * b[3]) % mod) % mod;
        return c;
    }

    friend istream& operator>>(istream& s, matrix& a) {
        for (size_t i = 0; i < a.m.size(); ++i) {
            s >> a[i];
        }
        return s;
    }

    friend ostream& operator<<(ostream& s, matrix const& a) {
        s << a[0] << " " << a[1] << "\n" << a[2] << " " << a[3] << "\n";
        return s;
    }

private:
    array<T, 4> m;
};

template <typename T>
struct tree {
    tree(vector<T> const& a, function<T(T const&, T const&)> func, T n)
            : bot_sz(get_sz(a.size())), neutral(n), t(2 * bot_sz - 1, neutral), f(func) {
        for (size_t i = 0; i < a.size(); ++i) {
            t[i + bot_sz - 1] = a[i];
        }
        for (int i = bot_sz - 2; i >= 0; --i) {
            t[i] = f(t[2*i + 1], t[2*i + 2]);
        }
    }

    void set(size_t i, T x) {
        size_t v = i + bot_sz - 1;
        t[v] = x;
        while (v != 0) {
            v = (v - 1) / 2;
            t[v] = f(t[2*v + 1], t[2*v + 2]);
        }
    }

    T get(size_t v, size_t l, size_t r, size_t a, size_t b) {
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
    using matrix_ = matrix<int32_t>;

    int n, m;
    cin >> mod >> n >> m;
    vector<matrix_> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    matrix_ E({1, 0, 0, 1});
    tree<matrix_> t(a, [](matrix_ const& a, matrix_ const& b){return a * b;}, E);

    size_t l, r;
    while (m--) {
        cin >> l >> r;
        cout << t.get(0, 0, t.bot_sz, l - 1, r) << "\n";
    }
}