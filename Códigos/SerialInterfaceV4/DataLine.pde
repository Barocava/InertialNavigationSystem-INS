public class dataLine{                  // Definición de la clase dataLine 
    
  private float dataBuffer=0;
  private float dataMin=0;
  private float dataMax=0;
  public int screenWidth=0;
  public int screenHeight=0;
  private float[] sampleBuffer= new float[2];
  private color dataColor=color(255,255,255);
  public int margin=40;
  public int timeBase=margin;
  public boolean timeOut=false;

    
  public  dataLine(color dataColor,int screenWidth ,int screenHeight,float dataMin, float dataMax){ 
    this.dataColor = dataColor;
    this.screenWidth = screenWidth;
    this.screenHeight = screenHeight;
    this.dataMin = dataMin;
    this.dataMax = dataMax;
    sampleBuffer[0]=height;   
  }
  
   public  dataLine(color dataColor,float dataMin, float dataMax){
    this.dataColor = dataColor;
    this.dataMin = dataMin;
    this.dataMax = dataMax;
    sampleBuffer[0]=height;   
  }
  
  public void getData(float data){
    dataBuffer=data;
    sampleBuffer[1]= map(dataBuffer,dataMin,dataMax,margin,screenHeight-margin);
  }
  
  public void drawData(){
    stroke(dataColor);
    strokeWeight(1);
    line(timeBase,screenHeight-sampleBuffer[0],timeBase+1,screenHeight-sampleBuffer[1]);
    sampleBuffer[0]=sampleBuffer[1];
    if(timeBase<screenWidth-margin/2){
      timeBase++;
    }
    else{
      timeOut=true;
      resetTime();
    }
  }
  
  public void resetTime(){
    timeBase=margin;
  } 
}