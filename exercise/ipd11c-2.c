#include "graph.h"
#include <libipd.h>
#include <assert.h>

static bool
has_path_helper(const_graph_t g,
                vertex_t curr,
                vertex_t goal,
                bool seen[graph_size(g)])
{
    if (curr == goal) {
        return true;
    }

    if (seen[curr]) {
        return false;
    }

    seen[curr] = true;

    vertex_t g_size = graph_size(g);

    for (vertex_t succ = 0; succ < g_size; ++succ) {
        if (graph_has_edge(g, curr, succ) &&
            has_path_helper(g, succ, goal, seen)) {
            return true;
        }
    }

    return false;
}

// Determines whether there is a path from node `src` to node `dst`
// whose length is `n` or fewer.
//
// Examples: See tests.
bool graph_has_path_n(const_graph_t g,
                      vertex_t src,
                      vertex_t dst,
                      size_t n)
{
    if(src>n ||dst>n){
        return false;
    }
    vertex_t g_size = graph_size(g);
    assert( src < g_size );
    assert( dst < g_size );

    bool seen[g_size];

    for (vertex_t v = 0; v < g_size; ++v) {
        seen[v] = false;
    }

    return has_path_helper(g, src, dst, seen);
}

// Builds the graph 0 -> 1 -> ... -> (N-2) -> (N-1).
static graph_t build_linear_graph(size_t N)
{
    graph_t g = graph_create(N);
    assert( g );

    for (vertex_t i = 0; i + 1 < N; ++i) {
        graph_add_edge(g, i, i + 1);
    }

    return g;
}

static void test_ghpn_3(void)
{
    graph_t g3  = build_linear_graph(3);

    CHECK( graph_has_path_n(g3, 0, 0, 0) );
    CHECK( graph_has_path_n(g3, 0, 0, 1) );
    CHECK( graph_has_path_n(g3, 0, 0, 2) );

    CHECK( ! graph_has_path_n(g3, 0, 1, 0) );
    CHECK( graph_has_path_n(g3, 0, 1, 1) );
    CHECK( graph_has_path_n(g3, 0, 1, 2) );

    CHECK( ! graph_has_path_n(g3, 0, 2, 0) );
    CHECK( ! graph_has_path_n(g3, 0, 2, 1) );
    CHECK( graph_has_path_n(g3, 0, 2, 2) );
    CHECK( graph_has_path_n(g3, 0, 2, 3) );

    CHECK( ! graph_has_path_n(g3, 2, 0, 10) );

    graph_destroy(g3);
}

static void test_ghpn_4(void)
{
    graph_t g4  = build_linear_graph(4);


    CHECK( ! graph_has_path_n(g4, 0, 3, 0) );
    CHECK( ! graph_has_path_n(g4, 0, 3, 1) );
    CHECK( graph_has_path_n(g4, 0, 3, 4) );
    CHECK( graph_has_path_n(g4, 1, 3, 4) );

    CHECK( ! graph_has_path_n(g4, 2, 0, 10) );

    graph_destroy(g4);
}

static void test_ghpn_16(void)
{
    graph_t g16 = graph_create(16);
    graph_add_edge(g16, 0, 1);
    graph_add_edge(g16, 1, 2);
    graph_add_edge(g16, 2, 3);
    graph_add_edge(g16, 3, 4);
    graph_add_edge(g16, 4, 5);
    graph_add_edge(g16, 5, 6);
    CHECK( ! graph_has_path_n(g16, 0, 3, 0) );
    CHECK( ! graph_has_path_n(g16, 0, 3, 1) );
    CHECK( graph_has_path_n(g16, 0, 3, 11) );
    CHECK( graph_has_path_n(g16, 1, 3, 11) );

    CHECK( ! graph_has_path_n(g16, 2, 0, 10) );
    CHECK(  graph_has_path_n(g16, 0, 5, 16) );

    graph_destroy(g16);
}

int main(void)
{
    test_ghpn_3();
    test_ghpn_4();
    test_ghpn_16();

}