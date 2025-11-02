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



// #include <cstdlib>
// #include <ctime>
// using namespace std; 





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
 
    PrimaryGraph pri_graph; 
    if(string(argv[1]) == "casestudy1") {
        // 0 5 上海 
        // 1 13 昆山
        // 2 55 南京
        // 3 54 杭州
        // 4 26 温州
        // 5 65 义乌
        // y_0_1: 1
        // y_0_3: 1
        // y_0_4: 1
        // y_0_5: 1
        // y_1_3: 1
        // y_1_4: -0
        // y_1_5: 0
        // y_2_0: 1
        // y_2_1: 1
        // y_2_3: 1
        // y_2_5: 0
        // y_3_4: 1
        // y_3_5: 0
        // y_5_4: 1
        pri_graph.link_table.push_back(vector<int>{1, 2, 3, 4, 5});
        pri_graph.link_table.push_back(vector<int>{0, 2, 3, 4, 5});
        pri_graph.link_table.push_back(vector<int>{0, 1, 3, 5});
        pri_graph.link_table.push_back(vector<int>{0, 1, 2, 4, 5});
        pri_graph.link_table.push_back(vector<int>{0, 1, 3, 5});
        pri_graph.link_table.push_back(vector<int>{0, 1, 2, 3, 4});
        pri_graph.cost_map[{2, 0}] = 305.0; // Nanjing to Shanghai 40
        pri_graph.cost_map[{2, 1}] = 266.0; // Nanjing to Kunshan 32
        pri_graph.cost_map[{2, 3}] = 274.0; // Nanjing to Hangzhou 27 
        pri_graph.cost_map[{2, 5}] = 410.0; // Nanjing to Yiwu 39
        pri_graph.cost_map[{0, 1}] = 63.0;  // Shanghai to Kunshan 9   
        pri_graph.cost_map[{0, 3}] = 176.0; // Shanghai to Hangzhou 20  x
        pri_graph.cost_map[{0, 5}] = 284.0; // Shanghai to Yiwu 32      x
        pri_graph.cost_map[{0, 4}] = 460.0; // Shanghai to Wenzhou 46
        pri_graph.cost_map[{1, 3}] = 173.0; // Kunshan to Hangzhou 15
        pri_graph.cost_map[{1, 5}] = 281.0; // Kunshan to Yiwu 27     
        pri_graph.cost_map[{1, 4}] = 457.0; // Kunshan to Wenzhou 44    x 
        pri_graph.cost_map[{3, 5}] = 132.0; // Hangzhou to Yiwu 12
        pri_graph.cost_map[{3, 4}] = 304.0; // Hangzhou to Wenzhou 32   x
        pri_graph.cost_map[{5, 4}] = 225.0; // Yiwu to Wenzhou 20
        pri_graph.demand_pairs.push_back({0, 1}); 
        pri_graph.demand_pairs.push_back({0, 2});
        pri_graph.demand_pairs.push_back({0, 3});
        pri_graph.demand_pairs.push_back({0, 4});
        pri_graph.demand_pairs.push_back({1, 3});
        pri_graph.demand_pairs.push_back({1, 4});  
        pri_graph.demand_pairs.push_back({2, 1});
        pri_graph.demand_pairs.push_back({2, 3});
        pri_graph.demand_pairs.push_back({2, 4});
        pri_graph.demand_pairs.push_back({3, 1});
        pri_graph.demand_pairs.push_back({3, 2});
        pri_graph.demand_pairs.push_back({3, 4});
        pri_graph.demand_pairs.push_back({4, 1});      
        pri_graph.demand_pairs.push_back({4, 2}); 
        pri_graph.demand_pairs.push_back({4, 3});
        pri_graph.demand_pairs.push_back({5, 0});
        pri_graph.demand_pairs.push_back({5, 1});
        pri_graph.demand_pairs.push_back({5, 2});
        pri_graph.demand_pairs.push_back({5, 3});
        pri_graph.demand_pairs.push_back({5, 4});      
    }
    else if(string(argv[1]) == "casestudy2") {
        // 0. 国立台湾大学医学院附设医院 25.040856, 121.518927          xxx 
        // 1. 三军总医院 25.071544, 121.591614
        // 2. 台北荣民总医院 25.119395, 121.520629
        // 3. 台北市立万芳医院 24.999731, 121.558606
        // 4. 台北长庚纪念医院及林口长庚纪念医院 25.062664, 121.365983
        // 5. 马偕纪念医院 25.057574, 121.524567
        // 6. 国泰综合医院 25.036644, 121.553851
        // 7. 新光吴火狮纪念医院 25.096122, 121.520198
        // 8. 亚东纪念医院 24.997014, 121.454258
        // 9. 卫生福利部双和医院 24.992613, 121.493158
        // 10. 佛教台北慈济医院 24.985695, 121.534998
        // 11. 国立台湾大学附设新竹医院 24.812725, 120.982867           xxx 
        // 12. 台中荣民总医院 24.184906, 120.603173
        // 13. 中国医药大学附设医院 24.156318, 120.679185
        // 14. 中山医学大学附设医院 24.120679, 120.650072
        // 15. 彰化基督教医院 24.074937, 120.541800                   xxx 
        // 16. 国立成功大学医学院附设医院 23.003235, 120.218567
        // 17. 奇美医院 23.021108, 120.222027                        xxx 
        // 18. 高雄荣民总医院 22.677447, 120.321933
        // 19. 高雄长庚纪念医院 22.649945, 120.356001
        // 20. 高雄医学大学附设中和纪念医院 22.646023, 120.309368       xxx 
        // 21. 佛教花莲慈济医院 24.005497, 121.625107                 xxx  

        // 22 基督复临安息日会医疗财团法人台安医院 25.028339, 121.548634
        // 23 天主教耕莘医疗财团法人耕莘医院 24.989678, 121.510728
        // 24 敏盛综合医院经国总院 25.016439, 121.305653
        // 25 马偕纪念医院新竹分院 24.799870, 120.990679
        // 26 国泰综合医院新竹分院 24.798147, 120.965385
        // 27 为恭纪念医院 24.687369, 120.907286
        // 28 卫生福利部丰原医院 24.242171, 120.726154
        // 29 童综合医院 24.244374, 120.533685
        // 30 佛教慈济综合医院台中分院 24.195175, 120.721074
        // 31 天主教圣马尔定医院 23.477146, 120.466590
        // 32 嘉义长庚纪念医院 23.464461, 120.283168              xxx
        // 33 台南市立医院 22.968597, 120.225676
        // 34 高雄市立大同医院 22.626965, 120.296822
        // 35 阮综合医院 22.615300, 120.297232
        // 36 马偕纪念医院台东分院 22.750807, 121.140061            xxx

        pri_graph.link_table.push_back(vector<int>{1, 2, 3, 4, 5, 6, 7});
        pri_graph.link_table.push_back(vector<int>{0, 2, 3, 4, 5, 6, 7});
        pri_graph.link_table.push_back(vector<int>{0, 1, 3, 4, 5, 6, 7});
        pri_graph.link_table.push_back(vector<int>{0, 1, 2, 4, 5, 6, 7});
        pri_graph.link_table.push_back(vector<int>{0, 1, 2, 3, 5, 6, 7});
        pri_graph.link_table.push_back(vector<int>{0, 1, 2, 3, 4, 6, 7});
        pri_graph.link_table.push_back(vector<int>{0, 1, 2, 3, 4, 5, 7});
        pri_graph.link_table.push_back(vector<int>{0, 1, 2, 3, 4, 5, 6});
        

        pri_graph.cost_map[{0, 1}] = 90.7;
        pri_graph.cost_map[{0, 2}] = 187.0;
        pri_graph.cost_map[{0, 3}] = 314.0;
        pri_graph.cost_map[{0, 4}] = 359.0;
        pri_graph.cost_map[{0, 5}] = 157.0;
        pri_graph.cost_map[{0, 6}] = 273.0;
        pri_graph.cost_map[{0, 7}] = 319.0;

        pri_graph.cost_map[{1, 2}] = 110.0;
        pri_graph.cost_map[{1, 3}] = 237.0;
        pri_graph.cost_map[{1, 4}] = 282.0;
        pri_graph.cost_map[{1, 5}] = 236.0;
        pri_graph.cost_map[{1, 6}] = 197.0;
        pri_graph.cost_map[{1, 7}] = 440.0;

        pri_graph.cost_map[{2, 3}] = 130.0;
        pri_graph.cost_map[{2, 4}] = 175.0;
        pri_graph.cost_map[{2, 5}] = 332.0;
        pri_graph.cost_map[{2, 6}] = 89.7;
        pri_graph.cost_map[{2, 7}] = 333.0;

        pri_graph.cost_map[{3, 4}] = 49.6;
        pri_graph.cost_map[{3, 5}] = 459.0;
        pri_graph.cost_map[{3, 6}] = 68.3;
        pri_graph.cost_map[{3, 7}] = 207.0;

        pri_graph.cost_map[{4, 5}] = 332.0;
        pri_graph.cost_map[{4, 6}] = 113.0;
        pri_graph.cost_map[{4, 7}] = 166.0;

        pri_graph.cost_map[{5, 6}] = 418.0;
        pri_graph.cost_map[{5, 7}] = 163.0;

        pri_graph.cost_map[{6, 7}] = 269.0; 

        // unordered_map<int, pair<double, double>> idx2latlon;
        // idx2latlon[0] = {25.040856, 121.518927}; //0 台北市医院集合  513
        // idx2latlon[1] = {24.812725, 120.982867}; //1 新竹市医院集合  137
        // idx2latlon[2] = {24.074937, 120.541800}; //2 彰化县医院集合  91
        // idx2latlon[3] = {23.021108, 120.222027}; //3 台南市医院集合  250
        // idx2latlon[4] = {22.646023, 120.309368}; //4 高雄市医院集合  778
        // idx2latlon[5] = {24.005497, 121.625107}; //5 花莲县医院集合  32
        // idx2latlon[6] = {23.464461, 120.283168}; //6 嘉义县医院集合  31
        // idx2latlon[7] = {22.750807, 121.140061}; //7 台东县医院集合  38 
        // for(int i = 0; i < 8; ++i) {
        //     for(int j = i+1; j < 8; ++j) {
        //         double buildcost = (idx2latlon[i].first - idx2latlon[j].first)*(idx2latlon[i].first - idx2latlon[j].first) + (idx2latlon[i].second - idx2latlon[j].second)*(idx2latlon[i].second - idx2latlon[j].second); 
        //         // 0-1, 1-2, 2-6, 6-3, 3-4, 4-7, 7-5, 5-0
        //         if( (i == 0 && j == 1) || 
        //             (i == 1 && j == 2) || 
        //             (i == 2 && j == 6) || 
        //             (i == 3 && j == 6) ||
        //             (i == 3 && j == 4) ||
        //             (i == 4 && j == 7) ||
        //             (i == 5 && j == 7) ||
        //             (i == 0 && j == 5) )
        //         { pri_graph.cost_map[{i, j}] = buildcost*1000; }  
        //         else
        //         { pri_graph.cost_map[{i, j}] = 5*buildcost*1000; } // 2 5 8
        //     }
        // }
        
        pri_graph.demand_pairs.push_back({0, 1});     
        pri_graph.demand_pairs.push_back({0, 2}); 
        pri_graph.demand_pairs.push_back({0, 3}); 
        pri_graph.demand_pairs.push_back({0, 4}); 
        pri_graph.demand_pairs.push_back({0, 5}); 
        pri_graph.demand_pairs.push_back({0, 7});
        pri_graph.demand_pairs.push_back({1, 0});
        pri_graph.demand_pairs.push_back({1, 2});
        pri_graph.demand_pairs.push_back({1, 3});
        pri_graph.demand_pairs.push_back({1, 4}); 
        pri_graph.demand_pairs.push_back({2, 0}); 
        pri_graph.demand_pairs.push_back({2, 3});
        pri_graph.demand_pairs.push_back({3, 0});
        pri_graph.demand_pairs.push_back({3, 4});
        pri_graph.demand_pairs.push_back({3, 5}); 
        pri_graph.demand_pairs.push_back({4, 0}); 
        pri_graph.demand_pairs.push_back({4, 1}); 
        pri_graph.demand_pairs.push_back({4, 3}); 
        pri_graph.demand_pairs.push_back({5, 7}); 
        pri_graph.demand_pairs.push_back({6, 0}); 
 

    }
    else {
        PrimaryGraph tmp_graph("../data/" + string(argv[1]) + ".txt"); //   "../data/abilene.txt"
        pri_graph = tmp_graph; 
    }

    // PrimaryGraph pri_graph("../data/" + string(argv[1]) + ".txt"); //   "../data/abilene.txt"
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
    // } 9 + 15x = 1 
 






    vector<double> v_j_star(Omega.size(), 1.0/Omega.size());  
    vector<int> Dpq(8, 8); 
    int n_dmd = 8;
    int fc_vc = 30; 
    int hop_parameter = 5;   
    double epsilong = 0.5;  



      





    // case study1 
    // 844: 211 


    // // vector<double> vjtotal = {0.0, 40, 32, 27, 38, 9, 20, 32, 46, 15, 27, 43, 12, 32, 20, 31, 3, 13, 0, 16, 0, 14, 0, 13, 0, 0, 4, 3, 2, 2, 0, 11, 0, 10, 0, 11, 0, 0, 3, 2, 27, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 6, 2, 2, 4, 0, 1, 1, 0, 17, 0, 13, 0, 0, 3, 3, 29, 0, 25, 0, 0, 15, 3, 0, 23, 1, 0, 0, 14, 15, 5, 0, 2, 2, 0, 0, 0, 0, 12, 28, 16, 12, 0, 14};
    
    // // for(int iter = 0; iter < 4; ++iter) {
    // //     vector<double> v_j_star(vjtotal.size()); 
    // //     vector<int> notzeroidx;
    // //     for(int i = 0; i < vjtotal.size(); ++i) {
    // //         if(vjtotal[i] > 0) {
    // //             notzeroidx.push_back(i);  
    // //             cout << i << endl; 
    // //         }
    // //     }
    // //     random_device rd_y;  
    // //     mt19937 gen_y(rd_y()); 
    // //     uniform_int_distribution<> dis_y(0, notzeroidx.size()-1);
    // //     for(int i = 0; i < 211; ++i) {   
    // //         int addidx = notzeroidx[dis_y(gen_y)]; 
    // //         // cout << addidx << " "; 
    // //         if(vjtotal[addidx] > 0) { 
    // //             v_j_star[addidx] += 1.0; 
    // //             vjtotal[addidx] -= 1.0; 
    // //         } 
    // //         else {
    // //             --i; 
    // //         }
    // //     }

    // //     for(const double& item : v_j_star) {
    // //         cout << item << ", "; 
    // //     }
    // //     cout << endl;

    // //     for(const double& item : vjtotal) {
    // //         cout << item << ", "; 
    // //     }
    // //     cout << endl;
    // // }
     

    // vector<double> v_j_star1 = {0, 1, 5, 1, 3, 3, 2, 2, 3, 4, 3, 3, 7, 9, 3, 2, 3, 7, 0, 5, 0, 4, 0, 1, 0, 0, 4, 3, 2, 2, 0, 3, 0, 6, 0, 3, 0, 0, 3, 2, 4, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 3, 2, 2, 4, 0, 1, 1, 0, 11, 0, 6, 0, 0, 3, 3, 4, 0, 6, 0, 0, 4, 3, 0, 4, 1, 0, 0, 5, 5, 1, 0, 2, 2, 0, 0, 0, 0, 5, 5, 4, 5, 0, 8};
    // vector<double> v_j_star2 = {0, 3, 2, 3, 6, 6, 10, 3, 5, 7, 3, 7, 4, 8, 2, 5, 0, 6, 0, 6, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 3, 0, 4, 0, 8, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 2, 0, 0, 0, 0, 0, 0, 0, 3, 0, 6, 0, 0, 0, 0, 9, 0, 8, 0, 0, 3, 0, 0, 5, 0, 0, 0, 9, 8, 4, 0, 0, 0, 0, 0, 0, 0, 7, 10, 6, 4, 0, 2};
    // vector<double> v_j_star3 = {0, 7, 9, 12, 10, 0, 8, 6, 12, 4, 8, 9, 1, 11, 8, 11, 0, 0, 0, 5, 0, 6, 0, 8, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 3, 0, 1, 0, 0, 0, 0, 16, 0, 8, 0, 0, 8, 0, 0, 8, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 3, 0, 4};
    // vector<double> v_j_star4 = {0, 29, 16, 11, 19, 0, 0, 21, 26, 0, 13, 24, 0, 4, 7, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0}; 
    // vector<double> v_j_star(v_j_star1.size()); 
    // for(int i  = 0; i < v_j_star.size(); ++i) {
    //     v_j_star[i] = v_j_star1[i] + v_j_star2[i] + v_j_star3[i] + v_j_star4[i];
    //     // cout << v_j_star[i] << endl; 
    // }


    // double d = accumulate(v_j_star.begin(), v_j_star.end(), 0.0);
    // cout << d << endl; 
    // for(double& item : v_j_star) {
    //     item /= d; 
    // }
    // d = accumulate(v_j_star.begin(), v_j_star.end(), 0.0);
    // cout << d << endl;
 
    

    // vector<int> Dpq = {1650, 3514, 1944, 12797, 3, 22, 5, 7, 59, 1, 1, 26, 11, 26, 172, 696, 5, 35, 14, 85}; 
    // int n_dmd = Dpq.size();
    // int fc_vc = 6000; 
    // int hop_parameter = 3;  
    // double epsilong = 0.8;   


    // // for(double epsilong = 0.1; epsilong < 2.1; epsilong += 0.1) {
    // //     vector<int> ds_opt = solve_mainproblem(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong);
    // //     vector<double> qval = solve_Q_giveny(pri_graph, Omega, vector<double>(), Dpq, n_dmd, fc_vc, hop_parameter, 0.0, ds_opt);
    // //     // out-of-sample performance 
    // //     double avg_val = 0.0; 
    // //     for(int i = 0; i < v_j_star1.size(); ++i) {
    // //         avg_val += v_j_star1[i] * qval[i]; 
    // //     }
    // //     avg_val /= accumulate(v_j_star1.begin(), v_j_star1.end(), 0.0);

    // //     cout << "epsilon: " << epsilong << ", " << avg_val << "." << endl; 
    // // }




















    // casestudy2   2544   636
    // // vector<double> vjtotal = {4, 9, 23, 26, 38, 13, 52, 3, 14, 17, 35, 7, 31, 8, 12, 39, 3, 15, 1, 58, 4, 11, 56, 5, 8, 51, 64, 16, 4, 4, 4, 4, 4, 4, 1, 1, 1, 2, 1, 1, 0, 0, 2, 0, 0, 0, 2, 0, 0, 2, 0, 0, 2, 2, 0, 7, 7, 5, 8, 5, 3, 4, 4, 6, 4, 3, 0, 0, 2, 0, 0, 0, 2, 0, 0, 2, 0, 0, 2, 2, 0, 15, 8, 11, 8, 2, 11, 13, 7, 4, 6, 7, 10, 5, 2, 4, 1, 8, 4, 2, 4, 5, 1, 8, 2, 5, 10, 8, 15, 2, 5, 9, 9, 2, 13, 2, 4, 8, 0, 5, 1, 13, 2, 3, 11, 2, 1, 11, 2, 3, 5, 20, 1, 1, 1, 32, 1, 2, 0, 0, 29, 0, 0, 0, 27, 0, 0, 27, 0, 0, 27, 27, 0, 5, 2, 7, 6, 6, 6, 4, 3, 4, 3, 3, 1, 0, 3, 1, 0, 2, 1, 0, 5, 2, 1, 1, 1, 1, 16, 1, 21, 0, 0, 20, 0, 9, 0, 30, 0, 6, 31, 0, 6, 25, 35, 6, 3, 3, 2, 3, 2, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 11, 3, 7, 5, 8, 10, 1, 3, 2, 1, 5, 4, 2, 2, 5, 1, 5, 1, 4, 3, 5, 6, 5, 9, 3, 1, 4, 1, 8, 2, 3, 4, 3, 1, 6, 1, 6, 3, 4, 0, 0, 29, 0, 0, 0, 27, 0, 0, 27, 0, 0, 27, 27, 0, 3, 3, 3, 1, 3, 0, 0, 1, 1, 0, 1, 1, 0, 3, 1, 1, 1, 2, 7, 0, 14, 0, 16, 1, 8, 15, 1, 7, 15, 13, 9, 8, 0, 3, 1, 1, 3, 4, 2, 1, 5, 1, 2, 0, 4, 0, 3, 2, 1, 6, 4, 2, 1, 5, 1, 4, 0, 6, 0, 1, 0, 35, 0, 0, 35, 0, 0, 35, 30, 0, 0, 0, 0, 1, 0, 0, 1, 0, 2, 0, 1, 0, 4, 1, 8, 4, 1, 7, 3, 8, 9, 1, 1, 1, 0, 1, 0, 0, 0, 0, 2, 2, 52, 3, 1, 46, 41, 4, 2, 1, 4, 1, 1, 0, 2, 3, 2, 7, 0, 6, 10, 1, 1, 44, 42, 4, 1, 1, 0, 3, 0, 6, 7, 35, 1, 6};
    
    // // for(int iter = 0; iter < 4; ++iter) {
    // //     vector<double> v_j_star(vjtotal.size()); 
    // //     vector<int> notzeroidx;
    // //     for(int i = 0; i < vjtotal.size(); ++i) {
    // //         if(vjtotal[i] > 0) {
    // //             notzeroidx.push_back(i);  
    // //             cout << i << endl; 
    // //         }
    // //     }
    // //     random_device rd_y;  
    // //     mt19937 gen_y(rd_y()); 
    // //     uniform_int_distribution<> dis_y(0, notzeroidx.size()-1);
    // //     for(int i = 0; i < 636; ++i) {   
    // //         int addidx = notzeroidx[dis_y(gen_y)]; 
    // //         // cout << addidx << " "; 
    // //         if(vjtotal[addidx] > 0) { 
    // //             v_j_star[addidx] += 1.0; 
    // //             vjtotal[addidx] -= 1.0; 
    // //         } 
    // //         else {
    // //             --i; 
    // //         }
    // //     }

    // //     for(const double& item : v_j_star) {
    // //         cout << item << ", "; 
    // //     }
    // //     cout << endl;

    // //     for(const double& item : vjtotal) {
    // //         cout << item << ", "; 
    // //     }
    // //     cout << endl;
    // // }
    
    
    // vector<double> v_j_star1 = {3, 4, 2, 3, 1, 2, 4, 3, 0, 1, 2, 5, 4, 3, 4, 3, 1, 3, 1, 2, 3, 0, 1, 1, 0, 0, 2, 3, 1, 2, 3, 3, 3, 2, 1, 1, 1, 2, 1, 1, 0, 0, 2, 0, 0, 0, 2, 0, 0, 2, 0, 0, 2, 1, 0, 3, 1, 5, 4, 2, 3, 2, 4, 4, 1, 2, 0, 0, 2, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 2, 0, 2, 4, 1, 3, 2, 2, 1, 4, 2, 3, 2, 4, 3, 2, 2, 1, 2, 2, 2, 2, 4, 1, 1, 2, 3, 5, 3, 6, 2, 2, 2, 3, 2, 4, 2, 4, 1, 0, 5, 1, 4, 2, 3, 1, 2, 1, 1, 2, 1, 3, 4, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 3, 0, 0, 1, 0, 0, 3, 0, 2, 1, 2, 3, 2, 2, 4, 3, 3, 1, 0, 2, 1, 0, 2, 1, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 0, 0, 3, 0, 1, 0, 4, 0, 5, 2, 0, 2, 3, 3, 1, 3, 2, 2, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 2, 3, 2, 3, 4, 1, 3, 2, 1, 2, 4, 2, 2, 3, 1, 4, 1, 1, 0, 5, 1, 3, 5, 3, 1, 4, 1, 3, 2, 2, 3, 3, 1, 2, 1, 2, 1, 3, 0, 0, 2, 0, 0, 0, 5, 0, 0, 1, 0, 0, 3, 2, 0, 3, 2, 3, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 2, 1, 1, 1, 1, 2, 0, 3, 0, 4, 1, 0, 4, 0, 2, 2, 1, 1, 7, 0, 3, 1, 1, 3, 0, 2, 1, 2, 1, 2, 0, 1, 0, 3, 2, 1, 2, 4, 2, 1, 2, 1, 3, 0, 2, 0, 1, 0, 4, 0, 0, 1, 0, 0, 2, 4, 0, 0, 0, 0, 1, 0, 0, 1, 0, 2, 0, 0, 0, 4, 1, 3, 2, 1, 2, 3, 2, 2, 1, 1, 1, 0, 1, 0, 0, 0, 0, 2, 2, 3, 3, 0, 1, 4, 0, 0, 1, 3, 1, 1, 0, 2, 1, 2, 2, 0, 2, 3, 1, 1, 5, 2, 4, 1, 1, 0, 3, 0, 5, 3, 2, 1, 4};
    // vector<double> v_j_star2 = {1, 5, 6, 3, 7, 2, 2, 0, 10, 3, 4, 2, 6, 5, 8, 4, 2, 5, 0, 6, 1, 4, 7, 3, 8, 1, 7, 5, 3, 2, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 3, 6, 0, 3, 3, 0, 2, 0, 2, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 9, 4, 4, 5, 0, 4, 6, 3, 2, 3, 5, 6, 2, 0, 2, 0, 5, 2, 0, 2, 1, 0, 4, 0, 2, 5, 1, 7, 0, 3, 6, 1, 0, 4, 0, 0, 7, 0, 0, 0, 5, 0, 0, 6, 0, 0, 4, 0, 2, 2, 8, 0, 0, 0, 7, 0, 2, 0, 0, 5, 0, 0, 0, 4, 0, 0, 1, 0, 0, 3, 3, 0, 2, 2, 5, 5, 1, 3, 2, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 4, 0, 3, 0, 0, 6, 0, 2, 0, 1, 0, 1, 7, 0, 4, 6, 2, 5, 0, 1, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 3, 1, 4, 2, 5, 6, 0, 0, 0, 0, 3, 0, 0, 0, 2, 0, 1, 0, 3, 3, 0, 3, 2, 4, 0, 0, 0, 0, 5, 0, 1, 1, 0, 0, 2, 0, 4, 2, 1, 0, 0, 3, 0, 0, 0, 3, 0, 0, 4, 0, 0, 2, 5, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 2, 0, 7, 0, 11, 0, 5, 3, 1, 4, 10, 3, 6, 1, 0, 0, 0, 0, 0, 4, 0, 0, 3, 0, 0, 0, 2, 0, 0, 0, 0, 4, 0, 0, 0, 3, 0, 1, 0, 4, 0, 0, 0, 4, 0, 0, 5, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 5, 2, 0, 3, 0, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 1, 6, 10, 2, 2, 0, 1, 0, 0, 0, 0, 2, 0, 2, 0, 4, 5, 0, 0, 4, 2, 0, 0, 0, 0, 0, 0, 1, 4, 5, 0, 2};
    // vector<double> v_j_star3 = {0, 0, 7, 10, 16, 9, 7, 0, 4, 13, 15, 0, 11, 0, 0, 23, 0, 7, 0, 14, 0, 7, 20, 1, 0, 13, 9, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 6, 0, 0, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 4, 2, 0, 0, 1, 5, 0, 5, 0, 0, 0, 0, 0, 0, 4, 0, 0, 4, 0, 0, 6, 0, 0, 0, 8, 0, 0, 0, 15, 0, 0, 0, 0, 7, 0, 0, 0, 10, 0, 0, 10, 0, 0, 8, 13, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 8, 0, 15, 0, 0, 8, 0, 6, 0, 8, 0, 0, 10, 0, 0, 14, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 17, 0, 0, 0, 19, 0, 0, 11, 0, 0, 7, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 4, 0, 1, 0, 3, 8, 0, 1, 3, 9, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 11, 0, 0, 5, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 0, 0, 13, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 2, 0, 0, 12, 10, 0, 0, 0, 0, 0, 0, 0, 0, 11, 0, 0};
    // vector<double> v_j_star4 = {0, 0, 8, 10, 14, 0, 39, 0, 0, 0, 14, 0, 10, 0, 0, 9, 0, 0, 0, 36, 0, 0, 28, 0, 0, 37, 46, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 17, 0, 0, 0, 9, 0, 0, 13, 0, 0, 15, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 2, 0, 0, 3, 0, 0, 0, 17, 0, 0, 12, 0, 0, 2, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 11, 0, 0, 15, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 17, 0, 0, 18, 0, 0, 23, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 26, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 28, 0, 0, 0, 0, 0, 0, 0, 0, 17, 0, 0}; 
    // vector<double> v_j_star(v_j_star1.size()); 
    // for(int i  = 0; i < v_j_star.size(); ++i) {
    //     v_j_star[i] = v_j_star1[i] + v_j_star2[i] + v_j_star3[i] + v_j_star4[i];
    //     // cout << v_j_star[i] << endl; 
    // }


    // double d = accumulate(v_j_star.begin(), v_j_star.end(), 0.0);
    // cout << d << endl; 
    // for(double& item : v_j_star) {
    //     item /= d; 
    // }
    // d = accumulate(v_j_star.begin(), v_j_star.end(), 0.0);
    // cout << d << endl;
 
 
    // vector<int> Dpq = {14, 17, 42, 88, 3, 2, 3, 1, 1, 2, 4, 3, 6, 3, 1, 15, 1, 8, 1, 1}; 
    // int n_dmd = Dpq.size();
    // int fc_vc = 30; 
    // int hop_parameter = 2;  
    // double epsilong = 10;  

    // // for(double epsilong = 2; epsilong < 10; epsilong += 1) {
    // //     vector<int> ds_opt = solve_mainproblem(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong);
    // //     vector<double> qval = solve_Q_giveny(pri_graph, Omega, vector<double>(), Dpq, n_dmd, fc_vc, hop_parameter, 0.0, ds_opt);
    // //     // out-of-sample performance 
    // //     double avg_val = 0.0; 
    // //     for(int i = 0; i < v_j_star4.size(); ++i) {
    // //         avg_val += v_j_star4[i] * qval[i]; 
    // //     }
    // //     avg_val /= accumulate(v_j_star4.begin(), v_j_star4.end(), 0.0);

    // //     cout << "epsilon: " << epsilong << ", " << avg_val << "." << endl; 
    // // } 





    // -------------------------- Traditional Method: L Shape (callback) -------------------------- 

    // cout << "LShape Callback Solve 算法开始运行" << endl; 
    // steady_clock::time_point callback_start = steady_clock::now();
    // vector<int> callback_opt = solve_mainproblem_lshape_callback(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong);  // solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, callback_opt);
    // steady_clock::time_point callback_end = steady_clock::now();
    // cout << "LShape Callback Solve 算法结束运行" << endl; 
    // cout << "LShape Callback Solve 算法耗时: " << (callback_end - callback_start).count() / 1e9 << " 秒" << endl; 



    // -------------------------- Traditional Method: Direct Solve (gurobi) --------------------------

    cout << "Direct Solve 算法开始运行" << endl; 
    steady_clock::time_point ds_start = steady_clock::now();
    vector<int> ds_opt = solve_mainproblem(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong);  // solve_mainproblem_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, ds_opt);
    steady_clock::time_point ds_end = steady_clock::now();
    cout << "Direct Solve 算法结束运行" << endl; 
    cout << "Direct Solve 算法耗时: " << (ds_end - ds_start).count() / 1e9 << " 秒" << endl; 







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

    // cout << "Integrate KNN 开始运行" << endl; 
    // steady_clock::time_point inte_knn_start = steady_clock::now();
    // int n_sim = 100;   
    // int k_max = 5; 
    // int T_size = int(Omega.size() * 0.05); 
    // T_size = T_size < k_max ? k_max : T_size; 

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

    // while(true) {
    //     cout << "curr best: " << best << endl;
    //     vector<double> Q_giveny = solve_Q_giveny(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, ystar); 

    //     //one implement ---
    //     // int opt_k = 1; 
    //     // unordered_set<double> st; 
    //     // for(const auto& item : Q_giveny) st.insert(item);
    //     // cout << st.size() << " vs " << T_size << endl; 
    //     // if(st.size() > T_size) {
    //     //     int n_del = st.size() - T_size;  
    //     //     for(int i = 0; i < n_del; ++i) {
    //     //         double next_remove_val = DBL_MAX; 
    //     //         double next_remove = DBL_MAX; 
    //     //         for(const double& item : st) {
    //     //             double nearest_diff = DBL_MAX; 
    //     //             double nearest = DBL_MAX; 
    //     //             for(const double& nearitem : st) {
    //     //                 if(item == nearitem) continue; 
    //     //                 if(abs(nearitem - item) < nearest_diff) {
    //     //                     nearest_diff = abs(nearitem - item);
    //     //                     nearest = nearitem; 
    //     //                 } 
    //     //             }
    //     //             // cout << item << " " << nearest << " " << nearest_diff << endl; 
    //     //             if(nearest_diff < next_remove_val) {
    //     //                 next_remove_val = nearest_diff;
    //     //                 next_remove = item; 
    //     //             }
    //     //         }
    //     //         cout << next_remove << " " << next_remove_val << endl; 
    //     //         st.erase(next_remove);
    //     //         cout << st.size() << endl; 
    //     //     }
    //     // }

    //     // vector<int> T; 
    //     // for(const double& item : st) {
    //     //     for(int i = 0; i < Q_giveny.size(); ++i) {
    //     //         if(item == Q_giveny[i]) {
    //     //             T.push_back(i);
    //     //             break; 
    //     //         }
    //     //     }
    //     // }

    //     // vector<int> map_n_to_m(Omega.size(), -1); 
    //     // for(int i = 0; i < Omega.size(); ++i) {
    //     //     int near_idx = -1; 
    //     //     double near_val = DBL_MAX; 
    //     //     for(const int& item : T) {
    //     //         if(abs(Q_giveny[item]-Q_giveny[i]) < near_val) {
    //     //             near_idx = item; 
    //     //             near_val = abs(Q_giveny[item]-Q_giveny[i]); 
    //     //         }
    //     //     }
    //     //     map_n_to_m[i] = near_idx; 
    //     // }
    //     // for(const int& item : T) {
    //     //     map_n_to_m[item] = item;
    //     // }
    //     // unordered_map<int, vector<int>> mp;
    //     // for(int i = 0; i < Omega.size(); ++i) {
    //     //     mp[i].push_back(map_n_to_m[i]); 
    //     // }



    //     //another implement ---
    //     int opt_k = -1; 
    //     double opt_val = DBL_MAX; 
    //     vector<int> T; 
    //     for(int k = 1; k <= k_max; ++k) { 
    //         unordered_set<int> st;
    //         for(int un = 0; un < Omega.size(); ++un) st.insert(un); 
    //         while(st.size() > T_size) {
    //             int min_idx = -1; 
    //             double min_val = DBL_MAX;
    //             for(const int& item : st) {
    //                 // 找到距离item最近的k个 
    //                 priority_queue<pair<double, int>, vector<pair<double, int>>, MyComparer> pq; 
    //                 for(const int& other : st) {
    //                     if(other != item) {
    //                         pq.push({abs(Q_giveny[item]-Q_giveny[other]), other});
    //                     }
    //                 }
    //                 int diff = 0; 
    //                 for(int i = 0; i < k; ++i) {
    //                     pair<double, int> tmp = pq.top(); pq.pop(); 
    //                     diff += Q_giveny[tmp.second];
    //                 } 
    //                 diff /= k; 
    //                 if(abs(Q_giveny[item]-diff) < min_val) {
    //                     min_val = abs(Q_giveny[item]-diff);
    //                     min_idx = item; 
    //                 }
    //             }  
    //             st.erase(min_idx); 
    //         } 
    //         // T 选好了，计算 obj
    //         double obj = 0.0; 
    //         for(int item = 0; item < Omega.size(); ++item) {
    //             if(st.count(item) == 0) {
    //                 priority_queue<pair<double, int>, vector<pair<double, int>>, MyComparer> pq; 
    //                 for(const int& other : st) { 
    //                     pq.push({abs(Q_giveny[item]-Q_giveny[other]), other}); 
    //                 }
    //                 int diff = 0; 
    //                 for(int i = 0; i < k; ++i) {
    //                     pair<double, int> tmp = pq.top(); pq.pop(); 
    //                     diff += Q_giveny[tmp.second];
    //                 } 
    //                 diff /= k; 
    //                 if(abs(Q_giveny[item]-diff) > obj) obj = abs(Q_giveny[item]-diff); 
    //             }
    //         }
    //         for(const int& item : st) cout << item << " "; 
    //         cout << endl; 
    //         cout << obj << endl; 
    //         if(obj < opt_val) {
    //             opt_val = obj;
    //             opt_k = k; 
    //             T.assign(st.begin(), st.end()); 
    //         }
    //     }
    //     cout << "T: " << endl; 
    //     for(const int& item : T) {
    //         cout << item << " ";
    //     }cout << endl;
    //     cout << opt_k << ": " << opt_val << endl;

    //     unordered_map<int, vector<int>> mp;
    //     unordered_set<int> Tset; 
    //     for(const int& item : T) Tset.insert(item);
    //     for(int item = 0; item < Omega.size(); ++item) {
    //         if(Tset.count(item) == 0) {
    //             priority_queue<pair<double, int>, vector<pair<double, int>>, MyComparer> pq; 
    //             for(const int& other : Tset) { 
    //                 pq.push({abs(Q_giveny[item]-Q_giveny[other]), other}); 
    //             } 
    //             for(int i = 0; i < opt_k; ++i) {
    //                 pair<double, int> tmp = pq.top(); pq.pop(); 
    //                 mp[item].push_back(tmp.second); 
    //             }  
    //         }
    //     }
    //     for(const pair<int, vector<int>>& item : mp) {
    //         cout << item.first << ": ";
    //         for(const int& nr : item.second) {
    //             cout << nr << " "; 
    //         }
    //         cout << endl; 
    //     } 


        
    //     // vector<int> next_ystar = solve_MLAug_KNN_withtarget(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, string(argv[1]), T, map_n_to_m, 0);
    //     vector<int> next_ystar = solve_MLInte_KNN(pri_graph, Omega, v_j_star, Dpq, n_dmd, fc_vc, hop_parameter, epsilong, string(argv[1]), opt_k, T, mp);
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
    // cout << "Integrate KNN 耗时: " << (inte_knn_end - inte_knn_start).count() / 1e9 << " 秒" << endl;
     














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