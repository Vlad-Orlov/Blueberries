import os
import numpy as np
import sys
import fileinput



def change_text(rindex):

    text = 'ya tut'
    new_text = '        AerogelRefractiveIndex[i] = ' + str(rindex) + '; //ya tut\n'

    path_to_file = '../../Blueberries/src/B_DetectorConstruction.cpp'
    x = fileinput.input(files=path_to_file, inplace=1)

    for line in x:
        if text in line:
            line = new_text
        sys.stdout.write(line)

    pass


for rindex in np.arange(1, 1.21, 0.1):

    change_text(rindex)

    os.system('cmake ../Blueberries')
    os.system('make -j')
    os.system('./Blueberries vis_file.mac data' + str(rindex) + '.root')
    os.system('clear')
