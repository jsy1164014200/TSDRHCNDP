#include"PrimaryGraph.h"
#include"MLAug.h"
#include"LShape.h"

#include<cfloat>  
#include<cmath>
using std::sqrt;
using std::abs; 

#include<iostream>
using std::cout;
using std::endl; 

#include<string>
using std::string; 
using std::to_string;

#include<vector>
using std::vector; 

#include<random>
using std::random_device; 
using std::mt19937; 
using std::uniform_int_distribution;

#include<chrono>
using std::chrono::steady_clock;  

#include <algorithm>
using std::sort;
using std::reverse; 
using std::accumulate; 

#include<unordered_map> 
using std::pair; 
using std::make_pair; 
using std::unordered_map; 

#include<unordered_set> 
using std::unordered_set;

#include<queue>
using std::priority_queue;

#include<fstream>
using std::fstream;
using std::ifstream;

#include<sstream>
using std::stringstream;








bool myless(const pair<double, int>& lhs, const pair<double, int>& rhs) {
    return lhs.first < rhs.first; 
}

class MyComparer {
public:
    bool operator()(const pair<double, int>& lhs, const pair<double, int>& rhs) {
        return lhs.first > rhs.first; 
    }
};








int main(int argc, char* argv[])
{ 









    PrimaryGraph pri_graph("../data/" + string(argv[1]) + ".txt"); //   "../data/abilene.txt"
    cout << pri_graph << endl; 

    vector<vector<int>> Omega; 
    // 边全是好的 
    Omega.push_back(vector<int>(pri_graph.cost_map.size(),1));  
    vector<int> one_vec(pri_graph.cost_map.size(),1);
    // 一个边坏了
    for(int i = 0; i < pri_graph.cost_map.size(); ++i) {
        one_vec[i] = 0; 
        Omega.push_back(one_vec);
        // 两个边坏了
        for(int j = i+1; j < pri_graph.cost_map.size(); ++j) {
            one_vec[j] = 0;
            Omega.push_back(one_vec);
            one_vec[j] = 1;
        }
        one_vec[i] = 1; 
    }
 

    // // 一个边坏了
    // for(int i = 0; i < pri_graph.cost_map.size(); ++i) {
    //     one_vec[i] = 0; 
    //     Omega.push_back(one_vec);
    //     // 两个边坏了
    //     for(int j = i+1; j < pri_graph.cost_map.size(); ++j) {
    //         one_vec[j] = 0;
    //         Omega.push_back(one_vec);
    //         for(int k = j+1; k < pri_graph.cost_map.size(); ++k) {
    //             one_vec[k] = 0;
    //             Omega.push_back(one_vec);
    //             for(int o = k+1; o < pri_graph.cost_map.size(); ++o) { 
    //                 one_vec[o] = 0;
    //                 Omega.push_back(one_vec);
    //                 one_vec[o] = 1;
    //             }
    //             one_vec[k] = 1;
    //         }
    //         one_vec[j] = 1;
    //     }
    //     one_vec[i] = 1; 
    // }
 
    vector<double> v_j_star(Omega.size(), 1.0/Omega.size());  
    // vector<int> Dpq(8, 8); 
    // int n_dmd = 8;
     
    vector<int> Dpq = {1, 1, 4, 4, 3, 1, 7, 2, 3, 6, 2, 1, 2, 7, 6, 1, 3, 3, 2, 3, 2, 4, 2, 1, 4, 5, 4, 1, 3, 7, 4, 2, 1, 2, 4, 4, 2, 1, 3, 5, 10}; 
    // {1,1,4,3,6,2,3,2,1,4,1,4,4,2,1};
//     C1_R5  ( C1 R5 ) 1 3580.00 UNLIMITED
//   C1_R6  ( C1 R6 ) 1 3580.00 UNLIMITED
//   C1_R7  ( C1 R7 ) 4 3580.00 UNLIMITED
//   C2_R10  ( C2 R10 ) 3 3580.00 UNLIMITED
//   C2_R12  ( C2 R12 ) 6 3580.00 UNLIMITED
//   C2_R9  ( C2 R9 ) 2 3580.00 UNLIMITED
//   C3_R15  ( C3 R15 ) 3 3580.00 UNLIMITED 
//   C4_R17  ( C4 R17 ) 2 3580.00 UNLIMITED
//   C4_R18  ( C4 R18 ) 1 3580.00 UNLIMITED
//   C5_R19  ( C5 R19 ) 4 3580.00 UNLIMITED
//   C5_R20  ( C5 R20 ) 1 3580.00 UNLIMITED
//   C6_R22  ( C6 R22 ) 4 3580.00 UNLIMITED
//   C7_R2  ( C7 R2 ) 4 3580.00 UNLIMITED
//   C7_R3  ( C7 R3 ) 2 3580.00 UNLIMITED
//   C7_R4  ( C7 R4 ) 1 3580.00 UNLIMITED 
    int n_dmd = Dpq.size();
    int fc_vc = 30; 
    int hop_parameter = 5;  // TODO 5
    double epsilong = 0.5;  








    // Omega.clear();
    // Omega.push_back(vector<int>(pri_graph.cost_map.size(),1));  
    // vector<int> opt_val{1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1};

    // vector<vector<bool>> mylink_table(pri_graph.link_table.size(), vector<bool>(pri_graph.link_table.size(), false));
    // int tmp_idx = 0; 
    // for(const auto& item : pri_graph.cost_map) { 
    //     if(opt_val[tmp_idx] == 1) { 
    //         mylink_table[item.first.first][item.first.second] = true;
    //         mylink_table[item.first.second][item.first.first] = true;
    //     }
    //     ++tmp_idx; 
    // }

    // // for(int i = 0; i < pri_graph.link_table.size(); ++i) {
    // //     for(int j = 0; j < pri_graph.link_table.size(); ++j) {
    // //         cout << mylink_table[i][j] << " ";
    // //     }
    // //     cout << endl; 
    // // }

    // // 节点数目
    // // 边数目
    // // 平均度 
    // // average hop: given
    // int n_node = 0;
    // int n_edge = 0;
    // double avg_degree = 0.0; 
    // double avg_hop = 0.0;
    // for(int i = 0; i < pri_graph.link_table.size(); ++i) {
    //     bool is_building = false;  
    //     for(int j = i+1; j < pri_graph.link_table.size(); ++j) {
    //         if(mylink_table[i][j]) {
    //             ++n_edge;
    //             is_building = true;  
    //         } 
    //     }
    //     if(is_building) ++n_node;  
    // }
    // for(int i = 0; i < pri_graph.link_table.size(); ++i) {
    //     int degree = 0;  
    //     for(int j = 0; j < pri_graph.link_table.size(); ++j) {
    //         if(mylink_table[i][j]) {
    //             ++degree; 
    //         } 
    //     }
    //     avg_degree += degree; 
    //     // cout << i << " degree: " << degree << endl; 
    // }

    
    
    // cout << "节点数目: " << n_node << endl; 
    // cout << "边数目: " << n_edge << endl; 
    // cout << "平均度: " << avg_degree / n_node << endl;  
    // solve_Q_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, opt_val); 



    // pioro40
    // vector<int> trad_solution{0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    
    // vector<int> knninte_solution{0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    // vector<int> lrinte_solution{0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    // // double trad_ov = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, trad_solution);
    // // double knninte_ov = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, knninte_solution);
    // // double lrinte_ov = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, lrinte_solution);
    // // cout << trad_ov << " " << knninte_ov << " " << lrinte_ov << endl; 

    // vector<double> tradcost = solve_Q_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, trad_solution);
    // vector<double> knnintecost = solve_Q_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, knninte_solution);
    // vector<double> lrintecost = solve_Q_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, lrinte_solution);

    // cout << accumulate(tradcost.begin(), tradcost.end(), 0.0) / Omega.size() << endl; 
    // cout << accumulate(knnintecost.begin(), knnintecost.end(), 0.0) / Omega.size() << endl; 
    // cout << accumulate(lrintecost.begin(), lrintecost.end(), 0.0) / Omega.size() << endl; 



    // sun
    // vector<int> trad_solution{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    
    // vector<int> knninte_solution{0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0};
    // vector<int> lrinte_solution{0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0};
    // // double trad_ov = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, trad_solution);
    // // double knninte_ov = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, knninte_solution);
    // // double lrinte_ov = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, lrinte_solution);
    // // cout << trad_ov << " " << knninte_ov << " " << lrinte_ov << endl; 

    // vector<double> tradcost = solve_Q_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, trad_solution);
    // vector<double> knnintecost = solve_Q_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, knninte_solution);
    // vector<double> lrintecost = solve_Q_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, lrinte_solution);

    // cout << accumulate(tradcost.begin(), tradcost.end(), 0.0) / Omega.size() << endl; 
    // cout << accumulate(knnintecost.begin(), knnintecost.end(), 0.0) / Omega.size() << endl; 
    // cout << accumulate(lrintecost.begin(), lrintecost.end(), 0.0) / Omega.size() << endl; 




    // ta2
    // vector<int> trad_solution{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    
    // vector<int> knninte_solution{1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0};
    // vector<int> lrinte_solution{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0};
    // // double trad_ov = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, trad_solution);
    // // double knninte_ov = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, knninte_solution);
    // // double lrinte_ov = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, lrinte_solution);
    // // cout << trad_ov << " " << knninte_ov << " " << lrinte_ov << endl; 

    // vector<double> tradcost = solve_Q_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, trad_solution);
    // vector<double> knnintecost = solve_Q_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, knninte_solution);
    // vector<double> lrintecost = solve_Q_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, lrinte_solution);

    // cout << accumulate(tradcost.begin(), tradcost.end(), 0.0) / Omega.size() << endl; 
    // cout << accumulate(knnintecost.begin(), knnintecost.end(), 0.0) / Omega.size() << endl; 
    // cout << accumulate(lrintecost.begin(), lrintecost.end(), 0.0) / Omega.size() << endl; 



    // zib54
    // vector<int> trad_solution{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0};
    
    // vector<int> knninte_solution{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0};
    // vector<int> lrinte_solution{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0};
    // // double trad_ov = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, trad_solution);
    // // double knninte_ov = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, knninte_solution);
    // // double lrinte_ov = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, lrinte_solution);
    // // cout << trad_ov << " " << knninte_ov << " " << lrinte_ov << endl; 

    // vector<double> tradcost = solve_Q_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, trad_solution);
    // vector<double> knnintecost = solve_Q_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, knninte_solution);
    // vector<double> lrintecost = solve_Q_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, lrinte_solution);

    // cout << accumulate(tradcost.begin(), tradcost.end(), 0.0) / Omega.size() << endl; 
    // cout << accumulate(knnintecost.begin(), knnintecost.end(), 0.0) / Omega.size() << endl; 
    // cout << accumulate(lrintecost.begin(), lrintecost.end(), 0.0) / Omega.size() << endl; 


    // rrs
    // vector<int> trad_solution{0,0,1,0,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1};
    
    // vector<int> knninte_solution{1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1};
    // vector<int> lrinte_solution{1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1};
    // // double trad_ov = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, trad_solution);
    // // double knninte_ov = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, knninte_solution);
    // // double lrinte_ov = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, lrinte_solution);
    // // cout << trad_ov << " " << knninte_ov << " " << lrinte_ov << endl; 

    // vector<double> tradcost = solve_Q_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, trad_solution);
    // vector<double> knnintecost = solve_Q_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, knninte_solution);
    // vector<double> lrintecost = solve_Q_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, lrinte_solution);

    // cout << accumulate(tradcost.begin(), tradcost.end(), 0.0) / Omega.size() << endl; 
    // cout << accumulate(knnintecost.begin(), knnintecost.end(), 0.0) / Omega.size() << endl; 
    // cout << accumulate(lrintecost.begin(), lrintecost.end(), 0.0) / Omega.size() << endl; 







    // -------------------------- Traditional Method: L Shape (callback) -------------------------- 

    // cout << "LShape Callback Solve 算法开始运行" << endl; 
    // steady_clock::time_point callback_start = steady_clock::now();
    // vector<int> callback_opt = solve_mainproblem_lshape_callback(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong);  // solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, callback_opt);
    // steady_clock::time_point callback_end = steady_clock::now();
    // cout << "LShape Callback Solve 算法结束运行" << endl; 
    // cout << "LShape Callback Solve 算法耗时: " << (callback_end - callback_start).count() / 1e9 << " 秒" << endl; 












    // -------------------------- Traditional Method: Direct Solve (gurobi) --------------------------

    // cout << "Direct Solve 算法开始运行" << endl; 
    // steady_clock::time_point ds_start = steady_clock::now();
    // vector<int> ds_opt = solve_mainproblem(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong);  // solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, ds_opt);
    // steady_clock::time_point ds_end = steady_clock::now();
    // cout << "Direct Solve 算法结束运行" << endl; 
    // cout << "Direct Solve 算法耗时: " << (ds_end - ds_start).count() / 1e9 << " 秒" << endl; 







    // -------------------------- KNN-Augmented Method -------------------------- 

    // ------------ Part 1: Generate Data -------------

    // int n_sim = 100; 
    // fstream f;
    // f.open("../results-learning/" + string(argv[1]) + "-sampled.csv", std::ios::out|std::ios::app);
    // vector<int> sampled_y(pri_graph.cost_map.size(), 1); 
    // random_device rd_y;  
    // mt19937 gen_y(rd_y());  
    // uniform_int_distribution<> dis_y(1, 10);
    // for(int sample_idx = 0; sample_idx < n_sim; ++sample_idx) {   
    //     for(int y_i = 0; y_i != pri_graph.cost_map.size(); ++y_i) {
    //         sampled_y[y_i] = dis_y(gen_y) < 6 ? 0 : 1;
    //     } 
    //     vector<double> Q_giveny = solve_Q_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, sampled_y);
    //     for(const double& item : Q_giveny) {
    //         f << item << " "; 
    //     }
    //     f << endl; 
    // }
    // f.close();  


    // ------------ Part 2: Learning -------------
    // runing reprelearning-knn.py to get T, (k=1), and a_i (i.e., k nearest neighbor)
    // will use ../output-learning


    // ------------ Part 3: Solve KNN-Augmented Problem -------------

    // cout << "KNN-Augmented 算法开始运行" << endl; 
    // steady_clock::time_point tstart = steady_clock::now(); 
    // vector<int> knnaug_result = solve_MLAug_KNN(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, string(argv[1]));
    // double knnaug_hatF = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, knnaug_result);
    // vector<int> next_result(knnaug_result.begin(), knnaug_result.end());
    // double next_target = knnaug_hatF;
    // while(true) {
    //     bool is_updated = false; 
    //     vector<int> nerb(next_result.begin(), next_result.end());
    //     for(int i = 0; i < next_result.size(); ++i) {
    //         nerb[i] = 1-nerb[i];
    //         double nerb_target = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, nerb);
    //         if(nerb_target < next_target) {
    //             next_target = nerb_target;
    //             next_result.assign(nerb.begin(), nerb.end());
    //             is_updated = true; 
    //         } 
    //         nerb[i] = 1-nerb[i];
    //     }
        
    //     if(!is_updated) break;  
    //     else {
    //         cout << "next target: " << next_target << endl; 
    //     }
    // }
    // cout << "End Objective: " << next_target << endl; 
    // cout << "y: " << endl; 
    // for(const int& item : next_result) {
    //     cout << item << ", "; 
    // }
    // steady_clock::time_point tend = steady_clock::now();
    // cout << "KNN-Augmented 算法结束运行" << endl; 
    // cout << "KNN-Augmented 算法耗时: " << (tend - tstart).count() / 1e9 << " 秒" << endl; 
    

















    // -------------------------- LR-Augmented Method -------------------------- 

    // ------------ Learning -------------
    // runing reprelearning-lr.py to get a_i  


    // cout << "LR-Augmented 算法开始运行" << endl; 
    // steady_clock::time_point tstart = steady_clock::now(); 

    // int xi = 8; 

    // // vector<vector<double>> dataset(100, vector<double>(Omega.size())); 
    // // ifstream datasetin("../results-learning/" + string(argv[1]) + "-sampled.csv"); 
    // // for(int i = 0; i < 100; ++i) {
    // //     for(int j = 0; j < Omega.size(); ++j) {
    // //         datasetin >> dataset[i][j]; 
    // //     } 
    // // }

    // // vector<int> T;
    // // vector<int> map_n_to_m(Omega.size(), -1); 
    // // ifstream fin("../output-learning/" + string(argv[1]) + "-outputsamples.txt"); 
    // // string line; 
    // // getline(fin, line); 
    // // stringstream ss(line); 
    // // int tmp; 
    // // while(ss >> tmp) {
    // //     T.push_back(tmp); 
    // // }
    // // while(getline(fin, line)) {
    // //     stringstream ss(line); 
    // //     int n, m; 
    // //     ss >> n >> m; 
    // //     map_n_to_m[n] = m; 
    // // }


    // // vector<vector<double>> feature_matrix(Omega.size(), vector<double>(xi, 0.0));    
    // // ifstream fin2("../output-learning/" + string(argv[1]) + "-feature.txt");  
    // // for(int i = 0; i != Omega.size(); ++i) {
    // //     getline(fin2, line); 
    // //     stringstream ss(line);  
    // //     for(int j = 0; j != xi; ++j) {
    // //         ss >> feature_matrix[i][j];  
    // //     }
    // // } 

    // // vector<double> trainerrors; 
    // // for(int i = 0; i < 100; ++i) {
    // //     double trainerror = solve_MLAug_LR_train(dataset[i], feature_matrix, T, xi); 
    // //     trainerrors.push_back(trainerror); 
    // // }
    // // sort(trainerrors.begin(), trainerrors.end());
    // // double L = (trainerrors[49]+trainerrors[50])/2.0; 
    // // vector<int> res = solve_MLAug_LR(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, string(argv[1]), xi, L); 
    
    // vector<int> res = solve_MLAug_LR_penal(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, string(argv[1]), xi, 0); 
    // double res_obj = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, res);

    // // L_init is optimal 
    // // while(true) {
    // //     cout << "curr best: " << res_obj << endl; 
    // //     L += 100.0; 
    // //     vector<int> next_res = solve_MLAug_LR(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, string(argv[1]), xi, L);
    // //     double next_obj =  solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, next_res);
    // //     if(next_obj < res_obj) {
    // //         res.assign(next_res.begin(), next_res.end());
    // //         res_obj = next_obj; 
    // //     }
    // //     else {
    // //         break; 
    // //     }
    // // }

    // vector<int> next_result(res.begin(), res.end());
    // double next_target = res_obj;
    // while(true) {
    //     bool is_updated = false; 
    //     vector<int> nerb(next_result.begin(), next_result.end());
    //     for(int i = 0; i < next_result.size(); ++i) {
    //         nerb[i] = 1-nerb[i];
    //         double nerb_target = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, nerb);
    //         if(nerb_target < next_target) {
    //             next_target = nerb_target;
    //             next_result.assign(nerb.begin(), nerb.end());
    //             is_updated = true; 
    //         } 
    //         nerb[i] = 1-nerb[i];
    //     }
        
    //     if(!is_updated) break;  
    //     else {
    //         cout << "next target: " << next_target << endl; 
    //     }
    // }
    // cout << "End Objective: " << next_target << endl; 
    // cout << "y: " << endl; 
    // for(const int& item : next_result) {
    //     cout << item << ", "; 
    // }
    // steady_clock::time_point tend = steady_clock::now();
    // cout << "LR-Augmented 算法结束运行" << endl; 
    // cout << "LR-Augmented 算法耗时: " << (tend - tstart).count() / 1e9 << " 秒" << endl; 
    

    


     















    // -------------------------- KNN-Integrate Method -------------------------- 

    cout << "Integrate KNN 开始运行" << endl; 
    steady_clock::time_point inte_knn_start = steady_clock::now();
    int n_sim = 100;   
    int k_max = 5; 
    int T_size = int(Omega.size() * 0.05); 
    T_size = T_size < k_max ? k_max : T_size; 

    vector<int> sampled_y(pri_graph.cost_map.size(), 1); 
    double best = DBL_MAX; 
    vector<int> ystar; 
    random_device rd_y;  
    mt19937 gen_y(rd_y());  
    uniform_int_distribution<> dis_y(1, 10);
    for(int sample_idx = 0; sample_idx < n_sim; ++sample_idx) {  
        // sample y 
        if(sample_idx != 0) { 
            for(int y_i = 0; y_i != pri_graph.cost_map.size(); ++y_i) {
                sampled_y[y_i] = dis_y(gen_y) < 6 ? 0 : 1;
            }
        } 
        double curr_obj = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, sampled_y);
        if(curr_obj < best) {
            best = curr_obj; 
            ystar.assign(sampled_y.begin(), sampled_y.end()); 
        }
    }

    while(true) {
        cout << "curr best: " << best << endl;
        vector<double> Q_giveny = solve_Q_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, ystar); 

        //one implement ---
        // int opt_k = 1; 
        // unordered_set<double> st; 
        // for(const auto& item : Q_giveny) st.insert(item);
        // cout << st.size() << " vs " << T_size << endl; 
        // if(st.size() > T_size) {
        //     int n_del = st.size() - T_size;  
        //     for(int i = 0; i < n_del; ++i) {
        //         double next_remove_val = DBL_MAX; 
        //         double next_remove = DBL_MAX; 
        //         for(const double& item : st) {
        //             double nearest_diff = DBL_MAX; 
        //             double nearest = DBL_MAX; 
        //             for(const double& nearitem : st) {
        //                 if(item == nearitem) continue; 
        //                 if(abs(nearitem - item) < nearest_diff) {
        //                     nearest_diff = abs(nearitem - item);
        //                     nearest = nearitem; 
        //                 } 
        //             }
        //             // cout << item << " " << nearest << " " << nearest_diff << endl; 
        //             if(nearest_diff < next_remove_val) {
        //                 next_remove_val = nearest_diff;
        //                 next_remove = item; 
        //             }
        //         }
        //         cout << next_remove << " " << next_remove_val << endl; 
        //         st.erase(next_remove);
        //         cout << st.size() << endl; 
        //     }
        // }

        // vector<int> T; 
        // for(const double& item : st) {
        //     for(int i = 0; i < Q_giveny.size(); ++i) {
        //         if(item == Q_giveny[i]) {
        //             T.push_back(i);
        //             break; 
        //         }
        //     }
        // }

        // vector<int> map_n_to_m(Omega.size(), -1); 
        // for(int i = 0; i < Omega.size(); ++i) {
        //     int near_idx = -1; 
        //     double near_val = DBL_MAX; 
        //     for(const int& item : T) {
        //         if(abs(Q_giveny[item]-Q_giveny[i]) < near_val) {
        //             near_idx = item; 
        //             near_val = abs(Q_giveny[item]-Q_giveny[i]); 
        //         }
        //     }
        //     map_n_to_m[i] = near_idx; 
        // }
        // for(const int& item : T) {
        //     map_n_to_m[item] = item;
        // }
        // unordered_map<int, vector<int>> mp;
        // for(int i = 0; i < Omega.size(); ++i) {
        //     mp[i].push_back(map_n_to_m[i]); 
        // }



        //another implement ---
        int opt_k = -1; 
        double opt_val = DBL_MAX; 
        vector<int> T; 
        for(int k = 1; k <= k_max; ++k) { 
            unordered_set<int> st;
            for(int un = 0; un < Omega.size(); ++un) st.insert(un); 
            while(st.size() > T_size) {
                int min_idx = -1; 
                double min_val = DBL_MAX;
                for(const int& item : st) {
                    // 找到距离item最近的k个 
                    priority_queue<pair<double, int>, vector<pair<double, int>>, MyComparer> pq; 
                    for(const int& other : st) {
                        if(other != item) {
                            pq.push({abs(Q_giveny[item]-Q_giveny[other]), other});
                        }
                    }
                    int diff = 0; 
                    for(int i = 0; i < k; ++i) {
                        pair<double, int> tmp = pq.top(); pq.pop(); 
                        diff += Q_giveny[tmp.second];
                    } 
                    diff /= k; 
                    if(abs(Q_giveny[item]-diff) < min_val) {
                        min_val = abs(Q_giveny[item]-diff);
                        min_idx = item; 
                    }
                }  
                st.erase(min_idx); 
            } 
            // T 选好了，计算 obj
            double obj = 0.0; 
            for(int item = 0; item < Omega.size(); ++item) {
                if(st.count(item) == 0) {
                    priority_queue<pair<double, int>, vector<pair<double, int>>, MyComparer> pq; 
                    for(const int& other : st) { 
                        pq.push({abs(Q_giveny[item]-Q_giveny[other]), other}); 
                    }
                    int diff = 0; 
                    for(int i = 0; i < k; ++i) {
                        pair<double, int> tmp = pq.top(); pq.pop(); 
                        diff += Q_giveny[tmp.second];
                    } 
                    diff /= k; 
                    if(abs(Q_giveny[item]-diff) > obj) obj = abs(Q_giveny[item]-diff); 
                }
            }
            for(const int& item : st) cout << item << " "; 
            cout << endl; 
            cout << obj << endl; 
            if(obj < opt_val) {
                opt_val = obj;
                opt_k = k; 
                T.assign(st.begin(), st.end()); 
            }
        }
        cout << "T: " << endl; 
        for(const int& item : T) {
            cout << item << " ";
        }cout << endl;
        cout << opt_k << ": " << opt_val << endl;

        unordered_map<int, vector<int>> mp;
        unordered_set<int> Tset; 
        for(const int& item : T) Tset.insert(item);
        for(int item = 0; item < Omega.size(); ++item) {
            if(Tset.count(item) == 0) {
                priority_queue<pair<double, int>, vector<pair<double, int>>, MyComparer> pq; 
                for(const int& other : Tset) { 
                    pq.push({abs(Q_giveny[item]-Q_giveny[other]), other}); 
                } 
                for(int i = 0; i < opt_k; ++i) {
                    pair<double, int> tmp = pq.top(); pq.pop(); 
                    mp[item].push_back(tmp.second); 
                }  
            }
        }
        for(const pair<int, vector<int>>& item : mp) {
            cout << item.first << ": ";
            for(const int& nr : item.second) {
                cout << nr << " "; 
            }
            cout << endl; 
        } 


        
        // vector<int> next_ystar = solve_MLAug_KNN_withtarget(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, string(argv[1]), T, map_n_to_m, 0);
        vector<int> next_ystar = solve_MLInte_KNN(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, string(argv[1]), opt_k, T, mp);
        double next_best = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, next_ystar);
        if(next_best < best) {
            best = next_best; 
            ystar.assign(next_ystar.begin(), next_ystar.end()); 
        }
        else {
            break; 
        } 
    }

    vector<int> next_result(ystar.begin(), ystar.end());
    double next_target = best;
    while(true) {
        bool is_updated = false; 
        vector<int> nerb(next_result.begin(), next_result.end());
        for(int i = 0; i < next_result.size(); ++i) {
            nerb[i] = 1-nerb[i];
            double nerb_target = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, nerb);
            if(nerb_target < next_target) {
                next_target = nerb_target;
                next_result.assign(nerb.begin(), nerb.end());
                is_updated = true; 
            } 
            nerb[i] = 1-nerb[i];
        }
        
        if(!is_updated) break;  
        else {
            cout << "next target: " << next_target << endl; 
        }
    }

    cout << "Integrate Objective: " << next_target << endl; 
    cout << "y: " << endl; 
    for(const int& item : next_result) {
        cout << item << ", "; 
    }
    cout << endl; 
     
    steady_clock::time_point inte_knn_end = steady_clock::now(); 
    cout << "Integrate KNN 耗时: " << (inte_knn_end - inte_knn_start).count() / 1e9 << " 秒" << endl;
     














    // -------------------------- LR-Integrate Method -------------------------- 



    // cout << "Integrate LR 开始运行" << endl; 
    // steady_clock::time_point inte_knn_start = steady_clock::now();

    // int n_sim = 100;  
    // int T_size = int(Omega.size() * 0.05);  
    // T_size = T_size < 5 ? 5 : T_size; 

    // vector<int> sampled_y(pri_graph.cost_map.size(), 1); 
    // double best = DBL_MAX; 
    // vector<int> ystar; 
    // random_device rd_y;  
    // mt19937 gen_y(rd_y());  
    // uniform_int_distribution<> dis_y(1, 10);
    // for(int sample_idx = 0; sample_idx < n_sim; ++sample_idx) {  
    //     // sample y 
    //     if(sample_idx != 0) { 
    //         for(int y_i = 0; y_i != pri_graph.cost_map.size(); ++y_i) {
    //             sampled_y[y_i] = dis_y(gen_y) < 6 ? 0 : 1;
    //         }
    //     } 
    //     double curr_obj = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, sampled_y);
    //     if(curr_obj < best) {
    //         best = curr_obj; 
    //         ystar.assign(sampled_y.begin(), sampled_y.end()); 
    //     }
    // }


    // // vector<int> ystar(pri_graph.cost_map.size(), 1); 
    // // double best = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, ystar);
    
    // while(true) {
    //     cout << "curr best: " << best << endl;
    //     vector<double> Q_giveny = solve_Q_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, ystar);
        
    //     vector<double> Q_diff_wtheta = solve_Q_giveny_theta(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, string(argv[1]), 0, 0, Q_giveny); 
    //     vector<pair<double, int>> Q_diff_wtheta_idx(Omega.size()); 
    //     for(int i = 0; i < Omega.size(); ++i) {
    //         Q_diff_wtheta_idx[i].first = Q_diff_wtheta[i];
    //         Q_diff_wtheta_idx[i].second = i; 
    //         // cout << i << ": " << Q_diff_wtheta_idx[i].first << endl; 
    //     }
    //     sort(Q_diff_wtheta_idx.begin(), Q_diff_wtheta_idx.end(), myless);
    //     reverse(Q_diff_wtheta_idx.begin(), Q_diff_wtheta_idx.end()); 
    //     for(int i = 0; i < Omega.size(); ++i) { 
    //         cout << Q_diff_wtheta_idx[i].first << " " << Q_diff_wtheta_idx[i].second << endl; 
    //     } 

    //     vector<int> T; 
    //     for(int i = 0; i < T_size; ++i) {
    //         T.push_back(Q_diff_wtheta_idx[i].second); 
    //     }
    //     cout << "T: "; 
    //     for(const int& item : T) {
    //         cout << item << ", "; 
    //     }
    //     cout << endl; 

    //     vector<int> map_n_to_m(Omega.size(), -1); 
    //     for(int i = 0; i < Omega.size(); ++i) {
    //         int near_idx = -1; 
    //         double near_val = DBL_MAX; 
    //         for(const int& item : T) {
    //             if(abs(Q_giveny[item]-Q_giveny[i]) < near_val) {
    //                 near_idx = item; 
    //                 near_val = abs(Q_giveny[item]-Q_giveny[i]); 
    //             }
    //         }
    //         map_n_to_m[i] = near_idx; 
    //     }
    //     for(const int& item : T) {
    //         map_n_to_m[item] = item;
    //     }

    //     for(const auto& item : map_n_to_m) {
    //         cout << item << endl;
    //     }
        
    //     vector<int> next_ystar = solve_MLAug_LR_withtarget(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, string(argv[1]), 0, 0, T, map_n_to_m, 0);
    //     double next_best = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, next_ystar);
    //     if(next_best < best) {
    //         best = next_best; 
    //         ystar.assign(next_ystar.begin(), next_ystar.end()); 
    //     }
    //     else {
    //         break; 
    //     }  
    // }

    // vector<int> next_result(ystar.begin(), ystar.end());
    // double next_target = best;
    // while(true) {
    //     bool is_updated = false; 
    //     vector<int> nerb(next_result.begin(), next_result.end());
    //     for(int i = 0; i < next_result.size(); ++i) {
    //         nerb[i] = 1-nerb[i];
    //         double nerb_target = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, nerb);
    //         if(nerb_target < next_target) {
    //             next_target = nerb_target;
    //             next_result.assign(nerb.begin(), nerb.end());
    //             is_updated = true; 
    //         } 
    //         nerb[i] = 1-nerb[i];
    //     }
        
    //     if(!is_updated) break;  
    //     else {
    //         cout << "next target: " << next_target << endl; 
    //     }
    // }

    // cout << "Integrate Objective: " << next_target << endl; 
    // cout << "y: " << endl; 
    // for(const int& item : next_result) {
    //     cout << item << ", "; 
    // }
    // cout << endl; 
     
    // steady_clock::time_point inte_knn_end = steady_clock::now(); 
    // cout << "Integrate LR 耗时: " << (inte_knn_end - inte_knn_start).count() / 1e9 << " 秒" << endl;




















    // cout << "Integrate LR 开始运行" << endl; 
    // steady_clock::time_point inte_knn_start = steady_clock::now();

    // int n_sim = 1;  
    // int T_size = int(Omega.size() * 0.05); 
    // if(T_size <= 8) T_size = 8; 

    // vector<int> sampled_y(pri_graph.cost_map.size(), 1); 
    // double best = DBL_MAX; 
    // vector<int> ystar; 
    // random_device rd_y;  
    // mt19937 gen_y(rd_y());  
    // uniform_int_distribution<> dis_y(1, 10);
    // for(int sample_idx = 0; sample_idx < n_sim; ++sample_idx) {  
    //     // sample y 
    //     if(sample_idx != 0) { 
    //         for(int y_i = 0; y_i != pri_graph.cost_map.size(); ++y_i) {
    //             sampled_y[y_i] = dis_y(gen_y) < 6 ? 0 : 1;
    //         }
    //     } 
    //     double curr_obj = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, sampled_y);
    //     if(curr_obj < best) {
    //         best = curr_obj; 
    //         ystar.assign(sampled_y.begin(), sampled_y.end()); 
    //     }
    // }

    // vector<double> Q_giveny = solve_Q_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, ystar);
    // vector<double> Q_diff_wtheta = solve_theta(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, string(argv[1]), Q_giveny);

 
    // while(true) {
    //     cout << "curr best: " << best << endl;
         
    //     vector<pair<double, int>> Q_diff_wtheta_idx(Omega.size()); 
    //     for(int i = 0; i < Omega.size(); ++i) {
    //         Q_diff_wtheta_idx[i].first = Q_diff_wtheta[i];
    //         Q_diff_wtheta_idx[i].second = i; 
    //         // cout << i << ": " << Q_diff_wtheta_idx[i].first << endl; 
    //     }
    //     sort(Q_diff_wtheta_idx.begin(), Q_diff_wtheta_idx.end(), myless);
    //     reverse(Q_diff_wtheta_idx.begin(), Q_diff_wtheta_idx.end()); 
    //     for(int i = 0; i < Omega.size(); ++i) { 
    //         cout << Q_diff_wtheta_idx[i].first << " " << Q_diff_wtheta_idx[i].second << endl; 
    //     } 

    //     vector<int> T; 
    //     for(int i = 0; i < T_size; ++i) {
    //         T.push_back(Q_diff_wtheta_idx[i].second); 
    //     } 

    //     vector<int> map_n_to_m(Omega.size(), -1); 
    //     for(int i = 0; i < Omega.size(); ++i) {
    //         int near_idx = -1; 
    //         double near_val = DBL_MAX; 
    //         for(const int& item : T) {
    //             if(abs(Q_giveny[item]-Q_giveny[i]) < near_val) {
    //                 near_idx = item; 
    //                 near_val = abs(Q_giveny[item]-Q_giveny[i]); 
    //             }
    //         }
    //         map_n_to_m[i] = near_idx; 
    //     }
    //     for(const int& item : T) {
    //         map_n_to_m[item] = item;
    //     }

         
        
    //     vector<double> app_Q(Omega.size());
    //     vector<int> next_ystar = solve_MLInte_LR(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, string(argv[1]), T, map_n_to_m, app_Q);
    //     double next_best = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, next_ystar);
        

    //     if(next_best < best) {
    //         best = next_best; 
    //         ystar.assign(next_ystar.begin(), next_ystar.end()); 
    //         vector<double> Q_giveny = solve_Q_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, ystar);
    //         for(int i = 0; i < Omega.size(); ++i) {
    //             Q_diff_wtheta[i] = abs(app_Q[i]-Q_giveny[i]); 
    //         } 
    //     }
    //     else {
    //         break; 
    //     }  
    // }

    // vector<int> next_result(ystar.begin(), ystar.end());
    // double next_target = best;
    // while(true) {
    //     bool is_updated = false; 
    //     vector<int> nerb(next_result.begin(), next_result.end());
    //     for(int i = 0; i < next_result.size(); ++i) {
    //         nerb[i] = 1-nerb[i];
    //         double nerb_target = solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, nerb);
    //         if(nerb_target < next_target) {
    //             next_target = nerb_target;
    //             next_result.assign(nerb.begin(), nerb.end());
    //             is_updated = true; 
    //         } 
    //         nerb[i] = 1-nerb[i];
    //     }
        
    //     if(!is_updated) break;  
    //     else {
    //         cout << "next target: " << next_target << endl; 
    //     }
    // }

    // cout << "Integrate Objective: " << next_target << endl; 
    // cout << "y: " << endl; 
    // for(const int& item : next_result) {
    //     cout << item << ", "; 
    // }
    // cout << endl; 
     
    // steady_clock::time_point inte_knn_end = steady_clock::now(); 
    // cout << "Integrate LR 耗时: " << (inte_knn_end - inte_knn_start).count() / 1e9 << " 秒" << endl;












    return EXIT_SUCCESS;
}