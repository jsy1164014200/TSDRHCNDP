import random 
import math 
import sys

import numpy as np 
import pandas as pd 
import networkx as nx 
from gensim.models import Word2Vec

instance_name = sys.argv[1]

with open("./output-learning/" + instance_name + "-outputsamples.txt", "r") as f:
    T = [int(x) for x in f.readline().split()]


# nwalk =50, nlength =20, ω =5, and ξ =16.
n_walk = 50 # 每个node随机游走次数
n_length = 20 # 随机游走长度
omega = 5 # 窗口大小
xi = 8
seed = 5200 # 随机种子





# Define the relationship graph
Q_y = pd.read_csv("./results-learning/" + instance_name + "-sampled.csv", header=None, sep=" ").iloc[:100, :-1] 
Q_y = 5*Q_y / Q_y.mean().mean() # avoid numercial issue 
# Q_y = (Q_y.max() - Q_y) / (Q_y.max() - Q_y.min()) 
avg_weight = 0
n_scenario = Q_y.shape[1]
edges = []
for i in range(n_scenario):
    for j in range(i+1, n_scenario):  
        weight = math.exp( (-(Q_y.iloc[:, i] - Q_y.iloc[:, j]).abs()).mean() )   
        # weight = 1 / ((Q_y.iloc[:, i] - Q_y.iloc[:, j]).abs()).mean() 
        edges.append((i, j, {"weight": weight}))
        avg_weight += weight
        # print(weight)
G = nx.Graph() 
G.add_edges_from(edges)
# print("avg_weight: ", avg_weight / (n_scenario*(n_scenario-1)/2))

# Do random walk on the relationship graph

def get_random_walk(graph, node, n_steps):
    """ Given a graph and a node, 
        return a random walk starting from the node 
    """
    local_path = [str(node)]
    current_node = node
    for _ in range(n_steps):
        neighbors = list(nx.neighbors(graph, current_node))
        if not neighbors:
            break
        weights = [graph[current_node][neighbor]['weight'] for neighbor in neighbors]
        current_node = random.choices(neighbors, weights=weights, k=1)[0]
        # current_node = random.choice(neighbors)
        local_path.append(str(current_node)) 
    return local_path

walks = []
for node in G.nodes():
    for _ in range(n_walk):
        walks.append(get_random_walk(G, node, n_length))


# Do word embedding on the random walks 

embedder = Word2Vec(
    sentences=walks,
    vector_size=xi,
    window=omega, 
    sg=1, 
    hs=1,  
    alpha=0.03, 
    min_alpha=0.0001, 
    seed=seed,
    )

features = []
with open("./output-learning/" + instance_name + "-feature.txt", "w") as f:
    for node in G.nodes(): 
        # f.write(f"{value.join(" ")}\n") 
        features.append(embedder.wv[str(node)])
        f.write(" ".join(str(x) for x in embedder.wv[str(node)]))
        f.write("\n")











    