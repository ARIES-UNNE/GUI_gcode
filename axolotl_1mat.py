# -*- coding: utf-8 -*-
"""
Created on Thu Nov 23 13:33:27 2023

@author: nicuma3

///////////////////////////////////////////////////
// Nieves Cubo Mateo - 23.11.2023 - ARIES        //
///////////////////////////////////////////////////

// Bicomponentiled wood structure square generator

 Parameters: L (square side), h (scaffold heigh) d (strand distance), 
 dn1 (diameter noozle mat 1), dn2 (diameter noozle mat 2), 
 nz (number of layers in z), ref point (x0,y0), 
 
   x0,y0 _________________
         ________________|  ^
        |_________________  |
         ________________|  |  L
        |_________________  | 
        _________________|  v
                   
           
         <--------------->
                 L 
           
"""


import numpy as np
import matplotlib.pyplot as plt
import math


# Variable declaration

# Independant variables

D=1.74 # Syringe/filament diameter for Volume (input) calc
nd1=0.410 # Noozle/needle diameter for Volume (output) calc

fr=600 #feedrate
flow_mat1 = 1 # Multiplier for flow adjustment
E_extruded = 0 # Initialization of extruded lenght


##################################################################
#                              USER SELECTION                    #
##################################################################
# For trials:

L = 20
p = 0.20 # infill density [0-1]
mat = 1
nz=8 #Number of Z-layers

# Dependant variables
ns1 = int(L/(nd1)*p)-1 #Number of strand, mat 1
dm1 = L/ns1 #Fix value for all the scaffold

AxoCenter = [65,45]  #Plate size [90,130] 

#Vector allocations or initiallization

xo=AxoCenter[0]-L/2
yo=AxoCenter[1]-L/2
zo = 0
    

###########################################################
#                     EXTRUSION FUNCTION
###########################################################
# Function to calculate the volume extruded, given a lenght
def extrusion(L,d,D):
    ext=pow(d/D,2)*L
    return ext
############################################################


 
    
###############################################################
#          Generation of G-Code and FILE writing
###############################################################

# Write a .txt with the obtained values, with the G-Code fomat:
file = open("Axo3_1mat.gcode","w+") # 'w' for write, the '+' to create the file if it does not exists        

# START G-CODE
file.writelines([
    "M107          ;fan off\n",
    "G21           ;set units to millimeters\n",
    "G90           ;use absolute coordinates\n",
    "M83           ;use relative distances for extrusion\n",
    "G92 E0        ;reset extruder position\n",
    "G4 S1         ;Wait 1 second\n",
    "M104 T0 S0    ;Switch off temperature of extruder 0\n",
    "M104 T1 S0    ;Switch off temperature of extruder 1\n",
    "M104 T2 S0    ;Switch off temperature of extruder 2\n",
    "A130 T0 S0    ;Close valve 0\n",
    "A130 T1 S0    ;Close valve 1\n",
    "A130 T2 S0    ;Close valve 2\n",
    "A140 T0 S0    ;Switch off pressure extruder 0\n",
    "A140 T1 S0    ;Switch off pressure extruder 1\n",
    "A140 T2 S0    ;Switch off pressure extruder 2\n",
    "M140 S0       ;T=0, no waits\n",
    "G28           ;Home\n"]) 



###############################################################
#                       LAYERS
###############################################################
mat1 = [] #3D array - layer for the ordered x & y values for the g-code of Mat1
mat2 = [] #3D array - layer for the ordered x & y values for the g-code of Mat2


######## MATERIAL 1 - T0


    
#x0 - Move to P0
file.write('G0 X{:.3f} Y{:.3f} Z{:.3f}\n'.format(xo, yo, 3))

#Set feedrate 
file.write("T0      ;Set extruder 0 as active\n")
file.write('G1 X{:.3f} Y{:.3f} Z{:.3f} F{:.0f} ;Set feedrate \n'.format(xo, yo, zo+nd1*0.65, fr))


for i in range (0,nz):
    
    #x0 - Move to P0
    if i!=0:
        file.write('G0 X{:.3f} Y{:.3f} Z{:.3f}\n'.format(xo, yo, zo+(i+1)*nd1*0.65))
    
    mat1_pos = [] #2D array - layer for the ordered x & y values for the g-code of Mat1
    mat2_pos = [] #2D array - layer for the ordered x & y values for the g-code of Mat2

    #Shell mat 1
    mat1_pos.append([xo,      yo])
    mat1_pos.append([xo+L,    yo])
    mat1_pos.append([xo+L,    yo+L])
    mat1_pos.append([xo,      yo+L])
    mat1_pos.append([xo,      yo])
    
    for n in range(0,int(ns1)+1):    
        if (i % 2) == 0: #even layer
            if (n % 2) == 0: #even strand
                mat1_pos.append([xo+n*dm1,      yo+L])
                mat1_pos.append([xo+n*dm1,      yo])
            else: #odd strand
                mat1_pos.append([xo+n*dm1,      yo])
                mat1_pos.append([xo+n*dm1,      yo+L])
        else:
            if (n % 2) == 0: #even strand
                mat1_pos.append([xo+L,      yo+n*dm1])
                mat1_pos.append([xo,    yo+n*dm1])
            else: #odd strand
                mat1_pos.append([xo,    yo+n*dm1])
                mat1_pos.append([xo+L,      yo+n*dm1])
    

    mat1.append(mat1_pos)
    
    #Select the extruder
    if i!=0:
        file.write("T0      ;Set extruder 0 as active\n")

    for o in range(1, len(mat1_pos)):   

        lenth = abs(math.sqrt((mat1_pos[o][0]-mat1_pos[o-1][0])**2 + (mat1_pos[o][1]-mat1_pos[o-1][1])**2))
        file.write('G1 X{:.3f} Y{:.3f} E{:.3f}\n'.format(mat1_pos[o][0], mat1_pos[o][1], extrusion(lenth,nd1,D)*flow_mat1))
    

# END G-CODE
file.writelines([
    "G4 S1         ;Wait 1 second\n",
    "A21 R0 G255 B0    ;Set chamber color to green\n",
    "M104 T0 S0    ;Switch off temperature of extruder 0\n",
    "M104 T1 S0    ;Switch off temperature of extruder 1\n",
    "M104 T2 S0    ;Switch off temperature of extruder 2\n",
    "A120 T0 S0   ;Disable surface offset feature\n",
    "A120 T1 S0   ;Disable surface offset feature\n",
    "A120 T2 S0   ;Disable surface offset feature\n",
    "A130 T0 S0    ;Close valve 0\n",
    "A130 T1 S0    ;Close valve 1\n",
    "A130 T2 S0    ;Close valve 2\n",
    "A140 T0 S0    ;Switch off pressure extruder 0\n",
    "A140 T1 S0    ;Switch off pressure extruder 1\n",
    "A140 T2 S0    ;Switch off pressure extruder 2\n",
    "M140 S0       ;T=0, no waits\n",
    "G28           ;Home\n"]) 


#Save and close file :)    
file.close

################################################################
#                             PLOT
################################################################
# Creation of an external squared figure with fixed size:
ax=plt.figure().gca()
plt.axis('square')
plt.xlabel('X')
plt.ylabel('Y')
plt.xlim(0, 130)
plt.ylim(0, 90)

for k in range(len(mat1)):
    xplot_mat1 = [mat1[k][i][0] for i in range(len(mat1[0]))]
    yplot_mat1 = [mat1[k][i][1] for i in range(len(mat1[0]))]

    #plot the final g-code values (red)     
    ax.plot(xplot_mat1,yplot_mat1,'r')

plt.show()
#################################################################
