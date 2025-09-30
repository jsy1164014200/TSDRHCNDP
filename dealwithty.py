from ctypes import pointer
import matplotlib.pyplot as plt
from mpl_toolkits.basemap import Basemap
import numpy as np

import os

failed_connections = {}

def orientation(p, q, r): 
    val = (q[1] - p[1]) * (r[0] - q[0]) - (q[0] - p[0]) * (r[1] - q[1])
    if val == 0:
        return 0  # 共线
    return 1 if val > 0 else 2  # 顺时针或逆时针

def on_segment(p, q, r):
    """检查点q是否在由p和r组成的线段上"""
    if min(p[0], r[0]) <= q[0] <= max(p[0], r[0]) and min(p[1], r[1]) <= q[1] <= max(p[1], r[1]):
        return True
    return False

def segments_intersect(a1, a2, b1, b2): 
    # 计算四个方向值
    o1 = orientation(a1, a2, b1)
    o2 = orientation(a1, a2, b2)
    o3 = orientation(b1, b2, a1)
    o4 = orientation(b1, b2, a2)
    
    # 一般情况：线段相交且不共线
    if o1 != o2 and o3 != o4:
        return True
    
    # 特殊情况：共线且有端点在另一条线段上
    # 检查b1是否在a1a2上
    if o1 == 0 and on_segment(a1, b1, a2):
        return True
    # 检查b2是否在a1a2上
    if o2 == 0 and on_segment(a1, b2, a2):
        return True
    # 检查a1是否在b1b2上
    if o3 == 0 and on_segment(b1, a1, b2):
        return True
    # 检查a2是否在b1b2上
    if o4 == 0 and on_segment(b1, a2, b2):
        return True
    
    # 不相交
    return False



def list_all_files(folder_path):
    file_list = []
    for root, dirs, files in os.walk(folder_path):
        for file in files:
            file_list.append(os.path.join(root, file))
    return file_list

def plot_world_map_with_lines(locations, connections, title="Map"): 
    # 创建图形和轴
    plt.figure(figsize=(16, 10))
    
    # 创建Basemap实例，使用米勒圆柱投影 26.652779, 116.881250       33.368539, 123.604404
    m = Basemap(projection='mill', llcrnrlat=26.652779, urcrnrlat=33.368539,
                llcrnrlon=116.881250, urcrnrlon=123.604404, resolution='l')
    
    # 绘制海岸线、国家边界和填充 continents
    m.drawcoastlines(linewidth=0.5)
    m.drawcountries(linewidth=0.5)
    m.fillcontinents(color='#f0e68c', lake_color='#87ceeb')
    
    # 绘制经纬线
    m.drawmeridians(np.arange(-180, 180, 30), labels=[False, False, False, True])
    m.drawparallels(np.arange(-90, 90, 30), labels=[True, False, False, False])
    

    # 绘制地点
    # citylatitudes = [32.04, 31.39, 31.23, 30.24, 28.01, 29.30]
    # citylongitudes = [118.78, 120.95, 121.47, 120.15, 120.65, 120.06]
    # citynames = ["Nanjing", "Kunshan", "Shanghai", "Hangzhou", "Wenzhou", "Yiwu"]
    # for i in range(6):
    #     x, y = m(citylatitudes[i], citylongitudes[i])
    #     m.plot(x, y, 'ro', markersize=8)  # 绘制红色圆点
    #     plt.text(x, y, citynames[i], fontsize=10)  # 添加地点名称

    # 定义一些地点及其经纬度 (纬度, 经度)
    cities = {
        'Nanjing': (32.065755, 118.807201),
        'Kunshan': (31.174643, 121.000890),
        'Shanghai': (31.230666, 121.470565),
        'Hangzhou': (30.279078, 120.168445),
        'Wenzhou': (27.995566, 120.695425),
        'Yiwu': (29.307762, 120.081285) 
    }
    
    # 定义要连接的城市对
    # city_connections = [
    #     ('Nanjing', 'Kunshan'),
    #     ('Nanjing', 'Shanghai'),
    #     ('Nanjing', 'Hangzhou'), 
    #     ('Nanjing', 'Yiwu'),
    #     ('Kunshan', 'Shanghai'),
    #     ('Kunshan', 'Hangzhou'),
    #     ('Kunshan', 'Wenzhou'),
    #     ('Kunshan', 'Yiwu'),
    #     ('Shanghai', 'Hangzhou'),
    #     ('Shanghai', 'Wenzhou'),
    #     ('Shanghai', 'Yiwu'),
    #     ('Hangzhou', 'Wenzhou'),
    #     ('Hangzhou', 'Yiwu'),
    #     ('Wenzhou', 'Yiwu'),
    # ]

    ########### solution #################
    # Objective: 3853.92 first stage cost: 2083 0.05 
    # city_connections = [
    #     ('Nanjing', 'Kunshan'),
    #     ('Nanjing', 'Shanghai'),
    #     ('Nanjing', 'Hangzhou'), 
    #     ('Nanjing', 'Yiwu'), 
    #     ('Kunshan', 'Yiwu'),
    #     ('Shanghai', 'Hangzhou'), 
    #     ('Hangzhou', 'Wenzhou'), 
    #     ('Wenzhou', 'Yiwu'),
    # ] 

    # Objective: 3969.89 first stage cost: 2530 0.1 --  0.5 0.6 0.7 
    # city_connections = [
    #     ('Nanjing', 'Kunshan'),
    #     ('Nanjing', 'Shanghai'),
    #     ('Nanjing', 'Hangzhou'), 
    #     ('Nanjing', 'Yiwu'),
    #     ('Kunshan', 'Shanghai'),  
    #     ('Kunshan', 'Yiwu'),
    #     ('Shanghai', 'Hangzhou'),
    #     ('Shanghai', 'Wenzhou'), 
    #     ('Hangzhou', 'Wenzhou'), 
    #     ('Wenzhou', 'Yiwu'),
    # ]
  

    # Objective: 4134.86  first stage cost: 2662 0.8- inf
    # city_connections = [
    #     ('Nanjing', 'Kunshan'),
    #     ('Nanjing', 'Shanghai'),
    #     ('Nanjing', 'Hangzhou'), 
    #     ('Nanjing', 'Yiwu'),
    #     ('Kunshan', 'Shanghai'), 
    #     ('Kunshan', 'Yiwu'),
    #     ('Shanghai', 'Hangzhou'),
    #     ('Shanghai', 'Wenzhou'), 
    #     ('Hangzhou', 'Wenzhou'),
    #     ('Hangzhou', 'Yiwu'),
    #     ('Wenzhou', 'Yiwu'),
    # ]  

    # Objective: 3893.73 first stage cost: 2378 hop 3  4
    city_connections = [
        ('Nanjing', 'Kunshan'),
        ('Nanjing', 'Shanghai'),
        ('Nanjing', 'Hangzhou'),  
        ('Kunshan', 'Shanghai'), 
        ('Kunshan', 'Yiwu'),
        ('Shanghai', 'Hangzhou'),
        ('Shanghai', 'Wenzhou'), 
        ('Hangzhou', 'Wenzhou'),
        ('Hangzhou', 'Yiwu'),
        ('Wenzhou', 'Yiwu'),
    ] 

    ########### solution #################


    # 绘制地点 
    for name, (lat, lon) in cities.items():
        x, y = m(lon, lat)  # 转换为地图坐标
        m.plot(x, y, 'ro', markersize=8)  # 绘制红色圆点
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
            plt.plot([x1, x2], [y1, y2], 'b-', linewidth=2, alpha=0.7)

    

    # 绘制地点 
    for name, (lat, lon) in locations.items():
        x, y = m(lon, lat)  # 转换为地图坐标
        # m.plot(x, y, 'ro', markersize=8)  # 绘制红色圆点
        # plt.text(x, y, name, fontsize=10)  # 添加地点名称 


    # 绘制连接线
    through_line = set() 
    for (place1, place2) in connections:
        if place1 in locations and place2 in locations:
            lat1, lon1 = locations[place1]
            lat2, lon2 = locations[place2]
            
            # 将经纬度转换为地图坐标
            x1, y1 = m(lon1, lat1)
            x2, y2 = m(lon2, lat2)
            
            # 绘制连线
            # m.drawgreatcircle(lon1, lat1, lon2, lat2, linewidth=2, color='b', alpha=0.7)
            # 改用直接绘制直线的方式，避免greatcircle的参数冲突
            # plt.plot([x1, x2], [y1, y2], 'b-', linewidth=2, alpha=0.7)

            for (city1, city2) in city_connections:
                if city1 in cities and city2 in cities:
                    city1lat, city1lon = cities[city1]
                    city2lat, city2lon = cities[city2]
                    # 将经纬度转换为地图坐标
                    city1x, city1y = m(city1lon, city1lat)
                    city2x, city2y = m(city2lon, city2lat)
                    if segments_intersect((city1x, city1y), (city2x, city2y), (x1,y1), (x2,y2)):
                        if city1 <= city2:
                            through_line.add((city1, city2))
                        else:
                            through_line.add((city2, city1))
                        # global failed_connections
                        # if (city1, city2) in failed_connections:
                        #     failed_connections[(city1, city2)] += 1
                        # else:
                        #     failed_connections[(city1, city2)] = 1
                        # print(f"from {city1} to {city2}") 
    print(through_line)

    

    # 添加标题
    plt.title(title, fontsize=16)
    
    # 显示图形
    plt.show()
    global failed_connections
    for item in through_line:
        if item in failed_connections:
            failed_connections[item] += 1
        else:
            failed_connections[item] = 1
    through_line = list(through_line)
    for i in range(len(through_line)):
        for j in range(i+1, len(through_line)): 
            item = (through_line[i][0], through_line[i][1], through_line[j][0], through_line[j][1])
            if item in failed_connections:
                failed_connections[item] += 1
            else:
                failed_connections[item] = 1










# 示例使用
if __name__ == "__main__":
    
    all_files = list_all_files("./jddata/tydata")
    for file in all_files:
        print(f"current deal with file {file}")
        with open(file) as fp:
            while True:
                line = fp.readline()   
                if not line:
                    break   
                fields = line.split()
                if len(fields[0]) == 5:
                    print(f"Find a new typhoon: {fields[7]}")
                    typhoon_name = fields[7] 
                    n_path = int(fields[2])
                    points = {} 
                    connections = [] 
                    for i in range(n_path):
                        line = fp.readline() 
                        path_info = line.split()
                        # print(f"{i} path: LAT {path_info[2]}, LONG {path_info[3]}")
                        lat, long = int(path_info[2]) / 10, int(path_info[3]) / 10
                        points[str(i)] = (lat, long) 
                        if i != 0:
                            connections.append((str(i-1), str(i)))
                    # print(points)
                    # print(connections)
                    # 绘制地图和连线
                    isplot = False 
                    for name, (x, y) in points.items():
                        if x >= 26.652779 and x <= 33.368539 and y >= 116.881250 and y <= 123.604404:
                            isplot = True 
                            # print(x,y)
                    if isplot:
                        plot_world_map_with_lines(points, connections, typhoon_name)  
       

    for k,v in failed_connections.items():
        print(k, v)

    # pri_graph.cost_map[{2, 0}] = 305.0; // Nanjing to Shanghai 40
    # pri_graph.cost_map[{2, 1}] = 266.0; // Nanjing to Kunshan 32
    # pri_graph.cost_map[{2, 3}] = 274.0; // Nanjing to Hangzhou 27 x
    # pri_graph.cost_map[{2, 5}] = 410.0; // Nanjing to Yiwu 39
    # pri_graph.cost_map[{0, 1}] = 63.0;  // Shanghai to Kunshan 9  x
    # pri_graph.cost_map[{0, 3}] = 176.0; // Shanghai to Hangzhou 20x
    # pri_graph.cost_map[{0, 5}] = 284.0; // Shanghai to Yiwu 32    x
    # pri_graph.cost_map[{0, 4}] = 460.0; // Shanghai to Wenzhou 46
    # pri_graph.cost_map[{1, 3}] = 173.0; // Kunshan to Hangzhou 15
    # pri_graph.cost_map[{1, 5}] = 281.0; // Kunshan to Yiwu 27     x
    # pri_graph.cost_map[{1, 4}] = 457.0; // Kunshan to Wenzhou 44  x
    # pri_graph.cost_map[{3, 5}] = 132.0; // Hangzhou to Yiwu 12
    # pri_graph.cost_map[{3, 4}] = 304.0; // Hangzhou to Wenzhou 32
    # pri_graph.cost_map[{5, 4}] = 225.0; // Yiwu to Wenzhou 20
    k2idx = { 
        0: ('Nanjing', 'Shanghai'),
        1: ('Kunshan', 'Nanjing'),
        2: ('Hangzhou', 'Nanjing'), 
        3: ('Nanjing', 'Yiwu'),
        4: ('Kunshan', 'Shanghai'),
        5: ('Hangzhou', 'Shanghai'),
        6: ('Shanghai', 'Yiwu'),
        7: ('Shanghai', 'Wenzhou'),
        8: ('Hangzhou', 'Kunshan'),
        9: ('Kunshan', 'Yiwu'),
        10: ('Kunshan', 'Wenzhou'),
        11: ('Hangzhou', 'Yiwu'),
        12: ('Hangzhou', 'Wenzhou'),
        13: ('Wenzhou', 'Yiwu'),
    }

    failtimes = [] 
    for i in range(14):
        if k2idx[i] in failed_connections:
            failtimes.append(failed_connections[k2idx[i]])
        else:
            failtimes.append(0)
    
    for i in range(14):
        for j in range(i+1, 14):
            edge1 = k2idx[i] 
            edge2 = k2idx[j] 
            k = (edge1[0], edge1[1], edge2[0], edge2[1])
            if k in failed_connections:
                failtimes.append(failed_connections[k])
            else:
                failtimes.append(0)
    
    print(failtimes)
    
                



        
