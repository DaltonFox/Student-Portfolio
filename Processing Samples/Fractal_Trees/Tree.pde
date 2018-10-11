class Tree 
{
  ArrayList<Branch> branches = new ArrayList<Branch>();
  ArrayList<Leaf> leaves = new ArrayList<Leaf>();
  float seed = random(2, 4);
  float rmax = 0;
  Tree() 
  {
    for (int i = 0; i < random(250, 1500); i++) 
    {
      Leaf lf = new Leaf(seed);
      if (abs(lf.pos.x) > abs(lf.pos.z))
      {         if (abs(lf.pos.x) > rmax)
          {
            rmax = abs(lf.pos.x);
          }
      }
      else
      {
        if (abs(lf.pos.z) > rmax)
        {
          rmax = abs(lf.pos.z);
        }
      }
      leaves.add(lf);
    }    
    Branch root = new Branch(new PVector(0, height / 2), new PVector(0, -1));
    branches.add(root);
    Branch current = new Branch(root);

    while (!closeEnough(current)) 
    {
      Branch trunk = new Branch(current);
      branches.add(trunk);
      current = trunk;
    }
  }

  boolean closeEnough(Branch b) 
  {

    for (Leaf l : leaves) 
    {
      float d = PVector.dist(b.pos, l.pos);
      if (d < max_dist) 
      {
        return true;
      }
    }
    return false;
  }

  void grow() 
  {
    for (Leaf l : leaves) 
    {
      Branch closest = null;
      PVector closestDir = null;
      float record = -1;

      for (Branch b : branches) 
      {
        PVector dir = PVector.sub(l.pos, b.pos);
        float d = dir.mag();
        if (d < min_dist) 
        {
          l.reached();
          closest = null;
          break;
        } else if (d > max_dist) 
        {
        } else if (closest == null || d < record) 
        {
          closest = b;
          closestDir = dir;
          record = d;
        }
      }
      if (closest != null) 
      {
        closestDir.normalize();
        closest.dir.add(closestDir);
        closest.count++;
      }
    }

    for (int i = leaves.size() - 1; i >= 0; i--) 
    {
      if (leaves.get(i).reached) 
      {
        if (leaves.get(i).pos.y > -height)
        {
          leaves.get(i).glow = true;
        }
        else
        {
          leaves.remove(i);
        }
      }
    }

    for (int i = branches.size() - 1; i >= 0; i--) 
    {
      Branch b = branches.get(i);
      if (b.count > 0) 
      {
        b.dir.div(b.count);
        PVector rand = PVector.random2D();
        rand.setMag(0.3);
        b.dir.add(rand);
        b.dir.normalize();
        Branch newB = new Branch(b);
        branches.add(newB);
        b.reset();
      }
    }
  }

  void show() 
  {
    for (int i = 0; i < leaves.size(); i++)
    {
      //leaves.get(i).show();
    }
    for (int i = 0; i < branches.size(); i++) 
    {
      Branch b = branches.get(i);
      if (b.parent != null) 
      {
        float sw = map(i, 0, branches.size(), seed * 1.45, 0);
        if (sw < 0.55){ sw = 0.55; }
        float op = map(i, 0, branches.size(), 500, 50);
        if (op > 255){ op = 255 ; }
        strokeWeight(sw);
        stroke(255, op);
        line(b.pos.x, b.pos.y, b.pos.z, b.parent.pos.x, b.parent.pos.y, b.parent.pos.z);
      }
    }
  }
}
