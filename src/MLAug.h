#pragma once 

#include"PrimaryGraph.h"

#include<vector>
#include<string>
#include<unordered_map> 



std::vector<int> solve_mainproblem(const PrimaryGraph& pri_graph, const std::vector<std::vector<int>>& Omega, const std::vector<double>& v_j_star, const std::vector<int>& Dpq, const int& n_dmd, const int& fc_vc, const int& hop_parameter, const double& epsilong);

std::vector<double> solve_Q_giveny(const PrimaryGraph& pri_graph, const std::vector<std::vector<int>>& Omega, const std::vector<double>& v_j_star, const std::vector<int>& Dpq, const int& n_dmd, const int& fc_vc, const int& hop_parameter, const double& epsilong, const std::vector<int>& sampled_y);

double solve_mainproblem_giveny(const PrimaryGraph& pri_graph, const std::vector<std::vector<int>>& Omega, const std::vector<double>& v_j_star, const std::vector<int>& Dpq, const int& n_dmd, const int& fc_vc, const int& hop_parameter, const double& epsilong, const std::vector<int>& y);




// KNN-Augmented 
std::vector<int> solve_MLAug_KNN(const PrimaryGraph& pri_graph, const std::vector<std::vector<int>>& Omega, const std::vector<double>& v_j_star, const std::vector<int>& Dpq, const int& n_dmd, const int& fc_vc, const int& hop_parameter, const double& epsilong, const std::string& instance_name);
 


// LR-Augmented 
double solve_MLAug_LR_train(const std::vector<double>& dataset, const std::vector<std::vector<double>>& feature_matrix, const std::vector<int>& T, const int& xi);

std::vector<int> solve_MLAug_LR(const PrimaryGraph& pri_graph, const std::vector<std::vector<int>>& Omega, const std::vector<double>& v_j_star, const std::vector<int>& Dpq, const int& n_dmd, const int& fc_vc, const int& hop_parameter, const double& epsilong, const std::string& instance_name, const int& xi, const int& L_bar);

std::vector<int> solve_MLAug_LR_penal(const PrimaryGraph& pri_graph, const std::vector<std::vector<int>>& Omega, const std::vector<double>& v_j_star, const std::vector<int>& Dpq, const int& n_dmd, const int& fc_vc, const int& hop_parameter, const double& epsilong, const std::string& instance_name, const int& xi, const int& L_bar); 

// KNN-Integrated
std::vector<int> solve_MLInte_KNN(const PrimaryGraph& pri_graph, const std::vector<std::vector<int>>& Omega, const std::vector<double>& v_j_star, const std::vector<int>& Dpq, const int& n_dmd, const int& fc_vc, const int& hop_parameter, const double& epsilong, const std::string& instance_name, const int& opt_k, std::vector<int> T, std::unordered_map<int, std::vector<int>> mp);



// LR-Integrated 

std::vector<double> solve_Q_giveny_theta(const PrimaryGraph& pri_graph, const std::vector<std::vector<int>>& Omega, const std::vector<double>& v_j_star, const std::vector<int>& Dpq, const int& n_dmd, const int& fc_vc, const int& hop_parameter, const double& epsilong, const std::string& instance_name, const int& xi, const int& L_bar, const std::vector<double>& Q_y_w);

std::vector<int> solve_MLAug_LR_withtarget(const PrimaryGraph& pri_graph, const std::vector<std::vector<int>>& Omega, const std::vector<double>& v_j_star, const std::vector<int>& Dpq, const int& n_dmd, const int& fc_vc, const int& hop_parameter, const double& epsilong, const std::string& instance_name, const int& xi, const int& L_bar, std::vector<int> T, std::vector<int> map_n_to_m, const double& target);




std::vector<double> solve_theta(const PrimaryGraph& pri_graph, const std::vector<std::vector<int>>& Omega, const std::vector<double>& v_j_star, const std::vector<int>& Dpq, const int& n_dmd, const int& fc_vc, const int& hop_parameter, const double& epsilong, const std::string& instance_name, const std::vector<double>& Q_y_w);

std::vector<int> solve_MLInte_LR(const PrimaryGraph& pri_graph, const std::vector<std::vector<int>>& Omega, const std::vector<double>& v_j_star, const std::vector<int>& Dpq, const int& n_dmd, const int& fc_vc, const int& hop_parameter, const double& epsilong, const std::string& instance_name, std::vector<int> T, std::vector<int> map_n_to_m, std::vector<double>& app_Q);

