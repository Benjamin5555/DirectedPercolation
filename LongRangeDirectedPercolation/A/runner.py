import subprocess
#subprocess.run(["ls", "-l","-a"])
import math 
import numpy as np
#import Plotter.plotter as bplt
import csv
import os
import glob
import sys
import re
from PIL import Image
import csv
import matplotlib.pyplot as plt
import datetime

#N=100000
#TIME_STEPS = 200000
N=2500
TIME_STEPS = 1,000,000
STEPS_PER_SAVE=1

SIGMA=2000
OUT_FILE_PATH = ""
p= 0.75 
#r= 0.0 
INIT_PROB =0.1
#OUT_FILE_PATH = sys.argv[1]
PARAMETER_FILE = "parameter.h"
RAW_DATA_OUT = "RawExperimentalOutput/"
out_data_path = "DataOutput/runs_result"
KEEP_DATA = False 
p_low  =0.1
p_high = 0.9
dp = 0.1

def write_parameters(p):
    with open(PARAMETER_FILE,"w") as f:
        f.write("#define N "+str(N)+"\n")
        f.write("#define TIME_STEPS "+str(TIME_STEPS)+"\n")
        f.write("#define STEPS_PER_SAVE "+str(STEPS_PER_SAVE)+"\n")
        f.write("#define OUT_FILE_PATH \""+str(OUT_FILE_PATH)+"\"\n")
        f.write("#define p "+str(float(p))+"\n")
        f.write("#define INIT_PROB "+str(INIT_PROB)+"\n")
        f.write("#define SIGMA "+str(SIGMA)+"\n")


def run_experiment(p):
    ExperimentName = "N"+str(N)+"T"+str(TIME_STEPS)+"p"+str(p)
    subprocess.run(["mkdir",RAW_DATA_OUT+ExperimentName])
    
    global OUT_FILE_PATH
    OUT_FILE_PATH = RAW_DATA_OUT+ExperimentName
    
    write_parameters(p)
    subprocess.run(["make", "clean"])
    subprocess.run(["make"])

    exp = subprocess.run(["./LRDPGTA"])

    return ExperimentName#exp.stdout.decode('utf-8')[:-1]

def collect_data(ExperimentName):
    """
        Checks experimental into numpy array and also gives activeCount in each time step output
    """
    #dir_files = os.listdir(OUT_FILE_PATH)
    
    files = glob.glob(OUT_FILE_PATH+"/*")
    files.sort(key= lambda s: [int(t) if t.isdigit() else t.lower() for t in re.split('(\d+)', s)])

    datas = [] 
    turbulentFractions = []
    for cfile in files:
        f =open(cfile,"r")#Open produced experiment
        datas.append(list(f.readline()))
        turbulentFractions.append(datas[-1].count("1")/N)
        f.close()
        if(not KEEP_DATA):
            subprocess.run(["rm",cfile])
    
    return np.arange(0,len(turbulentFractions)*STEPS_PER_SAVE,STEPS_PER_SAVE), turbulentFractions, datas 





def main():
    global out_data_path,p_low,p_high,dp,SIGMA, TIME_STEPS,STEPS_PER_SAVE, INIT_PROB
    SIGMA = float(sys.argv[1])
    p_low = float(sys.argv[2])
    p_high = float(sys.argv[3])
    dp = float(sys.argv[4])
    TIME_STEPS = int(sys.argv[5])
    STEPS_PER_SAVE = int(sys.argv[6])
    INIT_PROB = float(sys.argv[7])
    stime = str(datetime.datetime.now())
    out_data_path = out_data_path+stime+str((p_low,p_high,dp))
    with open(out_data_path+".csv",'w') as f:
        output_writer  = csv.writer(f)
        for p in np.arange(p_high,p_low,-1*dp): 
            subExpName = run_experiment(p)
            times,turbulentFraction,data = collect_data(subExpName) 
            
            
            print("Plotting")
            plt.scatter(np.log(times),np.log(turbulentFraction),label=p)
            print("Done Plotting")
            with open(out_data_path+str(SIGMA)+str(p)+'csv','w') as step_f:
                step_writer = csv.writer(step_f)
                for i in range(0,len(times)):
                    step_writer.writerow((N,TIME_STEPS,p,times[i],turbulentFraction[i]))
    plt.legend()
    plt.ticklabel_format(useOffset=False)

    plt.savefig(str(N)+str(TIME_STEPS)+str(p)+str(p_low)+","+str(p_high)+","+str(dp)+stime+".png") 
    plt.show() 



if __name__ == "__main__":
    main()
            
             
    
    
            #print("Gen image")
            #im = Image.fromarray(np.uint8(data*255))
            #im.save(expName+".png")
            #print("Done with image")
            #bplt.Plotting.colour_map_gen(data,expName+".png")  
def gen_image(data):
    print(data[0])
    print(data[-1])
    print("Gen image")
    im = Image.fromarray(np.uint8(data)*255)
    im.save(subExpName+"Sig"+str(SIGMA)+"InitProb"+str(INIT_PROB)+".png")
    print("Done with image")
    #bplt.Plotting.colour_map_gen(data,expName+".png")  



def fullExp(r):
    out_full_data_path = out_data_path+"full"+str(r)
    #for i in range(REPEATS):
    
    print((p,r))     
    
    #print(expName)
    
    
    
    
    plt.plot(times, turbulentFraction,label=r)
    with open(out_data_path+str(r)+'csv','w') as step_f:
        step_writer = csv.writer(step_f)
        for i in range(0,len(times)):
            step_writer.writerow((N,TIME_STEPS,p,r,times[i],turbulentFraction[i]))
    plt.legend()
    plt.ticklabel_format(useOffset=False)

    plt.savefig(str(N)+str(TIME_STEPS)+str(p)+str(r_lower)+","+str(r_upper)+","+str(dr)+str(datetime.datetime.now())+".png") 
    plt.show() 

    #plt.legend()
    #plt.ticklabel_format(useOffset=False)
    
    #plt.savefig(str(N)+str(TIME_STEPS)+str(p)+str(r_lower)+","+str(r_upper)+","+str(dr)+str(datetime.datetime.now())+".png") 
    #plt.show() 

