#include"LShape.h"
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

#include<cfloat>  
 

#include<cmath>
using std::sqrt;
using std::abs;
using std::round;


// 对于每个commodity 来说 
double solve_recourese_problem(const PrimaryGraph& pri_graph, const int& fc_vc, const int& hop_parameter, const vector<int>& y, const vector<int>& omega, const int& p, const int& q, const int& dpq, double& cpq, double& pi_s, vector<vector<double>>& pi) {
    double optimal_value = 0.0;


    vector<double*> x_l_ij_lbs;
    vector<double*> x_l_ij_ubs; 
    vector<double*> x_l_ij_objs;
    vector<char*> x_l_ij_types; 
    vector<string*> x_l_ij_names;

    int n_arcs = pri_graph.cost_map.size() * 2;
    for(int i = 0; i != hop_parameter; ++i) {
        x_l_ij_lbs.push_back(new double[n_arcs]);   
        x_l_ij_ubs.push_back(new double[n_arcs]);   
        x_l_ij_objs.push_back(new double[n_arcs]); 
        x_l_ij_types.push_back(new char[n_arcs]);
        x_l_ij_names.push_back(new string[n_arcs]); 

        int tmp_idx = 0; 
        for(const auto& item : pri_graph.cost_map) {
            x_l_ij_lbs[i][tmp_idx] = 0; 
            x_l_ij_lbs[i][tmp_idx+1] = 0;
            x_l_ij_ubs[i][tmp_idx] = GRB_INFINITY; // TODO 求对偶的话不要变 
            x_l_ij_ubs[i][tmp_idx+1] = GRB_INFINITY; // TODO
            x_l_ij_objs[i][tmp_idx] = dpq * item.second / fc_vc; // d_pq c_{ij}
            x_l_ij_objs[i][tmp_idx+1] = dpq * item.second / fc_vc; // d_pq c_{ji}
            x_l_ij_types[i][tmp_idx] = GRB_CONTINUOUS;  
            x_l_ij_types[i][tmp_idx+1] = GRB_CONTINUOUS;  
            x_l_ij_names[i][tmp_idx] = "x_" + to_string(i) + "_" + to_string(item.first.first) + "_" + to_string(item.first.second);
            x_l_ij_names[i][tmp_idx+1] = "x_" + to_string(i) + "_" + to_string(item.first.second) + "_" + to_string(item.first.first);
            tmp_idx += 2; 
        }

        // 打印一下所有变量的属性 看看对不对 
        // for(int j = 0; j != n_arcs; ++j) {
        //     cout << "obj: " << x_l_ij_objs[i][j] << " type: " << x_l_ij_types[i][j] << " name: " << x_l_ij_names[i][j] << endl;
        // }
        // cout << "-------------------------------------------------------------" << endl; 
    }


    try {
        
        // Create an environment
        GRBEnv env = GRBEnv(true); 
        env.set(GRB_IntParam_OutputFlag, 0);
        env.start();
        // Create an empty model
        GRBModel model = GRBModel(env);
        
        // Create variables
        vector<GRBVar*> x_l_ij; 
        for(int i = 0; i != hop_parameter; ++i) {
            x_l_ij.push_back( model.addVars(x_l_ij_lbs[i], x_l_ij_ubs[i], x_l_ij_objs[i], x_l_ij_types[i], x_l_ij_names[i], n_arcs) ); // GRBVar*
        }

        cpq = 0; 
        for(const auto& item : pri_graph.cost_map) {
            cpq += dpq * item.second / fc_vc; 
        }

        GRBVar s = model.addVar(0, GRB_INFINITY, -cpq, GRB_CONTINUOUS, "s");
   
        // Set objective 
        // model.setObjective(x + y + 2 * z, GRB_MAXIMIZE);
        model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);

        // cout << p << " " << q << endl; 

        // Constraints 
        GRBConstr s_cons = model.addConstr(s <= 1); 


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

            

            if(cur_node == p) { 
                GRBLinExpr tmp_linexp = 0; 
                for(const auto& item : outgoing_cur_node)  tmp_linexp += x_l_ij[0][item]; 
                model.addConstr( tmp_linexp - s == 0 ); 

                // TODO
                for(int i = 0; i != hop_parameter; ++i) {
                    for(const auto& item : into_cur_node) {
                        model.addConstr( x_l_ij[i][item] == 0 );
                    }
                }
            }
            else if(cur_node == q) {
                GRBLinExpr tmp_linexp = 0; 
                for(int i = 0; i != hop_parameter; ++i) {
                    for(const auto& item : into_cur_node) {
                        tmp_linexp += x_l_ij[i][item]; 
                    }
                }
                model.addConstr( tmp_linexp - s == 0 ); 

                // TODO
                for(int i = 0; i != hop_parameter; ++i) {
                    for(const auto& item : outgoing_cur_node) {
                        model.addConstr( x_l_ij[i][item] == 0 );
                    }
                } 
            }
            else {
                for(int i = 0; i != hop_parameter-1; ++i) {
                    GRBLinExpr lhs_linexp = 0;
                    for(const auto& item : into_cur_node) {
                        lhs_linexp += x_l_ij[i][item]; 
                    }
                    GRBLinExpr rhs_linexp = 0; 
                    for(const auto& item : outgoing_cur_node) {
                        rhs_linexp += x_l_ij[i+1][item]; 
                    }
                    model.addConstr( lhs_linexp - rhs_linexp == 0 ); 
                }

                // TODO
                for(const auto& item : outgoing_cur_node) {
                    model.addConstr( x_l_ij[0][item] == 0 ); 
                }
            }

        }

        for(int l = 0; l != hop_parameter; ++l) {
            for(int i = 0; i != 2*pri_graph.cost_map.size(); ++i) { 
                model.addConstr( x_l_ij[l][i] <= y[i/2] * omega[i/2], "x_"+to_string(l)+"_"+to_string(i) );
                // model.addConstr( x_l_ij[l][2*i+1] <= y[i] * omega[i], "x_"+to_string(l)+"_"+to_string(2*i+1) );
            }
        }

          



        


        // Optimize model 
        model.optimize(); 
        // cout << "Solution: " << endl; 
        // for(int i = 0; i != hop_parameter; ++i) {
        //     for(int j = 0; j != n_arcs; ++j) {
        //         if(x_l_ij[i][j].get(GRB_DoubleAttr_X) != 0)
        //             cout << x_l_ij[i][j].get(GRB_StringAttr_VarName) << ": " << x_l_ij[i][j].get(GRB_DoubleAttr_X) << endl; 
        //     }
        // } 
        // cout << s.get(GRB_StringAttr_VarName) << ": " << s.get(GRB_DoubleAttr_X) << endl; 
        optimal_value = cpq + model.get(GRB_DoubleAttr_ObjVal);
        // cout << "Objective: " << optimal_value << endl;

        pi_s = s_cons.get(GRB_DoubleAttr_Pi);
        for(int l = 0; l != hop_parameter; ++l) {
            pi.push_back(vector<double>(2*pri_graph.cost_map.size(), 0));
            for(int i = 0; i != 2*pri_graph.cost_map.size(); ++i) {
                pi[l][i] = model.getConstrByName("x_"+to_string(l)+"_"+to_string(i)).get(GRB_DoubleAttr_Pi); 
            }
        }

        double dual_opt_val = cpq + 1*pi_s;
        for(int l = 0; l != hop_parameter; ++l) {
            for(int i = 0; i != pri_graph.cost_map.size(); ++i) {
                dual_opt_val += y[i] * omega[i] * (pi[l][2*i] + pi[l][2*i+1]); 
            }
        }

        // cout << dual_opt_val << endl; 



    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }


    for(int i = 0; i != hop_parameter; ++i) {
        delete[] x_l_ij_lbs[i];
        delete[] x_l_ij_ubs[i]; 
        delete[] x_l_ij_objs[i]; 
        delete[] x_l_ij_types[i]; 
        delete[] x_l_ij_names[i];  
    } 

    return optimal_value; 

}


double solve_distribution_seperation(const PrimaryGraph& pri_graph, const vector<vector<int>>& Omega, const vector<double>& v_j_star, const double& epsilong, const vector<double>& Q_y_omega_vec, vector<double>& v_results) {
    double result = 0.0;

    double* v_lbs = new double[Omega.size()];
    double* v_ubs = new double[Omega.size()];
    double* v_objs = new double[Omega.size()];
    char* v_types = new char[Omega.size()];
    string* v_names = new string[Omega.size()];
    for(int i = 0; i != Omega.size(); ++i) {
        v_lbs[i] = 0;
        v_ubs[i] = GRB_INFINITY;
        v_objs[i] = Q_y_omega_vec[i]; 
        v_types[i] = GRB_CONTINUOUS; 
        v_names[i] = "v_" + to_string(i); 
    }
    // 打印一下所有变量的属性 看看对不对 
    // for(int i = 0; i != Omega.size(); ++i) {
    //     cout << "obj: " << v_objs[i] << " type: " << v_types[i] << " name: " << v_names[i] << endl;
    // } 

    vector<double*> k_ij_lbs;
    vector<double*> k_ij_ubs; 
    vector<double*> k_ij_objs;
    vector<char*> k_ij_types; 
    vector<string*> k_ij_names;
 
    for(int i = 0; i != Omega.size(); ++i) {
        k_ij_lbs.push_back(new double[Omega.size()]);   
        k_ij_ubs.push_back(new double[Omega.size()]);   
        k_ij_objs.push_back(new double[Omega.size()]); 
        k_ij_types.push_back(new char[Omega.size()]);
        k_ij_names.push_back(new string[Omega.size()]); 

        for(int j = 0; j != Omega.size(); ++j) {
            k_ij_lbs[i][j] = 0;  
            k_ij_ubs[i][j] = GRB_INFINITY; 
            k_ij_objs[i][j] = 0; 
            k_ij_types[i][j] = GRB_CONTINUOUS;   
            k_ij_names[i][j] = "k_" + to_string(i) + "_" + to_string(j);  
        }

        // 打印一下所有变量的属性 看看对不对 
        // for(int j = 0; j != Omega.size(); ++j) {
        //     cout << "obj: " << k_ij_objs[i][j] << " type: " << k_ij_types[i][j] << " name: " << k_ij_names[i][j] << endl;
        // }
        // cout << "-------------------------------------------------------------" << endl; 
    }



    try {
        // Create an environment
        GRBEnv env = GRBEnv(true); 
        env.set(GRB_IntParam_OutputFlag, 0);
        env.start();
        // Create an empty model
        GRBModel model = GRBModel(env);
        
        // Create variables
        GRBVar* v =  model.addVars(v_lbs, v_ubs, v_objs, v_types, v_names, Omega.size()); 
        vector<GRBVar*> k; 
        for(int i = 0; i != Omega.size(); ++i) {
            k.push_back( model.addVars(k_ij_lbs[i], k_ij_ubs[i], k_ij_objs[i], k_ij_types[i], k_ij_names[i], Omega.size()) ); // GRBVar*
        }
   
        // Set objective 
        // model.setObjective(x + y + 2 * z, GRB_MAXIMIZE);
        model.set(GRB_IntAttr_ModelSense, GRB_MAXIMIZE);

        // Constraints 
        for(int i = 0; i != Omega.size(); ++i) {
            GRBLinExpr tmp_linexp = 0;
            for(int j = 0; j != Omega.size(); ++j) {
                tmp_linexp += k[i][j]; 
            }
            model.addConstr(tmp_linexp == v[i]); 
        }

        for(int j = 0; j != Omega.size(); ++j) {
            GRBLinExpr tmp_linexp = 0;
            for(int i = 0; i != Omega.size(); ++i) {
                tmp_linexp += k[i][j]; 
            }
            model.addConstr(tmp_linexp == v_j_star[j]); 
        }

        GRBLinExpr tmp_linexp = 0;
        for(int i = 0; i != Omega.size(); ++i) {
            tmp_linexp += v[i];  
        }
        model.addConstr(tmp_linexp == 1); 

        GRBLinExpr tmp_linexp2 = 0;
        for(int i = 0; i != Omega.size(); ++i) { 
            for(int j = 0; j != Omega.size(); ++j) {
                double coef = 0.0; 
                for(int idx = 0; idx != pri_graph.cost_map.size(); ++idx) {
                    coef += (Omega[i][idx] - Omega[j][idx])*(Omega[i][idx] - Omega[j][idx]); 
                }
                coef = sqrt(coef); 
                tmp_linexp2 += coef*k[i][j]; 
            } 
        }
        model.addConstr(tmp_linexp2 <= epsilong); 

        


        // Optimize model 
        model.optimize(); 
        // cout << "Solution: " << endl; 
        for(int i = 0; i != Omega.size(); ++i) { 
            v_results[i] = v[i].get(GRB_DoubleAttr_X); 
            // cout << v[i].get(GRB_StringAttr_VarName) << ": " << v[i].get(GRB_DoubleAttr_X) << endl; 
        }  
        result = model.get(GRB_DoubleAttr_ObjVal); 
        // cout << "Objective: " << result << endl;


    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }

 
    delete[] v_lbs;
    delete[] v_ubs; 
    delete[] v_objs; 
    delete[] v_types; 
    delete[] v_names;  

    for(int i = 0; i != Omega.size(); ++i) {
        delete[] k_ij_lbs[i];
        delete[] k_ij_ubs[i]; 
        delete[] k_ij_objs[i]; 
        delete[] k_ij_types[i]; 
        delete[] k_ij_names[i];  
    } 

    return result;
}



class LshapeCallback: public GRBCallback {
public: 
    // master problem 传进来的参数  
    const PrimaryGraph& pri_graph;
    const vector<vector<int>>& Omega;
    const vector<double>& v_j_star;
    const vector<int>& Dpq;
    const int& n_dmd;
    const int& fc_vc;
    const int& hop_parameter;
    const double& epsilong;
    GRBVar*& y; 
    GRBVar& theta; 

    double UB;

    // 记录cut数量 
    int n_lazycuts;  
    vector<GRBLinExpr> lazycuts; 

    LshapeCallback(const PrimaryGraph& pri_graph, const vector<vector<int>>& Omega, 
                    const vector<double>& v_j_star, const vector<int>& Dpq, 
                    const int& n_dmd, const int& fc_vc, 
                    const int& hop_parameter, const double& epsilong,
                    GRBVar*& y, GRBVar& theta):
                                                pri_graph(pri_graph), Omega(Omega), 
                                                v_j_star(v_j_star), Dpq(Dpq), 
                                                n_dmd(n_dmd), fc_vc(fc_vc), 
                                                hop_parameter(hop_parameter), epsilong(epsilong), 
                                                y(y), theta(theta) {UB = DBL_MAX; n_lazycuts = 0;}

    ~LshapeCallback() {}

protected:
    void callback() {
        try {
            if (where == GRB_CB_MIPSOL) { // GRB_CB_MIPNODE 
                cout << "CALLBACK开始运行....................................................................................." << endl; 
                // TODO: add lazy cut , check the bounded constraint 
                // n_lazycuts  
                // addLazy  
                vector<int> curr_y(pri_graph.cost_map.size(), 0); 
                double* curr_y_tmp = getSolution(y, pri_graph.cost_map.size());  // round(0.9999) 1.0111 int(0.999) = 0  
                for(int i = 0; i != pri_graph.cost_map.size(); ++i) {
                    curr_y[i] = round(curr_y_tmp[i]); 
                    cout << curr_y[i] << " "; 
                }
                cout << endl; 
                double curr_theta = getSolution(theta);
                cout << curr_theta << endl; 

                vector<double> Q_y_omega_vec(Omega.size(), 0); 
                vector<vector<double>> cpq_vec(Omega.size(), vector<double>(n_dmd, 0)); // dual mutipler 
                vector<vector<double>> pi_s_vec(Omega.size(), vector<double>(n_dmd, 0)); // dual mutipler 
                vector<vector< vector<vector<double>> >> pi; // dual mutipler
                for(int i = 0; i != Omega.size(); ++i) {
                    pi.push_back(vector< vector<vector<double>> >()); 
                    for(int demand_idx = 0; demand_idx != n_dmd; ++demand_idx) {
                        pi[i].push_back(vector<vector<double>>()); 
                    }
                }

                for(int m = 0; m != Omega.size(); ++m) {
                    double Q_y_omega = 0.0; 
                    for(int demand_idx = 0; demand_idx != n_dmd; ++demand_idx) {  
                        Q_y_omega += solve_recourese_problem(pri_graph, fc_vc, hop_parameter, curr_y, Omega[m], pri_graph.demand_pairs[demand_idx].first, pri_graph.demand_pairs[demand_idx].second, Dpq[demand_idx], cpq_vec[m][demand_idx], pi_s_vec[m][demand_idx], pi[m][demand_idx]);
                    } 
                    Q_y_omega_vec[m] = Q_y_omega; 
                }


                double try_upd_ub = 0.0; 
                int tmp_idx = 0; 
                for(const auto& item : pri_graph.cost_map) {
                    try_upd_ub += item.second * curr_y[tmp_idx]; 
                    ++tmp_idx; 
                }
                vector<double> v_results(Omega.size(), 0); 
                double theta_bnd = solve_distribution_seperation(pri_graph, Omega, v_j_star, epsilong, Q_y_omega_vec, v_results); 
                try_upd_ub += theta_bnd; 
                    
                // if(UB > try_upd_ub) {
                //     UB = try_upd_ub; 
                // }  
                // if(UB > cur_sol) {
                // double opt_cut_val = 0; 
                GRBLinExpr opt_cut = 0;
                for(int m = 0; m != Omega.size(); ++m) {
                    // double each_m_val = 0; 
                    GRBLinExpr each_m = 0;
                    for(int demand_idx = 0; demand_idx != n_dmd; ++demand_idx) {  
                        // each_m_val += cpq_vec[m][demand_idx] + pi_s_vec[m][demand_idx];
                        GRBLinExpr tmp_linexp = cpq_vec[m][demand_idx] + pi_s_vec[m][demand_idx];
                        for(int l = 0; l != hop_parameter; ++l) {
                            for(int i = 0; i != pri_graph.cost_map.size(); ++i) {
                                // each_m_val += curr_y[i] * Omega[m][i] * (pi[m][demand_idx][l][2*i] + pi[m][demand_idx][l][2*i+1]);
                                tmp_linexp += y[i] * Omega[m][i] * (pi[m][demand_idx][l][2*i] + pi[m][demand_idx][l][2*i+1]); 
                            }
                        }
                        each_m += tmp_linexp;
                    }
                    // opt_cut_val += v_results[m] * each_m_val; 
                    opt_cut += v_results[m] * each_m; 
                }
                cout << theta_bnd << " " << curr_theta << endl; 
                if(theta_bnd > curr_theta) {
                    cout << "加cut" << endl; 
                    addLazy(opt_cut <= theta);
                    ++n_lazycuts;
                    lazycuts.push_back(opt_cut); 
                } else {
                    cout << "不加cut" << endl; 
                }

                

                cout << "CALLBACK结束运行....................................................................................." << endl;
            }
        } 
        catch (GRBException e) {
            cout << "Error number: " << e.getErrorCode() << endl;
            cout << e.getMessage() << endl;
        } 
        catch (...) {
            cout << "Error during callback" << endl;
        }
    }
};




std::vector<int> solve_mainproblem_lshape_callback(const PrimaryGraph& pri_graph, const std::vector<std::vector<int>>& Omega, const std::vector<double>& v_j_star, const std::vector<int>& Dpq, const int& n_dmd, const int& fc_vc, const int& hop_parameter, const double& epsilong) {
    // 使用lshape callback法解主问题， 直接解会out of memory 
    vector<int> result(pri_graph.cost_map.size(), 0); 
    
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
    //     cout << "obj: " << y_objs[i] << " type: " << y_types[i] << " name: " << y_names[i] << endl;
    // } 

    try {

        // Create an environment
        GRBEnv env = GRBEnv(); 
        // Create an empty model
        GRBModel model = GRBModel(env);
        
        // Create variables
        GRBVar* y =  model.addVars(y_lbs, y_ubs, y_objs, y_types, y_names, pri_graph.cost_map.size());        
        GRBVar theta = model.addVar(-GRB_INFINITY, GRB_INFINITY, 1, GRB_CONTINUOUS, "theta");
   
        // Set objective 
        // model.setObjective(x + y + 2 * z, GRB_MAXIMIZE);
        model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);

        // maybe need add constraint to bound the objective value 
        GRBLinExpr obj = theta;
        for(int i = 0; i != pri_graph.cost_map.size(); ++i) {
            obj += y_objs[i]*y[i];
        }  
        model.addConstr(obj >= 0);
        // model.addConstr(theta >= 0);

        model.set(GRB_IntParam_LazyConstraints, 1); 
        LshapeCallback bcb(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, y, theta);
        model.setCallback(&bcb);

        // Optimize model 
        // model.set(GRB_IntParam_Presolve, 0);
        // model.set(GRB_IntParam_Cuts, 0);
        // model.set(GRB_DoubleParam_Heuristics, 0);
        model.set(GRB_DoubleParam_TimeLimit, 3600*12);
            
        model.optimize();  
        cout << "Solution: " << endl; 
        for(int i = 0; i != pri_graph.cost_map.size(); ++i) {  
            result[i] = round(y[i].get(GRB_DoubleAttr_X));
            cout << y[i].get(GRB_StringAttr_VarName) << ": " << y[i].get(GRB_DoubleAttr_X) << endl; 
        } 
        cout << theta.get(GRB_StringAttr_VarName) << ": " << theta.get(GRB_DoubleAttr_X) << endl; 
        // LB = model.get(GRB_DoubleAttr_ObjVal); 
        cout << "Objective: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
        // cout << "UB: " << UB << endl;
        cout << "Callback addLazy count: " << bcb.n_lazycuts << endl; 
        cout << "Lazycuts: " << endl; 
        cout << bcb.lazycuts.size() << endl;  
        for(int i = 0; i != bcb.lazycuts.size(); ++i) {
            cout << bcb.lazycuts[i].getValue() << " " << theta.get(GRB_DoubleAttr_X) << endl; 
        }
 


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
    return result;
}