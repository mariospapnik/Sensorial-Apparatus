#Mapping curve area to curve length

import rhinoscriptsyntax as rs
import System.Drawing
import scriptcontext as sc
import Rhino

curves = rs.GetObjects("ta curves",4)
areaMax = 0


def CurveCenterPt(curve):
    Sum = [0,0,0]
    EditPoints = rs.CurveEditPoints(curve)
    for EditPoint in EditPoints:
        for i in range(0,3):
            Sum[i] = Sum[i] + EditPoint[i]
    for i in range(0,3):
        Sum[i] = Sum[i]/len(EditPoints)
    CenterPt = Sum
    return(CenterPt)


for curve in curves:
    area = rs.Area(curve)
    if area > areaMax:
        areaMax = area

for curve in curves:
    rs.EnableRedraw(False)
    CenterPt = CurveCenterPt(curve)
    area = rs.Area(curve)
    uplimit = rs.AddPoint(CenterPt[0],CenterPt[1],(area/areaMax)*56)
    rs.AddLine(CenterPt,uplimit)
    sc.escape_test()
    rs.EnableRedraw(True)
    Rhino.RhinoApp.Wait()  
