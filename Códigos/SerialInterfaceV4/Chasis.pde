PShape drawChasis(int centerX, int CenterY,float factor){ // Función para dibujar el chasis del modelo 3D
  translate(centerX,CenterY);
  float und= factor;
  float thick=factor;
  PShape chasis= createShape(GROUP);
  PShape s;
  PShape p;
  
  fill(yellow);
  s=createShape();
  s.beginShape();
  s.vertex(-11*und,-8*und,0);
  s.vertex(-3*und,-8*und,0);
  s.vertex(-3*und,-10*und,0);
  s.vertex(11*und,-10*und,0);
  s.vertex(11*und,10*und,0);
  s.vertex(-3*und,10*und,0);
  s.vertex(-3*und,8*und,0);
  s.vertex(-11*und,8*und,0);
  s.endShape(CLOSE);

  fill(khaki);
  p=createShape();
  p.beginShape();
  p.vertex(10*und,-2*und,thick);
  p.vertex(17*und,-2*und,thick);
  p.vertex(17*und,2*und,thick);
  p.vertex(10*und,2*und,thick);
  p.endShape(CLOSE);
  
  chasis.addChild(p);
  chasis.addChild(s);
  return chasis;
}