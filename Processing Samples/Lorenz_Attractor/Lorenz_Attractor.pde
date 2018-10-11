// Library for camera
import peasy.*;

// Lorenz Attractor Object
public class LorenzSystem
{
  PVector pos;
  PVector coef;
  
  ArrayList<PVector> points = new ArrayList<PVector>();
  
  int rgb;
  float opacity;
  
  LorenzSystem(PVector pos, PVector coef, int rgb, float opacity)
  {
    this.pos = pos;
    this.coef = coef;
    this.rgb = rgb;
    this.opacity = opacity;
  }
}

// Differentials
float dx, dy, dz;
float dt = 0.02;

// Lorenz Attractor List
ArrayList<LorenzSystem> system_list = new ArrayList<LorenzSystem>();

PeasyCam cam;

void setup()
{
  fullScreen(P3D);
  cam = new PeasyCam(this, 400);
   
  // A Red Lorenz System
  system_list.add(new LorenzSystem (new PVector(0.1, 0, 0), new PVector(10, 28, 8.0 / 3.0), #FF0000, 75));
  // A White Lorenz System
  system_list.add(new LorenzSystem (new PVector(0.1, 10, 0), new PVector(10, 28, 8.0 / 3.0), #FFFFFF, 125));
}

void draw()
{
  // Setup
  background(0, 0, 0);
  translate(0, 0, 0);
  noFill();
  scale(6);
  strokeWeight(0.2);

  // Render
  for (LorenzSystem system : system_list)
  {
    // Math https://en.wikipedia.org/wiki/Lorenz_system
    dx = (system.coef.x * (system.pos.y - system.pos.x)) * dt;
    dy = (system.pos.x * (system.coef.y - system.pos.z) -system.pos.y) * dt;
    dz = (system.pos.x * system.pos.y - system.coef.z * system.pos.z) * dt;
    
    system.pos.x += dx;
    system.pos.y += dy;
    system.pos.z += dz; 
    system.points.add(new PVector(system.pos.x, system.pos.y, system.pos.z));
  
    // Draw shape
    beginShape();
    for(PVector v: system.points)
    {
      stroke(system.rgb, system.opacity);
      vertex(v.x, v.y, v.z);
    }
    endShape();
  }
}
