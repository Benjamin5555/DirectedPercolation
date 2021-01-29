import subprocess
#subprocess.run(["ls", "-l","-a"])
import math 
import Plotter.plotter as bplt
import csv
N=10
TIME_STEPS=20
p= 0.75
r= 0.2
MAX_RUNS = 100
out_data_path = "runs_results.csv"

rs = []
final_active_states_count  = []
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

for s in range(0,MAX_RUNS):
    
    final_active_states_count.append(run_experiment(N,TIME_STEPS,p,r))
    rs.append(r)
    if(final_active_states_count[-1]>N/2):
        #Too much spreading, i.e. active states, reduce r
        r_upper = r+dr
    elif(final_active_states_count[-1]<N/2):
        #Too few active states, increase r
        r_lower = r - dr
    else:
        print("Finished")
        break #Equal, r perfect!
    r = (r_upper + r_lower)/2



with open(out_data_path,"w") as fm:
            wt = csv.writer(fm)
            wt.writerows(zip(rs,final_active_states_count))

print(r)
print(len(rs))
print(len(final_active_states_count))
bplt.Plotting.scatter(rs,[final_active_states_count],title=out_data_path+".png")
