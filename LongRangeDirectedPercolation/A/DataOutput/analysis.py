import  os
import numpy as np
import pandas as pd
import glob




for file_path in glob.glob(os.getcwd()+"/*csv") :
    df = pd.read_csv(file_path)
    data = df.to_numpy().T
    p = data[2][0]
    tf = data[4]

    print(p,len(np.where(tf==0)[0])==0,tf.std())


