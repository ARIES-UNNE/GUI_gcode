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


import math

# Read configuration100 values from a file
def read_configuration(filename):
    config_values = {}
    with open(filename, 'r') as file:
        line_number = 0
        for line in file:
            line_number += 1
            try:
                if ':' in line:
                    key, value = line.split(':')
                    key = key.strip()
                    value = value.strip()

                    # Intentar convertir a entero, si falla, probar con float, y finalmente dejar como string.
                    try:
                        value = int(value)
                    except ValueError:
                        try:
                            value = float(value)
                        except ValueError:
                            # Si no es int ni float, se queda como string.
                            value = value

                    config_values[key] = value

            except Exception as e:
                print(f"Error on line {line_number}: {e}")
    return config_values

# File containing the section values
config = read_configuration('section_values.txt')

# Variable declaration

# Independent variables
AxoCenter = [config.get('Center X Value'), config.get('Center Y Value')]



D=config.get('Filament (0)') # Syringe/filament diameter for Volume (input) calc
nd1  =config.get('Nozzle (1)')# Noozle/needle diameter for Volume (output) calc
nd2  = config.get('Nozzle (0)') # Noozle/needle diameter for Volume (output) calc

fr = 600 #feedrate

flow_mat1 = 1 # Multiplier for flow adjustment
flow_mat2 = 1 # Multiplier for flow adjustment

E_extruded = 0 # Initialization of extruded lenght


##################################################################
#                              USER SELECTION                    #
##################################################################


####################################
#   Choose btw circle or square    #
####################################
# For trials:
shape = config.get('Shape') 
L = config.get('Size 1')
mat = config.get('Total Materials') # Material selection
#dist = 1# Distance between strands
nz = config.get('Size 2') #Number of Z-layers


while(True):
    opc = config.get('Infill Index') # Material selection
    if (opc==1):
        dist = config.get('Strand Distance Value') # Material selection
        ns1 = (L/(dist+nd1))
        p = ns1*(nd1)/L
        break
    elif(opc==0):
        p = config.get('Infill Value decimal') # infill density [0-1]
        dist =  (nd1+nd2)*p/100
        ns1 = int(L/dist)-1 #Number of strand, mat 1
        ns2 = ns1 - 1
        break
    else:
        print("Error. Option number out of range. Try again. ")


# Dependant variables

if nd1>nd2:
    hl = 0.65*nd1
else:
    hl = 0.65*nd2
    
ns1 = int(L/(nd1+nd2)*p)-1 #Number of strand, mat 1
ns2 = ns1 - 1

dm1 = L/ns1 #Fix value for all the scaffold
dm2 = dm1           #Just an initial value. Changes in borders

#Vector allocations or initiallization

xo=AxoCenter[0]-L/2
yo=AxoCenter[1]-L/2
zo = 0


###########################################################
#                     EXTRUSION FUNCTION
###########################################################
# Function to calculate the volume extruded, given a lenght
def extrusion(L,d,D):
    ext = pow(d/D,2)*L
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

for i in range (0,nz):
   
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
    
    #Shell mat 2
    mat2_pos.append([xo+nd1/2+nd2/2,      yo+nd1/2])
    mat2_pos.append([xo+L-nd1/2-nd2/2,    yo+nd1/2])
    mat2_pos.append([xo+L-nd1/2-nd2/2,    yo+L-nd1/2-nd2/2])
    mat2_pos.append([xo+nd1/2,      yo+L-nd1/2-nd2/2])
    mat2_pos.append([xo+nd1/2,      yo+nd1/2+nd2/2])
    
    for n in range(1,int(ns1)+1):    
        if (i % 2) == 0: #even layer
            if (n % 2) == 0: #even strand
                mat2_pos.append([xo+(2*n-1)*dm1/2,      yo+L-nd1/2-nd2/2])
                mat2_pos.append([xo+(2*n-1)*dm1/2,      yo  +nd1/2+nd2/2])
            else: #odd strand
                mat2_pos.append([xo+(2*n-1)*dm1/2,      yo  +nd1/2+nd2/2])
                mat2_pos.append([xo+(2*n-1)*dm1/2,      yo+L-nd1/2-nd2/2])
        else:
            if (n % 2) == 0: #even strand
                mat2_pos.append([xo+L-nd1/2-nd2/2,      yo+(2*n-1)*dm1/2])
                mat2_pos.append([xo  +nd1/2+nd2/2,      yo+(2*n-1)*dm1/2])
            else: #odd strand
                mat2_pos.append([xo  +nd1/2+nd2/2,      yo+(2*n-1)*dm1/2])
                mat2_pos.append([xo+L-nd1/2-nd2/2,      yo+(2*n-1)*dm1/2])

    mat1.append(mat1_pos)
    mat2.append(mat2_pos)
    
    #Set feedrate 
    file.write("T0      ;Set extruder 0 as active\n")
    file.write('G1 X{:.3f} Y{:.3f} Z{:.3f} F{:.0f} ;Set feedrate \n'.format(xo, yo, zo+hl, fr))

    #Select the extruder 1 and move to P0
    file.write("T0      ;Set extruder 0 as active\n")
    file.write('G0 X{:.3f} Y{:.3f} Z{:.3f}\n'.format(xo, yo, zo+(i+1)*hl))

    for o in range(1, len(mat1_pos)):   

        length = abs(math.sqrt((mat1_pos[o][0]-mat1_pos[o-1][0])**2 + (mat1_pos[o][1]-mat1_pos[o-1][1])**2))
        file.write('G1 X{:.3f} Y{:.3f} E{:.3f}\n'.format(mat1_pos[o][0], mat1_pos[o][1], extrusion(length,nd1,D)*flow_mat1))
    
    
    #Select the extruder 2 and move to P0
    file.write("T1      ;Set extruder 1 as active\n")
    file.write('G0 X{:.3f} Y{:.3f} Z{:.3f}\n'.format(xo, yo, zo+(i+1)*hl))
    
    for o in range(1, len(mat2_pos)): 
           
        length = abs(math.sqrt((mat2_pos[o][0]-mat2_pos[o-1][0])**2 + (mat2_pos[o][1]-mat2_pos[o-1][1])**2))
        file.write('G1 X{:.3f} Y{:.3f} E{:.3f}\n'.format(mat2_pos[o][0], mat2_pos[o][1], extrusion(length,nd1,D)*flow_mat1))

    file.write('G1 Z{:.3f}\n'.format(zo+(i+2)*hl))
    
""" 
for i in range (0,nz):
    if (i % 2) == 0: #even layer
            for j in range (0,int(n/2)):
            np.append(mat1_pos, [P[0],      0)
            np.append(mat1_pos, [P[0]+L,      0)
            np.append(mat1_pos, [P[0]+L,      0)
            np.append(mat1_pos, [P[0],      0)
            
        for k in range (1,2*n-1):
       
            if (k % 2) == 0:
                ypos.append(ypos[-1])
                
            else:
                ypos.append(ypos[-2]+dist)
                
    else:    #odd layer
                
        for l in range (0,int(n/2)):
            ypos.append(P0[0])
            ypos.append(P0[0])
            ypos.append(P0[0]+L)
            ypos.append(P0[0]+L)    
            
        for m in range (1,2*n-1):
            
            if (m % 2) == 0:
                xpos.append(xpos[-2]+dist)
                
            else:
                xpos.append(xpos[-1])
                
        xpos.append(xpos[-2]+dist)
        xpos.append(xpos[-1]) 
        
"""



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
""" ax=plt.figure().gca()
plt.axis('square')
plt.xlabel('X')
plt.ylabel('Y')
plt.xlim(0, 130)
plt.ylim(0, 90)

for k in range(len(mat1)):
    xplot_mat1 = [mat1[k][i][0] for i in range(len(mat1[0]))]
    yplot_mat1 = [mat1[k][i][1] for i in range(len(mat1[0]))]

    xplot_mat2 = [mat2[k][i][0] for i in range(len(mat2[0]))]
    yplot_mat2 = [mat2[k][i][1] for i in range(len(mat2[0]))]

    #plot the final g-code values (red)     
    ax.plot(xplot_mat1,yplot_mat1,'r')
    ax.plot(xplot_mat2,yplot_mat2,'b')   
plt.show()   """
#################################################################
