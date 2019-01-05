#--------------------------------------------------------------------------------------------
#Mapping WAV file amplitudes to points in space according to the accelerometer values

import scriptcontext as sc
import rhinoscriptsyntax as rs
import Rhino

#some 
CurTime = 0
SampleRate = 44100/1000
ListAmpAverage = []
first = 0


#opening the wav text to take the amplitudes
fileWAV  = open("C:\\voiceMono2.txt","r")
amplitudes = fileWAV.readlines()
fileWAV.close()
for i in range(0,len(amplitudes)):
    amplitudes[i] = float(amplitudes[i])

#opening the time text to take the time intervals
fileTIME = open("C:\\time.txt","r")
TimeList = fileTIME.readlines()
fileTIME.close()
for i in range(0,len(TimeList)):
    TimeList[i] = float(TimeList[i])

#getting curve and extracting control points
line = rs.GetObject("curve", 4)
linePoints = rs.PolylineVertices(line)

#creating list with average amplitudes for the points with zeros inside
for i in range(0,len(linePoints)):
    ListAmpAverage.append(0)


# MAIN part.. loop through points to find amplitude
for i in range(0,9100): #or better (0,len(linePoints))
    CurTime = CurTime + TimeList[i]
    if CurTime > 5000 :
        
        if first == 0:
            startNo = i
            print startNo
            first = 1
            firstTime = CurTime
        CurSample = int(SampleRate*(CurTime-firstTime))
        SampleRangeMax = CurSample + SampleRate*TimeList[i]
        if SampleRangeMax > len(amplitudes):
            SampleRangeMax = len(amplitudes)
        SampleRangeMin = CurSample - SampleRate*TimeList[i-1]
        ampAverage = 0
        for j in range(int(SampleRangeMin),int(SampleRangeMax)):
            
            if amplitudes[CurSample] <0:
                sample =  - amplitudes[CurSample]
            else:
                sample =  amplitudes[CurSample]
                
            
            ampAverage = ampAverage + sample
        print('pt', i ,'out of', len(linePoints),'sample', CurSample)
        ampAverage = ampAverage /(SampleRangeMax - SampleRangeMin)
        ListAmpAverage[i] = ampAverage
        
        Rhino.RhinoApp.Wait()
        sc.escape_test()


AmpList = open("C:\\AmpList.txt", "w")
for amp in ListAmpAverage:
    AmpList.write("%f \n" %(amp))
AmpList.close()
