import peasy.*;

Tree tree;

PeasyCam cam;

float min_dist = 5;
float max_dist = 200;

void setup() 
{
  fullScreen(P3D);
  cam = new PeasyCam(this, 500);
  tree = new Tree();
}

void keyReleased()
{
  if (key == 'r' || key == 'R') 
  {
      tree = new Tree();
  }
}

void draw() 
{
  background(0, 0, 0);

  stroke(50);
  strokeWeight(1);
  float h = height * 1.25;
  for (int i = 0; i < (h / 20) + 1; i++)
  {
    line(-h, height / 2, -h + (40 * i), h, height / 2, -h + (40 * i));
    line(-h + (40 * i), height / 2, -h, -h + (40 * i), height / 2, h);
  }
  translate(0, height/2);
  rotateX(radians(90));
  stroke(#FFFFFF, 125);
  noFill();
  strokeWeight(2);
  ellipse(0, 0, tree.rmax * 2, tree.rmax * 2);
  rotateX(-radians(90));
  translate(0, -height/2);
  tree.show();
  tree.grow();
}