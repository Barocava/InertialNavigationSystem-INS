void AnglesV2(int Width, int Height,float MaxAngle,float MinAngle){
  textFont(myFont, 28.125e-3*Width);
  dataLinesScreenSize(Width,Height);
  kYaw.getData(K_RPY[2]);                                          //Extraer los datos para dibujarlos.
  kPitch.getData(K_RPY[1]);                                        //
  kRoll.getData(K_RPY[0]);                                         //
  text("Angulos RPY (45° grados por div)",Width/4,margin);
  infoTextAngle(Width,Height,MinAngle,MaxAngle);
  axis(Height,Width);                                              // Dibujar eje
  kYaw.drawData();                                                 // Dibujar gráfica Yaw
  kPitch.drawData();                                               // Dibujar gráfica Pitch
  kRoll.drawData();                                                // Dibujar gráfica Roll
  
  if(kRoll.timeOut){                                               // Fin de pantalla, reiniciar base de tiempo (Se usó el contador de Roll pero es igual para los tres).
    background(0);
    kRoll.timeOut=false;
  }
} 

void infoTextAngle(int Width, int Height, float MinAngle, float MaxAngle){  // función para dibujar el texto informatico en pantalla.
  float y;
  stroke(255);
  fill(0);
  rect(Width-Width*(156.25e-3),(23.44e-3)*Width,(140.625e-3)*Width,(93.75e-3)*Width);
  fill(255,0,0);
  text("Roll",Width-(125e-3*Width),(46.88e-3)*Width);              // SE TOMARON VALORES EMPÍRICOS PARA VARIAR EL TAMAÑO DEL PLOTTER SIN DESCUADRAR LOS RECUADROS (RELACION LINEAL)
  fill(100,200,255);
  text("Pitch",Width-(125e-3*Width),(78.125e-3)*Width);
  fill(0,255,0);
  text("Yaw",Width-(125e-3*Width),(109.38e-3)*Width);
  if(mouseX<Width && mouseY<Height){
  textFont(myFont, 17e-3*Width);
  stroke(255);
  fill(0);
  rect(Width-Width*(200e-3),Height-(90e-3)*Height,(170e-3)*Width,63.34e-3*Height);
  fill(255);
  y= map(mouseY,Height-margin,margin,MinAngle,MaxAngle);
  text("y="+y/10+"°",Width-(190e-3*Width),Height-50e-3*Height);
  }
  fill(255);
}

void axis(int Height,int Width){                                                                   // Función para dibujar las ejes principales de las gráficas y sus líneas de división.
  int lineNumber=40;
  int lineSpace=(Height-2*margin)/lineNumber;
  int i;
  int lineWidth=6;
  stroke(255);
  line(margin/2,Height-margin/2,margin/2,margin/2);                                                //Eje vertical del gráfico
  stroke(255);
  line(0,Height/2,Width-margin/2,Height/2);                                                        // Eje Horizontal del gráfico
  for(i=1;i<=lineNumber/2;i++){
    if(i%5==0){
      strokeWeight(2);
      line(margin/2-lineWidth,Height/2+i*lineSpace,margin/2+lineWidth,Height/2+i*lineSpace);
      line(margin/2-lineWidth,Height/2-i*lineSpace,margin/2+lineWidth,Height/2-i*lineSpace);       //Líneas grandes
    }
    strokeWeight(1);
    line(margin/2-lineWidth/2,Height/2+i*lineSpace,margin/2+lineWidth/2,Height/2+i*lineSpace);
    line(margin/2-lineWidth/2,Height/2-i*lineSpace,margin/2+lineWidth/2,Height/2-i*lineSpace);    // Líneas pequeñas
  } 
}