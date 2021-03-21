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
N=1000000
TIME_STEPS =5E6

STEPS_PER_SAVE=500
INIT_PROB=1

OUT_FILE_PATH = ""
PARAMETER_FILE = "parameter.h"
RAW_DATA_OUT = "RawExperimentalOutput/"
out_data_path = "DataOutput/runs_result"
KEEP_DATA = False


def write_parameters(p,r):
    with open(PARAMETER_FILE,"w") as f:
        f.write("#define N "+str(N)+"\n")
        f.write("#define TIME_STEPS "+str(TIME_STEPS)+"\n")
        f.write("#define STEPS_PER_SAVE "+str(STEPS_PER_SAVE)+"\n")
        f.write("#define OUT_FILE_PATH \""+str(OUT_FILE_PATH)+"\"\n")
        f.write("#define p "+str(float(p))+"\n")
        f.write("#define r "+str(float(r))+"\n")
        f.write("#define INIT_PROB "+str(INIT_PROB)+"\n")


def run_experiment(p,r):
    ExperimentName = "N"+str(N)+"T"+str(TIME_STEPS)+"p"+str(p)+"r"+str(r)
    subprocess.run(["mkdir",RAW_DATA_OUT+ExperimentName])
    
    global OUT_FILE_PATH
    OUT_FILE_PATH = RAW_DATA_OUT+ExperimentName
    
    write_parameters(p,r)
    subprocess.run(["make", "clean"])
    subprocess.run(["make"])

    exp = subprocess.run(["./DirectedPercolation"])

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

    return np.arange(0,len(turbulentFractions)*STEPS_PER_SAVE,STEPS_PER_SAVE), turbulentFractions 




def main():
    global out_data_path, r_lower, r_upper, dr, TIME_STEPS,STEPS_PER_SAVE
    #fullExperimentName =str(N)+","+str(TIME_STEPS)+","+str(r_lower)+","+str(r_upper)+","+str(dr)
    out_data_path = out_data_path+"spec"
    with open(out_data_path+".csv",'w') as f:
        output_writer  = csv.writer(f)
        
        #output_writer.writerow("N,TIME_STEPS,p,r,steps complete (/num_step_per_save),stDevCounts,init count, average count,average diff in counts between time steps")
        for p in [0.75]:
            for r in [0.4,0.5,0.45,0.46,0.454,0.455,0.4546,0.4547,0.45465]:
                out_full_data_path = out_data_path+"."+str(r)
        
                print((p,r))     
                
                subExpName = run_experiment(p,r)
                #print(expName)
        
                times,turbulentFraction = collect_data(subExpName)

                
                
                plt.plot(times, turbulentFraction,label=r)
                with open(out_data_path+str(r)+'csv','w') as step_f:
                    step_writer = csv.writer(step_f)
                    for i in range(0,len(times)):
                        step_writer.writerow((N,TIME_STEPS,p,r,times[i],turbulentFraction[i]))
            plt.xlabel("Time (Dimensionless)")
            plt.ylabel("Turbulent Fraction")
            plt.legend()
            plt.ticklabel_format(useOffset=False)

            plt.savefig(str(N)+str(TIME_STEPS)+str(p)+str(r_lower)+","+str(r_upper)+","+str(dr)+str(datetime.datetime.now())+".png") 
            plt.show() 
                #print("Gen image")
                #im = Image.fromarray(np.uint8(data*255))
                #im.save(expName+".png")
                #print("Done with image")
                #bplt.Plotting.colour_map_gen(data,expName+".png")  
    
if __name__ == "__main__":
    main()
            
             
    
    
            #print("Gen image")
            #im = Image.fromarray(np.uint8(data*255))
            #im.save(expName+".png")
            #print("Done with image")
            #bplt.Plotting.colour_map_gen(data,expName+".png")  
    

