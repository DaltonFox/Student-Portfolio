class Leaf 
{
  PVector pos;
  boolean reached = false;
  boolean glow = false;

  Leaf(float seed) 
  {
    pos = PVector.random3D();
    pos.mult(random(width / random(2, seed * 2)));
    pos.y -= height / seed;
  }

  void show()
  {
    if (glow)
    {
      //stroke(255, 0);
      
      pushMatrix();
      float[] rota = cam.getRotations();
      translate(pos.x, pos.y, pos.z);
      rotateX(rota[0]);
      rotateY(rota[1]);
      rotateZ(rota[2]);
      fill(#FF0000, 55);
      ellipse(0, 0, 50, 50);
      popMatrix();
    }
  }
  void reached() 
  {
    reached = true;
  }
}