#include <cstdlib>
#include <fstream>
#include <vector>
#include <stack>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;
using namespace boost::archive;
using namespace boost::serialization;

typedef adjacency_list<vecS, vecS, undirectedS> graph_t;
typedef typename graph_traits<graph_t>::vertex_descriptor vertex_t;
typedef typename graph_traits<graph_t>::edge_descriptor edge_t;
typedef typename graph_traits<graph_t>::edge_iterator e_iter_t;
typedef typename graph_traits<graph_t>::adjacency_iterator adj_iter_t;
typedef typename graph_traits<graph_t>::vertex_iterator v_iter_t;

namespace boost {
namespace serialization
{

    template <typename Archive>
    inline void load(Archive & ar, graph_t & graph, const unsigned int)
    {
        size_t V, E;
        ar >> make_nvp("V", V);
        ar >> make_nvp("E", E);

        for (size_t it = 0; it != E; ++it)
        {
            vertex_t u, v;
            ar >> make_nvp("u", u);
            ar >> make_nvp("v", v);
            add_edge(u, v, graph);
        }
    }

    template <typename Archive>
    inline void save(Archive & ar, graph_t const & graph, const unsigned int)
    {
        size_t const V = num_vertices(graph);
        size_t const E = num_edges(graph);

        ar << make_nvp("V", V);
        ar << make_nvp("E", E);

        e_iter_t begin, end;
        tie(begin, end) = edges(graph);
        for (e_iter_t it = begin; it != end; ++it)
        {
            vertex_t const u = source(*it, graph);
            vertex_t const v = target(*it, graph);
            ar << make_nvp("u", u);
            ar << make_nvp("v", v);
        }
    }

    template <typename Archive>
    inline void serialize(Archive & ar, graph_t & graph, unsigned int version)
    { split_free(ar, graph, version); }

}}

template <typename ColorSet>
void dfs_impl(graph_t const & graph, graph_t & trees, vertex_t const & root, ColorSet & marked)
{
    std::stack<vertex_t> st;
    st.push(root);

    while (!st.empty())
    {
        vertex_t u = st.top(); st.pop();
        marked[u] = true;
        adj_iter_t b, e;
        tie(b, e) = adjacent_vertices(u, graph);
        for (adj_iter_t it = b; it != e; ++it)
        {
            if (!marked[*it])
            {
                st.push(*it);
                add_edge(u, *it, trees);
            }
        }
    }
}

void dfs(graph_t const & graph, graph_t & tree, vertex_t const & root)
{
    std::vector<char> marked(num_vertices(graph), false);
    dfs_impl(graph, tree, root, marked);
}

void dfs(graph_t const & graph, graph_t & trees)
{
    std::vector<char> marked(num_vertices(graph), false);
    v_iter_t b, e;
    tie(b, e) = vertices(graph);
    for (v_iter_t it = b; it != e; ++it)
    {
        if (!marked[*it])
            dfs_impl(graph, trees, *it, marked);
    }
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "input and output file names expected" << std::endl;
        return EXIT_FAILURE;
    }

    graph_t graph;
    {
        std::ifstream ifs(argv[1]);
        text_iarchive ar(ifs, no_header);
        ar >> graph;
    }

    {
        graph_t tree;
        dfs(graph, tree);
        std::ofstream ofs(argv[2]);
        text_oarchive ar(ofs, no_header);
        ar << tree;
    }

    return EXIT_SUCCESS;
}
