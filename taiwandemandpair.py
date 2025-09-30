import pandas as pd 


result = {} 
for i in range(8):
    for j in range(8):
        if i != j:
            result[(i,j)] = 0 

print(result)

names = ["臺北市", "新竹市", "彰化縣", "臺南市", "高雄市", "花蓮縣", "嘉義縣", "臺東縣"]

with open("./taiwanorides.csv") as fp:
    lines = fp.readlines()
    for line in lines:
        tmp = line.split(",")
        str1 = tmp[0]
        str2 = tmp[-1]
        idx1, idx2 = -1, -1 
        for i in range(8):
            if names[i] in str1:
                idx1 = i 
                break 
        for i in range(8):
            if names[i] in str2:
                idx2 = i
                break 
        if idx1 != -1 and idx2 != -1 and idx1 != idx2:
            result[(idx1, idx2)] += 1 

for k, v in result.items():
    if v != 0:
        print(k, v)