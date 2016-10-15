void drawCylinder(int nPoints,float rad, float centerX, float CenterY, float centerZ){ // Función para dibujar cilindros negros (ruedas)
float i=0;
float res= 2*PI/nPoints;
float radius=rad;
float z=factor;
translate(centerX,CenterY,centerZ);
fill(0);  
  beginShape();
     for(i=0;i<2*PI;i+=res){
        vertex(radius*cos(i),radius*sin(i),0);
        vertex(radius*cos(i),radius*sin(i),z);
       }
  endShape();
}