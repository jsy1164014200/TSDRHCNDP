#pragma once 

#include<iostream>
#include<string> 
#include<vector>
#include<map> 

class Node {
public:
    std::string name; // node 的 name 
    double x; // node 的 x 坐标
    double y; // node 的 y 坐标
};

class Edge { // {i,j}
public:
    std::string v1_name; // edge 一个点的 name
    std::string v2_name; // edge 另一个点的 name
    double cost; // edge 的 fixed cost: f_{ij}
}; 

class Demand { // <p,q> 
public: 
    std::string v1_name;  // p name 
    std::string v2_name;  // q name 
};

class PrimaryGraph {
friend std::ostream& operator<<(std::ostream& output, const PrimaryGraph& pri_graph);

private:
    std::vector<Node> nodes; 
    std::vector<Edge> edges; 
    std::vector<Demand> demands; 
public:
    std::vector<std::vector<int>> link_table;
    std::map<std::pair<int,int>, double> cost_map; 
    std::vector<std::pair<int,int>> demand_pairs;


public:
    // 编辑器自己生成的构造函数
    PrimaryGraph() = default; 
    // 根据文件名字来构造
    PrimaryGraph(const std::string& file_name); 
}; 


std::ostream& operator<<(std::ostream& output, const PrimaryGraph& pri_graph); 