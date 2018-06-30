FloatList x;
FloatList y;
FloatList dens;
ArrayList<Place> places;
Place[][] pm;
float minY, maxY;
float minX, maxX;
float minD, maxD;
int width = 800;
int height = 800;
color black = color(0);
PFont labelFont;
float density, population;
String name;
int codePostal;

void setup(){
    size(800, 800);
    labelFont = loadFont("ArialMT-48.vlw");
    textFont(labelFont, 21);    
        ellipseMode(CENTER);
        noStroke();
    readData();
}

void draw(){
    background(255);
   
    for(Place place: places){
      place.draw();
    }
          text("Name:" + name 
    + " \t\t code postal:" + codePostal, 20,780); 
}

void readData(){
  String[] lines = loadStrings
    ("https://perso.telecom-paristech.fr/eagan/class/igr204/data/population.tsv");
  println("Loaded " + lines.length + " lines.");

  x = new FloatList();
  y = new FloatList();
  dens = new FloatList();
  places = new ArrayList<Place>();
  pm = new Place[801][801];
  
  for(int i=2; i<lines.length; i++){
    String[] columns = lines[i].split("\t");
    if(i==lines.length-1) println(lines[i]);
    if( columns[1].equals("NaN") || columns[2].equals("Nan")) continue; 
    Place place = new Place();
    place.postalCode = Integer.valueOf(columns[0]);
    place.longitude = Float.valueOf(columns[1]);
    place.latitude = float(columns[2]);
    place.name = columns[4];
    place.population = int(columns[5]);
    place.density = float(columns[6]);
    x.append(place.longitude);
    y.append(place.latitude);
    dens.append(place.density);
    places.add(place);
  }
  
  minX = x.min();
  maxX = x.max();
  minY = y.min();
  maxY = y.max();
  minD = dens.min();
  maxD = dens.max();
  
  for(Place place: places){
    pm[place.x()][place.y()] = place;

  }
}

void mouseMoved(){
  Place place = pm[mouseX][mouseY];
  if(place ==null) return;
  name = place.name;
  codePostal = place.postalCode;
}
  
