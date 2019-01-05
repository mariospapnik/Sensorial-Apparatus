#Scanning image brightness

import rhinoscriptsyntax as rs
import System.Drawing
import scriptcontext as sc
import Rhino


def MaxMinRange(curve,width,height):
    Xmin = width
    Xmax = 0
    Ymin = height
    Ymax = 0
    EditPoints = rs.CurveEditPoints(curve)
    for EditPt in EditPoints:
        if EditPt[0] > Xmax:
            Xmax = EditPt[0]
        if EditPt[0] < Xmin:
            Xmin = EditPt[0]
        
        if EditPt[1] > Ymax:
            Ymax = EditPt[1]
        if EditPt[1] < Ymin:
            Ymin = EditPt[1]
    if int(Xmin)<Xmin:
        Xmin = int(Xmin)+1
    if int(Xmax)>Xmax:
        Xmax = int(Xmax)-1
    if int(Ymin)<Ymin:
        Ymin = int(Ymin)+1
    if int(Ymax)>Ymax:
        Ymax = int(Ymax)-1
    XYrange = [[int(Xmin),int(Xmax)],[int(Ymin),int(Ymax)]]
    return(XYrange)

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

def PixelsInCurve(curve,XYrange):
    PixelList = []
    surface = rs.AddPlanarSrf(curve)
    for x in range(XYrange[0][0],XYrange[0][1]+1):
        for y in range(XYrange[1][0],XYrange[1][1]+1):
            if rs.IsPointOnSurface(surface,[x,y,0]) == True:
                PixelList.append([x,y,0])
    rs.DeleteObject(surface)
    return(PixelList)

def GetBrightness(XYList,bmp):
    BrightSum = 0
    i = 0
    for XY in XYList:
        pixel = bmp.GetPixel(XY[0],XY[1])
        BrightSum = BrightSum + (pixel.R + pixel.G + pixel.B)/3
        i = i+1
    BrightSum = BrightSum/i
    print(i,"samples")
    return(BrightSum)



bmp = System.Drawing.Bitmap( "C:\\1.jpg" )
width = bmp.Width
height = bmp.Height
curves = rs.GetObjects("ta curves",4)
#width = rs.GetInteger("width",1000)
#height = rs.GetInteger("height",1000)

for curve in curves:
    rs.EnableRedraw(False)
    XYrange = MaxMinRange(curve,width,height)
    CenterPt = CurveCenterPt(curve)
    PixelList = PixelsInCurve(curve,XYrange)
    AverageBrightness = GetBrightness(PixelList,bmp)
    print AverageBrightness
    uplimit = [CenterPt[0],CenterPt[1],512-(AverageBrightness*2)]
    uplimit = rs.AddPoint(uplimit)
    rs.AddLine(CenterPt,uplimit)
    area = rs.Area(curve)
    circle = rs.AddCircle(CenterPt,(area/1000))
    rs.ExtrudeCurvePoint(circle,uplimit)
    sc.escape_test()
    rs.EnableRedraw(True)
    Rhino.RhinoApp.Wait()
