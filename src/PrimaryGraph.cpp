#include"PrimaryGraph.h"

#include<iostream>
using std::ostream; 
using std::endl; 
using std::cerr; 

#include<string>
using std::string; 

#include<fstream>
using std::ifstream; 

#include<unordered_map>
using std::unordered_map; 

#include<utility>
using std::make_pair; 



ostream& operator<<(ostream& output, const PrimaryGraph& pri_graph){
    output << "ALL NODES:" << endl; 
    for(const auto& tmp_node : pri_graph.nodes)
        output << tmp_node.name << " " << tmp_node.x << " " << tmp_node.y << endl; 
    output << "ALL EDGES:" << endl; 
    for(const auto& tmp_edge : pri_graph.edges)
        output << tmp_edge.v1_name << " " << tmp_edge.v2_name << " " << tmp_edge.cost << endl; 
    output << "ALL DEMANDS:" << endl; 
    for(const auto& tmp_demand : pri_graph.demands)
        output << tmp_demand.v1_name << " " << tmp_demand.v2_name << endl; 

    output << "LINK TABLE:" << endl; 
    for(int i = 0; i != pri_graph.link_table.size(); ++i) {
        output << i << ": "; 
        for(const auto& item2 : pri_graph.link_table[i]) {
            output << item2 << " ";
        }
        output << endl; 
    }

    output << "COST MAP:" << endl; 
    for(const auto& item : pri_graph.cost_map) {
        output << "(" << item.first.first << ", " << item.first.second << "): " << item.second << endl;
    }

    output << "DEMAND PAIRS:" << endl; 
    for(const auto& item : pri_graph.demand_pairs) {
        output << "(" << item.first << ", " << item.second << ")    "; 
    }
    return output;            
}


PrimaryGraph::PrimaryGraph(const string& file_name) { 
        ifstream ifs(file_name); 
        if(!ifs.is_open()) {
            cerr << "open file error" << endl; 
            return; 
        }
        string tmp_str; 

        while(getline(ifs, tmp_str)) {
            if(tmp_str == "NODES (") break;  
        }
        Node tmp_node;
        ifs >> tmp_str;
        while(tmp_str != ")") {
            tmp_node.name = tmp_str; 
            ifs >> tmp_str; 
            ifs >> tmp_node.x;
            ifs >> tmp_node.y;
            ifs >> tmp_str;
            nodes.push_back(tmp_node); 
            ifs >> tmp_str; 
        } 

        while(getline(ifs, tmp_str)) {
            if(tmp_str == "LINKS (") break;  
        }
        Edge tmp_edge; 
        ifs >> tmp_str;
        while(tmp_str != ")") {
            ifs >> tmp_str; 
            ifs >> tmp_edge.v1_name;
            ifs >> tmp_edge.v2_name; 
            getline(ifs, tmp_str);  
            edges.push_back(tmp_edge);
            ifs >> tmp_str; 
        }

        while(getline(ifs, tmp_str)) {
            if(tmp_str == "DEMANDS (") break;  
        }
        Demand tmp_demand;
        ifs >> tmp_str;
        while(tmp_str != ")") {
            ifs >> tmp_str; 
            ifs >> tmp_demand.v1_name;
            ifs >> tmp_demand.v2_name; 
            ifs >> tmp_str; ifs >> tmp_str; ifs >> tmp_str; ifs >> tmp_str;
            demands.push_back(tmp_demand); 
            ifs >> tmp_str;
        } 
        ifs.close(); 

        // 把node name 变成 索引，同时建立领接表，hashmap 存一下 cost 
        unordered_map<string, int> name2idx; 
        for(int i = 0; i != nodes.size(); ++i) {
            name2idx[nodes[i].name] = i;
        }
        link_table.resize(nodes.size()); 
        for(const auto& item : edges) {
            int idx1 = name2idx[item.v1_name];
            int idx2 = name2idx[item.v2_name];
            if(cost_map.find(make_pair(idx1, idx2)) == cost_map.end() && cost_map.find(make_pair(idx2, idx1)) == cost_map.end()) {
                link_table[idx1].push_back(idx2);
                link_table[idx2].push_back(idx1);
                if(idx1 < idx2) {
                    cost_map[make_pair(idx1, idx2)] = (nodes[idx1].x - nodes[idx2].x)*(nodes[idx1].x - nodes[idx2].x) + (nodes[idx1].y - nodes[idx2].y)*(nodes[idx1].y - nodes[idx2].y);
                } 
                else {
                    cost_map[make_pair(idx2, idx1)] = (nodes[idx1].x - nodes[idx2].x)*(nodes[idx1].x - nodes[idx2].x) + (nodes[idx1].y - nodes[idx2].y)*(nodes[idx1].y - nodes[idx2].y);
                }
            }
        }
        for(const auto& item : demands) {
            int idx1 = name2idx[item.v1_name];
            int idx2 = name2idx[item.v2_name];
            demand_pairs.push_back(make_pair(idx1, idx2)); 
        }
         
    }