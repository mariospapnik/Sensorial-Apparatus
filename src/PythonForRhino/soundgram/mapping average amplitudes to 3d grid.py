#--------------------------------------------------------------------------------------------
#Mapping the average amplitudes to the 3d Grid

import scriptcontext as sc
import rhinoscriptsyntax as rs
import Rhino
from random import random as rnd
from random import choice as ch

#getting curve and extracting control points
line = rs.GetObject("curve", 4)
linePoints = rs.PolylineVertices(line)

#getting the List with the average amplitudes for each point
txtAMP = open("C:\\AmpList.txt", "r")
ListAmpAverage = txtAMP.readlines()
txtAMP.close()
for i in range(0,len(ListAmpAverage)):
    ListAmpAverage[i] = float(ListAmpAverage[i])

rs.EnableRedraw(False)

#creating the grid
gridSpacing = 4
gridx = 15
gridy = 11
gridz = 9
cubeList = [[[[] for z in xrange(gridz)] for y in xrange(gridy)]for x in xrange(gridx)]
for x in range(0,gridx):
    for y in range(0,gridy):
        for z in range(0,gridz):
            cubeList[x][y][z] = [gridSpacing*x,gridSpacing*y,gridSpacing*z,0,rs.AddPoint([gridSpacing*x,gridSpacing*y,gridSpacing*z])]
            
rs.EnableRedraw(True)

#definition for adding random points
def addRandomPts(radius,center):
    newpts = []
    for i in range(0,int(5*radius)):
        newpt = [rnd()*radius*ch([-1,1])+ center[0],rnd()*radius*ch([-1,1]) + center[1],rnd()*radius*ch([-1,1]) + center[2]]
        newpts.append(rs.AddPoint(newpt))
        rs.AddLine(newpt,[center[0],center[1],center[2]])
    print("RANDOM",len(newpts),"pts")
    return newpts


rs.EnableRedraw(False)
# loop in the grid to create the average amplitudes and the spheres
for x in range(0,gridx):
    for y in range(0,gridy):
        for z in range(0,gridz):
            radius = 0
            count = 0
            for i in range(247,len(linePoints)):
                distance = rs.Distance(cubeList[x][y][z][4],linePoints[i])
                if distance < 8:
                    count += 1
                    radius = radius + ListAmpAverage[i]
                    
            if radius != 0:
                radius = (radius*70)/count
                #rs.AddSphere(cubeList[x][y][z][4],radius)
                addRandomPts(radius,cubeList[x][y][z][0:3])
                
            print('added',count,'points, to the',x,y,z,'pt of the grid')
            cubeList[x][y][z][3] = radius
                
            Rhino.RhinoApp.Wait()
            sc.escape_test()

rs.EnableRedraw(True)
