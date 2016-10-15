void drawCar(float roll,float pitch,float yaw){               // Función para dibujar el modelo 3D de la plataforma completo y hacerlo girar según los ángulos RPY que recibe como parámetros.
  PShape chasis;
  background(126);
  translate(width/2,height/2);
  pushMatrix();
  chasis=drawChasis(0,0,factor);
  popMatrix();
  
  rotateY(radians(-yaw/10));
  rotateZ(radians(pitch/10));
  rotateX(radians(roll/10));

  
  rotateX(-PI/2);
  shape(chasis);
  rotateX(PI/2);
  pushMatrix();
  drawCylinder(1000,3.5*factor,-7*factor,factor,9*factor);
  popMatrix();
  pushMatrix();
  drawCylinder(1000,3.5*factor,-7*factor,factor,-10*factor);
  popMatrix();
  pushMatrix();
  drawCylinder(1000,1.5*factor,13.5*factor,3*factor,-.5*factor);
  popMatrix();
}