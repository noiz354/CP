#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath> // For log2

const int MAXN = 200005; // Maximum number of nodes
const int LOGN = 18;     // ceil(log2(200000)) is 17.6, so 18 is safe.

std::vector<int> adj[MAXN]; // Adjacency list for the tree
int parent[MAXN][LOGN];
int depth[MAXN]; // Depth of node from root (root is at depth 0)

// DFS to precompute depths and immediate parents
void dfs_precompute(int u, int p, int d) {
    depth[u] = d;
    parent[u][0] = p;

    for (int v : adj[u]) {
        if (v == p) continue;
        dfs_precompute(v, u, d + 1);
    }
}

// Function to initialize parent table for binary lifting
void build_lca_table(int n) {
    // Fill parent[u][k] for k > 0
    for (int k = 1; k < LOGN; ++k) {
        for (int i = 1; i <= n; ++i) { // Nodes are 1-indexed
            parent[i][k] = parent[parent[i][k-1]][k-1];
        }
    }
}

// Function to find LCA of u and v
int lca(int u, int v) {
    // 1. Ensure u is deeper than or at the same depth as v
    if (depth[u] < depth[v]) {
        std::swap(u, v);
    }

    // 2. Lift u up to the same depth as v
    for (int k = LOGN - 1; k >= 0; --k) {
        if (depth[u] - (1 << k) >= depth[v]) {
            u = parent[u][k];
        }
    }

    // 3. If u and v are the same node, it's the LCA
    if (u == v) {
        return u;
    }

    // 4. Lift u and v simultaneously until their parents are the same
    for (int k = LOGN - 1; k >= 0; --k) {
        if (parent[u][k] != parent[v][k]) {
            u = parent[u][k];
            v = parent[v][k];
        }
    }

    // The common parent is the LCA
    return parent[u][0];
}

// Function to get the number of edges on the path between u and v
int get_edges_on_path(int u, int v) {
    int common_ancestor = lca(u, v);
    return depth[u] + depth[v] - 2 * depth[common_ancestor];
}

int main() {
    // Optimize C++ standard streams for faster input/output
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n; // Number of attractions
    std::cin >> n;

    // Read tree edges
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        std::cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Precompute depths and immediate parents starting from root 1
    // parent[1][0] is 1 (root's parent is itself)
    dfs_precompute(1, 1, 0); 
    
    // Build the binary lifting parent table
    build_lca_table(n);

    long long total_path_lengths_sum = 0;

    // Iterate through all possible x values
    for (int x = 1; x <= n; ++x) {
        // Iterate through all possible y values (multiples of x, and y > x)
        // Start y from 2*x since y > x
        for (long long y = 2LL * x; y <= n; y += x) {
            // Calculate number of edges on path between x and y
            int edges = get_edges_on_path(x, y);
            // "Length of a path" is defined as number of attractions visited, which is edges + 1
            total_path_lengths_sum += (long long)edges + 1;
        }
    }

    std::cout << total_path_lengths_sum << std::endl;

    return 0;
}
