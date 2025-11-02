import pandas as pd 
import matplotlib.pyplot as plt
from mpl_toolkits.basemap import Basemap
import numpy as np

import math
 

def is_segment_circle_intersect(ox, oy, r, x1, y1, x2, y2):
    """
    判断线段与圆是否相交
    
    参数:
        ox, oy: 圆心坐标
        r: 圆的半径
        x1, y1: 线段端点A的坐标
        x2, y2: 线段端点B的坐标
    返回:
        bool: 若相交则返回True，否则返回False
    """
    # 计算端点到圆心的距离平方（避免开方，提高效率）
    d1_sq = (x1 - ox)**2 + (y1 - oy)** 2
    d2_sq = (x2 - ox)**2 + (y2 - oy)** 2
    
    # 若有端点在圆内或圆上，直接相交
    if d1_sq <= r**2 or d2_sq <= r**2:
        return True
    
    # 向量AB和AO
    abx = x2 - x1
    aby = y2 - y1
    aox = ox - x1
    aoy = oy - y1
    
    # 计算t的分子（点积）和分母（AB长度平方）
    dot = aox * abx + aoy * aby
    len_ab_sq = abx**2 + aby**2
    
    # 避免除以0（线段长度为0的特殊情况）
    if len_ab_sq == 0:
        return False  # 线段是点，且已判断不在圆内
    
    # 计算t
    t = dot / len_ab_sq
    
    # 投影点在A外侧（t < 0），最短距离是d1
    if t < 0:
        min_dist_sq = d1_sq
    # 投影点在B外侧（t > 1），最短距离是d2
    elif t > 1:
        min_dist_sq = d2_sq
    # 投影点在线段上，计算垂线距离平方
    else:
        # 投影点坐标
        proj_x = x1 + t * abx
        proj_y = y1 + t * aby
        # 垂线距离平方
        min_dist_sq = (proj_x - ox)**2 + (proj_y - oy)** 2
    
    # 若最短距离 <= 半径，则相交
    return min_dist_sq <= r**2

# 示例使用
if __name__ == "__main__": 
    data = pd.read_csv("./taiwandata/taiwan.csv")
    failed_connections = {}
    for dataidx in range(len(data)):
        # 创建图形和轴
        plt.figure(figsize=(16, 10))
        
        # 创建Basemap实例，使用米勒圆柱投影 25.385949, 119.801318       21.804996, 122.247667
        m = Basemap(projection='mill', llcrnrlat=21.804996, urcrnrlat=25.385949,
                    llcrnrlon=119.801318, urcrnrlon=122.247667, resolution='i')
        
        # 绘制海岸线、国家边界和填充 continents
        m.drawcoastlines(linewidth=0.5)
        m.drawcountries(linewidth=0.5)
        m.fillcontinents(color='#f0e68c', lake_color='#87ceeb')
        
        # 绘制经纬线
        m.drawmeridians(np.arange(-180, 180, 30), labels=[False, False, False, True])
        m.drawparallels(np.arange(-90, 90, 30), labels=[True, False, False, False])
        

         

        # # 定义一些地点及其经纬度 (纬度, 经度)
        # cities = {
        #     '0': (25.040856, 121.518927),
        #     '1': (25.071544, 121.591614),
        #     '2': (25.119395, 121.520629),
        #     '3': (24.999731, 121.558606),
        #     '4': (25.062664, 121.365983),
        #     '5': (25.057574, 121.524567),
        #     '6': (25.036644, 121.553851),
        #     '7': (25.096122, 121.520198),
        #     '8': (24.997014, 121.454258),
        #     '9': (24.992613, 121.493158), 
        #     '10': (24.985695, 121.534998), 
        #     '11': (24.812725, 120.982867), 
        #     '12': (24.184906, 120.603173), 
        #     '13': (24.156318, 120.679185),
        #     '14': (24.120679, 120.650072),
        #     '15': (24.074937, 120.541800),
        #     '16': (23.003235, 120.218567),
        #     '17': (23.021108, 120.222027),
        #     '18': (22.677447, 120.321933),
        #     '19': (22.649945, 120.356001),
        #     '20': (22.646023, 120.309368),
        #     '21': (24.005497, 121.625107),  
        #     '22': (25.028339, 121.548634),
        #     '23': (24.989678, 121.510728),
        #     '24': (25.016439, 121.305653),
        #     '25': (24.799870, 120.990679),
        #     '26': (24.798147, 120.965385),
        #     '27': (24.687369, 120.907286),
        #     '28': (24.242171, 120.726154),
        #     '29': (24.244374, 120.533685),
        #     '30': (24.195175, 120.721074),
        #     '31': (23.477146, 120.466590),
        #     '32': (23.464461, 120.283168),
        #     '33': (22.968597, 120.225676),
        #     '34': (22.626965, 120.296822), 
        #     '35': (22.615300, 120.297232),
        #     '36': (22.750807, 121.140061),
        # } 

        # # 绘制地点 
        # for name, (lat, lon) in cities.items():
        #     x, y = m(lon, lat)  # 转换为地图坐标
        #     m.plot(x, y, 'ro', markersize=2)  # 绘制红色圆点
        #     plt.text(x, y, name, fontsize=4)  # 添加地点名称 




        cities = {
            '0': (25.040856, 121.518927),
            # '1': (25.071544, 121.591614),
            # '2': (25.119395, 121.520629),
            # '3': (24.999731, 121.558606),
            # '4': (25.062664, 121.365983),
            # '5': (25.057574, 121.524567),
            # '6': (25.036644, 121.553851),
            # '7': (25.096122, 121.520198),
            # '8': (24.997014, 121.454258),
            # '9': (24.992613, 121.493158), 
            # '10': (24.985695, 121.534998), 
            '1': (24.812725, 120.982867), 
            # '12': (24.184906, 120.603173), 
            # '13': (24.156318, 120.679185),
            # '14': (24.120679, 120.650072),
            '2': (24.074937, 120.541800),
            # '16': (23.003235, 120.218567),
            '3': (23.021108, 120.222027),
            # '18': (22.677447, 120.321933),
            # '19': (22.649945, 120.356001),
            '4': (22.646023, 120.309368),
            '5': (24.005497, 121.625107),  
            # '22': (25.028339, 121.548634),
            # '23': (24.989678, 121.510728),
            # '24': (25.016439, 121.305653),
            # '25': (24.799870, 120.990679),
            # '26': (24.798147, 120.965385),
            # '27': (24.687369, 120.907286),
            # '28': (24.242171, 120.726154),
            # '29': (24.244374, 120.533685),
            # '30': (24.195175, 120.721074),
            # '31': (23.477146, 120.466590),
            '6': (23.464461, 120.283168),
            # '33': (22.968597, 120.225676),
            # '34': (22.626965, 120.296822), 
            # '35': (22.615300, 120.297232),
            '7': (22.750807, 121.140061),
        }
        city_connections = [] 
        for i in range(len(cities)):
            for j in range(i+1, len(cities)):
                city_connections.append((str(i), str(j)))
        # print(city_connections)
         

        ########## plot solution ############ 

        # Objective: 50938.7 first stage cost: 25770 hop2
        # city_connections = [('0', '1'), ('0', '2'), ('0', '5'), ('0', '6'), ('1', '2'), ('1', '5'), ('2', '3'), ('2', '4'), ('2', '5'),
        #                     ('2', '7'), ('3', '4'), ('3', '5'), ('3', '6'), ('4', '5'), ('4', '6') ] 
 
        # city_connections = [('0', '1'), ('0', '2'), ('0', '5'), ('0', '6'), ('1', '2'), ('1', '5'), ('2', '3'), ('2', '4'), ('2', '5'),
        #                     ('3', '4'), ('3', '5'), ('3', '6'), ('4', '5'), ('4', '6'), ('5', '7') ] 
 

        # city_connections = [('0', '1'), ('0', '2'), ('0', '3'), ('0', '5'), ('0', '6'), ('1', '2'), ('1', '3'), ('1', '5'), ('2', '3'), ('2', '4'),
        #                     ('3', '4'), ('3', '6'), ('3', '7'), ('4', '6') ] 

        # Objective: 40736.4
        # first stage cost: 16167.5
        # hop 3
        # city_connections = [('0', '1'), ('0', '2'), ('0', '5'), ('1', '2'), ('1', '5'), ('2', '3'), ('2', '4'),
        #                     ('2', '6'), ('3', '4'), ('3', '6'), ('4', '6'), ('5', '6'), ('6', '7') ] 

        # Objective: 38370.1
        # first stage cost: 15060.6
        # hop 4
        # city_connections = [('0', '1'), ('0', '2'), ('0', '5'), ('1', '5'), ('1', '6'), ('2', '3'), ('2', '5'),
        #                     ('2', '6'), ('3', '4'), ('3', '6'), ('3', '7'), ('4', '6'), ('4', '7') ] 
 

        # mom 2

        # mom 5 Objective: 60999.7 first stage cost: 29999.8
        # city_connections = [('0', '1'), ('0', '2'), ('0', '3'), ('0', '6'), ('1', '2'), ('1', '5'), ('1', '6'), ('2', '3'), ('2', '4'),
        #                     ('3', '4'), ('3', '5'), ('3', '6'), ('3', '7'), ('4', '6') ] 

        # moo 8 Objective: 67310.3 first stage cost: 34632.5
        # city_connections = [('0', '1'), ('0', '2'), ('0', '3'), ('0', '6'), ('1', '2'), ('1', '5'), ('1', '6'), ('2', '3'), ('2', '4'),
        #                     ('2', '5'), ('2', '7'), ('3', '4'), ('3', '6'), ('3', '7'), ('4', '6') ] 






        # hop 1 Objective: 35848.1
        # first stage cost: 2161.3
        # Direct Solve 算法结束运行
        # Direct Solve 算法耗时: 7.9162 秒
        city_connections = [('0', '1'), ('0', '2'), ('0', '3'), ('0', '4'), ('0', '5'), ('0', '7'), ('1', '2'), ('1', '4'), 
                            ('2', '3'), ('3', '4'), ('5', '7')]


        # hop 2 Objective: 5148.29
        # first stage cost: 2725.6
        # Direct Solve 算法结束运行
        # Direct Solve 算法耗时: 1545.88 秒
        # city_connections = [('0', '1'), ('0', '2'), ('0', '3'), ('0', '5'), ('0', '7'), ('1', '2'), ('1', '3'), ('1', '4'), ('1', '5'),
        #                     ('2', '3'), ('2', '4'), ('3', '4'), ('3', '6'), ('3', '7'), ('5', '7')]


        # hop 3 Objective: 4614.3
        # first stage cost: 2188
        # Direct Solve 算法结束运行
        # Direct Solve 算法耗时: 11249.4 秒
        # city_connections = [('0', '1'), ('0', '2'), ('0', '3'), ('0', '5'), ('1', '2'), ('1', '5'),
        #                     ('2', '3'), ('2', '4'), ('2', '6'), ('3', '4'), ('3', '7'), ('4', '6'), ('4', '7'), ('5', '7')]
          
 
        ########## plot solution ############






        # 绘制地点 
        for name, (lat, lon) in cities.items():
            x, y = m(lon, lat)  # 转换为地图坐标
            m.plot(x, y, 'ro', markersize=6)  # 绘制红色圆点
            plt.text(x, y, name, fontsize=10)  # 添加地点名称 

         
        # 绘制连接线
        for (place1, place2) in city_connections:
            if place1 in cities and place2 in cities:
                lat1, lon1 = cities[place1]
                lat2, lon2 = cities[place2]
                
                # 将经纬度转换为地图坐标
                x1, y1 = m(lon1, lat1)
                x2, y2 = m(lon2, lat2)
                
                # 绘制连线
                # m.drawgreatcircle(lon1, lat1, lon2, lat2, linewidth=2, color='b', alpha=0.7)
                # 改用直接绘制直线的方式，避免greatcircle的参数冲突
                plt.plot([x1, x2], [y1, y2], 'b-', linewidth=1, alpha=0.7)
                 



        eq_lat, eq_log, eq_rd = data.iloc[dataidx, 1], data.iloc[dataidx, 2], data.iloc[dataidx, 3] 
        x, y = m(eq_log, eq_lat)  # 转换为地图坐标
        # m.plot(x, y, 'ro', markersize=2)
        # 计算圆的坐标点
        # 地球半径(公里)
        earth_radius = 6371.0
        # 将半径转换为弧度
        radius_rad = eq_rd / earth_radius
        
        # 生成角度
        angles = np.linspace(0, 2*np.pi, 100)
        
        # 计算圆上各点的经纬度
        circle_lats = []
        circle_lons = []
        for angle in angles:
            # 计算公式基于球面几何
            lat_rad = np.radians(eq_lat)
            lon_rad = np.radians(eq_log)
            
            new_lat_rad = np.arcsin(
                np.sin(lat_rad) * np.cos(radius_rad) +
                np.cos(lat_rad) * np.sin(radius_rad) * np.cos(angle)
            )
            
            new_lon_rad = lon_rad + np.arctan2(
                np.sin(angle) * np.sin(radius_rad) * np.cos(lat_rad),
                np.cos(radius_rad) - np.sin(lat_rad) * np.sin(new_lat_rad)
            )
            
            # 将弧度转换为度
            circle_lats.append(np.degrees(new_lat_rad))
            circle_lons.append(np.degrees(new_lon_rad))
        
        # 将圆的经纬度转换为图上坐标并绘制
        x_circle, y_circle = m(circle_lons, circle_lats)
        # m.plot(x_circle, y_circle, 'b-', linewidth=2)


        break_lines = []
        for pot1, pot2 in city_connections: 
            lat1, lon1 = cities[pot1]
            lat2, lon2 = cities[pot2] 
            # 将经纬度转换为地图坐标
            x1, y1 = m(lon1, lat1)
            x2, y2 = m(lon2, lat2)
            rds = math.sqrt( (x-x_circle[0])*(x-x_circle[0]) + (y-y_circle[0])*(y-y_circle[0]) )
            if is_segment_circle_intersect(x, y, rds, x1, y1, x2, y2):
                break_lines.append((pot1, pot2))
        print(break_lines)

        for item in break_lines:
            if item in failed_connections:
                failed_connections[item] += 1
            else:
                failed_connections[item] = 1
        break_lines = list(break_lines)
        for i in range(len(break_lines)):
            for j in range(i+1, len(break_lines)): 
                item = (break_lines[i][0], break_lines[i][1], break_lines[j][0], break_lines[j][1])
                if item in failed_connections:
                    failed_connections[item] += 1
                else:
                    failed_connections[item] = 1


        # 添加标题
        plt.title("Earthquakes of Taiwan", fontsize=16)
        
        # 显示图形
        plt.show()  
 
        
        
    
    
    for k,v in failed_connections.items():
        print(k, v) 

    k2idx = {}
    num_cities = 8 
    kidx = 0 
    for i in range(num_cities):
        for j in range(i+1, num_cities):
            k2idx[kidx] = (str(i), str(j))
            kidx += 1 

    print(k2idx)
        
    failtimes = [] 
    for i in range(len(k2idx)):
        if k2idx[i] in failed_connections:
            failtimes.append(failed_connections[k2idx[i]])
        else:
            failtimes.append(0)
    
    for i in range(len(k2idx)):
        for j in range(i+1, len(k2idx)):
            edge1 = k2idx[i] 
            edge2 = k2idx[j] 
            k = (edge1[0], edge1[1], edge2[0], edge2[1])
            if k in failed_connections:
                failtimes.append(failed_connections[k])
            else:
                failtimes.append(0)
    
    print(failtimes)
    

    