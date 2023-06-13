#include <chrono>
#include <iostream>
#include <string.h>
#include <fstream>
#include <time.h>
#include <vector>
#include <queue>

// АиСД-2, 2023, КДЗ 3
// Константинов Никита Дмитриев БПИ 214
// Visual Studio Code
// Количество операций подсчитывается через глобальную переменную oper
// В коде прокоментировано что считалось элементарной операцией в каждом случае

typedef std::pair<int, int> Vay;
#define kInf 1000000
const int kMaxn = 10001;

int dist_d[kMaxn];
std::vector<Vay> vays[kMaxn];

int dist_b[kMaxn];
struct VayC {
    int a, b, cost;
};
std::vector<VayC> vaycs;

int dist_f_m[kMaxn][kMaxn];
int dist_f[kMaxn][kMaxn];

void diikstra(int s, int n) {
    dist_d[s] = 0;
    std::priority_queue<Vay> q;
    q.push(std::make_pair(0, s));
    while (!q.empty()) {
        int u = q.top().second;
        int cur = -q.top().first;
        q.pop();
        if (cur > dist_d[u]) {
            continue;
        }
        for (size_t i = 0; i < vays[u].size(); ++i) {
            int to = vays[u][i].first;
            int len = vays[u][i].second;
            if (dist_d[to] > dist_d[u] + len) {
                dist_d[to] = dist_d[u] + len;
                q.push(std::make_pair(-dist_d[to], to));
            }
        }
    }
}

void belman(int s, int n, int m) {
    dist_b[s] = 0;
    int x;
    for (int i = 0; i < n; ++i) {
        x = -1;
        for (int j = 0; j < m; ++j) {
            if (dist_b[vaycs[j].a] < kInf) {
                if (dist_b[vaycs[j].b] > dist_b[vaycs[j].a] + vaycs[j].cost) {
                    dist_b[vaycs[j].b] = std::max(-kInf, dist_b[vaycs[j].a] + vaycs[j].cost);
                    x = vaycs[j].b;
                }
            }
        }
    }
}

void floyd(int n) {
    for (size_t k = 0; k < n; ++k) {
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                if ((dist_f[i][k] < kInf) && (dist_f[k][j] < kInf)) {
                    dist_f[i][j] = std::min(dist_f[i][j], dist_f[i][k] + dist_f[k][j]);
                }
            }
        }
    }
}

int main() {
    int n;
    n = 10;
    int m = 0;
    int c;
    srand(1);
    // Значение времени
    int64_t time = 0;
    std::fstream read;
    std::fstream write;
    int tries = 10;
    write.open("result_ful.csv", std::ios::out);
    write << "Количество вершин;Количество рёбер;Дейкстра;Форд-Беллман;Флойд-Уоршел;" << std::endl;
    for (int k = 10; k <= 1010; k += 50) {
        std::cout << k << std::endl;
        write << k << ";";
        m = 0;
        for (int i = 0; i < k; ++i) {
            vays[i].clear();
            for (int j = 0; j < k; ++j) {
                if (i == j) {
                    dist_f_m[i][j] = 0;
                } else {
                    dist_f_m[i][j] = kInf;
                }
            }
        }
        vaycs.clear();
        // make full
        for (int i = 0; i < k - 1; ++i) {
            for (int j = i + 1; j < k; ++j) {
                c = rand() % 10 + 1;
                dist_f_m[i][j] = c;
                dist_f_m[j][i] = c;
                vays[i].push_back(std::make_pair(j, c));
                vays[j].push_back(std::make_pair(i, c));
                VayC newv{i, j, c};
                vaycs.push_back(newv);
                VayC newv1{j, i, c};
                vaycs.push_back(newv1);
                m += 2;
            }
        }
        write << m / 2 << ";";
        time = 0;
        for (int l = 0; l < tries; ++l) {
            for (int i = 0; i < k; ++i) {
                dist_d[i] = kInf;
            }
            auto start = std::chrono::high_resolution_clock::now();
            diikstra(0, k);
            auto end = std::chrono::high_resolution_clock::now();
            time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            // std::cout << dist_d[k - 1] << std::endl;
        }
        write << time / tries << ";";
        time = 0;
        for (int l = 0; l < tries; ++l) {
            for (int i = 0; i < k; ++i) {
                dist_b[i] = kInf;
            }
            auto start = std::chrono::high_resolution_clock::now();
            belman(0, k, m);
            auto end = std::chrono::high_resolution_clock::now();
            time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            // std::cout << dist_b[k - 1] << std::endl;
        }
        write << time / tries << ";";
        time = 0;
        for (int l = 0; l < tries; ++l) {
            for (int i = 0; i < k; ++i) {
                for (int j = 0; j < k; ++j) {
                    dist_f[i][j] = dist_f_m[i][j];
                }
            }
            auto start = std::chrono::high_resolution_clock::now();
            floyd(k);
            auto end = std::chrono::high_resolution_clock::now();
            time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            // std::cout << dist_f[0][k - 1] << std::endl;
        }
        write << time / tries;
        write << std::endl;
    }
    write.close();
    write.open("result_half.csv", std::ios::out);
    write << "Количество вершин;Количество рёбер;Коэффициент "
             "плотности;Дейкстра;Форд-Беллман;Флойд-Уоршел;"
          << std::endl;
    for (int k = 10; k <= 1010; k += 50) {
        std::cout << k << std::endl;
        write << k << ";";
        m = 0;
        for (int i = 0; i < k; ++i) {
            vays[i].clear();
            for (int j = 0; j < k; ++j) {
                if (i == j) {
                    dist_f_m[i][j] = 0;
                } else {
                    dist_f_m[i][j] = kInf;
                }
            }
        }
        vaycs.clear();
        // make half
        for (int i = 0; i < k - 1; ++i) {
            c = rand() % 10 + 1;
            dist_f_m[i][i + 1] = c;
            dist_f_m[i + 1][i] = c;
            vays[i].push_back(std::make_pair(i + 1, c));
            vays[i + 1].push_back(std::make_pair(i, c));
            VayC newv{i, i + 1, c};
            vaycs.push_back(newv);
            VayC newv1{i + 1, i, c};
            vaycs.push_back(newv1);
            m += 2;
        }
        int m1 = k * (k - 1);
        m1 /= 2;
        m1 -= 2;
        m1 -= m;
        for (int i = 0; i < m1; i += 2) {
            int a = 0;
            int b = 0;
            while (dist_f_m[a][b] != kInf) {
                a = rand() % k;
                b = rand() % k;
            }
            c = rand() % 10 + 1;
            dist_f_m[a][b] = c;
            dist_f_m[b][a] = c;
            vays[a].push_back(std::make_pair(b, c));
            vays[b].push_back(std::make_pair(a, c));
            VayC newv{a, b, c};
            vaycs.push_back(newv);
            VayC newv1{b, a, c};
            vaycs.push_back(newv1);
            m += 2;
        }
        double m2 = (double)m / (k * (k - 1));
        std::cout << m2 << std::endl;
        write << m / 2 << ";";
        write << m2 << ";";
        time = 0;
        for (int l = 0; l < tries; ++l) {
            for (int i = 0; i < k; ++i) {
                dist_d[i] = kInf;
            }
            auto start = std::chrono::high_resolution_clock::now();
            diikstra(0, k);
            auto end = std::chrono::high_resolution_clock::now();
            time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            // std::cout << dist_d[k - 1] << std::endl;
        }
        write << time / tries << ";";
        time = 0;
        for (int l = 0; l < tries; ++l) {
            for (int i = 0; i < k; ++i) {
                dist_b[i] = kInf;
            }
            auto start = std::chrono::high_resolution_clock::now();
            belman(0, k, m);
            auto end = std::chrono::high_resolution_clock::now();
            time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            // std::cout << dist_b[k - 1] << std::endl;
        }
        write << time / tries << ";";
        time = 0;
        for (int l = 0; l < tries; ++l) {
            for (int i = 0; i < k; ++i) {
                for (int j = 0; j < k; ++j) {
                    dist_f[i][j] = dist_f_m[i][j];
                }
            }
            auto start = std::chrono::high_resolution_clock::now();
            floyd(k);
            auto end = std::chrono::high_resolution_clock::now();
            time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            // std::cout << dist_f[0][k - 1] << std::endl;
        }
        write << time / tries;
        write << std::endl;
    }
    write.close();
    write.open("result_tree.csv", std::ios::out);
    write << "Количество вершин;Количество рёбер;Дейкстра;Форд-Беллман;Флойд-Уоршел;" << std::endl;
    for (int k = 10; k <= 1010; k += 50) {
        std::cout << k << std::endl;
        write << k << ";";
        m = 0;
        for (int i = 0; i < k; ++i) {
            vays[i].clear();
            for (int j = 0; j < k; ++j) {
                if (i == j) {
                    dist_f_m[i][j] = 0;
                } else {
                    dist_f_m[i][j] = kInf;
                }
            }
        }
        vaycs.clear();
        // make half
        for (int i = 1; i < k; ++i) {
            c = rand() % 10 + 1;
            int b = rand() % i;
            dist_f_m[i][b] = c;
            dist_f_m[b][i] = c;
            vays[i].push_back(std::make_pair(b, c));
            vays[b].push_back(std::make_pair(i, c));
            VayC newv{i, b, c};
            vaycs.push_back(newv);
            VayC newv1{b, i, c};
            vaycs.push_back(newv1);
            m += 2;
        }
        write << m / 2 << ";";
        time = 0;
        for (int l = 0; l < tries; ++l) {
            for (int i = 0; i < k; ++i) {
                dist_d[i] = kInf;
            }
            auto start = std::chrono::high_resolution_clock::now();
            diikstra(0, k);
            auto end = std::chrono::high_resolution_clock::now();
            time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            // std::cout << dist_d[k - 1] << std::endl;
        }
        write << time / tries << ";";
        time = 0;
        for (int l = 0; l < tries; ++l) {
            for (int i = 0; i < k; ++i) {
                dist_b[i] = kInf;
            }
            auto start = std::chrono::high_resolution_clock::now();
            belman(0, k, m);
            auto end = std::chrono::high_resolution_clock::now();
            time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            // std::cout << dist_b[k - 1] << std::endl;
        }
        write << time / tries << ";";
        time = 0;
        for (int l = 0; l < tries; ++l) {
            for (int i = 0; i < k; ++i) {
                for (int j = 0; j < k; ++j) {
                    dist_f[i][j] = dist_f_m[i][j];
                }
            }
            auto start = std::chrono::high_resolution_clock::now();
            floyd(k);
            auto end = std::chrono::high_resolution_clock::now();
            time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            // std::cout << dist_f[0][k - 1] << std::endl;
        }
        write << time / tries;
        write << std::endl;
    }
    write.close();
    return 0;
}