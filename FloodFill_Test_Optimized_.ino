// minimum distnce to center from a point
#define min_dist(i,j) min(min(abs(i-center)+abs(j-center),abs(i-center+1)+abs(j-center)),min(abs(i-center)+abs(j-center+1),abs(i-center+1)+abs(j-center+1))); 

class Node {
public : 
  byte walls;
  short cost;  
};

//Search
boolean TO_CENTER = true;
boolean TO_START = false;

//Map
const byte mapSize = 16;
byte center = mapSize/2;
Node maze[mapSize][mapSize];
const byte discoWalls[mapSize][mapSize] = {
    11,11,11,9,5,5,5,5,5,5,5,5,5,5,5,3,
    10,10,8,2,9,5,5,5,5,5,5,5,5,5,5,2,
    12,0,2,10,10,13,5,5,3,9,5,5,5,5,3,10,
    9,6,10,10,10,9,5,5,0,4,9,5,5,5,2,10,
    8,5,6,10,10,8,5,1,4,3,12,5,5,3,10,10,
    8,5,5,6,10,10,9,4,7,14,9,5,5,6,10,10,
    10,9,5,5,6,8,0,7,9,3,12,5,5,3,10,10,
    10,8,5,5,5,6,14,9,2,8,5,5,5,6,14,14,
    10,10,9,5,5,3,11,9,6,8,1,5,1,5,1,3,
    10,12,4,5,3,10,8,5,5,6,12,1,0,5,2,10,
    8,5,5,3,10,10,12,5,5,5,5,6,12,5,2,10,
    8,5,3,10,10,10,9,5,5,5,5,5,5,5,4,2,
    9,3,10,10,10,10,12,5,5,5,5,5,5,5,3,10,
    9,0,2,10,10,12,5,5,5,5,5,5,5,5,6,14,
    10,10,8,2,12,5,5,5,5,5,5,5,5,5,5,3,
    14,14,14,12,5,5,5,5,5,5,5,5,5,5,5,6};
//  11, 11, 11, 13,  1,  3,
//  8,  0,   4,  5,  6, 10,
//  10, 14,  9,  3, 11, 10,
//  10, 11, 12,  2,  8,  2,
//  8,  4,  3, 12,  6, 10,
//  14, 13,  4,  5,  5,  6,};

//Distance
unsigned long timeDiff=0, curTime=0, progTime=0;
int motorSpeed=50, tempDist=0;
int botDistX=0, botDistY=300;
int curNodeX=0, curNodeY=0;

//Directions
enum Direction {
  NORTH=0, EAST, SOUTH, WEST};
Direction direct=NORTH;

//Constants
const short tileSize = 20;
const short timeDelay = 50;

void setup()  { 
  for(int i=0; i<mapSize; i++) {
    for(int j=0; j<mapSize; j++){
      maze[i][j].cost = min_dist(i,j);
      maze[i][j].walls = 0;
    }   
  }
  Serial.begin(9600);
  
  //Debugging - Initial Settings
//  Serial.println("Current Direction: " + String(direct));
//  Serial.println("Distance traveled: " + String(botDistX) + "," + String(botDistY));
//  Serial.println("Current Node: " + String(curNodeX) + "," + String(curNodeY));
//  Serial.println("Wall information: " + String(maze[curNodeY][curNodeX].walls));
//  for(int i=0; i<mapSize; i++) {
//    for(int j=0; j<mapSize; j++) {
//      if(i==curNodeY && j==curNodeX) {
//        Serial.print("X  ");
//      }
//      else
//        Serial.print(String(maze[i][j].cost) + "  ");
//    }
//    Serial.println();
//  }
//  Serial.println();
//  for(int i=0; i<mapSize; i++) {
//    for(int j=0; j<mapSize; j++){
//      Serial.print(String(maze[i][j].walls) + "  ");  
//    }
//    Serial.println();
//  }
//  Serial.println();
} 

void loop()  {
  while(true) {
    //Time to run program
    timeDiff = micros() - curTime;
    curTime = micros();

    progTime += timeDiff;

    //Calculate Distance and determine current square
    tempDist = (timeDiff/1000 * motorSpeed)/1000;
    addDist(tempDist);  
    curNodeX = botDistX / tileSize;
    curNodeY = botDistY / tileSize;

    //Get wall information and update node costs
    maze[curNodeY][curNodeX].walls=discoWalls[curNodeY][curNodeX];
    updateCosts();
    getNextDirection();

    //Check for maze completion
    if(TO_CENTER && inCenter(curNodeX,curNodeY)) {
//      Serial.println("REACHED CENTER!");
//      Serial.println("SOLUTION TIME: " + String(progTime));
      progTime = 0;
      TO_START = true;
      TO_CENTER = false;
      for(int i=0; i<mapSize; i++) {
        for(int j=0; j<mapSize; j++){
          maze[i][j].cost = abs(i+j);
        }
      }
      updateCosts();      
    }
    //Check for return trip
    else if(TO_START && curNodeX == 0 && curNodeY == 0) {
//      Serial.println("REACHED START!");
//      Serial.println("SOLUTION TIME: " + String(progTime));
      progTime = 0;
      TO_START = false;      
      TO_CENTER = true;
      for(int i=0; i<mapSize; i++) {
        for(int j=0; j<mapSize; j++){
          maze[i][j].cost = min_dist(i,j);
        }
      } 
     updateCosts(); 
   }

//Debug information
//  Serial.println("Program run time: " + String(timeDiff-timeDelay*1000) + " ms");
//  Serial.println("Current Direction: " + String(direct));
//  Serial.println("Distance traveled: " + String(botDistX) + "," + String(botDistY));
//  Serial.println("Current Node: " + String(curNodeX) + "," + String(curNodeY));
//  Serial.println("Wall information: " + String(maze[curNodeY][curNodeX].walls));
//  for(int i=0; i<mapSize; i++) {
//    for(int j=0; j<mapSize; j++) {
//      if(i==curNodeY && j==curNodeX) {
//        Serial.print("X  ");
//      }
//      else
//        Serial.print(String(maze[i][j].cost) + "  ");
//    }
//    Serial.println();
//  }
//  Serial.println();
//  for(int i=0; i<mapSize; i++) {
//    for(int j=0; j<mapSize; j++){
//      Serial.print(String(maze[i][j].walls) + "  ");  
//    }
//    Serial.println();
//  }
//  delay(timeDelay);
  }
  //  Serial.println("TARGET FOUND!!!!");
}

//Add distance traveled since last loop iteration based on current direction
void addDist(int dist){
  //  Serial.println("Entering addDist()");
  if(direct==NORTH){
    botDistY -= dist;
  }
  else if(direct==SOUTH){
    botDistY += dist;
  }
  else if(direct==EAST){
    botDistX += dist;
  }
  else
    botDistX -= dist;  
  //  Serial.println("Exiting addDist()");
  //  Serial.println();
}

byte getWalls() {
  //  Serial.println("Entering getWalls()");
  //  Serial.println("Exiting getWalls()");
  //  Serial.println();
  return 00000000;
}

//Iterate through each square until all squares have at least one square of lower value adjacent to them.
//This is accomplished by comparing the cost of each adjacent neighbor determined by the surrounding walls.
//Once the minimum value of the neighbors is taken, it's compared to the current square's cost.
void updateCosts() {
//  Serial.println("Entering updateCosts()");
  boolean check = true;
  while(check) {
    check = false;
    for(int i=0; i<mapSize; i++) {
      for(int j=0; j<mapSize; j++) {
        short neighbors[4] = {-1,-1,-1,-1};
        short tempCost = mapSize*mapSize;
        byte walls = maze[i][j].walls;
        if(!bitRead(walls,NORTH) && i-1 >= 0){
          neighbors[NORTH] = maze[i-1][j].cost;    
        }
        if(!bitRead(walls,EAST) && j+1 < mapSize) {
          neighbors[EAST] = maze[i][j+1].cost;
        }
        if(!bitRead(walls,SOUTH) && i+1 < mapSize) {
          neighbors[SOUTH] = maze[i+1][j].cost;
        }
        if(!bitRead(walls,WEST) && j-1 >= 0) {
          neighbors[WEST] = maze[i][j-1].cost;
        }
        for(int k=0; k<4; k++) {
          if(neighbors[k] != -1 && neighbors[k] < tempCost) {
            tempCost = neighbors[k];
          }       
        }
        //        Serial.println("Neighbors for " + String(i) + "," + String(j) + " :" + String(neighbors[0]) + "," + String(neighbors[1]) + "," + String(neighbors[2]) + "," + String(neighbors[3]));
        //        tempCost = min(min(neighbors[0], neighbors[1]), min(neighbors[2], neighbors[3]));
        //        Serial.println("Temp Cost is : " + String(tempCost));
        if(TO_CENTER) {
          if(tempCost >= maze[i][j].cost && !inCenter(i,j) && tempCost != mapSize*mapSize) {
            //          Serial.println("Changing Node Cost");
            maze[i][j].cost = tempCost + 1;
            check = true;
          }
        }
        else if(TO_START){
          if(tempCost >= maze[i][j].cost && !(i == 0 && j == 0) && tempCost != mapSize*mapSize) {
            maze[i][j].cost = tempCost + 1;
            check = true;
          }
        }
      }
//      Serial.println("Exiting updateCosts()");
    }
  }
}

//The lowest cost square is determined by using wall data for the current square.
void getNextDirection() {
  //  Serial.println("Entering getNextDirection()");
  Node tempNode = maze[curNodeY][curNodeX];
  byte walls = maze[curNodeY][curNodeX].walls;
  //  Serial.println("Current wall information" + String(maze[curNodeY][curNodeX].walls));
  int tempCost = 1000;
  if(!bitRead(walls,0) && curNodeY > 0){
    direct = NORTH;
    tempCost = maze[curNodeY-1][curNodeX].cost;    
  }
  if(!bitRead(walls,1) && maze[curNodeY][curNodeX+1].cost < tempCost && curNodeX < mapSize-1) {
    direct = EAST;
    tempCost = maze[curNodeY][curNodeX+1].cost;
  }
  if(!bitRead(walls,2) && maze[curNodeY+1][curNodeX].cost < tempCost && curNodeY < mapSize-1) {
    direct = SOUTH;
    tempCost = maze[curNodeY+1][curNodeX].cost;
  }
  if(!bitRead(walls,3) && maze[curNodeY][curNodeX-1].cost < tempCost && curNodeX > 0) {
    direct = WEST;
    tempCost = maze[curNodeY][curNodeX-1].cost;
  }

  //debug info
  //  Serial.println("Next Direction: " + String(direct));
  //  Serial.println("Exiting getNextDirection()");
  //  Serial.println();
}

//Checks to see if current square is at the center of the maze.
boolean inCenter(int x, int y) {
  if((x == center && y == center) || (x == center-1 && y == center) 
        || (x == center && y == center-1) || (x == center-1 && y == center-1)) {
          return true;
  }
  return false;
}
