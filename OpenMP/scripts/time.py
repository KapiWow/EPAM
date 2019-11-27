#!/usr/bin/python3.6

import os
import sys

n_exec = int(sys.argv[2])
exec_name = sys.argv[1]
count_exec = 10

n_list = [n_exec]
#thread_count = [1 2 3 4 5 6 7 8]
thread_count = 8

n=n_exec
for i in range(8):
    n_exec = n_exec+n
    n_list.append(n_exec)

res = []

for i in n_list:
    #avg_parallel = 0
    #avg_seq = 0
    avg = 0
    time_list = [0] * thread_count
    print(time_list)
    for j in range(count_exec): 
        os.system('./' + exec_name + ' ' + str(i))
        file = open("test", "r")
        for k in range(len(time_list)):
            time_list[k] += int(file.readline())
        #res_parallel = int(file.readline())
        #res_seq = int(file.readline())
        #avg_parallel = avg_parallel + res_parallel
        #avg_seq = avg_seq + res_seq
    for j in range(len(time_list)):
        time_list[j] = time_list[j]/count_exec
    res.append(time_list)

print(res)
    #avg_parallel = avg/count_exec

lines = []
header = "n"
for i in range(len(n_list)):
    header = header + str(i) + " "
lines.append(header)

for i in range(len(n_list)):
    line = str(n_list[i])
    #print(str(n_list[i]), ' '.join(res[i]))
    for j in res[i]:
        line += " "
        line += str(j)
    print(line)
    line += '\n'
    lines.append(line)

f = open("data","w+")
f.writelines(lines)
