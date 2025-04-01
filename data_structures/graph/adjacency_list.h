#include<iostream>
#include<vector>
#include<unordered_map>
#include<fstream>

template<typename Vertex>
class FlexibleGraph {
public:
	void addEdge(const Vertex& from, const Vertex& to) {
		adj[from].push_back(to);
	}

	void print() {
		for (const auto& [person, nbs] : adj) {
			std::cout << person << "的朋友: ";
			for (const auto& nb : nbs) {
				std::cout << nb << " ";
			}
			std::cout << "\n";
		}
	}


protected:
	std::unordered_map<Vertex, std::vector<Vertex>>adj;
};

template<typename Vertex>
class usefulGraph:public FlexibleGraph<Vertex>{
public:
	//1.检查是否存在一条边
	bool hasEdge(const Vertex& from, const Vertex& to) {
		if (this->adj.find(from) == this->adj.end())return false;
		for (const auto& _friend : this->adj.at(from)) {
			if (_friend == to)return true;
		}
		return false;
	}

	//2.检查某个顶点的度
	size_t degree(const Vertex& v) const{
		return this->adj.count(v) ? this->adj.at(v).size() : 0;
	}

	//3.删除一条边
	void removeEdge(const Vertex& from, const Vertex& to) {
		if (this->adj.find(from) == this->adj.end()) return;
		auto& friends = this->adj.at(from);
		friends.erase(std::remove(friends.begin(), friends.end(), to),friends.end());
	}

};

template<typename Vertex>
void visualize(const usefulGraph<Vertex>& graph, const std::string& filename) {
	std::ofstream dotFile(filename + ".dot");
	dotFile << "digraph G{\n";

}

int main() {
	FlexibleGraph<std::string> socialNetwork;
	socialNetwork.addEdge("Alice", "Bob");
	socialNetwork.addEdge("Alice", "Charlie");
	socialNetwork.print();

	//测试usefulGraph类
	usefulGraph<std::string> userfulsocialNetwork;
	userfulsocialNetwork.addEdge("Alice", "Bob");
	userfulsocialNetwork.addEdge("Alice", "Charlie");
	std::cout << "Alice 和 Bob 之间是否有边: " << (userfulsocialNetwork.hasEdge("Alice", "Bob") ? "是" : "否") << std::endl;
	std::cout << "Alice 的度: " << userfulsocialNetwork.degree("Alice") << std::endl;
	std::cout << "Bob和Charlie之间是否有边：" << (userfulsocialNetwork.hasEdge("Bob", "Charlie") ? "是" : "否") << std::endl;
	userfulsocialNetwork.removeEdge("Alice", "Bob");
	std::cout << "Alice 和 Bob 之间是否有边: " << (userfulsocialNetwork.hasEdge("Alice", "Bob") ? "是" : "否") << std::endl;
	std::cout << "删除 Alice 到 Bob 的边后，Alice 的度: " << userfulsocialNetwork.degree("Alice") << std::endl;

}