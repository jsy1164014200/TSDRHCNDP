#pragma once 

#include"PrimaryGraph.h"

#include<vector>
#include<string>




std::vector<int> solve_mainproblem_lshape_callback(const PrimaryGraph& pri_graph, const std::vector<std::vector<int>>& Omega, const std::vector<double>& v_j_star, const std::vector<int>& Dpq, const int& n_dmd, const int& fc_vc, const int& hop_parameter, const double& epsilong);
