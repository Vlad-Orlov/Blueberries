import os
import numpy as np
import sys
import fileinput



def change_text(position):

    text = 'ya tut'
    new_text = '              G4ThreeVector position(' + str(position) + '*mm, //ya tut\n'

    path_to_file = '../LHCbLumi-CONE/src/L_PrimaryGeneratorAction.cpp'
    x = fileinput.input(files=path_to_file, inplace=1)

    for line in x:
        if text in line:
            line = new_text
        sys.stdout.write(line)

    pass
for position in np.arange(0, 2.01, 0.05):

    change_text(position)

    os.system('cmake ../LHCbLumi-CONE')
    os.system('make -j')
    os.system('./LHCbLumi vis_file.mac data.root')
    os.system('clear')
    generate_plot(position)