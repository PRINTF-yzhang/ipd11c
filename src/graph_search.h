#pragma once

#include "graph.h"

// Determines whether there is a path from node `src` to node `dst`.
bool graph_has_path(const_graph_t g, vertex_t src, vertex_t dst);

