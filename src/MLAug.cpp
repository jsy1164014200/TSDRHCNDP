#include"MLAug.h"
#include"PrimaryGraph.h"
#include"gurobi_c++.h" 


#include<iostream>
using std::cout;
using std::endl; 

#include<string>
using std::string; 
using std::to_string;

#include<vector>
using std::vector; 

#include<numeric>
using std::accumulate;

#include<cmath>
using std::sqrt;

#include<random>
using std::random_device; 
using std::mt19937; 
using std::uniform_int_distribution;

#include<fstream>
using std::fstream;
using std::ifstream;

#include<sstream>
using std::stringstream;

#include<cmath>
using std::round;

#include<unordered_set>
using std::unordered_set;

#include<unordered_map>
using std::unordered_map; 

std::vector<int> solve_mainproblem(const PrimaryGraph& pri_graph, const std::vector<std::vector<int>>& Omega, const std::vector<double>& v_j_star, const std::vector<int>& Dpq, const int& n_dmd, const int& fc_vc, const int& hop_parameter, const double& epsilong) {
    vector<int> result;

    double* y_lbs = new double[pri_graph.cost_map.size()];
    double* y_ubs = new double[pri_graph.cost_map.size()];
    double* y_objs = new double[pri_graph.cost_map.size()];
    char* y_types = new char[pri_graph.cost_map.size()];
    string* y_names = new string[pri_graph.cost_map.size()];
    int tmp_idx = 0; 
    for(const auto& item : pri_graph.cost_map) {
        y_lbs[tmp_idx] = 0;  
        y_ubs[tmp_idx] = 1; 
        y_objs[tmp_idx] = item.second;  
        y_types[tmp_idx] = GRB_BINARY;   
        y_names[tmp_idx] = "y_" + to_string(item.first.first) + "_" + to_string(item.first.second); 
        ++tmp_idx; 
    }
    // 打印一下所有变量的属性 看看对不对 
    // for(int i = 0; i != pri_graph.cost_map.size(); ++i) {
        // cout << "obj: " << y_objs[i] << " type: " << y_types[i] << " name: " << y_names[i] << endl;
    // } 

    double* q_i_lbs = new double[Omega.size()]; 
    double* q_i_ubs = new double[Omega.size()];
    double* q_i_objs = new double[Omega.size()];
    char* q_i_types = new char[Omega.size()];
    string* q_i_names = new string[Omega.size()]; 
    for(int i = 0; i != Omega.size(); ++i) {
        q_i_lbs[i] = -GRB_INFINITY;
        q_i_ubs[i] = GRB_INFINITY;
        q_i_objs[i] = v_j_star[i];
        q_i_types[i] = GRB_CONTINUOUS;
        q_i_names[i] = "q_" + to_string(i); 
        // cout << "obj: " << q_i_objs[i] << " type: " << q_i_types[i] << " name: " << q_i_names[i] << endl;
    }


    // 所有的x
    int n_arcs = pri_graph.cost_map.size() * 2;
    vector<vector< vector<double*> >> x_l_ij_lbs;
    vector<vector< vector<double*> >> x_l_ij_ubs; 
    vector<vector< vector<double*> >> x_l_ij_objs;
    vector<vector< vector<char*> >> x_l_ij_types; 
    vector<vector< vector<string*> >> x_l_ij_names;
    for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) {
        x_l_ij_lbs.push_back(vector< vector<double*> >());
        x_l_ij_ubs.push_back(vector< vector<double*> >());
        x_l_ij_objs.push_back(vector< vector<double*> >());
        x_l_ij_types.push_back(vector< vector<char*> >());
        x_l_ij_names.push_back(vector< vector<string*> >());
        for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {
            x_l_ij_lbs[sene_idx].push_back( vector<double*>());
            x_l_ij_ubs[sene_idx].push_back( vector<double*>());
            x_l_ij_objs[sene_idx].push_back( vector<double*>());
            x_l_ij_types[sene_idx].push_back( vector<char*>());
            x_l_ij_names[sene_idx].push_back( vector<string*>());
            for(int hop_idx = 0; hop_idx != hop_parameter; ++hop_idx){
                x_l_ij_lbs[sene_idx][dmd_idx].push_back( new double[n_arcs]);
                x_l_ij_ubs[sene_idx][dmd_idx].push_back( new double[n_arcs]);
                x_l_ij_objs[sene_idx][dmd_idx].push_back( new double[n_arcs]);
                x_l_ij_types[sene_idx][dmd_idx].push_back( new char[n_arcs]);
                x_l_ij_names[sene_idx][dmd_idx].push_back( new string[n_arcs]);
                int tmp_idx = 0; 
                for(const auto& item : pri_graph.cost_map) {
                    x_l_ij_lbs[sene_idx][dmd_idx][hop_idx][tmp_idx] = 0; 
                    x_l_ij_lbs[sene_idx][dmd_idx][hop_idx][tmp_idx+1] = 0;
                    x_l_ij_ubs[sene_idx][dmd_idx][hop_idx][tmp_idx] = 1;  
                    x_l_ij_ubs[sene_idx][dmd_idx][hop_idx][tmp_idx+1] = 1;
                    x_l_ij_objs[sene_idx][dmd_idx][hop_idx][tmp_idx] = 0; // Dpq[dmd_idx] * item.second / fc_vc; // d_pq c_{ij}
                    x_l_ij_objs[sene_idx][dmd_idx][hop_idx][tmp_idx+1] = 0; // Dpq[dmd_idx] * item.second / fc_vc; // d_pq c_{ji}
                    x_l_ij_types[sene_idx][dmd_idx][hop_idx][tmp_idx] = GRB_CONTINUOUS;  
                    x_l_ij_types[sene_idx][dmd_idx][hop_idx][tmp_idx+1] = GRB_CONTINUOUS;  
                    x_l_ij_names[sene_idx][dmd_idx][hop_idx][tmp_idx] = "x_" + to_string(sene_idx) + "_" + to_string(dmd_idx) + "_" + to_string(hop_idx) + "_" + to_string(item.first.first) + "_" + to_string(item.first.second);
                    x_l_ij_names[sene_idx][dmd_idx][hop_idx][tmp_idx+1] = "x_" + to_string(sene_idx) + "_" + to_string(dmd_idx) + "_" + to_string(hop_idx) + "_" + to_string(item.first.second) + "_" + to_string(item.first.first);
                    // cout << "obj: " << x_l_ij_objs[sene_idx][dmd_idx][hop_idx][tmp_idx] << " type: " << x_l_ij_types[sene_idx][dmd_idx][hop_idx][tmp_idx] << " name: " << x_l_ij_names[sene_idx][dmd_idx][hop_idx][tmp_idx] << endl;
                    // cout << "obj: " << x_l_ij_objs[sene_idx][dmd_idx][hop_idx][tmp_idx+1] << " type: " << x_l_ij_types[sene_idx][dmd_idx][hop_idx][tmp_idx+1] << " name: " << x_l_ij_names[sene_idx][dmd_idx][hop_idx][tmp_idx+1] << endl;
                    tmp_idx += 2; 
                } 
            }
        }
    }

     



    try {

        // Create an environment
        GRBEnv env = GRBEnv(); 
        // Create an empty model
        GRBModel model = GRBModel(env);
        
        // Create variables
        GRBVar* y =  model.addVars(y_lbs, y_ubs, y_objs, y_types, y_names, pri_graph.cost_map.size());  
        GRBVar* q_i = model.addVars(q_i_lbs, q_i_ubs, q_i_objs, q_i_types, q_i_names, Omega.size()); 
        GRBVar eta = model.addVar(0.0, GRB_INFINITY, epsilong, GRB_CONTINUOUS, "eta"); 

        vector<vector<vector<GRBVar*>>> x;
        for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) { 
            x.push_back(vector<vector<GRBVar*>>());
            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) { 
                x[sene_idx].push_back(vector<GRBVar*>());
                for(int hop_idx = 0; hop_idx != hop_parameter; ++hop_idx){ 
                    x[sene_idx][dmd_idx].push_back(     model.addVars(x_l_ij_lbs[sene_idx][dmd_idx][hop_idx], x_l_ij_ubs[sene_idx][dmd_idx][hop_idx], x_l_ij_objs[sene_idx][dmd_idx][hop_idx], x_l_ij_types[sene_idx][dmd_idx][hop_idx], x_l_ij_names[sene_idx][dmd_idx][hop_idx], n_arcs)     );
                }
            }
        } 

        vector<vector<GRBVar>> s(Omega.size(), vector<GRBVar>(n_dmd));
        for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) { 
            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) { 
                s[sene_idx][dmd_idx] = model.addVar(0, 1, 0, GRB_CONTINUOUS, "s_" + to_string(sene_idx) + "_" + to_string(dmd_idx));
            }
        }
 
        vector<GRBVar> Q_y_w(Omega.size()); 
        for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) {
            Q_y_w[sene_idx] = model.addVar(-GRB_INFINITY, GRB_INFINITY, 0, GRB_CONTINUOUS, "Q_"+to_string(sene_idx)); 
        }

   
        // Set objective 
        // GRBLinExpr obj = epsilong*eta;
        // for(int i = 0; i != pri_graph.cost_map.size(); ++i) {
        //     obj += y_objs[i]*y[i];
        // } 
        // for(int i = 0; i != Omega.size(); ++i) {
        //     obj += v_j_star[i]*q_i[i];
        // }
        // model.addConstr(obj >= 0);
        // model.setObjective(x + y + 2 * z, GRB_MAXIMIZE);
        model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);

        

        // constraints  
        for(int j = 0; j != Omega.size(); ++j) {
            for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) {
                double coef = 0.0; 
                for(int idx = 0; idx != pri_graph.cost_map.size(); ++idx) {
                    coef += (Omega[sene_idx][idx] - Omega[j][idx])*(Omega[sene_idx][idx] - Omega[j][idx]); 
                }
                coef = sqrt(coef); 
                model.addConstr(Q_y_w[sene_idx] <= coef*eta + q_i[j]); 
            }
        }



        for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) {
            GRBLinExpr gx = 0; 
            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {
                double cpq = 0; 
                for(const auto& item : pri_graph.cost_map) {
                    cpq += Dpq[dmd_idx] * item.second / fc_vc; 
                }
                gx += cpq - cpq*s[sene_idx][dmd_idx]; 
                for(int hop_idx = 0; hop_idx != hop_parameter; ++hop_idx) {
                    int tmp_idx = 0; 
                    for(const auto& item : pri_graph.cost_map) {
                        gx += x[sene_idx][dmd_idx][hop_idx][tmp_idx] * (Dpq[dmd_idx] * item.second / fc_vc);
                        gx += x[sene_idx][dmd_idx][hop_idx][tmp_idx+1] * (Dpq[dmd_idx] * item.second / fc_vc);
                        tmp_idx += 2; 
                    }
                }
            }

            model.addConstr(gx <= Q_y_w[sene_idx]);  

             



            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {
                for(int cur_node = 0; cur_node != pri_graph.link_table.size(); ++cur_node) {
                    vector<int> outgoing_cur_node; 
                    vector<int> into_cur_node; 
                    int tmp_idx = 0; 
                    for(const auto& item : pri_graph.cost_map) {
                        if(item.first.first == cur_node) {
                            outgoing_cur_node.push_back(tmp_idx);
                            into_cur_node.push_back(tmp_idx+1);
                        }
                        if(item.first.second == cur_node) {
                            into_cur_node.push_back(tmp_idx); 
                            outgoing_cur_node.push_back(tmp_idx+1); 
                        }
                        tmp_idx += 2; 
                    }

                    
                    if(cur_node == pri_graph.demand_pairs[dmd_idx].first) { 
                        GRBLinExpr tmp_linexp = 0; 
                        for(const auto& item : outgoing_cur_node)  tmp_linexp += x[sene_idx][dmd_idx][0][item]; 
                        model.addConstr( tmp_linexp - s[sene_idx][dmd_idx] == 0 );  

                        for(int i = 0; i != hop_parameter; ++i) {
                            for(const auto& item : into_cur_node) {
                                model.addConstr( x[sene_idx][dmd_idx][i][item] == 0 ); 
                            }
                        }
                    }
                    else if(cur_node == pri_graph.demand_pairs[dmd_idx].second) {
                        GRBLinExpr tmp_linexp = 0; 
                        for(int i = 0; i != hop_parameter; ++i) {
                            for(const auto& item : into_cur_node) {
                                tmp_linexp += x[sene_idx][dmd_idx][i][item]; 
                            }
                        }
                        model.addConstr( tmp_linexp - s[sene_idx][dmd_idx] == 0 );  
                        
                        for(int i = 0; i != hop_parameter; ++i) {
                            for(const auto& item : outgoing_cur_node) {
                                model.addConstr( x[sene_idx][dmd_idx][i][item] == 0 ); 
                            }
                        } 
                    }
                    else {
                        for(int i = 0; i != hop_parameter-1; ++i) {
                            GRBLinExpr lhs_linexp = 0;
                            for(const auto& item : into_cur_node) {
                                lhs_linexp += x[sene_idx][dmd_idx][i][item]; 
                            }
                            GRBLinExpr rhs_linexp = 0; 
                            for(const auto& item : outgoing_cur_node) {
                                rhs_linexp += x[sene_idx][dmd_idx][i+1][item]; 
                            }
                            model.addConstr( lhs_linexp - rhs_linexp == 0 ); 
                        }

                        for(const auto& item : outgoing_cur_node) {
                            model.addConstr( x[sene_idx][dmd_idx][0][item] == 0 );  
                        }
                    } 
                }

                for(int l = 0; l != hop_parameter; ++l) {
                    for(int i = 0; i != 2*pri_graph.cost_map.size(); ++i) { 
                        model.addConstr( x[sene_idx][dmd_idx][l][i] <= y[i/2] * Omega[sene_idx][i/2], "x_"+to_string(l)+"_"+to_string(i) ); 
                        // model.addConstr( x_l_ij[l][2*i+1] <= y[i] * omega[i], "x_"+to_string(l)+"_"+to_string(2*i+1) );
                    }
                }

            }


        }

        // Optimize model 
        // model.set(GRB_IntParam_Presolve, 0);
        // model.set(GRB_DoubleParam_Heuristics, 0);
        model.set(GRB_DoubleParam_TimeLimit, 3600*12);
            
        model.optimize();  
        cout << "Solution: " << endl; 
        double objective = 0.0;
        for(int i = 0; i != pri_graph.cost_map.size(); ++i) {  
            cout << y[i].get(GRB_StringAttr_VarName) << ": " << y[i].get(GRB_DoubleAttr_X) << endl;  
            result.push_back(round(y[i].get(GRB_DoubleAttr_X)));
            objective += y_objs[i] * (y[i].get(GRB_DoubleAttr_X));
        } 
        cout << eta.get(GRB_StringAttr_VarName) << ": " << eta.get(GRB_DoubleAttr_X) << endl; 
        for(int i = 0; i != Omega.size(); ++i) { 
            cout << q_i[i].get(GRB_StringAttr_VarName) << ": " << q_i[i].get(GRB_DoubleAttr_X) << endl;
        }
        cout << "Objective: " << model.get(GRB_DoubleAttr_ObjVal) << endl; 
        cout << "first stage cost: " << objective << endl;
 
        


    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }


 
    delete[] y_lbs;
    delete[] y_ubs; 
    delete[] y_objs; 
    delete[] y_types; 
    delete[] y_names;  
    
    delete[] q_i_lbs;
    delete[] q_i_ubs;
    delete[] q_i_objs;
    delete[] q_i_types;
    delete[] q_i_names;

    for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) { 
        for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) { 
            for(int hop_idx = 0; hop_idx != hop_parameter; ++hop_idx){ 
                delete[] x_l_ij_lbs[sene_idx][dmd_idx][hop_idx];
                delete[] x_l_ij_ubs[sene_idx][dmd_idx][hop_idx];
                delete[] x_l_ij_objs[sene_idx][dmd_idx][hop_idx];
                delete[] x_l_ij_types[sene_idx][dmd_idx][hop_idx];
                delete[] x_l_ij_names[sene_idx][dmd_idx][hop_idx]; 
            }
        }
    }

    return result; 
}


std::vector<double> solve_Q_giveny(const PrimaryGraph& pri_graph, const std::vector<std::vector<int>>& Omega, const std::vector<double>& v_j_star, const std::vector<int>& Dpq, const int& n_dmd, const int& fc_vc, const int& hop_parameter, const double& epsilong, const std::vector<int>& sampled_y) {

    vector<double> result(Omega.size(), 0); 

    
    int n_arcs = pri_graph.cost_map.size() * 2;
    double* ub = new double[n_arcs];
    for(int i = 0; i != n_arcs; ++i) {
        ub[i] = 1.0;
    }

    try {
        // // Create an environment
        // GRBEnv env = GRBEnv(); 
        // // Create an empty model
        // GRBModel model = GRBModel(env);

        GRBEnv env = GRBEnv(true); 
        env.set(GRB_IntParam_OutputFlag, 0);
        env.start();
        // Create an empty model
        GRBModel model = GRBModel(env);

        vector<vector<GRBVar*>> x(n_dmd, vector<GRBVar*>(hop_parameter, nullptr));
        for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {  
            for(int hop_idx = 0; hop_idx != hop_parameter; ++hop_idx){ 
                x[dmd_idx][hop_idx] = model.addVars(nullptr, ub, nullptr, nullptr, nullptr, n_arcs);
            }
        }
        
        vector<GRBVar> s(n_dmd);
        for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) { 
            s[dmd_idx] = model.addVar(0, 1, 0, GRB_CONTINUOUS);
        } 

        // constraints 
        for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {
            for(int cur_node = 0; cur_node != pri_graph.link_table.size(); ++cur_node) {
                vector<int> outgoing_cur_node; 
                vector<int> into_cur_node; 
                int tmp_idx = 0; 
                for(const auto& item : pri_graph.cost_map) {
                    if(item.first.first == cur_node) {
                        outgoing_cur_node.push_back(tmp_idx);
                        into_cur_node.push_back(tmp_idx+1);
                    }
                    if(item.first.second == cur_node) {
                        into_cur_node.push_back(tmp_idx); 
                        outgoing_cur_node.push_back(tmp_idx+1); 
                    }
                    tmp_idx += 2; 
                }

                
                if(cur_node == pri_graph.demand_pairs[dmd_idx].first) { 
                    GRBLinExpr tmp_linexp = 0; 
                    for(const auto& item : outgoing_cur_node)  tmp_linexp += x[dmd_idx][0][item]; 
                    model.addConstr( tmp_linexp - s[dmd_idx] == 0 );  

                    //  
                    for(int i = 0; i != hop_parameter; ++i) {
                        for(const auto& item : into_cur_node) {
                            model.addConstr( x[dmd_idx][i][item] == 0 ); 
                        }
                    }
                }
                else if(cur_node == pri_graph.demand_pairs[dmd_idx].second) {
                    GRBLinExpr tmp_linexp = 0; 
                    for(int i = 0; i != hop_parameter; ++i) {
                        for(const auto& item : into_cur_node) {
                            tmp_linexp += x[dmd_idx][i][item]; 
                        }
                    }
                    model.addConstr( tmp_linexp - s[dmd_idx] == 0 );  
                    //  
                    for(int i = 0; i != hop_parameter; ++i) {
                        for(const auto& item : outgoing_cur_node) {
                            model.addConstr( x[dmd_idx][i][item] == 0 ); 
                        }
                    } 
                }
                else {
                    for(int i = 0; i != hop_parameter-1; ++i) {
                        GRBLinExpr lhs_linexp = 0;
                        for(const auto& item : into_cur_node) {
                            lhs_linexp += x[dmd_idx][i][item]; 
                        }
                        GRBLinExpr rhs_linexp = 0; 
                        for(const auto& item : outgoing_cur_node) {
                            rhs_linexp += x[dmd_idx][i+1][item]; 
                        }
                        model.addConstr( lhs_linexp - rhs_linexp == 0 ); 
                    }

                    //  
                    for(const auto& item : outgoing_cur_node) {
                        model.addConstr( x[dmd_idx][0][item] == 0 );  
                    }
                } 
            }

        }

        // Set objective 
        GRBLinExpr gx = 0; 
        for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {
            double cpq = 0; 
            for(const auto& item : pri_graph.cost_map) {
                cpq += Dpq[dmd_idx] * item.second / fc_vc; 
            }
            gx += cpq - cpq*s[dmd_idx]; 
            for(int hop_idx = 0; hop_idx != hop_parameter; ++hop_idx) {
                int tmp_idx = 0; 
                for(const auto& item : pri_graph.cost_map) {
                    gx += x[dmd_idx][hop_idx][tmp_idx] * Dpq[dmd_idx] * item.second / fc_vc;
                    gx += x[dmd_idx][hop_idx][tmp_idx+1] * Dpq[dmd_idx] * item.second / fc_vc;
                    tmp_idx += 2; 
                }
            }
        }
        model.setObjective(gx, GRB_MINIMIZE);

        int total_statisfied = 0; 
        for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) {

            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {
                for(int l = 0; l != hop_parameter; ++l) {
                    for(int i = 0; i != 2*pri_graph.cost_map.size(); ++i) { 
                        x[dmd_idx][l][i].set(GRB_DoubleAttr_UB, sampled_y[i/2] * Omega[sene_idx][i/2]);
                        // model.addConstr( x[dmd_idx][l][i] <= sampled_y[i/2] * Omega[sene_idx][i/2], "x_"+to_string(l)+"_"+to_string(i) );  
                    }
                }
            }
            model.optimize();  
            
            int dmd_underhop = 0; 
            double avg_hop = 0.0;
            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {
                if(round(s[dmd_idx].get(GRB_DoubleAttr_X)) == 0) continue;
                // cout << "-----------dmd_idx: " << dmd_idx << "--------------"<<endl; 
                // cout << "s: " << s[dmd_idx].get(GRB_DoubleAttr_X) << endl; 
                ++dmd_underhop;
                int used_hop = -1;
                for(int hop_idx = 0; hop_idx < hop_parameter; ++hop_idx) {
                    // cout << hop_idx << ": ";  
                    for(int i = 0; i < n_arcs; ++i) {
                        // cout << x[dmd_idx][hop_idx][i].get(GRB_DoubleAttr_X) << " "; 
                        if(round(x[dmd_idx][hop_idx][i].get(GRB_DoubleAttr_X)) == 1) used_hop = hop_idx+1; 
                    } 
                    // cout << endl; 
                }
                avg_hop += used_hop; 
                // cout << "used_hop: " << used_hop << endl; 
            }
            avg_hop /= dmd_underhop; 
            cout << "满足hop的需求数量: " << dmd_underhop << endl; 
            cout << "avg hop: " << avg_hop << endl; 

            total_statisfied += dmd_underhop; 
            
            result[sene_idx] = model.get(GRB_DoubleAttr_ObjVal); 
        }
        cout << "满足的total demand: " << total_statisfied << endl; 
        cout << "不同fail场景下平均：" << double(total_statisfied) / Omega.size() << endl; 
        
    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }

    delete[] ub; 
    return result;
}


double solve_mainproblem_giveny(const PrimaryGraph& pri_graph, const std::vector<std::vector<int>>& Omega, const std::vector<double>& v_j_star, const std::vector<int>& Dpq, const int& n_dmd, const int& fc_vc, const int& hop_parameter, const double& epsilong, const std::vector<int>& y) {
    double result = 0.0; 
    // vector<int> tightT;
    // unordered_set<int> tmptightT; 

    double* q_i_lbs = new double[Omega.size()]; 
    double* q_i_ubs = new double[Omega.size()];
    double* q_i_objs = new double[Omega.size()];
    char* q_i_types = new char[Omega.size()];
    string* q_i_names = new string[Omega.size()]; 
    for(int i = 0; i != Omega.size(); ++i) {
        q_i_lbs[i] = -GRB_INFINITY;
        q_i_ubs[i] = GRB_INFINITY;
        q_i_objs[i] = v_j_star[i];
        q_i_types[i] = GRB_CONTINUOUS;
        q_i_names[i] = "q_" + to_string(i); 
        // cout << "obj: " << q_i_objs[i] << " type: " << q_i_types[i] << " name: " << q_i_names[i] << endl;
    }


    vector<double> Q_y_w = solve_Q_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, y); 

     



    try {

        // // Create an environment
        // GRBEnv env = GRBEnv(); 
        // // Create an empty model
        // GRBModel model = GRBModel(env);

        GRBEnv env = GRBEnv(true); 
        env.set(GRB_IntParam_OutputFlag, 0);
        env.start();
        // Create an empty model
        GRBModel model = GRBModel(env);
        
        // Create variables
        GRBVar* q_i = model.addVars(q_i_lbs, q_i_ubs, q_i_objs, q_i_types, q_i_names, Omega.size()); 
        GRBVar eta = model.addVar(0.0, GRB_INFINITY, epsilong, GRB_CONTINUOUS, "eta"); 

        // Set objective 
        // model.setObjective(x + y + 2 * z, GRB_MAXIMIZE);
        model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);

        // GRBLinExpr obj = epsilong*eta;
        // for(int i = 0; i != pri_graph.cost_map.size(); ++i) {
        //     obj += y_objs[i]*y[i];
        // } 
        // for(int i = 0; i != Omega.size(); ++i) {
        //     obj += v_j_star[i]*q_i[i];
        // }
        // model.addConstr(obj >= 0);

        // constraints  
        for(int j = 0; j != Omega.size(); ++j) {
            for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) {
                double coef = 0.0; 
                for(int idx = 0; idx != pri_graph.cost_map.size(); ++idx) {
                    coef += (Omega[sene_idx][idx] - Omega[j][idx])*(Omega[sene_idx][idx] - Omega[j][idx]); 
                }
                coef = sqrt(coef); 
                model.addConstr(Q_y_w[sene_idx] <= coef*eta + q_i[j]); 
            }
        }
         

        // Optimize model 
        // model.set(GRB_IntParam_Presolve, 0);
        // model.set(GRB_DoubleParam_Heuristics, 0);
        model.set(GRB_DoubleParam_TimeLimit, 3600*12);
        model.optimize();  
        // cout << "Solution: " << endl; 

        // double curr_eta = eta.get(GRB_DoubleAttr_X); 
        // for(int j = 0; j != Omega.size(); ++j) {
        //     double curr_q_i = q_i[j].get(GRB_DoubleAttr_X); 
        //     for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) {
        //         double coef = 0.0; 
        //         for(int idx = 0; idx != pri_graph.cost_map.size(); ++idx) {
        //             coef += (Omega[sene_idx][idx] - Omega[j][idx])*(Omega[sene_idx][idx] - Omega[j][idx]); 
        //         }
        //         coef = sqrt(coef); 
        //         double diff = coef*curr_eta + curr_q_i - Q_y_w[sene_idx];
        //         if(diff <= 0.0001) { 
        //             tmptightT.insert(sene_idx);
        //         } 
        //     }
        // }
        // tightT.assign(tmptightT.begin(), tmptightT.end()); 


        // cout << "Solution: " << endl; 
        double objective = 0.0;
        int tmp_idx = 0;
        for(const auto& item : pri_graph.cost_map) {
            objective += item.second * y[tmp_idx];
            ++tmp_idx;
        }
        // for(int i = 0; i != pri_graph.cost_map.size(); ++i) {    
        //     objective += pri_graph.cost_map[i].second * y[i];
        // } 
        // cout << eta.get(GRB_StringAttr_VarName) << ": " << eta.get(GRB_DoubleAttr_X) << endl; 
        // for(int i = 0; i != Omega.size(); ++i) { 
        //     cout << q_i[i].get(GRB_StringAttr_VarName) << ": " << q_i[i].get(GRB_DoubleAttr_X) << endl;
        // }
        result = model.get(GRB_DoubleAttr_ObjVal) + objective; 
        // cout << "End Objective: " << model.get(GRB_DoubleAttr_ObjVal) + objective << endl;  
 
        // fstream fout; 
        // fout.open("../results-ssa/atlanta", std::ios::app); 
        // fout << model.get(GRB_DoubleAttr_ObjVal) + objective << endl; 
        // fout.close(); 


    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }

    
    delete[] q_i_lbs;
    delete[] q_i_ubs;
    delete[] q_i_objs;
    delete[] q_i_types;
    delete[] q_i_names;
    return result; 
    
}








// KNN-Augmented 
std::vector<int> solve_MLAug_KNN(const PrimaryGraph& pri_graph, const std::vector<std::vector<int>>& Omega, const std::vector<double>& v_j_star, const std::vector<int>& Dpq, const int& n_dmd, const int& fc_vc, const int& hop_parameter, const double& epsilong, const std::string& instance_name) {
    vector<int> result;  

    vector<int> T;
    vector<int> map_n_to_m(Omega.size(), -1); 
    ifstream fin("../output-learning/" + instance_name + "-outputsamples.txt"); 
    string line; 
    getline(fin, line); 
    stringstream ss(line); 
    int tmp; 
    while(ss >> tmp) {
        T.push_back(tmp); 
    }
    while(getline(fin, line)) {
        stringstream ss(line); 
        int n, m; 
        ss >> n >> m; 
        map_n_to_m[n] = m; 
    }
    cout << "Omege size: " << Omega.size() << endl;
    for(const int& item : T) {
        cout << item << " "; 
    }
    cout << endl; 
    
    for(const int& item : map_n_to_m) {
        cout << item << " "; 
    }
    cout << endl; 

    int n_arcs = pri_graph.cost_map.size() * 2;
    double* ub = new double[n_arcs];
    for(int i = 0; i != n_arcs; ++i) {
        ub[i] = 1.0;
    }

    try {

        // Create an environment
        GRBEnv env = GRBEnv(); 
        // Create an empty model
        GRBModel model = GRBModel(env);
        
        // Create variables 
        GRBVar* y =  model.addVars(pri_graph.cost_map.size(), GRB_BINARY); 

        vector<double> y_objs(pri_graph.cost_map.size()); 
        int tmp_idx = 0; 
        for(const auto& item : pri_graph.cost_map) {
            y_objs[tmp_idx] = item.second;
            y[tmp_idx].set(GRB_DoubleAttr_Obj, item.second);
            y[tmp_idx].set(GRB_StringAttr_VarName, "y_" + to_string(item.first.first) + "_" + to_string(item.first.second));
            ++tmp_idx; 
        } 
 

        GRBVar* q_i = model.addVars(Omega.size(), GRB_CONTINUOUS); 
        for(int i = 0; i != Omega.size(); ++i) {
            q_i[i].set(GRB_DoubleAttr_LB, -GRB_INFINITY);
            q_i[i].set(GRB_DoubleAttr_Obj, v_j_star[i]);
            q_i[i].set(GRB_StringAttr_VarName, "q_" + to_string(i));
        }

        GRBVar eta = model.addVar(0.0, GRB_INFINITY, epsilong, GRB_CONTINUOUS, "eta"); 


        vector<vector<vector<GRBVar*>>> x;
        for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) {
            x.push_back(vector<vector<GRBVar*>>());
        }
        for(const int& sene_idx : T) { 
            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) { 
                x[sene_idx].push_back(vector<GRBVar*>());
                for(int hop_idx = 0; hop_idx != hop_parameter; ++hop_idx){ 
                    x[sene_idx][dmd_idx].push_back(     model.addVars(nullptr, nullptr, nullptr, nullptr, nullptr, n_arcs)     );
                }
            }
        } 

     
        vector<GRBVar*> s(Omega.size());
        for(const int& sene_idx : T) { 
            s[sene_idx] = model.addVars(n_dmd, GRB_CONTINUOUS);
            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) { 
                s[sene_idx][dmd_idx].set(GRB_DoubleAttr_UB, 1);
                s[sene_idx][dmd_idx].set(GRB_StringAttr_VarName, "s_" + to_string(sene_idx) + "_" + to_string(dmd_idx));
            }
        }

 



        GRBVar* Q_y_w = model.addVars(Omega.size(), GRB_CONTINUOUS); 
        for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) {
            Q_y_w[sene_idx].set(GRB_DoubleAttr_LB, -GRB_INFINITY);
            Q_y_w[sene_idx].set(GRB_StringAttr_VarName, "Q_"+to_string(sene_idx)); 
        }

  

   
        // Set objective 
        // model.setObjective(x + y + 2 * z, GRB_MAXIMIZE);
        model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);

        // GRBLinExpr obj = epsilong*eta;
        // for(int i = 0; i != pri_graph.cost_map.size(); ++i) {
        //     obj += y_objs[i]*y[i];
        // } 
        // for(int i = 0; i != Omega.size(); ++i) {
        //     obj += v_j_star[i]*q_i[i];
        // }
        // model.addConstr(obj >= 0);

        // constraints  
        for(int j = 0; j != Omega.size(); ++j) {
            for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) {
                double coef = 0.0; 
                for(int idx = 0; idx != pri_graph.cost_map.size(); ++idx) {
                    coef += (Omega[sene_idx][idx] - Omega[j][idx])*(Omega[sene_idx][idx] - Omega[j][idx]); 
                }
                coef = sqrt(coef); 
                model.addConstr(Q_y_w[map_n_to_m[sene_idx]] <= coef*eta + q_i[j]); 
            }
        }
 


        for(const int& sene_idx : T) {

            GRBLinExpr gx = 0; 
            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {
                double cpq = 0; 
                for(const auto& item : pri_graph.cost_map) {
                    cpq += Dpq[dmd_idx] * item.second / fc_vc; 
                }
                gx += cpq - cpq*s[sene_idx][dmd_idx]; 
                for(int hop_idx = 0; hop_idx != hop_parameter; ++hop_idx) {
                    int tmp_idx = 0; 
                    for(const auto& item : pri_graph.cost_map) {
                        gx += x[sene_idx][dmd_idx][hop_idx][tmp_idx] * Dpq[dmd_idx] * item.second / fc_vc;
                        gx += x[sene_idx][dmd_idx][hop_idx][tmp_idx+1] * Dpq[dmd_idx] * item.second / fc_vc;
                        tmp_idx += 2; 
                    }
                }
            }
            model.addConstr(gx == Q_y_w[sene_idx]); 



            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {
                for(int cur_node = 0; cur_node != pri_graph.link_table.size(); ++cur_node) {
                    vector<int> outgoing_cur_node; 
                    vector<int> into_cur_node; 
                    int tmp_idx = 0; 
                    for(const auto& item : pri_graph.cost_map) {
                        if(item.first.first == cur_node) {
                            outgoing_cur_node.push_back(tmp_idx);
                            into_cur_node.push_back(tmp_idx+1);
                        }
                        if(item.first.second == cur_node) {
                            into_cur_node.push_back(tmp_idx); 
                            outgoing_cur_node.push_back(tmp_idx+1); 
                        }
                        tmp_idx += 2; 
                    }

                    
                    if(cur_node == pri_graph.demand_pairs[dmd_idx].first) { 
                        GRBLinExpr tmp_linexp = 0; 
                        for(const auto& item : outgoing_cur_node)  tmp_linexp += x[sene_idx][dmd_idx][0][item]; 
                        model.addConstr( tmp_linexp - s[sene_idx][dmd_idx] == 0 );  

                        //  
                        for(int i = 0; i != hop_parameter; ++i) {
                            for(const auto& item : into_cur_node) {
                                model.addConstr( x[sene_idx][dmd_idx][i][item] == 0 ); 
                            }
                        }
                    }
                    else if(cur_node == pri_graph.demand_pairs[dmd_idx].second) {
                        GRBLinExpr tmp_linexp = 0; 
                        for(int i = 0; i != hop_parameter; ++i) {
                            for(const auto& item : into_cur_node) {
                                tmp_linexp += x[sene_idx][dmd_idx][i][item]; 
                            }
                        }
                        model.addConstr( tmp_linexp - s[sene_idx][dmd_idx] == 0 );  
                        //  
                        for(int i = 0; i != hop_parameter; ++i) {
                            for(const auto& item : outgoing_cur_node) {
                                model.addConstr( x[sene_idx][dmd_idx][i][item] == 0 ); 
                            }
                        } 
                    }
                    else {
                        for(int i = 0; i != hop_parameter-1; ++i) {
                            GRBLinExpr lhs_linexp = 0;
                            for(const auto& item : into_cur_node) {
                                lhs_linexp += x[sene_idx][dmd_idx][i][item]; 
                            }
                            GRBLinExpr rhs_linexp = 0; 
                            for(const auto& item : outgoing_cur_node) {
                                rhs_linexp += x[sene_idx][dmd_idx][i+1][item]; 
                            }
                            model.addConstr( lhs_linexp - rhs_linexp == 0 ); 
                        }

                        //  
                        for(const auto& item : outgoing_cur_node) {
                            model.addConstr( x[sene_idx][dmd_idx][0][item] == 0 );  
                        }
                    } 
                }

                for(int l = 0; l != hop_parameter; ++l) {
                    for(int i = 0; i != 2*pri_graph.cost_map.size(); ++i) { 
                        model.addConstr( x[sene_idx][dmd_idx][l][i] <= y[i/2] * Omega[sene_idx][i/2], "x_"+to_string(l)+"_"+to_string(i) ); 
                        // model.addConstr( x_l_ij[l][2*i+1] <= y[i] * omega[i], "x_"+to_string(l)+"_"+to_string(2*i+1) );
                    }
                }

            }
  
        }

        
 

        // Optimize model 
        // model.set(GRB_IntParam_Presolve, 0);
        // model.set(GRB_DoubleParam_Heuristics, 0);
        model.set(GRB_DoubleParam_TimeLimit, 3600*12);
            
        model.optimize();  
        cout << "Solution: " << endl; 
        double objective = 0.0;
        for(int i = 0; i != pri_graph.cost_map.size(); ++i) {  
            cout << y[i].get(GRB_StringAttr_VarName) << ": " << y[i].get(GRB_DoubleAttr_X) << endl; 
            objective += y_objs[i] * (y[i].get(GRB_DoubleAttr_X));
            result.push_back(round(y[i].get(GRB_DoubleAttr_X)));
        } 
        cout << eta.get(GRB_StringAttr_VarName) << ": " << eta.get(GRB_DoubleAttr_X) << endl; 
        for(int i = 0; i != Omega.size(); ++i) { 
            cout << q_i[i].get(GRB_StringAttr_VarName) << ": " << q_i[i].get(GRB_DoubleAttr_X) << endl;
        }
         
        for(const int& item : T) {
            cout << Q_y_w[item].get(GRB_StringAttr_VarName) << ": " << Q_y_w[item].get(GRB_DoubleAttr_X) << endl;
        }  


        cout << "Objective: " << model.get(GRB_DoubleAttr_ObjVal) << endl; 
        cout << "first stage cost: " << objective << endl;
 
        


    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }


 
    delete[] ub;
    return result; 

}




// LR-Augmented 
double solve_MLAug_LR_train(const std::vector<double>& dataset, const std::vector<std::vector<double>>& feature_matrix, const std::vector<int>& T, const int& xi) {
    double trainerror = 0.0; 

    try {
        // // Create an environment
        // GRBEnv env = GRBEnv(); 
        // // Create an empty model
        // GRBModel model = GRBModel(env); 

        GRBEnv env = GRBEnv(true); 
        env.set(GRB_IntParam_OutputFlag, 0);
        env.start();
        // Create an empty model
        GRBModel model = GRBModel(env);


        GRBVar* theta1 = model.addVars(xi, GRB_CONTINUOUS); 
        for(int i = 0; i != xi; ++i) {
            theta1[i].set(GRB_DoubleAttr_LB, -GRB_INFINITY); 
            theta1[i].set(GRB_StringAttr_VarName, "theta1_" + to_string(i));
        }
 
        GRBVar* abs = model.addVars(dataset.size(), GRB_CONTINUOUS); 
        for(int i = 0; i != dataset.size(); ++i) { 
            abs[i].set(GRB_StringAttr_VarName, "abs_" + to_string(i));
        }
 

        for(const int& i : T) {
            GRBLinExpr tmp_linexp = 0;  
            for(int j = 0; j != xi; ++j) {
                tmp_linexp += theta1[j] * feature_matrix[i][j]; 
            } 
            model.addConstr(tmp_linexp - dataset[i] <= abs[i]); 
            model.addConstr(dataset[i] - tmp_linexp <= abs[i]);
        }

        GRBLinExpr obj = 0; 
        for(const int& i : T) {
            obj += abs[i]; 
        }
        model.setObjective(obj, GRB_MINIMIZE); 

        model.set(GRB_DoubleParam_TimeLimit, 3600*12);
            
        model.optimize();  
        // cout << "Objective: " << model.get(GRB_DoubleAttr_ObjVal) << endl;  

        
        for(const int& i : T) {
            trainerror += abs[i].get(GRB_DoubleAttr_X); 
        }
        trainerror /= T.size(); 

    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }


    return trainerror; 
}



std::vector<int> solve_MLAug_LR(const PrimaryGraph& pri_graph, const std::vector<std::vector<int>>& Omega, const std::vector<double>& v_j_star, const std::vector<int>& Dpq, const int& n_dmd, const int& fc_vc, const int& hop_parameter, const double& epsilong, const std::string& instance_name, const int& xi, const int& L_bar) {
    vector<int> result; 

    vector<int> T;
    vector<int> notT;
    vector<int> map_n_to_m(Omega.size(), -1);
    vector<int> weight_m(Omega.size(), 0); 

    ifstream fin("../output-learning/" + instance_name + "-outputsamples.txt"); 
    string line; 
    getline(fin, line); 
    stringstream ss(line); 
    int tmp; 
    while(ss >> tmp) {
        T.push_back(tmp); 
    }
    
    while(getline(fin, line)) {
        stringstream ss(line); 
        int n, m; 
        ss >> n >> m; 
        map_n_to_m[n] = m; 
        ++weight_m[m]; 
        if(n != m) {
            notT.push_back(n); 
        }
    }
    // cout << "T: " << endl; 
    // for(const int& item : T) {
    //     cout << item << " "; 
    // }
    // cout << endl; 
    // cout << "T size: " << T.size() << endl;  

    // cout << "notT: " << endl; 
    // for(const int& item : notT) {
    //     cout << item << " "; 
    // }
    // cout << endl; 
    // cout << "notT size: " << notT.size() << endl; 
    
    // for(const int& item : map_n_to_m) {
    //     cout << item << " "; 
    // }
    // cout << endl; 

    // int sum_weight = 0; 
    // for(const int& item : weight_m) {
    //     cout << item << " "; 
    //     sum_weight += item; 
    // }
    // cout << endl; 
    // cout << sum_weight << endl;  

     



    vector<vector<double>> feature_matrix(Omega.size(), vector<double>(xi, 0.0));    
    ifstream fin2("../output-learning/" + instance_name + "-feature.txt");  
    for(int i = 0; i != Omega.size(); ++i) {
        getline(fin2, line); 
        stringstream ss(line);  
        for(int j = 0; j != xi; ++j) {
            ss >> feature_matrix[i][j];  
        }
    } 
    // for(int i = 0; i != Omega.size(); ++i) {
    //     for(int j = 0; j != xi; ++j) {
    //         cout << feature_matrix[i][j] << " "; 
    //     }
    //     cout << endl; 
    // } 

    

    int n_arcs = pri_graph.cost_map.size() * 2;
    double* ub = new double[n_arcs];
    for(int i = 0; i != n_arcs; ++i) {
        ub[i] = 1.0;
    }

    try {

        // Create an environment
        GRBEnv env = GRBEnv(); 
        // Create an empty model
        GRBModel model = GRBModel(env);
        
        // Create variables 
        GRBVar* y =  model.addVars(pri_graph.cost_map.size(), GRB_BINARY); 

        vector<double> y_objs(pri_graph.cost_map.size()); 
        int tmp_idx = 0; 
        for(const auto& item : pri_graph.cost_map) {
            y_objs[tmp_idx] = item.second;
            y[tmp_idx].set(GRB_DoubleAttr_Obj, item.second);
            y[tmp_idx].set(GRB_StringAttr_VarName, "y_" + to_string(item.first.first) + "_" + to_string(item.first.second));
            ++tmp_idx; 
        } 
 

        GRBVar* q_i = model.addVars(Omega.size(), GRB_CONTINUOUS); 
        for(int i = 0; i != Omega.size(); ++i) {
            q_i[i].set(GRB_DoubleAttr_LB, -GRB_INFINITY);
            q_i[i].set(GRB_DoubleAttr_Obj, v_j_star[i]);
            q_i[i].set(GRB_StringAttr_VarName, "q_" + to_string(i));
        }

        GRBVar eta = model.addVar(0.0, GRB_INFINITY, epsilong, GRB_CONTINUOUS, "eta"); 
 

        GRBVar* theta = model.addVars(xi, GRB_CONTINUOUS); 
        for(int i = 0; i != xi; ++i) {
            theta[i].set(GRB_DoubleAttr_LB, -GRB_INFINITY); 
            theta[i].set(GRB_StringAttr_VarName, "theta_" + to_string(i));
        }

        vector<vector<vector<GRBVar*>>> x;
        for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) {
            x.push_back(vector<vector<GRBVar*>>());
        }
        for(const int& sene_idx : T) { 
            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) { 
                x[sene_idx].push_back(vector<GRBVar*>());
                for(int hop_idx = 0; hop_idx != hop_parameter; ++hop_idx){ 
                    x[sene_idx][dmd_idx].push_back(     model.addVars(nullptr, nullptr, nullptr, nullptr, nullptr, n_arcs)     );
                }
            }
        } 

     
        vector<GRBVar*> s(Omega.size());
        for(const int& sene_idx : T) { 
            s[sene_idx] = model.addVars(n_dmd, GRB_CONTINUOUS);
            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) { 
                s[sene_idx][dmd_idx].set(GRB_DoubleAttr_UB, 1);
                s[sene_idx][dmd_idx].set(GRB_StringAttr_VarName, "s_" + to_string(sene_idx) + "_" + to_string(dmd_idx));
            }
        }



        GRBVar* Q_y_w = model.addVars(Omega.size(), GRB_CONTINUOUS); 
        for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) {
            Q_y_w[sene_idx].set(GRB_DoubleAttr_LB, -GRB_INFINITY);
            Q_y_w[sene_idx].set(GRB_StringAttr_VarName, "Q_"+to_string(sene_idx)); 
        }

        GRBVar* atheta = model.addVars(Omega.size(), GRB_CONTINUOUS); 
        for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) {
            atheta[sene_idx].set(GRB_DoubleAttr_LB, -GRB_INFINITY);
            atheta[sene_idx].set(GRB_StringAttr_VarName, "atheta_"+to_string(sene_idx)); 
        }

        GRBVar* abs = model.addVars(Omega.size(), GRB_CONTINUOUS); 
        for(int i = 0; i != Omega.size(); ++i) { 
            abs[i].set(GRB_StringAttr_VarName, "abs_" + to_string(i));
        }

   
        // Set objective 
        // model.setObjective(x + y + 2 * z, GRB_MAXIMIZE);
        model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);


        // constraints  
        for(int j = 0; j != Omega.size(); ++j) {
            for(const int& m : T) {
                double coef = 0.0; 
                for(int idx = 0; idx != pri_graph.cost_map.size(); ++idx) {
                    coef += (Omega[m][idx] - Omega[j][idx])*(Omega[m][idx] - Omega[j][idx]); 
                }
                coef = sqrt(coef); 
                model.addConstr(Q_y_w[m] <= coef*eta + q_i[j]); 
            }

            for(const int& n : notT) {    
                double coef = 0.0; 
                for(int idx = 0; idx != pri_graph.cost_map.size(); ++idx) {
                    coef += (Omega[n][idx] - Omega[j][idx])*(Omega[n][idx] - Omega[j][idx]); 
                }
                coef = sqrt(coef); 
                model.addConstr(atheta[n] <= coef*eta + q_i[j]);  
            }
        }

        GRBLinExpr endcons = 0;
        for(const int& m : T) { 
            model.addConstr(abs[m] >= Q_y_w[m]-atheta[m]);  
            model.addConstr(abs[m] >= atheta[m]-Q_y_w[m]); 
            endcons += weight_m[m]*abs[m]; 
        }
        model.addConstr(endcons <= L_bar*Omega.size());   


        for(int i = 0; i < Omega.size(); ++i) {
            GRBLinExpr app_gx = 0;  
            for(int j = 0; j != xi; ++j) {
                app_gx += theta[j]*feature_matrix[i][j]; 
            }
            model.addConstr(app_gx == atheta[i]); 
        }


        for(const int& sene_idx : T) {

            GRBLinExpr gx = 0; 
            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {
                double cpq = 0; 
                for(const auto& item : pri_graph.cost_map) {
                    cpq += Dpq[dmd_idx] * item.second / fc_vc; 
                }
                gx += cpq - cpq*s[sene_idx][dmd_idx]; 
                for(int hop_idx = 0; hop_idx != hop_parameter; ++hop_idx) {
                    int tmp_idx = 0; 
                    for(const auto& item : pri_graph.cost_map) {
                        gx += x[sene_idx][dmd_idx][hop_idx][tmp_idx] * Dpq[dmd_idx] * item.second / fc_vc;
                        gx += x[sene_idx][dmd_idx][hop_idx][tmp_idx+1] * Dpq[dmd_idx] * item.second / fc_vc;
                        tmp_idx += 2; 
                    }
                }
            }
            model.addConstr(gx == Q_y_w[sene_idx]); 



            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {
                for(int cur_node = 0; cur_node != pri_graph.link_table.size(); ++cur_node) {
                    vector<int> outgoing_cur_node; 
                    vector<int> into_cur_node; 
                    int tmp_idx = 0; 
                    for(const auto& item : pri_graph.cost_map) {
                        if(item.first.first == cur_node) {
                            outgoing_cur_node.push_back(tmp_idx);
                            into_cur_node.push_back(tmp_idx+1);
                        }
                        if(item.first.second == cur_node) {
                            into_cur_node.push_back(tmp_idx); 
                            outgoing_cur_node.push_back(tmp_idx+1); 
                        }
                        tmp_idx += 2; 
                    }

                    
                    if(cur_node == pri_graph.demand_pairs[dmd_idx].first) { 
                        GRBLinExpr tmp_linexp = 0; 
                        for(const auto& item : outgoing_cur_node)  tmp_linexp += x[sene_idx][dmd_idx][0][item]; 
                        model.addConstr( tmp_linexp - s[sene_idx][dmd_idx] == 0 );  

                        //  
                        for(int i = 0; i != hop_parameter; ++i) {
                            for(const auto& item : into_cur_node) {
                                model.addConstr( x[sene_idx][dmd_idx][i][item] == 0 ); 
                            }
                        }
                    }
                    else if(cur_node == pri_graph.demand_pairs[dmd_idx].second) {
                        GRBLinExpr tmp_linexp = 0; 
                        for(int i = 0; i != hop_parameter; ++i) {
                            for(const auto& item : into_cur_node) {
                                tmp_linexp += x[sene_idx][dmd_idx][i][item]; 
                            }
                        }
                        model.addConstr( tmp_linexp - s[sene_idx][dmd_idx] == 0 );  
                        //  
                        for(int i = 0; i != hop_parameter; ++i) {
                            for(const auto& item : outgoing_cur_node) {
                                model.addConstr( x[sene_idx][dmd_idx][i][item] == 0 ); 
                            }
                        } 
                    }
                    else {
                        for(int i = 0; i != hop_parameter-1; ++i) {
                            GRBLinExpr lhs_linexp = 0;
                            for(const auto& item : into_cur_node) {
                                lhs_linexp += x[sene_idx][dmd_idx][i][item]; 
                            }
                            GRBLinExpr rhs_linexp = 0; 
                            for(const auto& item : outgoing_cur_node) {
                                rhs_linexp += x[sene_idx][dmd_idx][i+1][item]; 
                            }
                            model.addConstr( lhs_linexp - rhs_linexp == 0 ); 
                        }

                        //  
                        for(const auto& item : outgoing_cur_node) {
                            model.addConstr( x[sene_idx][dmd_idx][0][item] == 0 );  
                        }
                    } 
                }

                for(int l = 0; l != hop_parameter; ++l) {
                    for(int i = 0; i != 2*pri_graph.cost_map.size(); ++i) { 
                        model.addConstr( x[sene_idx][dmd_idx][l][i] <= y[i/2] * Omega[sene_idx][i/2], "x_"+to_string(l)+"_"+to_string(i) ); 
                        // model.addConstr( x_l_ij[l][2*i+1] <= y[i] * omega[i], "x_"+to_string(l)+"_"+to_string(2*i+1) );
                    }
                }

            }

 

 
        }

        
 

        // Optimize model 
        // model.set(GRB_IntParam_Presolve, 0);
        // model.set(GRB_DoubleParam_Heuristics, 0);
        model.set(GRB_DoubleParam_TimeLimit, 3600*12);
            
        model.optimize();  
        cout << "Solution: " << endl; 
        double objective = 0.0;
        for(int i = 0; i != pri_graph.cost_map.size(); ++i) {  
            cout << y[i].get(GRB_StringAttr_VarName) << ": " << y[i].get(GRB_DoubleAttr_X) << endl; 
            objective += y_objs[i] * (y[i].get(GRB_DoubleAttr_X));
            result.push_back(round(y[i].get(GRB_DoubleAttr_X)));
        } 
        // cout << eta.get(GRB_StringAttr_VarName) << ": " << eta.get(GRB_DoubleAttr_X) << endl; 
        // for(int i = 0; i != Omega.size(); ++i) { 
        //     cout << q_i[i].get(GRB_StringAttr_VarName) << ": " << q_i[i].get(GRB_DoubleAttr_X) << endl;
        // }
        
        // for(int i = 0; i != Omega.size(); ++i) {
        //     cout << abs[i].get(GRB_StringAttr_VarName) << ": " << abs[i].get(GRB_DoubleAttr_X) << endl;
        // }
        // for(const int& item : T) {
        //     cout << Q_y_w[item].get(GRB_StringAttr_VarName) << ": " << Q_y_w[item].get(GRB_DoubleAttr_X) << endl;
        // }
        // vector<double> output_theta(xi); 
        // for(int i = 0; i != xi; ++i) {
        //     output_theta[i] = theta[i].get(GRB_DoubleAttr_X);
        // } 
        // for(const int& item : T) {
        //     double val = 0.0;
        //     for(int i = 0; i != xi; ++i) {
        //         val += output_theta[i]*feature_matrix[item][i];
        //     }
        //     cout << "app_gx[" << item << "]: " << val << endl;
        // }

        cout << "first stage cost: " << objective << endl;
        cout << "Objective: " << model.get(GRB_DoubleAttr_ObjVal) << endl; 

    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }

    delete[] ub;
    return result; 
}


std::vector<int> solve_MLAug_LR_penal(const PrimaryGraph& pri_graph, const std::vector<std::vector<int>>& Omega, const std::vector<double>& v_j_star, const std::vector<int>& Dpq, const int& n_dmd, const int& fc_vc, const int& hop_parameter, const double& epsilong, const std::string& instance_name, const int& xi, const int& L_bar) {
    vector<int> result; 

    vector<int> T;
    vector<int> notT;
    vector<int> map_n_to_m(Omega.size(), -1);
    vector<int> weight_m(Omega.size(), 0); 

    ifstream fin("../output-learning/" + instance_name + "-outputsamples.txt"); 
    string line; 
    getline(fin, line); 
    stringstream ss(line); 
    int tmp; 
    while(ss >> tmp) {
        T.push_back(tmp); 
    }
    
    while(getline(fin, line)) {
        stringstream ss(line); 
        int n, m; 
        ss >> n >> m; 
        map_n_to_m[n] = m; 
        ++weight_m[m]; 
        if(n != m) {
            notT.push_back(n); 
        }
    }
    // cout << "T: " << endl; 
    // for(const int& item : T) {
    //     cout << item << " "; 
    // }
    // cout << endl; 
    // cout << "T size: " << T.size() << endl;  

    // cout << "notT: " << endl; 
    // for(const int& item : notT) {
    //     cout << item << " "; 
    // }
    // cout << endl; 
    // cout << "notT size: " << notT.size() << endl; 
    
    // for(const int& item : map_n_to_m) {
    //     cout << item << " "; 
    // }
    // cout << endl; 

    // int sum_weight = 0; 
    // for(const int& item : weight_m) {
    //     cout << item << " "; 
    //     sum_weight += item; 
    // }
    // cout << endl; 
    // cout << sum_weight << endl;  

     



    vector<vector<double>> feature_matrix(Omega.size(), vector<double>(xi, 0.0));    
    ifstream fin2("../output-learning/" + instance_name + "-feature.txt");  
    for(int i = 0; i != Omega.size(); ++i) {
        getline(fin2, line); 
        stringstream ss(line);  
        for(int j = 0; j != xi; ++j) {
            ss >> feature_matrix[i][j];  
        }
    } 
    // for(int i = 0; i != Omega.size(); ++i) {
    //     for(int j = 0; j != xi; ++j) {
    //         cout << feature_matrix[i][j] << " "; 
    //     }
    //     cout << endl; 
    // } 

    

    int n_arcs = pri_graph.cost_map.size() * 2;
    double* ub = new double[n_arcs];
    for(int i = 0; i != n_arcs; ++i) {
        ub[i] = 1.0;
    }

    try {

        // Create an environment
        GRBEnv env = GRBEnv(); 
        // Create an empty model
        GRBModel model = GRBModel(env);
        
        // Create variables 
        GRBVar* y =  model.addVars(pri_graph.cost_map.size(), GRB_BINARY); 

        vector<double> y_objs(pri_graph.cost_map.size()); 
        int tmp_idx = 0; 
        for(const auto& item : pri_graph.cost_map) {
            y_objs[tmp_idx] = item.second;
            y[tmp_idx].set(GRB_DoubleAttr_Obj, item.second);
            y[tmp_idx].set(GRB_StringAttr_VarName, "y_" + to_string(item.first.first) + "_" + to_string(item.first.second));
            ++tmp_idx; 
        } 
 

        GRBVar* q_i = model.addVars(Omega.size(), GRB_CONTINUOUS); 
        for(int i = 0; i != Omega.size(); ++i) {
            q_i[i].set(GRB_DoubleAttr_LB, -GRB_INFINITY);
            q_i[i].set(GRB_DoubleAttr_Obj, v_j_star[i]);
            q_i[i].set(GRB_StringAttr_VarName, "q_" + to_string(i));
        }

        GRBVar eta = model.addVar(0.0, GRB_INFINITY, epsilong, GRB_CONTINUOUS, "eta"); 
 

        GRBVar* theta = model.addVars(xi, GRB_CONTINUOUS); 
        for(int i = 0; i != xi; ++i) {
            theta[i].set(GRB_DoubleAttr_LB, -GRB_INFINITY); 
            theta[i].set(GRB_StringAttr_VarName, "theta_" + to_string(i));
        }

        vector<vector<vector<GRBVar*>>> x;
        for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) {
            x.push_back(vector<vector<GRBVar*>>());
        }
        for(const int& sene_idx : T) { 
            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) { 
                x[sene_idx].push_back(vector<GRBVar*>());
                for(int hop_idx = 0; hop_idx != hop_parameter; ++hop_idx){ 
                    x[sene_idx][dmd_idx].push_back(     model.addVars(nullptr, nullptr, nullptr, nullptr, nullptr, n_arcs)     );
                }
            }
        } 

     
        vector<GRBVar*> s(Omega.size());
        for(const int& sene_idx : T) { 
            s[sene_idx] = model.addVars(n_dmd, GRB_CONTINUOUS);
            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) { 
                s[sene_idx][dmd_idx].set(GRB_DoubleAttr_UB, 1);
                s[sene_idx][dmd_idx].set(GRB_StringAttr_VarName, "s_" + to_string(sene_idx) + "_" + to_string(dmd_idx));
            }
        }



        GRBVar* Q_y_w = model.addVars(Omega.size(), GRB_CONTINUOUS); 
        for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) {
            Q_y_w[sene_idx].set(GRB_DoubleAttr_LB, -GRB_INFINITY);
            Q_y_w[sene_idx].set(GRB_StringAttr_VarName, "Q_"+to_string(sene_idx)); 
        }

        GRBVar* atheta = model.addVars(Omega.size(), GRB_CONTINUOUS); 
        for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) {
            atheta[sene_idx].set(GRB_DoubleAttr_LB, -GRB_INFINITY);
            atheta[sene_idx].set(GRB_StringAttr_VarName, "atheta_"+to_string(sene_idx)); 
        }

        GRBVar* abs = model.addVars(Omega.size(), GRB_CONTINUOUS); 
        for(int i = 0; i != Omega.size(); ++i) { 
            abs[i].set(GRB_StringAttr_VarName, "abs_" + to_string(i));
            abs[i].set(GRB_DoubleAttr_Obj, weight_m[i]/double(Omega.size())*99999);
        }

   
        // Set objective 
        // model.setObjective(x + y + 2 * z, GRB_MAXIMIZE);
        model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);


        // constraints  
        for(int j = 0; j != Omega.size(); ++j) {
            for(const int& m : T) {
                double coef = 0.0; 
                for(int idx = 0; idx != pri_graph.cost_map.size(); ++idx) {
                    coef += (Omega[m][idx] - Omega[j][idx])*(Omega[m][idx] - Omega[j][idx]); 
                }
                coef = sqrt(coef); 
                model.addConstr(Q_y_w[m] <= coef*eta + q_i[j]); 
            }

            for(const int& n : notT) {    
                double coef = 0.0; 
                for(int idx = 0; idx != pri_graph.cost_map.size(); ++idx) {
                    coef += (Omega[n][idx] - Omega[j][idx])*(Omega[n][idx] - Omega[j][idx]); 
                }
                coef = sqrt(coef); 
                model.addConstr(atheta[n] <= coef*eta + q_i[j]);  
            }
        }

        // GRBLinExpr endcons = 0;
        for(const int& m : T) { 
            model.addConstr(abs[m] >= Q_y_w[m]-atheta[m]);  
            model.addConstr(abs[m] >= atheta[m]-Q_y_w[m]); 
            // endcons += weight_m[m]*abs[m]; 
        }
        // model.addConstr(endcons <= L_bar*Omega.size());   


        for(int i = 0; i < Omega.size(); ++i) {
            GRBLinExpr app_gx = 0;  
            for(int j = 0; j != xi; ++j) {
                app_gx += theta[j]*feature_matrix[i][j]; 
            }
            model.addConstr(app_gx == atheta[i]); 
        }


        for(const int& sene_idx : T) {

            GRBLinExpr gx = 0; 
            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {
                double cpq = 0; 
                for(const auto& item : pri_graph.cost_map) {
                    cpq += Dpq[dmd_idx] * item.second / fc_vc; 
                }
                gx += cpq - cpq*s[sene_idx][dmd_idx]; 
                for(int hop_idx = 0; hop_idx != hop_parameter; ++hop_idx) {
                    int tmp_idx = 0; 
                    for(const auto& item : pri_graph.cost_map) {
                        gx += x[sene_idx][dmd_idx][hop_idx][tmp_idx] * Dpq[dmd_idx] * item.second / fc_vc;
                        gx += x[sene_idx][dmd_idx][hop_idx][tmp_idx+1] * Dpq[dmd_idx] * item.second / fc_vc;
                        tmp_idx += 2; 
                    }
                }
            }
            model.addConstr(gx == Q_y_w[sene_idx]); 



            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {
                for(int cur_node = 0; cur_node != pri_graph.link_table.size(); ++cur_node) {
                    vector<int> outgoing_cur_node; 
                    vector<int> into_cur_node; 
                    int tmp_idx = 0; 
                    for(const auto& item : pri_graph.cost_map) {
                        if(item.first.first == cur_node) {
                            outgoing_cur_node.push_back(tmp_idx);
                            into_cur_node.push_back(tmp_idx+1);
                        }
                        if(item.first.second == cur_node) {
                            into_cur_node.push_back(tmp_idx); 
                            outgoing_cur_node.push_back(tmp_idx+1); 
                        }
                        tmp_idx += 2; 
                    }

                    
                    if(cur_node == pri_graph.demand_pairs[dmd_idx].first) { 
                        GRBLinExpr tmp_linexp = 0; 
                        for(const auto& item : outgoing_cur_node)  tmp_linexp += x[sene_idx][dmd_idx][0][item]; 
                        model.addConstr( tmp_linexp - s[sene_idx][dmd_idx] == 0 );  

                        //  
                        for(int i = 0; i != hop_parameter; ++i) {
                            for(const auto& item : into_cur_node) {
                                model.addConstr( x[sene_idx][dmd_idx][i][item] == 0 ); 
                            }
                        }
                    }
                    else if(cur_node == pri_graph.demand_pairs[dmd_idx].second) {
                        GRBLinExpr tmp_linexp = 0; 
                        for(int i = 0; i != hop_parameter; ++i) {
                            for(const auto& item : into_cur_node) {
                                tmp_linexp += x[sene_idx][dmd_idx][i][item]; 
                            }
                        }
                        model.addConstr( tmp_linexp - s[sene_idx][dmd_idx] == 0 );  
                        //  
                        for(int i = 0; i != hop_parameter; ++i) {
                            for(const auto& item : outgoing_cur_node) {
                                model.addConstr( x[sene_idx][dmd_idx][i][item] == 0 ); 
                            }
                        } 
                    }
                    else {
                        for(int i = 0; i != hop_parameter-1; ++i) {
                            GRBLinExpr lhs_linexp = 0;
                            for(const auto& item : into_cur_node) {
                                lhs_linexp += x[sene_idx][dmd_idx][i][item]; 
                            }
                            GRBLinExpr rhs_linexp = 0; 
                            for(const auto& item : outgoing_cur_node) {
                                rhs_linexp += x[sene_idx][dmd_idx][i+1][item]; 
                            }
                            model.addConstr( lhs_linexp - rhs_linexp == 0 ); 
                        }

                        //  
                        for(const auto& item : outgoing_cur_node) {
                            model.addConstr( x[sene_idx][dmd_idx][0][item] == 0 );  
                        }
                    } 
                }

                for(int l = 0; l != hop_parameter; ++l) {
                    for(int i = 0; i != 2*pri_graph.cost_map.size(); ++i) { 
                        model.addConstr( x[sene_idx][dmd_idx][l][i] <= y[i/2] * Omega[sene_idx][i/2], "x_"+to_string(l)+"_"+to_string(i) ); 
                        // model.addConstr( x_l_ij[l][2*i+1] <= y[i] * omega[i], "x_"+to_string(l)+"_"+to_string(2*i+1) );
                    }
                }

            }

 

 
        }

        
 

        // Optimize model 
        // model.set(GRB_IntParam_Presolve, 0);
        // model.set(GRB_DoubleParam_Heuristics, 0);
        model.set(GRB_DoubleParam_TimeLimit, 3600*12);
            
        model.optimize();  
        cout << "Solution: " << endl; 
        double objective = 0.0;
        for(int i = 0; i != pri_graph.cost_map.size(); ++i) {  
            cout << y[i].get(GRB_StringAttr_VarName) << ": " << y[i].get(GRB_DoubleAttr_X) << endl; 
            objective += y_objs[i] * (y[i].get(GRB_DoubleAttr_X));
            result.push_back(round(y[i].get(GRB_DoubleAttr_X)));
        } 
        // cout << eta.get(GRB_StringAttr_VarName) << ": " << eta.get(GRB_DoubleAttr_X) << endl; 
        // for(int i = 0; i != Omega.size(); ++i) { 
        //     cout << q_i[i].get(GRB_StringAttr_VarName) << ": " << q_i[i].get(GRB_DoubleAttr_X) << endl;
        // }
        
        // for(int i = 0; i != Omega.size(); ++i) {
        //     cout << abs[i].get(GRB_StringAttr_VarName) << ": " << abs[i].get(GRB_DoubleAttr_X) << endl;
        // }
        // for(const int& item : T) {
        //     cout << Q_y_w[item].get(GRB_StringAttr_VarName) << ": " << Q_y_w[item].get(GRB_DoubleAttr_X) << endl;
        // }
        // vector<double> output_theta(xi); 
        // for(int i = 0; i != xi; ++i) {
        //     output_theta[i] = theta[i].get(GRB_DoubleAttr_X);
        // } 
        // for(const int& item : T) {
        //     double val = 0.0;
        //     for(int i = 0; i != xi; ++i) {
        //         val += output_theta[i]*feature_matrix[item][i];
        //     }
        //     cout << "app_gx[" << item << "]: " << val << endl;
        // }

        cout << "first stage cost: " << objective << endl;
        cout << "Objective: " << model.get(GRB_DoubleAttr_ObjVal) << endl; 

    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }

    delete[] ub;
    return result; 
}








// KNN-Integrated 
std::vector<int> solve_MLInte_KNN(const PrimaryGraph& pri_graph, const std::vector<std::vector<int>>& Omega, const std::vector<double>& v_j_star, const std::vector<int>& Dpq, const int& n_dmd, const int& fc_vc, const int& hop_parameter, const double& epsilong, const std::string& instance_name, const int& opt_k, std::vector<int> T, std::unordered_map<int, std::vector<int>> mp) {
    vector<int> result;  

    unordered_set<int> st(T.begin(), T.end());
    vector<int> notT;
    for(int i = 0; i < Omega.size(); ++i) {
        if(st.count(i) == 0) notT.push_back(i); 
    }

    int n_arcs = pri_graph.cost_map.size() * 2;
    double* ub = new double[n_arcs];
    for(int i = 0; i != n_arcs; ++i) {
        ub[i] = 1.0;
    }

    try {

        // Create an environment
        GRBEnv env = GRBEnv(); 
        // Create an empty model
        GRBModel model = GRBModel(env);
        
        // Create variables 
        GRBVar* y =  model.addVars(pri_graph.cost_map.size(), GRB_BINARY); 
 

        vector<double> y_objs(pri_graph.cost_map.size()); 
        int tmp_idx = 0; 
        for(const auto& item : pri_graph.cost_map) {
            y_objs[tmp_idx] = item.second;
            y[tmp_idx].set(GRB_DoubleAttr_Obj, item.second);
            y[tmp_idx].set(GRB_StringAttr_VarName, "y_" + to_string(item.first.first) + "_" + to_string(item.first.second)); 
            ++tmp_idx; 
        } 
 

        GRBVar* q_i = model.addVars(Omega.size(), GRB_CONTINUOUS); 
        for(int i = 0; i != Omega.size(); ++i) {
            q_i[i].set(GRB_DoubleAttr_LB, -GRB_INFINITY);
            q_i[i].set(GRB_DoubleAttr_Obj, v_j_star[i]);
            q_i[i].set(GRB_StringAttr_VarName, "q_" + to_string(i)); 
        }

        GRBVar eta = model.addVar(0.0, GRB_INFINITY, epsilong, GRB_CONTINUOUS, "eta");  

        vector<vector<vector<GRBVar*>>> x;
        for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) {
            x.push_back(vector<vector<GRBVar*>>());
        }
        for(const int& sene_idx : T) { 
            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) { 
                x[sene_idx].push_back(vector<GRBVar*>());
                for(int hop_idx = 0; hop_idx != hop_parameter; ++hop_idx){ 
                    x[sene_idx][dmd_idx].push_back(     model.addVars(nullptr, nullptr, nullptr, nullptr, nullptr, n_arcs)     );
                }
            }
        } 

     
        vector<GRBVar*> s(Omega.size());
        for(const int& sene_idx : T) { 
            s[sene_idx] = model.addVars(n_dmd, GRB_CONTINUOUS);
            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) { 
                s[sene_idx][dmd_idx].set(GRB_DoubleAttr_UB, 1);
                s[sene_idx][dmd_idx].set(GRB_StringAttr_VarName, "s_" + to_string(sene_idx) + "_" + to_string(dmd_idx));
            }
        }

 



        GRBVar* Q_y_w = model.addVars(Omega.size(), GRB_CONTINUOUS); 
        for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) {
            Q_y_w[sene_idx].set(GRB_DoubleAttr_LB, -GRB_INFINITY);
            Q_y_w[sene_idx].set(GRB_StringAttr_VarName, "Q_"+to_string(sene_idx)); 
        }

  

   
        // Set objective   
        model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE); 
        

        // GRBLinExpr obj = epsilong*eta;
        // for(int i = 0; i != pri_graph.cost_map.size(); ++i) {
        //     obj += y_objs[i]*y[i];
        // } 
        // for(int i = 0; i != Omega.size(); ++i) {
        //     obj += v_j_star[i]*q_i[i];
        // }
        // model.addConstr(obj >= 0);

        // constraints  
        for(int j = 0; j != Omega.size(); ++j) {
            for(const int& sene_idx : T) {
                double coef = 0.0; 
                for(int idx = 0; idx != pri_graph.cost_map.size(); ++idx) {
                    coef += (Omega[sene_idx][idx] - Omega[j][idx])*(Omega[sene_idx][idx] - Omega[j][idx]); 
                }
                coef = sqrt(coef); 
                model.addConstr(Q_y_w[sene_idx] <= coef*eta + q_i[j]); 
            }

            for(const int& sene_idx : notT) {
                double coef = 0.0; 
                for(int idx = 0; idx != pri_graph.cost_map.size(); ++idx) {
                    coef += (Omega[sene_idx][idx] - Omega[j][idx])*(Omega[sene_idx][idx] - Omega[j][idx]); 
                }
                coef = sqrt(coef); 
                GRBLinExpr tmp;
                for(const int& item : mp[sene_idx]) {
                    tmp += Q_y_w[item]; 
                } 
                tmp = tmp / opt_k; 
                model.addConstr(tmp <= coef*eta + q_i[j]); 
            }
        }
 


        for(const int& sene_idx : T) {

            GRBLinExpr gx = 0; 
            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {
                double cpq = 0; 
                for(const auto& item : pri_graph.cost_map) {
                    cpq += Dpq[dmd_idx] * item.second / fc_vc; 
                }
                gx += cpq - cpq*s[sene_idx][dmd_idx]; 
                for(int hop_idx = 0; hop_idx != hop_parameter; ++hop_idx) {
                    int tmp_idx = 0; 
                    for(const auto& item : pri_graph.cost_map) {
                        gx += x[sene_idx][dmd_idx][hop_idx][tmp_idx] * Dpq[dmd_idx] * item.second / fc_vc;
                        gx += x[sene_idx][dmd_idx][hop_idx][tmp_idx+1] * Dpq[dmd_idx] * item.second / fc_vc;
                        tmp_idx += 2; 
                    }
                }
            }
            model.addConstr(gx == Q_y_w[sene_idx]); 



            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {
                for(int cur_node = 0; cur_node != pri_graph.link_table.size(); ++cur_node) {
                    vector<int> outgoing_cur_node; 
                    vector<int> into_cur_node; 
                    int tmp_idx = 0; 
                    for(const auto& item : pri_graph.cost_map) {
                        if(item.first.first == cur_node) {
                            outgoing_cur_node.push_back(tmp_idx);
                            into_cur_node.push_back(tmp_idx+1);
                        }
                        if(item.first.second == cur_node) {
                            into_cur_node.push_back(tmp_idx); 
                            outgoing_cur_node.push_back(tmp_idx+1); 
                        }
                        tmp_idx += 2; 
                    }

                    
                    if(cur_node == pri_graph.demand_pairs[dmd_idx].first) { 
                        GRBLinExpr tmp_linexp = 0; 
                        for(const auto& item : outgoing_cur_node)  tmp_linexp += x[sene_idx][dmd_idx][0][item]; 
                        model.addConstr( tmp_linexp - s[sene_idx][dmd_idx] == 0 );  

                        //  
                        for(int i = 0; i != hop_parameter; ++i) {
                            for(const auto& item : into_cur_node) {
                                model.addConstr( x[sene_idx][dmd_idx][i][item] == 0 ); 
                            }
                        }
                    }
                    else if(cur_node == pri_graph.demand_pairs[dmd_idx].second) {
                        GRBLinExpr tmp_linexp = 0; 
                        for(int i = 0; i != hop_parameter; ++i) {
                            for(const auto& item : into_cur_node) {
                                tmp_linexp += x[sene_idx][dmd_idx][i][item]; 
                            }
                        }
                        model.addConstr( tmp_linexp - s[sene_idx][dmd_idx] == 0 );  
                        //  
                        for(int i = 0; i != hop_parameter; ++i) {
                            for(const auto& item : outgoing_cur_node) {
                                model.addConstr( x[sene_idx][dmd_idx][i][item] == 0 ); 
                            }
                        } 
                    }
                    else {
                        for(int i = 0; i != hop_parameter-1; ++i) {
                            GRBLinExpr lhs_linexp = 0;
                            for(const auto& item : into_cur_node) {
                                lhs_linexp += x[sene_idx][dmd_idx][i][item]; 
                            }
                            GRBLinExpr rhs_linexp = 0; 
                            for(const auto& item : outgoing_cur_node) {
                                rhs_linexp += x[sene_idx][dmd_idx][i+1][item]; 
                            }
                            model.addConstr( lhs_linexp - rhs_linexp == 0 ); 
                        }

                        //  
                        for(const auto& item : outgoing_cur_node) {
                            model.addConstr( x[sene_idx][dmd_idx][0][item] == 0 );  
                        }
                    } 
                }

                for(int l = 0; l != hop_parameter; ++l) {
                    for(int i = 0; i != 2*pri_graph.cost_map.size(); ++i) { 
                        model.addConstr( x[sene_idx][dmd_idx][l][i] <= y[i/2] * Omega[sene_idx][i/2], "x_"+to_string(l)+"_"+to_string(i) ); 
                        // model.addConstr( x_l_ij[l][2*i+1] <= y[i] * omega[i], "x_"+to_string(l)+"_"+to_string(2*i+1) );
                    }
                }

            }
  
        }

        
 

        // Optimize model 
        // model.set(GRB_IntParam_Presolve, 0);
        // model.set(GRB_DoubleParam_Heuristics, 0);
        model.set(GRB_DoubleParam_TimeLimit, 3600*12);
            
        model.optimize();  
        cout << "Solution: " << endl; 
        double objective = 0.0;
        for(int i = 0; i != pri_graph.cost_map.size(); ++i) {  
            cout << y[i].get(GRB_StringAttr_VarName) << ": " << y[i].get(GRB_DoubleAttr_X) << endl; 
            objective += y_objs[i] * (y[i].get(GRB_DoubleAttr_X));
            result.push_back(round(y[i].get(GRB_DoubleAttr_X)));
        } 
        cout << eta.get(GRB_StringAttr_VarName) << ": " << eta.get(GRB_DoubleAttr_X) << endl; 
        for(int i = 0; i != Omega.size(); ++i) { 
            cout << q_i[i].get(GRB_StringAttr_VarName) << ": " << q_i[i].get(GRB_DoubleAttr_X) << endl;
        }
         
        for(const int& item : T) {
            cout << Q_y_w[item].get(GRB_StringAttr_VarName) << ": " << Q_y_w[item].get(GRB_DoubleAttr_X) << endl;
        }  


        cout << "Objective: " << model.get(GRB_DoubleAttr_ObjVal) << endl; 
        cout << "first stage cost: " << objective << endl;
 
        


    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }


 
    delete[] ub;
    return result; 
}



 







 std::vector<double> solve_theta(const PrimaryGraph& pri_graph, const std::vector<std::vector<int>>& Omega, const std::vector<double>& v_j_star, const std::vector<int>& Dpq, const int& n_dmd, const int& fc_vc, const int& hop_parameter, const double& epsilong, const std::string& instance_name, const std::vector<double>& Q_y_w) {
    vector<double> abs_result(Omega.size(), 0);
    try {

        // // Create an environment
        // GRBEnv env = GRBEnv(); 
        // // Create an empty model
        // GRBModel model = GRBModel(env); 

        GRBEnv env = GRBEnv(true); 
        env.set(GRB_IntParam_OutputFlag, 0);
        env.start();
        // Create an empty model
        GRBModel model = GRBModel(env);

        // GRBVar theta0 = model.addVar(-GRB_INFINITY, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "theta0");
        GRBVar* theta1 = model.addVars(pri_graph.cost_map.size(), GRB_CONTINUOUS); 
        for(int i = 0; i != pri_graph.cost_map.size(); ++i) {
            theta1[i].set(GRB_DoubleAttr_LB, -GRB_INFINITY); 
            theta1[i].set(GRB_StringAttr_VarName, "theta1_" + to_string(i));
        }
 
 
 
        GRBVar* abs = model.addVars(Omega.size(), GRB_CONTINUOUS); 
        for(int i = 0; i != Omega.size(); ++i) { 
            abs[i].set(GRB_StringAttr_VarName, "abs_" + to_string(i));
        }
 

        for(int i = 0; i != Omega.size(); ++i) {
            GRBLinExpr tmp_linexp = 0; 
            for(int j = 0; j != pri_graph.cost_map.size(); ++j) {
                tmp_linexp += theta1[j] * Omega[i][j]; 
            }  
            model.addConstr(tmp_linexp - Q_y_w[i] <= abs[i]); 
            model.addConstr(Q_y_w[i] - tmp_linexp <= abs[i]);
        }

        GRBLinExpr obj = 0; 
        for(int i = 0; i != Omega.size(); ++i) {
            obj += abs[i]; 
        }
        model.setObjective(obj, GRB_MINIMIZE); 

        model.set(GRB_DoubleParam_TimeLimit, 3600*12);
            
        model.optimize();    
        // cout << "Solution: " << endl;   
        // for(int i = 0; i != pri_graph.cost_map.size(); ++i) { 
        //     // result[i] = theta[i].get(GRB_DoubleAttr_X); 
        //     cout << theta[i].get(GRB_StringAttr_VarName) << ": " << theta[i].get(GRB_DoubleAttr_X) << endl;
        // }
        
        // for(int i = 0; i != Omega.size(); ++i) {
        //     abs_result[i] = abs[i].get(GRB_DoubleAttr_X);
        //     cout << abs[i].get(GRB_StringAttr_VarName) << ": " << abs[i].get(GRB_DoubleAttr_X) << endl;
        // }
        


        // cout << "Objective: " << model.get(GRB_DoubleAttr_ObjVal) << endl;  
 
        


    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }


    return abs_result; 
}






std::vector<int> solve_MLInte_LR(const PrimaryGraph& pri_graph, const std::vector<std::vector<int>>& Omega, const std::vector<double>& v_j_star, const std::vector<int>& Dpq, const int& n_dmd, const int& fc_vc, const int& hop_parameter, const double& epsilong, const std::string& instance_name, std::vector<int> T, std::vector<int> map_n_to_m, std::vector<double>& app_Q_return) {
    vector<int> result; 

    unordered_set<int> st(T.begin(), T.end());
    vector<int> notT;
    for(int i = 0; i < Omega.size(); ++i) {
        if(st.count(i) == 0) notT.push_back(i); 
    }
    


    int n_arcs = pri_graph.cost_map.size() * 2;
    double* ub = new double[n_arcs];
    for(int i = 0; i != n_arcs; ++i) {
        ub[i] = 1.0;
    }

    try {

        // Create an environment
        GRBEnv env = GRBEnv(); 
        // Create an empty model
        GRBModel model = GRBModel(env);
        

        // Create variables 
        GRBVar* y =  model.addVars(pri_graph.cost_map.size(), GRB_BINARY); 

        vector<double> y_objs(pri_graph.cost_map.size()); 
        int tmp_idx = 0; 
        for(const auto& item : pri_graph.cost_map) {
            
            y_objs[tmp_idx] = item.second;
            y[tmp_idx].set(GRB_DoubleAttr_Obj, item.second);
            y[tmp_idx].set(GRB_StringAttr_VarName, "y_" + to_string(item.first.first) + "_" + to_string(item.first.second));
            ++tmp_idx; 
        } 
 

        GRBVar* q_i = model.addVars(Omega.size(), GRB_CONTINUOUS); 
        for(int i = 0; i != Omega.size(); ++i) {
             
            q_i[i].set(GRB_DoubleAttr_LB, -GRB_INFINITY);
            q_i[i].set(GRB_DoubleAttr_Obj, v_j_star[i]);
            q_i[i].set(GRB_StringAttr_VarName, "q_" + to_string(i));
        }

        GRBVar eta = model.addVar(0.0, GRB_INFINITY, epsilong, GRB_CONTINUOUS, "eta"); 
         

        model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);
        
        // GRBVar theta0 = model.addVar(-GRB_INFINITY, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "theta0");
        GRBVar* theta1 = model.addVars(pri_graph.cost_map.size(), GRB_CONTINUOUS); 
        for(int i = 0; i != pri_graph.cost_map.size(); ++i) {
            theta1[i].set(GRB_DoubleAttr_LB, -GRB_INFINITY); 
            theta1[i].set(GRB_StringAttr_VarName, "theta1_" + to_string(i));
        }
  

        vector<vector<vector<GRBVar*>>> x;
        for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) {
            x.push_back(vector<vector<GRBVar*>>());
        }
        for(const int& sene_idx : T) { 
            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) { 
                x[sene_idx].push_back(vector<GRBVar*>());
                for(int hop_idx = 0; hop_idx != hop_parameter; ++hop_idx){ 
                    x[sene_idx][dmd_idx].push_back(     model.addVars(nullptr, nullptr, nullptr, nullptr, nullptr, n_arcs)     );
                }
            }
        } 

     
        vector<GRBVar*> s(Omega.size());
        for(const int& sene_idx : T) { 
            s[sene_idx] = model.addVars(n_dmd, GRB_CONTINUOUS);
            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) { 
                s[sene_idx][dmd_idx].set(GRB_DoubleAttr_UB, 1);
                s[sene_idx][dmd_idx].set(GRB_StringAttr_VarName, "s_" + to_string(sene_idx) + "_" + to_string(dmd_idx));
            }
        }


 




        GRBVar* Q_y_w = model.addVars(Omega.size(), GRB_CONTINUOUS); 
        for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) {
            Q_y_w[sene_idx].set(GRB_DoubleAttr_LB, -GRB_INFINITY);
            Q_y_w[sene_idx].set(GRB_StringAttr_VarName, "Q_"+to_string(sene_idx)); 
        }

        GRBVar* app_Q = model.addVars(Omega.size(), GRB_CONTINUOUS); 
        for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) {
            app_Q[sene_idx].set(GRB_DoubleAttr_LB, -GRB_INFINITY);
            app_Q[sene_idx].set(GRB_StringAttr_VarName, "app_gx_"+to_string(sene_idx)); 
        }

        for(int n = 0; n < Omega.size(); ++n) {    
            GRBLinExpr app_gx = 0; 
            for(int i = 0; i != pri_graph.cost_map.size(); ++i) {
                app_gx += theta1[i] * Omega[n][i]; 
            }  
            model.addConstr(app_Q[n] == app_gx);  
        }

        // GRBVar* abs = model.addVars(Omega.size(), GRB_CONTINUOUS); 
        // for(int i = 0; i != Omega.size(); ++i) { 
        //     abs[i].set(GRB_StringAttr_VarName, "abs_" + to_string(i));
        // }

    

        // constraints  
        for(int j = 0; j != Omega.size(); ++j) {
            for(const int& m : T) {
                double coef = 0.0; 
                for(int idx = 0; idx != pri_graph.cost_map.size(); ++idx) {
                    coef += (Omega[m][idx] - Omega[j][idx])*(Omega[m][idx] - Omega[j][idx]); 
                }
                coef = sqrt(coef); 
                model.addConstr(Q_y_w[m] <= coef*eta + q_i[j]); 
            }

            for(const int& n : notT) {    
                double coef = 0.0; 
                for(int idx = 0; idx != pri_graph.cost_map.size(); ++idx) {
                    coef += (Omega[n][idx] - Omega[j][idx])*(Omega[n][idx] - Omega[j][idx]); 
                }
                coef = sqrt(coef); 
                model.addConstr(app_Q[n] <= coef*eta + q_i[j]);   
            }
        }

        for(const int& n : notT) {
            model.addConstr(app_Q[n] >= Q_y_w[map_n_to_m[n]]);  
        }
        

        // GRBLinExpr endcons = 0;
        for(const int& m : T) { 
            model.addConstr(app_Q[m] == Q_y_w[m]);  
        } 


        for(const int& sene_idx : T) {

            GRBLinExpr gx = 0; 
            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {
                double cpq = 0; 
                for(const auto& item : pri_graph.cost_map) {
                    cpq += Dpq[dmd_idx] * item.second / fc_vc; 
                }
                gx += cpq - cpq*s[sene_idx][dmd_idx]; 
                for(int hop_idx = 0; hop_idx != hop_parameter; ++hop_idx) {
                    int tmp_idx = 0; 
                    for(const auto& item : pri_graph.cost_map) {
                        gx += x[sene_idx][dmd_idx][hop_idx][tmp_idx] * Dpq[dmd_idx] * item.second / fc_vc;
                        gx += x[sene_idx][dmd_idx][hop_idx][tmp_idx+1] * Dpq[dmd_idx] * item.second / fc_vc;
                        tmp_idx += 2; 
                    }
                }
            }
            model.addConstr(gx == Q_y_w[sene_idx]); 



            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {
                for(int cur_node = 0; cur_node != pri_graph.link_table.size(); ++cur_node) {
                    vector<int> outgoing_cur_node; 
                    vector<int> into_cur_node; 
                    int tmp_idx = 0; 
                    for(const auto& item : pri_graph.cost_map) {
                        if(item.first.first == cur_node) {
                            outgoing_cur_node.push_back(tmp_idx);
                            into_cur_node.push_back(tmp_idx+1);
                        }
                        if(item.first.second == cur_node) {
                            into_cur_node.push_back(tmp_idx); 
                            outgoing_cur_node.push_back(tmp_idx+1); 
                        }
                        tmp_idx += 2; 
                    }

                    
                    if(cur_node == pri_graph.demand_pairs[dmd_idx].first) { 
                        GRBLinExpr tmp_linexp = 0; 
                        for(const auto& item : outgoing_cur_node)  tmp_linexp += x[sene_idx][dmd_idx][0][item]; 
                        model.addConstr( tmp_linexp - s[sene_idx][dmd_idx] == 0 );  

                        //  
                        for(int i = 0; i != hop_parameter; ++i) {
                            for(const auto& item : into_cur_node) {
                                model.addConstr( x[sene_idx][dmd_idx][i][item] == 0 ); 
                            }
                        }
                    }
                    else if(cur_node == pri_graph.demand_pairs[dmd_idx].second) {
                        GRBLinExpr tmp_linexp = 0; 
                        for(int i = 0; i != hop_parameter; ++i) {
                            for(const auto& item : into_cur_node) {
                                tmp_linexp += x[sene_idx][dmd_idx][i][item]; 
                            }
                        }
                        model.addConstr( tmp_linexp - s[sene_idx][dmd_idx] == 0 );  
                        //  
                        for(int i = 0; i != hop_parameter; ++i) {
                            for(const auto& item : outgoing_cur_node) {
                                model.addConstr( x[sene_idx][dmd_idx][i][item] == 0 ); 
                            }
                        } 
                    }
                    else {
                        for(int i = 0; i != hop_parameter-1; ++i) {
                            GRBLinExpr lhs_linexp = 0;
                            for(const auto& item : into_cur_node) {
                                lhs_linexp += x[sene_idx][dmd_idx][i][item]; 
                            }
                            GRBLinExpr rhs_linexp = 0; 
                            for(const auto& item : outgoing_cur_node) {
                                rhs_linexp += x[sene_idx][dmd_idx][i+1][item]; 
                            }
                            model.addConstr( lhs_linexp - rhs_linexp == 0 ); 
                        }

                        //  
                        for(const auto& item : outgoing_cur_node) {
                            model.addConstr( x[sene_idx][dmd_idx][0][item] == 0 );  
                        }
                    } 
                }

                for(int l = 0; l != hop_parameter; ++l) {
                    for(int i = 0; i != 2*pri_graph.cost_map.size(); ++i) { 
                        model.addConstr( x[sene_idx][dmd_idx][l][i] <= y[i/2] * Omega[sene_idx][i/2], "x_"+to_string(l)+"_"+to_string(i) ); 
                        // model.addConstr( x_l_ij[l][2*i+1] <= y[i] * omega[i], "x_"+to_string(l)+"_"+to_string(2*i+1) );
                    }
                }

            }


            // for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {
            //     // s^pq 对应的constraints
            //     double cpq = 0; 
            //     for(const auto& item : pri_graph.cost_map) {
            //         cpq += Dpq[dmd_idx] * item.second / fc_vc; 
            //     }                
            //     model.addConstr( -dualvar_b[sene_idx][dmd_idx] + dualvar_d[sene_idx][dmd_idx] + dualvar_f[sene_idx][dmd_idx] <= -cpq );

            //     // x^pq, 1 对应的constraints 
            //     // i == p 出发的
            //     int cur_node = pri_graph.demand_pairs[dmd_idx].first;  
            //     int tmp_idx = 0; 
            //     for(const auto& item : pri_graph.cost_map) {
            //         int arc_idx = -1;
            //         int end_node = -1;  
            //         if(item.first.first == cur_node) {
            //             arc_idx = tmp_idx; 
            //             end_node = item.first.second;  
            //         }
            //         if(item.first.second == cur_node) { 
            //             arc_idx = tmp_idx + 1; 
            //             end_node = item.first.first;  
            //         }
            //         if(arc_idx != -1 && end_node == pri_graph.demand_pairs[dmd_idx].second) {
            //             model.addConstr( dualvar_b[sene_idx][dmd_idx] - dualvar_d[sene_idx][dmd_idx] + dualvar_e[sene_idx][dmd_idx][0][arc_idx] <= Dpq[dmd_idx] * item.second / fc_vc ); 
            //         }
            //         if(arc_idx != -1 && end_node != pri_graph.demand_pairs[dmd_idx].second) {
            //             model.addConstr( dualvar_b[sene_idx][dmd_idx] + dualvar_c[sene_idx][dmd_idx][0][end_node] + dualvar_e[sene_idx][dmd_idx][0][arc_idx] <= Dpq[dmd_idx] * item.second / fc_vc ); 
            //         }
            //         tmp_idx += 2; 
            //     }

            //     // x^pq, 2-L , q 结尾
            //     // int end_node = pri_graph.demand_pairs[dmd_idx].second; 
            //     // for(int hop_idx = 1; hop_idx != hop_parameter; ++hop_idx) {
            //     //     int tmp_idx = 0; 
            //     //     for(const auto& item : pri_graph.cost_map) {
            //     //         int arc_idx = -1;
            //     //         int start_node = -1;  
            //     //         if(item.first.second == end_node) {
            //     //             arc_idx = tmp_idx; 
            //     //             start_node = item.first.first;  
            //     //         }
            //     //         if(item.first.first == end_node) { 
            //     //             arc_idx = tmp_idx + 1; 
            //     //             start_node = item.first.second;  
            //     //         } 
            //     //         if(arc_idx != -1) {
            //     //             model.addConstr( -dualvar_c[sene_idx][dmd_idx][hop_idx-1][start_node] - dualvar_d[sene_idx][dmd_idx] + dualvar_e[sene_idx][dmd_idx][hop_idx][arc_idx] <= Dpq[dmd_idx] * item.second / fc_vc ); 
            //     //         }
            //     //         tmp_idx += 2; 
            //     //     }
            //     // }


            //     for(int hop_idx = 1; hop_idx != hop_parameter; ++hop_idx) {
            //         int tmp_idx = 0; 
            //         for(const auto& item : pri_graph.cost_map) {
            //             int start_node = item.first.first; 
            //             int end_node = item.first.second; 
            //             if(start_node != pri_graph.demand_pairs[dmd_idx].first && end_node != pri_graph.demand_pairs[dmd_idx].second) {
            //                 model.addConstr( -dualvar_c[sene_idx][dmd_idx][hop_idx-1][start_node] + dualvar_c[sene_idx][dmd_idx][hop_idx][end_node] + dualvar_e[sene_idx][dmd_idx][hop_idx][tmp_idx] <= Dpq[dmd_idx] * item.second / fc_vc ); 
            //             }
            //             if(start_node != pri_graph.demand_pairs[dmd_idx].first && end_node == pri_graph.demand_pairs[dmd_idx].second) {
            //                 model.addConstr( -dualvar_c[sene_idx][dmd_idx][hop_idx-1][start_node] - dualvar_d[sene_idx][dmd_idx] + dualvar_e[sene_idx][dmd_idx][hop_idx][tmp_idx] <= Dpq[dmd_idx] * item.second / fc_vc ); 
            //             } 
            //             start_node = item.first.second; 
            //             end_node = item.first.first; 
            //             if(start_node != pri_graph.demand_pairs[dmd_idx].first && end_node != pri_graph.demand_pairs[dmd_idx].second) {
            //                 model.addConstr( -dualvar_c[sene_idx][dmd_idx][hop_idx-1][start_node] + dualvar_c[sene_idx][dmd_idx][hop_idx][end_node] + dualvar_e[sene_idx][dmd_idx][hop_idx][tmp_idx+1] <= Dpq[dmd_idx] * item.second / fc_vc ); 
            //             }
            //             if(start_node != pri_graph.demand_pairs[dmd_idx].first && end_node == pri_graph.demand_pairs[dmd_idx].second) {
            //                 model.addConstr( -dualvar_c[sene_idx][dmd_idx][hop_idx-1][start_node] - dualvar_d[sene_idx][dmd_idx] + dualvar_e[sene_idx][dmd_idx][hop_idx][tmp_idx+1] <= Dpq[dmd_idx] * item.second / fc_vc ); 
            //             } 
            //             tmp_idx += 2; 
            //         }
            //     }

            // }


            // GRBLinExpr dualobj = 0;
            // for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {  
            //     for(int hop_idx = 0; hop_idx != hop_parameter; ++hop_idx){  
            //         for(int arc_idx = 0; arc_idx != n_arcs; ++arc_idx) {
            //             dualobj += dualvar_e_y[sene_idx][dmd_idx][hop_idx][arc_idx] * Omega[sene_idx][arc_idx/2]; 
            //         }
            //     }
            // }
            // for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {  
            //     double cpq = 0; 
            //     for(const auto& item : pri_graph.cost_map) {
            //         cpq += Dpq[dmd_idx] * item.second / fc_vc; 
            //     }   
            //     dualobj += cpq; 
            //     dualobj += dualvar_f[sene_idx][dmd_idx]; 
            // } 

            // model.addConstr(Q_y_w[sene_idx] == dualobj); 

 
        }

        
 

        // Optimize model 
        // model.set(GRB_IntParam_Presolve, 0);
        // model.set(GRB_DoubleParam_Heuristics, 0);
        model.set(GRB_DoubleParam_TimeLimit, 3600*12);
            
        model.optimize();  
        cout << "Solution: " << endl; 
        double objective = 0.0;
        for(int i = 0; i != pri_graph.cost_map.size(); ++i) {  
            cout << y[i].get(GRB_StringAttr_VarName) << ": " << y[i].get(GRB_DoubleAttr_X) << endl; 
            objective += y_objs[i] * (y[i].get(GRB_DoubleAttr_X));
            result.push_back(round(y[i].get(GRB_DoubleAttr_X)));
        } 
        cout << eta.get(GRB_StringAttr_VarName) << ": " << eta.get(GRB_DoubleAttr_X) << endl; 
        for(int i = 0; i != Omega.size(); ++i) {
            app_Q_return[i] =  app_Q[i].get(GRB_DoubleAttr_X); 
            cout << q_i[i].get(GRB_StringAttr_VarName) << ": " << q_i[i].get(GRB_DoubleAttr_X) << endl;
        }
        
        // for(int i = 0; i != Omega.size(); ++i) {
        //     cout << abs[i].get(GRB_StringAttr_VarName) << ": " << abs[i].get(GRB_DoubleAttr_X) << endl;
        // }
        // for(const int& item : T) {
        //     cout << Q_y_w[item].get(GRB_StringAttr_VarName) << ": " << Q_y_w[item].get(GRB_DoubleAttr_X) << endl;
        // }
        // vector<double> output_theta(xi); 
        // for(int i = 0; i != xi; ++i) {
        //     output_theta[i] = theta[i].get(GRB_DoubleAttr_X);
        // } 
        // for(const int& item : T) {
        //     double val = 0.0;
        //     for(int i = 0; i != xi; ++i) {
        //         val += output_theta[i]*feature_matrix[item][i];
        //     }
        //     cout << "app_gx[" << item << "]: " << val << endl;
        // }

        cout << "first stage cost: " << objective << endl;
        cout << "Objective: " << model.get(GRB_DoubleAttr_ObjVal) << endl; 
        
 
        


    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }


 
    delete[] ub;

    return result; 
}





std::vector<double> solve_Q_giveny_theta(const PrimaryGraph& pri_graph, const std::vector<std::vector<int>>& Omega, const std::vector<double>& v_j_star, const std::vector<int>& Dpq, const int& n_dmd, const int& fc_vc, const int& hop_parameter, const double& epsilong, const std::string& instance_name, const int& xi, const int& L_bar, const std::vector<double>& Q_y_w) {

    vector<double> abs_result(Omega.size(), 0);

    // string line; 
    // vector<vector<double>> feature_matrix(Omega.size(), vector<double>(xi, 0.0));    
    // ifstream fin2("../output-learning/" + instance_name + "-feature.txt");  
    // for(int i = 0; i != Omega.size(); ++i) {
    //     getline(fin2, line); 
    //     stringstream ss(line);  
    //     for(int j = 0; j != xi; ++j) {
    //         ss >> feature_matrix[i][j];  
    //     }
    // } 
    // for(int i = 0; i != Omega.size(); ++i) {
    //     for(int j = 0; j != xi; ++j) {
    //         cout << feature_matrix[i][j] << " "; 
    //     }
    //     cout << endl; 
    // }  
    
    try {

        // Create an environment
        GRBEnv env = GRBEnv(); 
        // Create an empty model
        GRBModel model = GRBModel(env); 

        GRBVar* theta = model.addVars(pri_graph.cost_map.size(), GRB_CONTINUOUS); 
        for(int i = 0; i != pri_graph.cost_map.size(); ++i) {
            theta[i].set(GRB_DoubleAttr_LB, -GRB_INFINITY); 
            theta[i].set(GRB_StringAttr_VarName, "theta_" + to_string(i));
        }
 
        GRBVar theta0 = model.addVar(-GRB_INFINITY, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "theta0"); 
 
        GRBVar* abs = model.addVars(Omega.size(), GRB_CONTINUOUS); 
        for(int i = 0; i != Omega.size(); ++i) { 
            abs[i].set(GRB_StringAttr_VarName, "abs_" + to_string(i));
        }
 

        for(int i = 0; i != Omega.size(); ++i) {
            GRBLinExpr tmp_linexp = 0; 
            for(int j = 0; j != pri_graph.cost_map.size(); ++j) {
                tmp_linexp += theta[j] * Omega[i][j]; 
            }
            tmp_linexp += theta0; 
            model.addConstr(tmp_linexp - Q_y_w[i] <= abs[i]); 
            model.addConstr(Q_y_w[i] - tmp_linexp <= abs[i]);
        }

        GRBLinExpr obj = 0; 
        for(int i = 0; i != Omega.size(); ++i) {
            obj += abs[i]; 
        }
        model.setObjective(obj, GRB_MINIMIZE); 

        model.set(GRB_DoubleParam_TimeLimit, 3600*12);
            
        model.optimize();  
        cout << "Solution: " << endl;   
        for(int i = 0; i != pri_graph.cost_map.size(); ++i) { 
            // result[i] = theta[i].get(GRB_DoubleAttr_X); 
            cout << theta[i].get(GRB_StringAttr_VarName) << ": " << theta[i].get(GRB_DoubleAttr_X) << endl;
        }
        
        for(int i = 0; i != Omega.size(); ++i) {
            abs_result[i] = abs[i].get(GRB_DoubleAttr_X);
            cout << abs[i].get(GRB_StringAttr_VarName) << ": " << abs[i].get(GRB_DoubleAttr_X) << endl;
        }
        


        cout << "Objective: " << model.get(GRB_DoubleAttr_ObjVal) << endl;  
 
        


    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }


    return abs_result; 

}



std::vector<int> solve_MLAug_LR_withtarget(const PrimaryGraph& pri_graph, const std::vector<std::vector<int>>& Omega, const std::vector<double>& v_j_star, const std::vector<int>& Dpq, const int& n_dmd, const int& fc_vc, const int& hop_parameter, const double& epsilong, const std::string& instance_name, const int& xi, const int& L_bar, std::vector<int> T, std::vector<int> map_n_to_m, const double& target) {
    vector<int> result; 

    unordered_set<int> st(T.begin(), T.end());
    vector<int> notT;
    for(int i = 0; i < Omega.size(); ++i) {
        if(st.count(i) == 0) notT.push_back(i); 
    }
    


    int n_arcs = pri_graph.cost_map.size() * 2;
    double* ub = new double[n_arcs];
    for(int i = 0; i != n_arcs; ++i) {
        ub[i] = 1.0;
    }

    try {

        // Create an environment
        GRBEnv env = GRBEnv(); 
        // Create an empty model
        GRBModel model = GRBModel(env);
        
        GRBLinExpr realobj = 0.0; 

        // Create variables 
        GRBVar* y =  model.addVars(pri_graph.cost_map.size(), GRB_BINARY); 

        vector<double> y_objs(pri_graph.cost_map.size()); 
        int tmp_idx = 0; 
        for(const auto& item : pri_graph.cost_map) {
            realobj += y[tmp_idx] * item.second; 
            y_objs[tmp_idx] = item.second;
            y[tmp_idx].set(GRB_DoubleAttr_Obj, item.second);
            y[tmp_idx].set(GRB_StringAttr_VarName, "y_" + to_string(item.first.first) + "_" + to_string(item.first.second));
            ++tmp_idx; 
        } 
 

        GRBVar* q_i = model.addVars(Omega.size(), GRB_CONTINUOUS); 
        for(int i = 0; i != Omega.size(); ++i) {
            realobj += q_i[i] * v_j_star[i]; 
            q_i[i].set(GRB_DoubleAttr_LB, -GRB_INFINITY);
            q_i[i].set(GRB_DoubleAttr_Obj, v_j_star[i]);
            q_i[i].set(GRB_StringAttr_VarName, "q_" + to_string(i));
        }

        GRBVar eta = model.addVar(0.0, GRB_INFINITY, epsilong, GRB_CONTINUOUS, "eta"); 
        realobj += eta*epsilong; 

        GRBVar realobj_obs = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "realobj_obs"); 
        model.addConstr(realobj_obs >= realobj - target);
        model.addConstr(realobj_obs >= target - realobj);
        if(target == 0) {
            model.setObjective(realobj+0.0, GRB_MINIMIZE);
        }
        else {
            model.setObjective(realobj_obs+0.0, GRB_MINIMIZE);
        }
        

        GRBVar* theta = model.addVars(pri_graph.cost_map.size(), GRB_CONTINUOUS); 
        for(int i = 0; i != pri_graph.cost_map.size(); ++i) {
            theta[i].set(GRB_DoubleAttr_LB, -GRB_INFINITY); 
            theta[i].set(GRB_StringAttr_VarName, "theta_" + to_string(i));
        }
        GRBVar theta0 = model.addVar(-GRB_INFINITY, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "theta0");

        vector<vector<vector<GRBVar*>>> x;
        for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) {
            x.push_back(vector<vector<GRBVar*>>());
        }
        for(const int& sene_idx : T) { 
            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) { 
                x[sene_idx].push_back(vector<GRBVar*>());
                for(int hop_idx = 0; hop_idx != hop_parameter; ++hop_idx){ 
                    x[sene_idx][dmd_idx].push_back(     model.addVars(nullptr, nullptr, nullptr, nullptr, nullptr, n_arcs)     );
                }
            }
        } 

     
        vector<GRBVar*> s(Omega.size());
        for(const int& sene_idx : T) { 
            s[sene_idx] = model.addVars(n_dmd, GRB_CONTINUOUS);
            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) { 
                s[sene_idx][dmd_idx].set(GRB_DoubleAttr_UB, 1);
                s[sene_idx][dmd_idx].set(GRB_StringAttr_VarName, "s_" + to_string(sene_idx) + "_" + to_string(dmd_idx));
            }
        }


 
        GRBVar* app_Q = model.addVars(Omega.size(), GRB_CONTINUOUS); 
        for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) {
            app_Q[sene_idx].set(GRB_DoubleAttr_LB, -GRB_INFINITY);
            app_Q[sene_idx].set(GRB_StringAttr_VarName, "app_gx_"+to_string(sene_idx)); 
        }

        for(int n = 0; n < Omega.size(); ++n) {    
            GRBLinExpr app_gx = theta0; 
            for(int i = 0; i != pri_graph.cost_map.size(); ++i) {
                app_gx += theta[i] * Omega[n][i]; 
            }  
            model.addConstr(app_Q[n] == app_gx);  
        }



        GRBVar* Q_y_w = model.addVars(Omega.size(), GRB_CONTINUOUS); 
        for(int sene_idx = 0; sene_idx != Omega.size(); ++sene_idx) {
            Q_y_w[sene_idx].set(GRB_DoubleAttr_LB, -GRB_INFINITY);
            Q_y_w[sene_idx].set(GRB_StringAttr_VarName, "Q_"+to_string(sene_idx)); 
        }

        // GRBVar* abs = model.addVars(Omega.size(), GRB_CONTINUOUS); 
        // for(int i = 0; i != Omega.size(); ++i) { 
        //     abs[i].set(GRB_StringAttr_VarName, "abs_" + to_string(i));
        // }

    

        // constraints  
        for(int j = 0; j != Omega.size(); ++j) {
            for(const int& m : T) {
                double coef = 0.0; 
                for(int idx = 0; idx != pri_graph.cost_map.size(); ++idx) {
                    coef += (Omega[m][idx] - Omega[j][idx])*(Omega[m][idx] - Omega[j][idx]); 
                }
                coef = sqrt(coef); 
                model.addConstr(Q_y_w[m] <= coef*eta + q_i[j]); 
            }

            for(const int& n : notT) {                    
                double coef = 0.0; 
                for(int idx = 0; idx != pri_graph.cost_map.size(); ++idx) {
                    coef += (Omega[n][idx] - Omega[j][idx])*(Omega[n][idx] - Omega[j][idx]); 
                }
                coef = sqrt(coef); 
                model.addConstr(app_Q[n] <= coef*eta + q_i[j]);  
                 
            }
        }

        for(const int& n : notT) {
            model.addConstr(app_Q[n] >= Q_y_w[map_n_to_m[n]]);  
        }
        

        // GRBLinExpr endcons = 0;
        for(const int& m : T) { 
            model.addConstr(app_Q[m] == Q_y_w[m]);   // >= 
        } 



        for(const int& sene_idx : T) {

            GRBLinExpr gx = 0; 
            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {
                double cpq = 0; 
                for(const auto& item : pri_graph.cost_map) {
                    cpq += Dpq[dmd_idx] * item.second / fc_vc; 
                }
                gx += cpq - cpq*s[sene_idx][dmd_idx]; 
                for(int hop_idx = 0; hop_idx != hop_parameter; ++hop_idx) {
                    int tmp_idx = 0; 
                    for(const auto& item : pri_graph.cost_map) {
                        gx += x[sene_idx][dmd_idx][hop_idx][tmp_idx] * Dpq[dmd_idx] * item.second / fc_vc;
                        gx += x[sene_idx][dmd_idx][hop_idx][tmp_idx+1] * Dpq[dmd_idx] * item.second / fc_vc;
                        tmp_idx += 2; 
                    }
                }
            }
            model.addConstr(gx == Q_y_w[sene_idx]); 



            for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {
                for(int cur_node = 0; cur_node != pri_graph.link_table.size(); ++cur_node) {
                    vector<int> outgoing_cur_node; 
                    vector<int> into_cur_node; 
                    int tmp_idx = 0; 
                    for(const auto& item : pri_graph.cost_map) {
                        if(item.first.first == cur_node) {
                            outgoing_cur_node.push_back(tmp_idx);
                            into_cur_node.push_back(tmp_idx+1);
                        }
                        if(item.first.second == cur_node) {
                            into_cur_node.push_back(tmp_idx); 
                            outgoing_cur_node.push_back(tmp_idx+1); 
                        }
                        tmp_idx += 2; 
                    }

                    
                    if(cur_node == pri_graph.demand_pairs[dmd_idx].first) { 
                        GRBLinExpr tmp_linexp = 0; 
                        for(const auto& item : outgoing_cur_node)  tmp_linexp += x[sene_idx][dmd_idx][0][item]; 
                        model.addConstr( tmp_linexp - s[sene_idx][dmd_idx] == 0 );  

                        //  
                        for(int i = 0; i != hop_parameter; ++i) {
                            for(const auto& item : into_cur_node) {
                                model.addConstr( x[sene_idx][dmd_idx][i][item] == 0 ); 
                            }
                        }
                    }
                    else if(cur_node == pri_graph.demand_pairs[dmd_idx].second) {
                        GRBLinExpr tmp_linexp = 0; 
                        for(int i = 0; i != hop_parameter; ++i) {
                            for(const auto& item : into_cur_node) {
                                tmp_linexp += x[sene_idx][dmd_idx][i][item]; 
                            }
                        }
                        model.addConstr( tmp_linexp - s[sene_idx][dmd_idx] == 0 );  
                        //  
                        for(int i = 0; i != hop_parameter; ++i) {
                            for(const auto& item : outgoing_cur_node) {
                                model.addConstr( x[sene_idx][dmd_idx][i][item] == 0 ); 
                            }
                        } 
                    }
                    else {
                        for(int i = 0; i != hop_parameter-1; ++i) {
                            GRBLinExpr lhs_linexp = 0;
                            for(const auto& item : into_cur_node) {
                                lhs_linexp += x[sene_idx][dmd_idx][i][item]; 
                            }
                            GRBLinExpr rhs_linexp = 0; 
                            for(const auto& item : outgoing_cur_node) {
                                rhs_linexp += x[sene_idx][dmd_idx][i+1][item]; 
                            }
                            model.addConstr( lhs_linexp - rhs_linexp == 0 ); 
                        }

                        //  
                        for(const auto& item : outgoing_cur_node) {
                            model.addConstr( x[sene_idx][dmd_idx][0][item] == 0 );  
                        }
                    } 
                }

                for(int l = 0; l != hop_parameter; ++l) {
                    for(int i = 0; i != 2*pri_graph.cost_map.size(); ++i) { 
                        model.addConstr( x[sene_idx][dmd_idx][l][i] <= y[i/2] * Omega[sene_idx][i/2], "x_"+to_string(l)+"_"+to_string(i) ); 
                        // model.addConstr( x_l_ij[l][2*i+1] <= y[i] * omega[i], "x_"+to_string(l)+"_"+to_string(2*i+1) );
                    }
                }

            }


            // for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {
            //     // s^pq 对应的constraints
            //     double cpq = 0; 
            //     for(const auto& item : pri_graph.cost_map) {
            //         cpq += Dpq[dmd_idx] * item.second / fc_vc; 
            //     }                
            //     model.addConstr( -dualvar_b[sene_idx][dmd_idx] + dualvar_d[sene_idx][dmd_idx] + dualvar_f[sene_idx][dmd_idx] <= -cpq );

            //     // x^pq, 1 对应的constraints 
            //     // i == p 出发的
            //     int cur_node = pri_graph.demand_pairs[dmd_idx].first;  
            //     int tmp_idx = 0; 
            //     for(const auto& item : pri_graph.cost_map) {
            //         int arc_idx = -1;
            //         int end_node = -1;  
            //         if(item.first.first == cur_node) {
            //             arc_idx = tmp_idx; 
            //             end_node = item.first.second;  
            //         }
            //         if(item.first.second == cur_node) { 
            //             arc_idx = tmp_idx + 1; 
            //             end_node = item.first.first;  
            //         }
            //         if(arc_idx != -1 && end_node == pri_graph.demand_pairs[dmd_idx].second) {
            //             model.addConstr( dualvar_b[sene_idx][dmd_idx] - dualvar_d[sene_idx][dmd_idx] + dualvar_e[sene_idx][dmd_idx][0][arc_idx] <= Dpq[dmd_idx] * item.second / fc_vc ); 
            //         }
            //         if(arc_idx != -1 && end_node != pri_graph.demand_pairs[dmd_idx].second) {
            //             model.addConstr( dualvar_b[sene_idx][dmd_idx] + dualvar_c[sene_idx][dmd_idx][0][end_node] + dualvar_e[sene_idx][dmd_idx][0][arc_idx] <= Dpq[dmd_idx] * item.second / fc_vc ); 
            //         }
            //         tmp_idx += 2; 
            //     }

            //     // x^pq, 2-L , q 结尾
            //     // int end_node = pri_graph.demand_pairs[dmd_idx].second; 
            //     // for(int hop_idx = 1; hop_idx != hop_parameter; ++hop_idx) {
            //     //     int tmp_idx = 0; 
            //     //     for(const auto& item : pri_graph.cost_map) {
            //     //         int arc_idx = -1;
            //     //         int start_node = -1;  
            //     //         if(item.first.second == end_node) {
            //     //             arc_idx = tmp_idx; 
            //     //             start_node = item.first.first;  
            //     //         }
            //     //         if(item.first.first == end_node) { 
            //     //             arc_idx = tmp_idx + 1; 
            //     //             start_node = item.first.second;  
            //     //         } 
            //     //         if(arc_idx != -1) {
            //     //             model.addConstr( -dualvar_c[sene_idx][dmd_idx][hop_idx-1][start_node] - dualvar_d[sene_idx][dmd_idx] + dualvar_e[sene_idx][dmd_idx][hop_idx][arc_idx] <= Dpq[dmd_idx] * item.second / fc_vc ); 
            //     //         }
            //     //         tmp_idx += 2; 
            //     //     }
            //     // }


            //     for(int hop_idx = 1; hop_idx != hop_parameter; ++hop_idx) {
            //         int tmp_idx = 0; 
            //         for(const auto& item : pri_graph.cost_map) {
            //             int start_node = item.first.first; 
            //             int end_node = item.first.second; 
            //             if(start_node != pri_graph.demand_pairs[dmd_idx].first && end_node != pri_graph.demand_pairs[dmd_idx].second) {
            //                 model.addConstr( -dualvar_c[sene_idx][dmd_idx][hop_idx-1][start_node] + dualvar_c[sene_idx][dmd_idx][hop_idx][end_node] + dualvar_e[sene_idx][dmd_idx][hop_idx][tmp_idx] <= Dpq[dmd_idx] * item.second / fc_vc ); 
            //             }
            //             if(start_node != pri_graph.demand_pairs[dmd_idx].first && end_node == pri_graph.demand_pairs[dmd_idx].second) {
            //                 model.addConstr( -dualvar_c[sene_idx][dmd_idx][hop_idx-1][start_node] - dualvar_d[sene_idx][dmd_idx] + dualvar_e[sene_idx][dmd_idx][hop_idx][tmp_idx] <= Dpq[dmd_idx] * item.second / fc_vc ); 
            //             } 
            //             start_node = item.first.second; 
            //             end_node = item.first.first; 
            //             if(start_node != pri_graph.demand_pairs[dmd_idx].first && end_node != pri_graph.demand_pairs[dmd_idx].second) {
            //                 model.addConstr( -dualvar_c[sene_idx][dmd_idx][hop_idx-1][start_node] + dualvar_c[sene_idx][dmd_idx][hop_idx][end_node] + dualvar_e[sene_idx][dmd_idx][hop_idx][tmp_idx+1] <= Dpq[dmd_idx] * item.second / fc_vc ); 
            //             }
            //             if(start_node != pri_graph.demand_pairs[dmd_idx].first && end_node == pri_graph.demand_pairs[dmd_idx].second) {
            //                 model.addConstr( -dualvar_c[sene_idx][dmd_idx][hop_idx-1][start_node] - dualvar_d[sene_idx][dmd_idx] + dualvar_e[sene_idx][dmd_idx][hop_idx][tmp_idx+1] <= Dpq[dmd_idx] * item.second / fc_vc ); 
            //             } 
            //             tmp_idx += 2; 
            //         }
            //     }

            // }


            // GRBLinExpr dualobj = 0;
            // for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {  
            //     for(int hop_idx = 0; hop_idx != hop_parameter; ++hop_idx){  
            //         for(int arc_idx = 0; arc_idx != n_arcs; ++arc_idx) {
            //             dualobj += dualvar_e_y[sene_idx][dmd_idx][hop_idx][arc_idx] * Omega[sene_idx][arc_idx/2]; 
            //         }
            //     }
            // }
            // for(int dmd_idx = 0; dmd_idx != n_dmd; ++dmd_idx) {  
            //     double cpq = 0; 
            //     for(const auto& item : pri_graph.cost_map) {
            //         cpq += Dpq[dmd_idx] * item.second / fc_vc; 
            //     }   
            //     dualobj += cpq; 
            //     dualobj += dualvar_f[sene_idx][dmd_idx]; 
            // } 

            // model.addConstr(Q_y_w[sene_idx] == dualobj); 

 
        }

        
 

        // Optimize model 
        // model.set(GRB_IntParam_Presolve, 0);
        // model.set(GRB_DoubleParam_Heuristics, 0);
        model.set(GRB_DoubleParam_TimeLimit, 3600*12);
            
        model.optimize();  
        cout << "Solution: " << endl; 
        double objective = 0.0;
        for(int i = 0; i != pri_graph.cost_map.size(); ++i) {  
            cout << y[i].get(GRB_StringAttr_VarName) << ": " << y[i].get(GRB_DoubleAttr_X) << endl; 
            objective += y_objs[i] * (y[i].get(GRB_DoubleAttr_X));
            result.push_back(round(y[i].get(GRB_DoubleAttr_X)));
        } 
        cout << eta.get(GRB_StringAttr_VarName) << ": " << eta.get(GRB_DoubleAttr_X) << endl; 
        for(int i = 0; i != Omega.size(); ++i) { 
            cout << q_i[i].get(GRB_StringAttr_VarName) << ": " << q_i[i].get(GRB_DoubleAttr_X) << endl;
        }
        
        // for(int i = 0; i != Omega.size(); ++i) {
        //     cout << abs[i].get(GRB_StringAttr_VarName) << ": " << abs[i].get(GRB_DoubleAttr_X) << endl;
        // }
        // for(const int& item : T) {
        //     cout << Q_y_w[item].get(GRB_StringAttr_VarName) << ": " << Q_y_w[item].get(GRB_DoubleAttr_X) << endl;
        // }
        // vector<double> output_theta(xi); 
        // for(int i = 0; i != xi; ++i) {
        //     output_theta[i] = theta[i].get(GRB_DoubleAttr_X);
        // } 
        // for(const int& item : T) {
        //     double val = 0.0;
        //     for(int i = 0; i != xi; ++i) {
        //         val += output_theta[i]*feature_matrix[item][i];
        //     }
        //     cout << "app_gx[" << item << "]: " << val << endl;
        // }

        cout << "first stage cost: " << objective << endl;
        cout << "Objective: " << model.get(GRB_DoubleAttr_ObjVal) << endl; 
        
 
        


    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }


 
    delete[] ub;

    return result; 
}