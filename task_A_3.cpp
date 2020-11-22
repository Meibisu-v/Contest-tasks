#include <iostream>
#include <cstdlib>
#include <vector>
#include <list>
#include <set>
#include <algorithm>


class Graph {
protected:
	size_t vertex_count, edge_count;
	bool is_directed;

public:
	typedef size_t vertex;
	Graph(size_t vertex_count, bool is_directed) :
	    vertex_count(vertex_count), is_directed(is_directed), edge_count(0) {}

	size_t get_vertex_count() const {
		return vertex_count;
	}

	size_t get_edge_count() const {
		if (is_directed == 0) {
			return edge_count / 2;
		}
		return edge_count;
	}

	bool get_direction() const {
		return is_directed;
	}

	virtual void add_edge(const vertex& start, const vertex& finish) = 0;
	virtual size_t get_vertex_deg(const vertex& vertex) const = 0;
	virtual std::vector<std::vector<vertex>> get_adjList() const = 0;
	virtual std::vector<vertex> get_neighbors(const vertex& v) const = 0;
};


class graph_adj_list : public Graph {
private:
	std::vector<std::vector<vertex>> adj_list_;

public:
	graph_adj_list(size_t vertex_count, bool is_directed) : Graph(vertex_count, is_directed), adj_list_(vertex_count + 1) {}

	void add_edge(const vertex& start, const vertex& finish) override {
		adj_list_[start].push_back(finish);
		if (!is_directed) {
			adj_list_[finish].push_back(start);
		}
		edge_count++;
	}

	size_t get_vertex_deg(const vertex& vertex) const override {
		return adj_list_[vertex].size();
	}

	std::vector<std::vector<vertex>> get_adjList() const override {
		return adj_list_;
	}

	std::vector<vertex> get_neighbors(const vertex& vertex) const override {
		return adj_list_[vertex];
	}
};


enum colors {NOT_COLOR = 0, WHITE, GRAY, BLACK};

struct DFS_data {
    std::vector<colors> color;
    int start;
    bool is_root;
    size_t time;
    std::set<int> answer;
    std::vector<int> t_in, t_up;
};

void DFS_visit(const Graph &graph, DFS_data &data) {
    data.color[data.start] = GRAY;
    size_t children_count = 0;
    ++data.time;
    data.t_in[data.start] = data.time;
    data.t_up[data.start] = data.time;
    auto neighbors = graph.get_neighbors(data.start);
    for (const auto &neigh : neighbors) {
        if (data.color[neigh] != WHITE) {
            data.t_up[data.start] = std::min(data.t_up[data.start], data.t_in[neigh]);
        }
        if (data.color[neigh] == WHITE) {
            bool is_root = data.is_root;
            int start = data.start;
            data.is_root = false;
            data.start = neigh;
            DFS_visit(graph, data);
            data.is_root = is_root;
            data.start = data.start;
            ++children_count;
            data.t_up[data.start] = std::min(data.t_up[data.start], data.t_up[neigh]);
            if (data.t_in[data.start] <= data.t_up[neigh] && !data.is_root) {
                data.answer.insert(data.start);
            }
        }
    }
    if (data.is_root && children_count >= 2) {
        data.answer.insert(data.start);
    }
    data.color[data.start] = BLACK;
}


std::set<int> cut_vertices(const Graph &graph) {
    DFS_data data;
    data.color.resize(graph.get_vertex_count() + 1, WHITE);
    data.time = 0;
    for (Graph::vertex i = 1; i < graph.get_vertex_count() + 1; ++i) {
        if (data.color[i] == WHITE) {
            std::vector<int> t_in(graph.get_vertex_count() + 1), t_out(graph.get_vertex_count() + 1);
            data.t_in = t_in;
            data.t_up = t_out;
            data.start = i;
            data.is_root = true;
            data.time = 0;
            DFS_visit(graph, data);
        }
    }
    return data.answer;
}


int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    size_t vertex, edges;
    std::cin >> vertex >> edges;

    graph_adj_list list_edges(vertex, false);

    for (size_t i = 0; i < edges; ++i){
        size_t from, to;
        std::cin >> from >> to;
        list_edges.add_edge(from, to);
    }

    std::set<int> points_of_articulation = cut_vertices(list_edges);
    std::cout << points_of_articulation.size() << '\n';
    for (auto i : points_of_articulation) {
        std::cout << i << '\n';
    }
    return 0;
}
