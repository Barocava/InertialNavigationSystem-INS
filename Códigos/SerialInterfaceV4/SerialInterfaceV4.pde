import processing.serial.*; 
import java.io.*;

/*
*****************************************************************
**                   SERIAL INTERFACE V4                       **
**                                                             **
** Programa para evaluar gráficamente el desempeño del INS.    **
** Recibe los datos por comunicación serial y la muestra       **
** en pantalla con plotter 2D o un módelo de 3D.               **
**                                                             **
** AUTORES: Bruno Barone y Jhonny Rodríguez.                   **
** GID Mecatrónica de la universidad Simón Bolívar .           **
** Agosto, 2016                                                **        
** Venezuela.                                                  **
*****************************************************************
*/




final  byte sizeOfBuffer = 20;                           //Tamaño de la trama a recibir por serial (Sin cabecera).
final  int  margin  = 40;                                //Margen utilizado para dibujar la interfaz. Multiplo de cuarenta para que no se descuadre.
final color red= color(255, 0, 0);
final color blue= color(100,200, 255);
final color green= color(0,255,0);
final color white= color(255);
final color yellow= color(255,255,0);
final color khaki= color(240,230,140);


Serial myPort;                                          // Objeto vinculado al puerto serial.
PFont myFont;                                           // Fuente utilizada.
byte[] dataBuffer = new byte[sizeOfBuffer];             // Arreglo de datos utilizado para recibir por serial.
short[] K_RPY= new short[3];                            // Arreglo de datos que almacena los ángulos filtrados por FK.
float dataMin=-1800;                                    // Cota mínima de grados a mostrar en la gráfica (180*10 recordar que los datos vienen multiplicados por 10).
float dataMax=-dataMin;                                 // Cota máxima.
float factor= width/10;
byte[] latitud = new byte[4];                           // Arreglo de datos para almacenar la latitud.
byte[] longitud = new byte[4];                          // Arreglo de datos para almacenar la longitud.
byte[] altitud = new byte[4];                           // Arreglo de datos para almacenar la altitud.
dataLine kRoll=   new dataLine(red,dataMin,dataMax);    // Inicialización de los objetos de línea de gráficas a dibujar (Ver pestaña DataLine)
dataLine kPitch=  new dataLine(blue,dataMin,dataMax);   //
dataLine kYaw=    new dataLine(green,dataMin,dataMax);  //
PrintWriter writer;                                     // Objeto escritor de archivos. (Usado para el GEOLOCATOR)
int xPos=margin;                                        // Base de tiempo del plotter.
char command=' ';
boolean commandFlag=false;                              

void setup() { 
  //fullScreen();
  size(640,480,P3D);
  myFont = loadFont("CourierNewPS-BoldMT-48.vlw");  
  // List all the available serial ports: 
  printArray(Serial.list()); 
  myPort = new Serial(this, Serial.list()[0], 57600); 
  myPort.buffer(sizeOfBuffer+1);                       // Con esto se configura "serial event" a activarse cada 21 bytes de recepción (20 datos + 1 cabecera)
  println(width,height);
  smooth();
  background(0);
} 

void draw() { 
  float lat=0; 
  float lon=0;
  float alt=0;
  
  // AnglesV2(width,height,dataMin,dataMax);            Función para gráficar los ángulos de rotación en función del tiempo.
  //drawCar(K_RPY[0],K_RPY[1],K_RPY[2]);                Función para mostrar por pantalla un modelo 3D giratorio de la plataforma. 
  
  lat=BYTE_TO_FLOAT(latitud);
  lon=BYTE_TO_FLOAT(longitud);
  alt=BYTE_TO_FLOAT(altitud);
  
   writer = createWriter("data.js");                    //Creación del archivo usado por el GEOLOCATOR.
   writer.print("Pos = ["+lat+","+lon+","+alt+"];");
   writer.close();
   //printKalman();
}
 
void serialEvent(Serial myPort) {                       // Evento de recepción de datos.
  byte i;
  if(myPort.read()=='('){
    for(i=0;i<sizeOfBuffer;i++){
      byte dataReading= (byte)myPort.read();            
      dataBuffer[i]=dataReading;
      ExtractData();
    }
  }
  if(commandFlag){                                      // Si se ha introducido un comando a la plataforma, enviarlo.
        myPort.write(command);
        commandFlag=false;
    }
} 

void keyPressed(){                                      // Evento de entrada por teclado.
  command=key;
  commandFlag=true;
}

short dataUnion(byte MSB, byte LSB){                    // Función para unir dos bytes en un "short".
  short Out=0;
  Out= MSB;
  Out<<=8;
  Out|=LSB;
  return Out;
}

void ExtractData(){                                     //Rutina para extraer del buffer de recepción los datos de la trama.
  int factor=1;
  K_RPY[0]= dataUnion(dataBuffer[1],dataBuffer[0]);
  K_RPY[0]/=factor;
  K_RPY[1]= dataUnion(dataBuffer[3],dataBuffer[2]);
  K_RPY[1]/=factor;
  K_RPY[2]= dataUnion(dataBuffer[5],dataBuffer[4]);
  K_RPY[2]/=factor;
  latitud[0]=dataBuffer[6];
  latitud[1]=dataBuffer[7];
  latitud[2]=dataBuffer[8];
  latitud[3]=dataBuffer[9];
  longitud[0]=dataBuffer[10];
  longitud[1]=dataBuffer[11];
  longitud[2]=dataBuffer[12];
  longitud[3]=dataBuffer[13];
  altitud[0]=dataBuffer[14];
  altitud[1]=dataBuffer[15];
  altitud[2]=dataBuffer[16];
  altitud[3]=dataBuffer[17];
}

float[] getKalman(){                                   //Rutina que extrae del arreglo de datos del filtro de Kalman los ángulos RPY.
  byte i;
  float[] angle=new float[3];
  for(i=0;i<3;i++){
     angle[i]=K_RPY[i]; 
  }
  return angle;
}

void printKalman(){                                    // Imprimir resultados del FK.
  float[] angles=getKalman();
  println("Ángulos de Euler");
  println("");
  printArray(angles);
  println("");
}

void dataLinesScreenSize(int Width,int Height){        // Rutina para configurar el espacio de dibujo de las gráficas 2D.
  kYaw.screenWidth=Width;
  kYaw.screenHeight=Height;
  kPitch.screenWidth=Width;
  kPitch.screenHeight=Height;
  kRoll.screenWidth=Width;
  kRoll.screenHeight=Height;
}