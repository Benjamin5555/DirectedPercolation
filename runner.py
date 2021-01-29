import subprocess
#subprocess.run(["ls", "-l","-a"])
import math 
import numpy as np
import Plotter.plotter as bplt
import csv
N=100000
TIME_STEPS=200000
p= 0.75
r= 0.2
MAX_RUNS = 25 
SAMPLES_PER_r =5
out_data_path = "runs_results.csv"

rs = []
final_active_states_counts  = []
r_upper = 1 
r_lower =0
dr =  0.1

def truncate(f, n):
    '''Truncates/pads a float f to n decimal places without rounding'''
    s = '{}'.format(f)
    if 'e' in s or 'E' in s:
        return '{0:.{1}f}'.format(f, n)
    i, p, d = s.partition('.')
    return '.'.join([i, (d+'0'*n)[:n]])


def run_experiment(N,TIME_STEPS,p,r):

    exp = subprocess.run(["./DirectedPercolation",str(N),str(TIME_STEPS),str(p),str(r)],stdout=subprocess.PIPE)



    ExperimentName = exp.stdout.decode('utf-8')[:-1]
    print("PYName:")
    print(ExperimentName)
    print(truncate(r,5))
    
    f =open(ExperimentName,"r")#Open produced experiment
    f.seek(N*(TIME_STEPS-1))#Seek to last line
    
    count = (f.readline().count("1"))#Count number of active states remaining
    f.close()

    return count

r=0
for s in range(0,MAX_RUNS):
    rs.append(r)
    final_active_states_counts.append([])
    for n in range(0,SAMPLES_PER_r): 
        final_active_states_counts[-1].append(run_experiment(N,TIME_STEPS,p,r))
    r = r +1/MAX_RUNS 
    
    
    
    
    
    
    
    
    

with open(out_data_path,"w") as fm:
            wt = csv.writer(fm)
            wt.writerows(zip(rs,final_active_states_counts))

print(r)
print(len(rs))
print(len(final_active_states_counts))
bplt.Plotting.scatter(rs,final_active_states_counts,title=out_data_path+".png")
