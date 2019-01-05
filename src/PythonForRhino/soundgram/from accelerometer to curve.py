#--------------------------------------------------------------------------------------------
#Converting accelerometer values to a 3d polyline

import scriptcontext as sc
import rhinoscriptsyntax as rs
import Rhino

file  = open("C:\\default.txt","r")
lines = file.readlines()
file.close()
lines.pop(0)

velocityLast = velocity = [0,0,0]
PointsList = []
TimeList = []
firstPoint = rs.AddPoint(0,0,0)
PointsList.append(firstPoint)
LastPoint = [0,0,0]
acceleration = [0,0,0]

for line in lines:
    #print line
    tokens = line.split()
    #print tokens
    #acceleration = [0,0,0]
    displacement = [0,0,0]
    
    time = (float(tokens[3])/1000)
    #print time
    
    for j in range(0,3):
        acceleration[j] = float(tokens[j])
    print('acceleration', acceleration)
    
    for k in range(0,3):
        displacement[k] = (1/2)*(acceleration[k])*(time**2)
        #displacement[k] = velocityLast[k]*time + (1/2)*(acceleration[k])*(time**2)
        
    print('displacement', displacement)
    
    for l in range(0,3):
        velocity[l] = velocityLast[l] + acceleration[l]*time
    
    #displacement = rs.AddPoint(displacement)
    #print ('last point prin',LastPoint)
    LastPoint = rs.PointAdd(LastPoint, displacement)
    #print ('last point',LastPoint)
    LastPoint = rs.AddPoint(LastPoint)
    PointsList.append(LastPoint)
    velocityLast = velocity
    TimeList.append(time*1000)
    
    Rhino.RhinoApp.Wait()
    sc.escape_test()
            
            
timefile = open("C:\\time.txt","w")
for segment in TimeList:
    timefile.write("%f \n" %(segment))
timefile.close()
#print PointsList
#rs.AddPolyline(PointsList)
rs.AddCurve(PointsList,1)
