#include "graph.h"
#include <libipd.h>
#include <assert.h>

static void one_node_graph(void)
{
    graph_t g1 = graph_create(1);
    assert( g1 );

    CHECK_SIZE( graph_size(g1), 1 );
    CHECK( ! graph_has_edge(g1, 0, 0) );

    graph_add_edge(g1, 0, 0);
    CHECK( graph_has_edge(g1, 0, 0) );

    graph_remove_edge(g1, 0, 0);
    CHECK( ! graph_has_edge(g1, 0, 0) );

    graph_destroy(g1);
}

static void two_node_graph(void)
{
    graph_t g2 = graph_create(2);
    assert( g2 );

    CHECK_SIZE( graph_size(g2), 2 );

    CHECK( ! graph_has_edge(g2, 0, 0) );
    CHECK( ! graph_has_edge(g2, 0, 1) );
    CHECK( ! graph_has_edge(g2, 1, 0) );
    CHECK( ! graph_has_edge(g2, 1, 1) );

    graph_add_edge(g2, 0, 1);

    CHECK( ! graph_has_edge(g2, 0, 0) );
    CHECK( graph_has_edge(g2, 0, 1) );
    CHECK( ! graph_has_edge(g2, 1, 0) );
    CHECK( ! graph_has_edge(g2, 1, 1) );

    graph_destroy(g2);
}

static void four_node_graph(void)
{
    graph_t g4 = graph_create(4);
    assert( g4 );

    CHECK_SIZE( graph_size(g4), 4 );

    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 4; ++j)
            CHECK( ! graph_has_edge(g4, i, j) );

    for (size_t i = 0; i < 4; ++i)
        graph_add_edge(g4, i, (i + 1) % 4);

    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 4; ++j)
            if ((i + 1) % 4 == j)
                CHECK( graph_has_edge(g4, i, j) );
            else
                CHECK( ! graph_has_edge(g4, i, j) );

    graph_destroy(g4);
}

// Builds a graph with N vertices, where every vertex v has outgoing
// edges to every vertex u > v. Then, for each vertex v in turn, gets
// v’s successors in an array of capacity M.
static void test_get_successors(size_t const N, size_t const M)
//M is the capacity that its trying to get the successors into
{
    vertex_t out[M];
    graph_t g = graph_create(N);

    assert( g );

    for (size_t i = 0; i < N; ++i) {
        for (size_t j = i + 1; j < N; ++j) {
            graph_add_edge(g, i, j);
        }
    }

    for (size_t i = 0; i < N; ++i) {
        //the number of successors it has
        size_t degree = N - i - 1;
        size_t needed = graph_get_successors(g, i, M, out);

        CHECK_SIZE( needed, degree );

        //its not gonna use more space than it needs/ we give it
        size_t size = needed < M ? needed : M;
        //the out is going to be filled with successors that we expect
        // in descending order
        for (size_t j = 0; j < size; ++j)
            CHECK_SIZE( out[j], j + i + 1 );
    }

    graph_destroy(g);
}

int main(void)
{
    one_node_graph();
    two_node_graph();
    four_node_graph();

    test_get_successors(6, 0);
    test_get_successors(6, 3);
    test_get_successors(6, 5);
    test_get_successors(6, 6);
    test_get_successors(6, 10);
    test_get_successors(60, 30);
}
