//2 mics +2 photcells +1 capacitive sensor
// mic output range
// photo output range
// cap output range




//libraries
    #include <CapacitiveSensor.h>
    #include <Servo.h>


//General use variables declare
    int TotalSamples = 10;
    int allcounter;
    float SmoothRate = 0.5;
    unsigned long TotalSamplingTimer;
    unsigned long TimeBeforeServosMove;


//mics variables declare
    int MicMinMaxArray[2][2] = {{1024,0},{1024,0}};
    int MicVolumesArray[2];
    int MicCurrentValueArray[2];
    int MicSampleSpan = 10;  //ms
    unsigned long MicTimer;
    int MicValueMIN = 80;
    int MicValueMAX = 800;
    int MicFinSamplesArray[2];
    int OLDMicFinSamplesArray[2] = {80,700};

//photocell variables declare
    int PhotocellCalibrationTime = 7000;
    int LightMinMaxArray[2][2] = {{1023,0},{1023,0}};
    int LightLevelArray[2];
    unsigned long PhotocellTimer;
    int MappedLightLevelArray[2];
    int PhotoSampleSpan = 10; //ms
    int LightSum0;
    int LightSum1;
    int counter;
    int PhotoValueMIN = 0;
    int PhotoValueMAX = 400;
    int PhotoFinSamplesArray[2];
    int OLDPhotoFinSamplesArray[2];

//capacitive sensor variables declare
    CapacitiveSensor capSensor = CapacitiveSensor(4,2);
    int CapValue = capSensor.capacitiveSensor(70);
    int CapSensorTimeout = 50; //ms
    int CapFinSample;
    int OLDCapFinSample = 80;
    int CapValueMIN = 80;
    int CapValueMAX = 500;
    int TouchLimit = 1000;
    int TouchFlag = 0;


//Servos variables declare
    int SDelayMIN = 165;  //min + max delay in ms for the servos to turn
    int SDelayMAX = 1000;
    int MaxR = 650;
    int m0 = 0;
    int m1 = 0;
    int p2 = 0;
    int p3 = 0;
    int c = 0;
    //servos 180  (1,2,3) 
    Servo s1;  
    Servo s2;
    Servo s3;
    int s1p = 3;                 //at PINS (3,5,6)
    int s2p = 5; 
    int s3p = 6;
    int s1U = 0;                 //+ speed
    int s2U = 0; 
    int s3U = 0;
    int s1R = 0;                 //+ rotation
    int s2R = 0; 
    int s3R = 0;
    int i1 = 1;                   //+degrees iterations
    int i2 = 1;
    int i3 = 1;
    int s1T = 0;                 //+  time needed
    int s2T = 0; 
    int s3T = 0; 
    int s1Delay = 0;             //+ delays for the loops
    int s2Delay = 0; 
    int s3Delay = 0; 
    int i1count = 1;            //+ iterations counters
    int i2count = 1;
    int i3count = 1;
    
    
    //servos 360   (4,5,6)  + speed + rotation
    Servo s4;
    Servo s5;
    Servo s6;
    int s4p = 9;                //at PINS (9,10,11) 
    int s5p = 10;
    int s6p = 11;
    int s4U = 0;                //+ speed
    int s5U = 0;
    int s6U = 0;
    int s4R = 0;                //+ rotation
    int s5R = 0;
    int s6R = 0;
    int s4T = 0;                //+ time needed
    int s5T = 0;
    int s6T = 0;
    int s4Umicro = 0;
    int s5Umicro = 0;
    int s6Umicro = 0;
    
    int Umin = 0;
    int MaxRotation = 0;
    
    unsigned long ServoTimer = 0;
    
    
    //GUINO variables declare
    
    int Gm0 = 0;
    int Gm1 = 0;
    int Gp2 = 0;
    int Gp3 = 0;
    int Gc = 0;
    int Gs4U = 0;
    int Gs5U = 0;
    int Gs6U = 0;
    
    int Gs4R = 0;
    int Gs5R = 0;
    int Gs6R = 0;
    
    int GcRAW = 0;

    
   
    


void setup() 
{
//  Serial.begin(9600); 
  gBegin(34236); 
  
  
  resetMicValues(MicMinMaxArray, MicTimer);  //mic 
  
  PhotoCellCalibration(PhotocellTimer, LightLevelArray, LightMinMaxArray);  //photo
  resetPhotoValues(LightSum0, LightSum1, counter, PhotocellTimer);
  
  capSensor.set_CS_Timeout_Millis(CapSensorTimeout); //cap sensor
  
  s1.attach(s1p);  // set 180 servos
  s2.attach(s2p);
  s3.attach(s3p);
  
  s1.write(1);
  s2.write(1);
  s3.write(1);
  
  s4.attach(s4p);  // set 360 servos
  s5.attach(s5p);
  s6.attach(s6p);
  
}
  


void loop()
{
    guino_update();
    gSetColor(0,33,57);
  //LOOP FOR THE WHOLE SAMPLING (10 times)
    for (allcounter = 0; allcounter<TotalSamples; allcounter++)
    {
    
    
   
      // MIC READ *********************
          resetMicValues(MicMinMaxArray, MicTimer);
          
          //while millis has NOT reached the sampleSpan interval
          while (millis() - MicTimer < MicSampleSpan) 
          {    
          MicCurrentValueArray[0] = analogRead(A0);
          MicCurrentValueArray[1] = analogRead(A1);
          
          MicCurrentValueCheck( MicCurrentValueArray, MicMinMaxArray);
          }
            
          //after millis has reached the sampleSpan interval
          
           LimitMicValues(MicVolumesArray);
          
           for (int i=0; i<2; i++) 
           { MicVolumesArray[i] = MicMinMaxArray[i][1] - MicMinMaxArray[i][0]; }
           
              Gm0 = MicVolumesArray[0];      //GUINO mic values print
              Gm1 = MicVolumesArray[1];
              gUpdateValue(&Gm0);
              gUpdateValue(&Gm1);
              
            delay(1);    
            
            
          //adding volumes to the TOTAL MIC SAMPLES (range 200~800...(300~400 average loud))
          MicFinSamplesArray[0] = MicFinSamplesArray[0] + MicVolumesArray[0]; 
          MicFinSamplesArray[1] = MicFinSamplesArray[1] + MicVolumesArray[1]; 
          
      
      
      
      
      // PHOTOCELL READ ********************
          resetPhotoValues(LightSum0, LightSum1, counter, PhotocellTimer);
          
          //while millis has NOT reached the sampleSpan interval
          while (millis() - PhotocellTimer < PhotoSampleSpan) 
          {
           LightLevelArray[0] = analogRead(A2);
           LightLevelArray[1] = analogRead(A3);
           MappedLightLevelArray[0] = map(LightLevelArray[0],LightMinMaxArray[0][0],LightMinMaxArray[0][1],PhotoValueMIN,PhotoValueMAX);
           MappedLightLevelArray[1] = map(LightLevelArray[1],LightMinMaxArray[1][0],LightMinMaxArray[1][1],PhotoValueMIN,PhotoValueMAX);
           
           LightSum0 = LightSum0 + MappedLightLevelArray[0];
           LightSum1 = LightSum1 + MappedLightLevelArray[1];
           counter += 1;
           
          }
        
            //after millis has reached the sampleSpan interval
            LightSum0 = LightSum0/counter;
            LightSum1 = LightSum1/counter;
            
            
             Gp2 = LightSum0;       //GUINO photocell values print
             Gp3 = LightSum1;
             gUpdateValue(&Gp2);
             gUpdateValue(&Gp3);
             
            delay(1);
            
            
           //adding intensities to the TOTAL PHOTO SAMPLES (range 0~400)
            PhotoFinSamplesArray[0] = PhotoFinSamplesArray[0] + LightSum0; 
            PhotoFinSamplesArray[1] = PhotoFinSamplesArray[1] + LightSum1;
            

      
      
      // CAPACITIVE SENSOR READ *****************
          int CapValue = capSensor.capacitiveSensor(70);
          
          GcRAW = CapValue;
          gUpdateValue(&GcRAW);
          
          LimitCapValues(CapValue, TouchFlag);  //check to limit the extreme values
           
//           Gc = CapValue;
//           gUpdateValue(&Gc);

    
          //adding capacitive sensor value to the TOTAL CAP SAMPLES (range 30~500)
          CapFinSample = CapFinSample + CapValue;
          delay(1);
  
  
  }    
  
  //FINAL SAMPLES 
        CalcFinalSamples( MicFinSamplesArray, PhotoFinSamplesArray, CapFinSample, SmoothRate ); //calculate final samples with 0.5 smoothing filter
        
//        println(CapValue);
        
              

//  //SERVOS MOVE CALCULATION ******************************************************
         
        GetSensorsValues(m0, m1, p2, p3, c, MicFinSamplesArray , PhotoFinSamplesArray, CapFinSample); 
//        c = random(CapValueMIN, 100);
         Gc = c;
           gUpdateValue(&Gc);

        delay(1);
        
        
        //MAPPING  mic+photo values to each 360 servos' delay (165~1000ms)  /60 degrees
        //  S4(m1,p3) / S5(m0,p2)/ S6(m1,p2)
        //  for the 180 servos  s4=s2 / s5=s3 / s6 = s1
        s4U = MapSamplesToSpeed(m1,p3);
        s2U = s4U;
        s5U = MapSamplesToSpeed(m0,p2);
        s3U = s5U;
        s6U = MapSamplesToSpeed(m1,p2);
        s1U = s6U;
        
        
        Umin = s4U;    //find MAX current DELAY
        if (s5U < Umin) { Umin = s5U; }
        if (s6U < Umin) { Umin = s6U; }
        
        
        MaxRotation = map(c,CapValueMIN,CapValueMAX,0,MaxR); //map cap sense to max rotation
        
       
            //start mapping Cap value to each 360 servos' rotation (0~540 = 1.5 turns) using speed value 
            // the more the delay the lesser the rotation
        s4R = map(s4U,Umin,SDelayMAX,MaxRotation,0);     
        s5R = map(s5U,Umin,SDelayMAX,MaxRotation,0); 
        s6R = map(s6U,Umin,SDelayMAX,MaxRotation,0);
        
        if (s4R < 10){ s4R = 25;}
        if (s5R < 10){ s5R = 25;}
        if (s6R < 10){ s6R = 25;}
         
                                  //find servos 180 rotation 
          if (s4R > 360) {s2R = 180;}
          else {s2R = s4R/2;}
             
          if (s5R > 360) { s3R = 180; }
          else {s3R = s5R/2;}
  
          if (s6R > 360){s1R = 180; }
          else {s1R = s6R/2;} 
  
  
//          if (s1R<10){ s1R = 25;}   //make sure the rotation never ends up lower than 3..
//          if (s2R<10){ s2R = 25;}
//          if (s3R<10){ s3R = 25;}
        
          
         //map servos360 speed from ms/60 to microsec -------- 1500-2000 so to start turning counter clockwise first
//         s4Umicro = int((165/s4U)*500 + 1500);
          float s4Temp = ((float)165/(float)s4U)*500 +1500;
          float s5Temp = ((float)165/(float)s5U)*500 +1500;
          float s6Temp = ((float)165/(float)s6U)*500 +1500;
          s4Umicro = (int)s4Temp;
          s5Umicro = (int)s5Temp;
          s6Umicro = (int)s6Temp;
         
         
         //find time needed for the 360s to move at the current speed 
         s4T = int((s4R/60)*s4U);
         s5T = int((s5R/60)*s5U);
         s6T = int((s6R/60)*s6U);
         
         // time needed for the 180s to move
         s1T = int((s1R/60)*165);
         s2T = int((s2R/60)*165);
         s3T = int((s3R/60)*165);
                 
         // find the delays for each 1 degree loop for the 180s
         s1Delay = int((s6T)/s1R);
         s2Delay = int((s4T)/s2R);
         s3Delay = int((s5T)/s3R);
                  
         i1=i2=i3=1;  //zero the degree iterations
         i1count = i2count = i3count = 1;
             
             
         //start the 360 servos!!! ******************************************
         ServoTimer = millis();
         s4.writeMicroseconds(s4Umicro);
         s5.writeMicroseconds(s5Umicro);
         s6.writeMicroseconds(s6Umicro);
         
         
         //GUINO UPDATE S360 SPEEDS
         Gs4U = s4Umicro;
         Gs5U = s5Umicro;
         Gs6U = s6Umicro;
         gUpdateValue(&Gs4U);
         gUpdateValue(&Gs5U);
         gUpdateValue(&Gs6U);
         

         while (!((i1==0)&&(i2==0)&&(i3==0)))   //loop for the 180 to move with spesific delay to match the speed of the 360
         {
         
         if ((i1>0)&&(i1 <= s1R))      //servo 1
         {
           s1.write(i1); 
           gUpdateValue(&i1);  //GUINO UPDATE s180 SPEEDS
           if (i1==s1R)
           {
             i1count = - i1count;
             s6.writeMicroseconds(3000-s6Umicro); 
             Gs6U = 3000-s6Umicro;  //GUINO UPDATE S360 SPEEDS
              gUpdateValue(&Gs6U);             
           }
           i1 = i1 + i1count;
           delay(s1Delay);
           }
          
         if ((i2>0)&&(i2 <= s2R))      //servo2
         {
           s2.write(i2); 
           gUpdateValue(&i2);   //GUINO UPDATE s180 SPEEDS
           if (i2==s2R)
           {
             i2count = - i2count;
             s4.writeMicroseconds(3000-s4Umicro);
              Gs4U = 3000-s4Umicro;  //GUINO UPDATE S360 SPEEDS
              gUpdateValue(&Gs4U);               
           }
           i2 = i2 + i2count;
           delay(s2Delay);
           }
           
           
         if ((i3>0)&&(i3 <= s3R))      //servo3
         {
           s3.write(i3); 
           gUpdateValue(&i3);  //GUINO UPDATE s180 SPEEDS
           if (i3==s3R)
           {
             i3count = - i3count;
             s5.writeMicroseconds(3000-s5Umicro);
             Gs5U = 3000-s5Umicro;  //GUINO UPDATE S360 SPEEDS
              gUpdateValue(&Gs5U);             
           }
           i3 = i3 + i3count;
           delay(s3Delay);          
           }
                    
         }  //end of while
         
         
         // stop the 360 servos
         s4.writeMicroseconds(1500);
         s5.writeMicroseconds(1500);
         s6.writeMicroseconds(1500);
         
         Gs4U = 1500;
         Gs5U = 1500;
         Gs6U = 1500;
         gUpdateValue(&Gs4U);
         gUpdateValue(&Gs5U);
         gUpdateValue(&Gs6U);
      
      
        ServoTimer = millis()-ServoTimer;
        
       //clearing the total samples
      SaveTotalSamplesAndClear(OLDMicFinSamplesArray, MicFinSamplesArray, PhotoFinSamplesArray, OLDCapFinSample, CapFinSample, TouchFlag);


}
    




//FUNCTIONS ********************

//GENERAL FUNCTIONS ***************
    
    void SaveTotalSamplesAndClear(int OLDMicFinSamplesArray[2], int MicFinSamplesArray[2], int PhotoFinSamplesArray[2], int &OLDCapFinSample, int &CapFinSample, int &TouchFlag)
    {
     for (int i=0; i<2; i++)
     { 
      OLDMicFinSamplesArray[i] = MicFinSamplesArray[i]; 
      MicFinSamplesArray[i] = 150;
      OLDPhotoFinSamplesArray[i] = PhotoFinSamplesArray[i]; 
      PhotoFinSamplesArray[i] = 0;
     }
      OLDCapFinSample = CapFinSample;
      CapFinSample = 0;
      TouchFlag = 0;
    }
    
    void CalcFinalSamples(int MicFinSamplesArray[2], int PhotoFinSamplesArray[2], int &CapFinSample, float SmoothRate)
    {
      for (int i=0; i<2; i++)
     { 
      MicFinSamplesArray[i] = (MicFinSamplesArray[i]/TotalSamples)* SmoothRate + OLDMicFinSamplesArray[i]* (1-SmoothRate);
      PhotoFinSamplesArray[i] = (PhotoFinSamplesArray[i]/TotalSamples)* SmoothRate + OLDPhotoFinSamplesArray[i]* (1-SmoothRate);
     }
     CapFinSample = (CapFinSample/TotalSamples)* SmoothRate + OLDCapFinSample* (1-SmoothRate);
    }


//MIC FUNCTIONS **************

    void resetMicValues(int MicMinMaxArray[2][2],unsigned long &MicTimer)
    {
      for (int i = 0; i < 2; i +=1)
      {
      MicMinMaxArray[i][0] = 1024;
      MicMinMaxArray[i][1] = 0;
      }
      MicTimer = millis(); 
    }
  
    void MicCurrentValueCheck(int MicCurrentValueArray[2],int MicMinMaxArray[2][2] ){
      for (int i = 0; i < 2; i +=1)
      {
      if (MicCurrentValueArray[i] < MicMinMaxArray[i][0]) {
          MicMinMaxArray[i][0] = MicCurrentValueArray[i];
      } 
      if (MicCurrentValueArray[i] > MicMinMaxArray[i][1]) {
          MicMinMaxArray[i][1] = MicCurrentValueArray[i];
      } 
      }
    }

    void LimitMicValues(int MicVolumesArray[2]) {
      for (int i = 0; i < 2; i +=1)
      {
        if (MicVolumesArray[i] < MicValueMIN){
          MicVolumesArray[i] = MicValueMIN;
        }
        if (MicVolumesArray[i] > MicValueMAX){
          MicVolumesArray[i] = MicValueMAX;
        }
      }
    }


//PHOTOCELL FUNCTIONS *************************   

    void PhotoCellCalibration(unsigned long &PhotocellTimer,int LightLevelArray[2] , int LightMinMaxArray[2][2])
    {
    PhotocellTimer = millis();
      while (millis() - PhotocellTimer < PhotocellCalibrationTime)
      {
        {
          LightLevelArray[0] =analogRead(A2);
          LightLevelArray[1] =analogRead(A3);
        
          for (int i=0; i<2; i+=1)  //LightMinMaxArray[i][x] x is 0 for MIN 1 for MAX
         { 
           if(LightMinMaxArray[i][0] > LightLevelArray[i]){
           LightMinMaxArray[i][0] = LightLevelArray[i];
           }
           if(LightMinMaxArray[i][1] < LightLevelArray[i]){
           LightMinMaxArray[i][1] = LightLevelArray[i];
           }
         }
        }
      }
    }
       
      void resetPhotoValues(int &LightSum0, int &LightSum1, int &counter,long unsigned int &PhotocellTimer )
    {
      LightSum0 = LightSum1 = counter = 0;
      PhotocellTimer = millis();
    }
 
 
 //CAPACITIVE FUNCTIONS *************************
 
      void  LimitCapValues(int &CapValue, int &TouchFlag)
      {
        if ((CapValue == -2)||(CapValue == -1)) {
          TouchFlag = 1;
          CapValue = CapValueMAX;
        }
        if (CapValue > TouchLimit) {
          TouchFlag = 1;
        }
        if (CapValue > CapValueMAX) {
          CapValue = CapValueMAX;
        }
         if (CapValue < CapValueMIN) {
          CapValue = CapValueMIN + 10;   // +10 to maintain at least a small movement... (40+10=90)
        }
      }
  
 //SERVOS FUNCTIONS *************************
 
      void GetSensorsValues(int &m0,int &m1, int &p2, int &p3, int &c, int MicFinSamplesArray[2] , int PhotoFinSamplesArray[2], int &CapFinSample)
      { 
        m0 = MicFinSamplesArray[0];
        m1 = MicFinSamplesArray[1];
        p2 = PhotoFinSamplesArray[0];
        if (p2<0) {p2 = -p2;}  // in some cases turned below zero
        p3 = PhotoFinSamplesArray[1];
        if (p3<0) {p3 = -p3;}  // in some cases turned below zero
        c =  CapFinSample;
      } 
      
      int MapSamplesToSpeed(int MicValue, int PhotoValue)
      {
        int MapMicValue = map(MicValue,MicValueMIN,MicValueMAX,SDelayMAX,SDelayMIN);   //from 100~800 to 1000~165..INVERTED analogy
        int MapPhotoValue = map(PhotoValue,PhotoValueMIN,PhotoValueMAX,SDelayMAX,SDelayMIN);
        int resultU =  int(MapMicValue*0.5 + MapPhotoValue*0.5) ; 
        return resultU; 
      }   
      
 // GUINO FUNCTIONS ********************************
 
       void gInit()
      {

        gAddLabel("Sound samples",1);
        gAddSpacer(1);
      
        
        gAddMovingGraph("microphone 1",MicValueMIN,MicValueMAX, &Gm0, 10);
        gAddSlider(MicValueMIN,MicValueMAX,"microphone 1", &Gm0);
        
        gAddMovingGraph("microphone 2",MicValueMIN,MicValueMAX, &Gm1, 10);
        gAddSlider(MicValueMIN,MicValueMAX,"microphone 2", &Gm1);
        
        gAddColumn();
         
        gAddLabel("Light samples",1);
        gAddSpacer(1);
        
        gAddMovingGraph("photocell 1",PhotoValueMIN,PhotoValueMAX, &Gp2, 10);
        gAddSlider(PhotoValueMIN,PhotoValueMAX,"photocell 1", &Gp2);
        
        gAddMovingGraph("photocell 2",PhotoValueMIN,PhotoValueMAX, &Gp3, 10);
        gAddSlider(PhotoValueMIN,PhotoValueMAX,"photocell 2", &Gp3);
        
        
        
        gAddColumn();
         
        gAddLabel("Capacitive samples",1);
        gAddSpacer(1);
        
        gAddMovingGraph("capacitive",CapValueMIN,CapValueMAX, &Gc, 10);
        gAddSlider(CapValueMIN,CapValueMAX,"capacitive", &Gc);
        
        gAddSlider(CapValueMIN,CapValueMAX,"capacitive RAW", &GcRAW);
        
        gAddColumn();
        
        gAddLabel("Servos 180",1);
        gAddRotarySlider(0,180,"1/",&i1);
        gAddRotarySlider(0,180,"2/",&i2);
        gAddRotarySlider(0,180,"3/",&i3);
        
        gAddSpacer(1);
        gAddLabel("Servos 360",1);
        
        gAddRotarySlider(1000,2000,"4/",&Gs4U);
        gAddRotarySlider(1000,2000,"5/",&Gs5U);
        gAddRotarySlider(1000,2000,"6/",&Gs6U);      
      }
      
      void gButtonPressed(int id)
      { }
      
      void gItemUpdated(int id)
      { }
