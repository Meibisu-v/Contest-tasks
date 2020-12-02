#include <iostream>
#include <cstdlib>
#include <vector>
#include <list>
#include <algorithm>
#include <queue>

struct vertex_and_weight_of_edge {
	long long weigth_of_outgoing_edge;
	Graph_weight::vertex vertex;
};

class Graph_weight{
protected:
	size_t vertex_count, edge_count;
	bool is_directed;

public:
	typedef size_t vertex;
	Graph_weight(size_t vertex_count, bool is_directed) :
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

	std::vector<vertex_and_weight_of_edge> get_neighbors(vertex vertex) const;

	virtual size_t get_vertex_deg(vertex vertex) const = 0;
};



class graph_adj_list_weight : public Graph_weight {
private:
	std::vector<std::vector<vertex_and_weight_of_edge>> adj_list_;

public:
	graph_adj_list_weight(size_t vertex_count, bool is_directed) :
	    Graph_weight(vertex_count, is_directed), adj_list_(vertex_count + 1) {}

	void add_edge(vertex start, vertex finish, long long weight) {
		adj_list_[start].push_back({finish, weight});
		if (!is_directed) {
			adj_list_[finish].push_back({start, weight});
		}
		edge_count++;
	}

	size_t get_vertex_deg(vertex vertex) const {
		return adj_list_[vertex].size();
	}

	std::vector<std::vector<vertex_and_weight_of_edge>> get_adjList() const {
		return adj_list_;
	}

	std::vector<vertex_and_weight_of_edge> get_neighbors(vertex vertex) const {
		return adj_list_[vertex];
	}
};

size_t find_MST(const Graph_weight &graph) {
    
    std::priority_queue<vertex_and_weight_of_edge, std::vector<vertex_and_weight_of_edge>, std::greater<vertex_and_weight_of_edge>> next_vert;
    std::vector<bool> used(graph.get_vertex_count() + 1, 0);
    
    next_vert.push({0, 1}); //weight, num
    size_t weight_MST = 0;

    while (!next_vert.empty()) {
        int curr_vert = next_vert.top().vertex;
        int curr_dist = next_vert.top().weigth_of_outgoing_edge;
        next_vert.pop();
        if (used[curr_vert]) continue;
        used[curr_vert] = 1;
        weight_MST += curr_dist;
        auto neighbors = graph.get_neighbors(curr_vert);
        for (auto i : neighbors) {
            if (!used[i.vertex]) {
                next_vert.push({i.weigth_of_outgoing_edge, i.vertex});
            }
        }
    }
	return weight_MST;
}


int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    size_t num_vertex, num_edges;
    std::cin >> num_vertex >> num_edges;

    graph_adj_list_weight graph(num_vertex, false);

    for (size_t i = 0; i < num_edges; ++i){
        size_t from, to;
        long long len;
        std::cin >> from >> to >> len;
        graph.add_edge(from, to, len);
    }
    int answer = find_MST(graph);
    std::cout << answer;
    return 0;
}