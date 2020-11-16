#include <iostream>
#include <cstdlib>
#include <vector>
#include <list>
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


class Graph_adj_list : public Graph {
private:
	std::vector<std::vector<vertex>> adj_list_;

public:
	Graph_adj_list(size_t vertex_count, bool is_directed) : Graph(vertex_count, is_directed), adj_list_(vertex_count + 1) {}

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

    void remove_miltiple_edges() {
        for (Graph::vertex i = 1; i <= vertex_count; ++i) {
            std::sort(adj_list_[i].begin(), adj_list_[i].end());
            std::unique(adj_list_[i].begin(), adj_list_[i].end());
        }
    }
};


enum colors {NOT_COLOR = 0, WHITE, BLACK};

void color_graph(const Graph &graph, std::vector<colors> &color,
                 Graph::vertex start, colors cur_color, bool &cant_color){
    color[start] = cur_color;
    for (const auto &neigh : graph.get_neighbors(start)){
        if (color[neigh] == NOT_COLOR){
            cur_color = (cur_color == WHITE) ? BLACK : WHITE; 
            color_graph(graph, color, neigh, cur_color, cant_color);
        } else {
            if (color[neigh] == cur_color) {
                cant_color = true;
            }
        }
    }

}

bool is_bipartite(const Graph &graph) {
    std::vector<colors> color(graph.get_vertex_count() + 1, NOT_COLOR);
    bool cant_color = false;
    for (Graph::vertex i = 1; i <= graph.get_vertex_count(); ++i){
        if (color[i] == NOT_COLOR) {
            color_graph(graph, color, i, WHITE, cant_color);
            if (cant_color) {
                return false;
            }
        }
    }
    return true;
}


int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    size_t students_cnt, pair_students_cnt;
    std::cin >> students_cnt >> pair_students_cnt;

    Graph_adj_list exchange_graph(students_cnt, false);

    for (size_t i = 0; i < pair_students_cnt; ++i){
        size_t first_student, second_student;
        std::cin >> first_student >> second_student;
        exchange_graph.add_edge(first_student, second_student);
    }

    exchange_graph.remove_miltiple_edges();
    
    if (is_bipartite(exchange_graph)) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
    return 0;
}
