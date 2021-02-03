import subprocess
#subprocess.run(["ls", "-l","-a"])
import math 
import numpy as np
import Plotter.plotter as bplt
import csv
import os
N=100000
TIME_STEPS=200000
p= 0.75
MAX_RUNS = 10  
SAMPLES_PER_r =1
out_data_path = "runs_resultsBroad.csv"
data_out_dir = "RawExperimentalOutput"
r= 0 

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


def run_experiment(N=N,TIME_STEPS=TIME_STEPS,p=p,r=r):
    
    #Have to remake the file each run due to quicker to have things in file
    #subprocess.run(["make"])
    #exp = subprocess.run(["./DirectedPercolation"],stdout=subprocess.PIPE)

    #ExperimentName = exp.stdout.decode('utf-8')[:-1]
    ExperimentName ="TESTING" 
    print(ExperimentName)
    datas = np.empty((0,1000),int) 
    counts = []
    for entry in os.scandir(data_out_dir):
        f =open(entry.path,"r")#Open produced experiment
        data = np.array(list(f.readline()),dtype=int)
        counts.append(np.count_nonzero(data))
        print(entry.path) 
        f.close()
        var = np.array(counts)
        datas = np.append(datas,[data],axis=0)
    
    bplt.Plotting.colour_map_gen(datas,ExperimentName+".png")
    

run_experiment()

#with open(out_data_path,"w") as fm:
#    wt = csv.writer(fm)
#
#    r=0
#    for s in range(0,MAX_RUNS):
#        rs.append(r)
#        final_active_states_counts.append([])
#        for n in range(0,SAMPLES_PER_r): 
#            final_active_states_counts[-1].append(run_experiment(N,TIME_STEPS,p,r))
#        r = r +1/MAX_RUNS 
#    
#        wt.writerow((rs[-1],final_active_states_counts[-1]))
#bplt.Plotting.scatter(rs,np.array(final_active_states_counts).T,title=out_data_path+".png")
