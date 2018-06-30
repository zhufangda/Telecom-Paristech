class Place{
  float longitude;
  float latitude;
  String name;
  int postalCode;
  float population;
  float density;
  
  void draw(){
    fill(0,0,255, gray());
    set(x(), y(), 255);
    ellipse(x(), y(), r(),r());
  }
  
  int x(){
     return int(map(this.longitude, minX, maxX, 0, 800));
  }
  
  int y(){
    return int(map(this.latitude, minY, maxY, 800, 0));
  }
  
  int r(){
    float surface = density * population * 0.35;
    return int( 0.00048 * sqrt(surface));
  }
  
  
  int gray(){
    return int(map(density, minD, maxD, 100, 150)); 
  }
}
