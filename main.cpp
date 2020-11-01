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

enum colors{white, gray, black};

void dfs_visit(const Graph &graph, int start, std::list<int> &answer,
               std::vector<colors> &colors, int &flag) {
    colors[start] = gray;
    auto get_neigh = graph.get_neighbors(start);
    for (auto elem : get_neigh) {
        if (colors[elem] == white) {
            dfs_visit(graph, elem, answer, colors, flag);
        }
    }
    colors[start] = black;
    answer.push_front(start);
}

std::list<int> top_sort(const Graph &graph) {
    std::vector<colors> colors(graph.get_vertex_count() + 1, white);
    std::list<int> answer;

    for (size_t i = 1; i < graph.get_vertex_count(); ++i) {
        if (colors[i] == white) {
            int flag = 0;
            dfs_visit(graph, i, answer, colors, flag);
        }
    }
    return answer;
}


void components(const Graph &graph, std::vector<colors> &color,
                int start, std::list<int> &elements) {
    color[start] = gray;
    for (auto i : graph.get_neighbors(start)){
        if (color[i] == white) {
            elements.push_front(i);
            components(graph, color, i, elements);
        }
    }
}


int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    size_t vertex, edges;
    std::cin >> vertex >> edges;

    graph_adj_list list_edg(vertex, true);
    
    for (size_t i = 0; i < edges; ++i){
        size_t from, to;
        std::cin >> from >> to;
        list_edg.add_edge(from, to);
    }

    std::vector<colors> color(vertex + 1, white);
    std::vector<std::pair<int, int>> t_out(vertex + 1);


    std::list<int> order = top_sort(list_edg);

    graph_adj_list graph_transp(vertex + 1, true);
    for (size_t i = 1; i <= vertex; ++i) {
        auto get_negh = list_edg.get_neighbors(i);
        for (auto elem : get_negh) {
            graph_transp.add_edge(elem, i);
        }
    }

    std::vector<colors> vis_color(vertex + 1, white);
    int comp_cnt = 0;
    std::vector<int> list_comp(vertex + 1, 0);

    for (auto i : order) {
        if (vis_color[i] == white) {
            std::list<int> elements;
            elements.push_back(i);
            components(graph_transp, vis_color, i, elements);
            ++comp_cnt;
            for (auto i : elements) {
                list_comp[i] = comp_cnt;
            }
        }
    }
    std::cout << comp_cnt << '\n';
    for (size_t i = 1; i <= vertex; ++i) {
        std::cout << list_comp[i] << ' ';
    }
    return 0;
}
