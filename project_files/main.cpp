//This part is calling libraries for the further usage of some methods
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
//This part is enumerators for images given
using namespace std;

enum tRoadTileType{CBL=1,CBR,CTL,CTR,C,SH,SV,TB,TL,TR,TT};
enum tVehicleType{car1=1, car2, car3, car4, car5, car6};
enum tWayPointdir{Down=90,Left=180,Right=0,Up=270};
enum tLightState{Green=1,Red};


// The road tile class
class RoadTile
{
	private:
		tRoadTileType t;
		float x; //x coordinate of the top left corner of the road tile
		float y; //y coordinate of the top left corner of the road tile
		sf::Texture texture; //tile texture object
		sf::Sprite sprite;  //tile sprite object
		
	public: 
		RoadTile(tRoadTileType t, int row , int col);
		void draw(sf::RenderWindow& window); 
		
};

//The vehicle class
class Vehicle{
	private:
		tVehicleType t;
		float x, y, angle;
		bool origin_set;
		sf::Texture texture; 
		sf::Sprite sprite;
	
	public:
		Vehicle(tVehicleType t, float x, float y, float angle);
		void move(float &x, float &y, float &angle, sf::RenderWindow& window);
		void move2(float &x, float &y, float &angle, sf::RenderWindow& window, int w_x, int w_y);
		void getPosition(float &x, float &y);
};

// The waypoint class
class Waypoint{
	tWayPointdir diir;
	tRoadTileType type;
	float x; //Global x coordinate of the waypoint
	float y; //Global y coordinate of the waypoint
	int dir; //direction of the waypoint (one of the 4 available directions)
	int idx; 
	int next1; //Global index of the first alternative next waypoint
	int next2; //Global index of the second alternative next waypoint
	int next3; //Global index of the third alternative next waypoint
	sf::Texture texture; //waypoint texture object
	sf::Sprite sprite; //waypoint sprite object
	
	public:
		Waypoint(tWayPointdir dir, tRoadTileType type, int row, int col, int idx, int next1, int next2, int next3);
		int getNext();
		int getIndex();
		void getPosition(float &x, float &y, float &dir);
		void draw(sf::RenderWindow& window);
};

class TrafficLight{
	float x;
	float y;
	float dir;
	tLightState state;
	sf:Texture redTexture;
	sf:Texture greenTexture;
	sf:Sprite sprite;
	TrafficLight *next;
	public:
		TrafficLight(float x, float y, float dir, tLightState state);
		void getPosition(float &x, float&y, float &dir);
		void draw();
		tLightState getState();
		void setState(tLightState state);
};

class TrafficLightGroup{
	private :
		TrafficLight *head ;
		TrafficLight *greenLight;
		float time;
		float duration;
	public :
		TrafficLightGroup(float duration) ;
		void add(TrafficLight *light) ;
		void simulate(float timestep) ;
};

// The constructor for roadtile
RoadTile::RoadTile(tRoadTileType t, int row , int col){
	x = (col-1)*239;
	y= (row-1)*239;
	this->t = t;
}

//The constructor for vehicle
Vehicle::Vehicle(tVehicleType t, float x, float y, float angle){
	this->x = x;
	this->y = y;
	this->angle = angle;
	this->t = t;
	
//This part is for loading the wanted car type
	string path = "images/vehicles/";
 	switch(t){
 		case CBL:
 			path+="car1.png";
 			break;
 		case CBR:
 			path+="car2.png";
 			break;
		case CTL:
			path+="car3.png";
			break;
		case CTR:
			path+="car4.png";
			break;
		case C:
			path+="car5.png";
			break;
		case SH:
			path+="car6.png";
			break;
		default:
			break;
		}
		
		if (!this->texture.loadFromFile(path))
		{
			cout << "Could not find the image file" << endl;
		}	
 	
	 
		this->sprite.setTexture(texture);
	
		sf::FloatRect boundingBox = sprite.getGlobalBounds();
	 	//Set the sprite rotation origin to the center of the bounding box
	 	sprite.setOrigin(sf::Vector2f(boundingBox.width / 2, boundingBox.height / 2)); 
	 	sprite.setRotation(angle);
		
}

// The defination of draw function for RoadTile class
void RoadTile::draw(sf::RenderWindow& window){
 	string path = "images/roadpieces/";
 	switch(t){
 		case CBL:
 			path+="corner-bottomleft.png";
 			break;
 		case CBR:
 			path+="corner-bottomright.png";
 			break;
		case CTL:
			path+="corner-topleft.png";
			break;
		case CTR:
			path+="corner-topright.png";
			break;
		case C:
			path+="cross.png";
			break;
		case SH:
			path+="straight-horizontal.png";
			break;
		case SV:
			path+="straight-vertical.png";
			break;
		case TB:
			path+="t-bottom.png";
			break;
		case TL:
			path+="t-left.png";
			break;
		case TR:
			path+="t-right.png";
			break;
		case TT:
			path+="t-top.png";
			break;
		default:
			break;
	}
	
 	if (!this->texture.loadFromFile(path))
	{
		cout << "Could not find the image file" << endl;
	}	
 	
	 
	this->sprite.setTexture(texture);
	
	 //Move car sprite to x,y position
	this->sprite.setPosition(x, y);
	
	//Draw the car sprite to screen
	window.draw(this->sprite);
}

// The constructor for roadtile
Waypoint::Waypoint(tWayPointdir dir, tRoadTileType type, int row, int col, int idx, int next1, int next2, int next3){
	diir = dir;
	this->type = type;
	x = (col-1)*239;
	y = (row-1)*239;
	this->idx = idx;
	this->next1 = next1;
	this->next2 = next2;
	this->next3 = next3;
}

// Defination of getposition function for waypoint class
void Waypoint::getPosition(float &x, float &y, float &dir){
	x = this->x;
	y = this->y;
	dir = this->diir;
}

// Defination of getposition function for vehicle class
void Vehicle::getPosition(float &x, float &y){
	x = this->x;
	y = this->y;
}

//// Defination of draw function for waypoint class
void Waypoint::draw(sf::RenderWindow& window){
 	string path = "images/waypoints/";
 	switch(diir){
 		case Down:
 			path+="down.png";
 			break;
 		case Left:
 			path+="left.png";
 			break;
		case Up:
			path+="up.png";
			break;
		case Right:
			path+="right.png";
			break;
		default:
			break;	
	}
	
	// This type draws waypoints onto the specific type of tiles to predetermined coordinates
	switch(type){
 		case CBL:
 			if(idx==0){
				x += 118;
				y += 20;
			}else{
				x += 218;
				y += 121;
			}
 			
 			break;
 		case CBR:
 			if(idx==0){
				x += 20;
				y += 121;
			}else{
				x += 118;
				y += 20;
			}
 			break;
		case CTL:
			if(idx==0){
				x += 118;
				y += 218;
			}else{
				x += 218;
				y += 121;
			}
			break;
		case CTR:
			if(idx==0){
				x += 20;
				y += 121;
			}else{
				x += 118;
				y += 218;
			}
			
			break;
		case C:
			if(idx==0){
				x += 20;
				y += 121;
			}else if(idx==1){
				x += 118;
				y += 20;
			}else if(idx == 2){
				x += 218;
				y += 121;
			}else{
				x += 118;
				y += 218;
			}
			
			break;
		case SH:
			if(idx==0){
				x += 20;
				y += 121;
			}else{
				x += 172;
				y += 121;
			}
			break;
		case SV:
			if(idx==0){
				x += 118;
				y += 20;
			}else{
				x += 118;
				y += 172;
			}
			
			break;
		case TB:
			if(idx==0){
				x += 20;
				y += 121;
			}else if(idx==1){
				x += 118;
				y += 20;
			}else{
				x += 218;
				y += 121;
			}
			
			break;
		case TL:
			if(idx==0){
				x += 118;
				y += 20;
			}else if(idx==1){
				x += 218;
				y += 121;
			}else{
				x += 118;
				y += 218;
			}
			
			break;
		case TR:
			if(idx==0){
				x += 118;
				y += 20;
			}else if(idx==1){
				x += 20;
				y += 121;
			}else{
				x += 118;
				y += 218;
			}
			break;
		case TT:
			if(idx==0){
				x += 20;
				y += 121;
			}else if(idx==1){
				x += 118;
				y += 218;
			}else{
				x += 218;
				y += 121;
			}
			
			break;
		default:
			break;
	}
	
	if (!this->texture.loadFromFile(path))
	{
		cout << "Could not find the image file" << endl;
	}	
 	
	 
	this->sprite.setTexture(texture);
	
	 //Move car sprite to x,y position
	this->sprite.setPosition(x, y);
	
	//Draw the car sprite to screen
	window.draw(this->sprite);

}
// Defination of getnext function for waypoint class
int Waypoint::getNext(){
	int random;
	if(next3>=0){
		random=rand()%3;
	}else if(next2>=0){
		random=rand()%2;
	}else if(next1>=0){
		random = 0; 
	}else{
		return -2;
	}
	
	switch(random){
		case 0:
			return next1; 
		case 1:
			return next2;
		case 2:
			return next3; 
		default:
			return -2; 
	}
}

//  Defination of not-smooth move function for waypoint class
void Vehicle::move(float &x, float &y, float &angle, sf::RenderWindow& window){
	int increment=1;
	
	if((int)this->angle==270 && angle==0 && this->angle!=angle){
		if(this->y>y){
			this->y-=increment;
		}else{
			this->angle=angle;
		}
	}else if((int)this->angle==270 && angle==180 && this->angle!=angle){
		if(this->y>y){
			this->y-=increment;
		}else{
			this->angle=angle;
		}
	}else if((int)this->angle==90 && angle == 0 && this->angle!=angle){
		if(this->y<y){
			this->y+=increment;
		}else{
			this->angle=angle;
		}
	}else if((int)this->angle==90 && angle == 180 && this->angle!=angle){
		if(this->y<y){
			this->y+=increment;
		}else{
			this->angle=angle;
		}
	}else if((int)this->angle==0 && angle==90 && this->angle!=angle){
		if(this->x<x){
			this->x+=increment;
		}else{
			this->angle=angle;
		}
	}else if((int)this->angle==0 && angle==270 && this->angle!=angle){
		if(this->x<x){
			this->x+=increment;
		}else{
			this->angle=angle;
		}
	}else if((int)this->angle==180 && angle == 90 && this->angle!=angle){
		if(this->x>x){
			this->x-=increment;
		}else{
			this->angle=angle;
		}
	}else if((int)this->angle==180 && angle == 270 && this->angle!=angle){
		if(this->x>x){
			this->x-=increment;
		}else{
			this->angle=angle;
		}
	}else if((int)this->angle%360==angle){
		if((int)angle%360==0)
			this->x+=increment;
		else if(angle==180)
			this->x-=increment;
		else if(angle==90)
			this->y+=increment;
		else if(angle==270)
			this->y-=increment;
	}

	sprite.setPosition(this->x, this->y);
	sprite.setRotation(this->angle);
	window.draw(sprite);
	
}

// The defination of smooth move of car 
void Vehicle::move2(float &x, float &y, float &angle, sf::RenderWindow& window,int w_x, int w_y){
	int increment=1;
	
	switch(int(this->angle)%360){
		case 0:
			this->angle = 0;
			break;
		case 90:
			this->angle = 90;
			break;
		case 180:
			this->angle = 180;
			break;
		case -90: 
		case 270:
			this->angle = 270;
			break;
		default:
			break;
	}
	
	if((int)this->angle>=270 && (int)this->angle<=360 && angle==0 && (this->y>y) && this->angle!=angle){
		this->angle += increment;
		this->x = x+(sin(this->angle*3.141592653589793238463/180)) * 100;
		this->y = w_y-(cos(this->angle*3.141592653589793238463/180)) * 97;
	}else if((int)this->angle<=270 && (int)this->angle>=180 && angle == 180 && this->y>y && this->angle!=angle){
		this->angle -= increment;
		this->x = x-(sin(this->angle*3.141592653589793238463/180)) * 100;
		this->y = w_y+(cos(this->angle*3.141592653589793238463/180)) * 97;
	}else if((int)this->angle<=90 && (int)this->angle>=0 && angle == 0 && this->y<y && this->angle!=angle){
		this->angle -= increment;
		this->x = x-(sin(this->angle*3.141592653589793238463/180)) * 100;
		this->y = w_y+(cos(this->angle*3.141592653589793238463/180)) * 101;
	}else if((int)this->angle>=90 && (int)this->angle<=180 && angle == 180 && this->y<y && (int)this->angle!=angle){
		this->angle += increment;
		this->x = x+(sin(this->angle*3.141592653589793238463/180)) * 98;
		this->y = w_y-(cos(this->angle*3.141592653589793238463/180)) * 101;
	}else if((int)this->angle>=0 && (int)this->angle<=90 && angle==90 && (this->x<x) && this->angle!=angle){
		this->angle += increment;
		this->x = w_x+(sin(this->angle*3.141592653589793238463/180)) * 98;
		this->y = y-(cos(this->angle*3.141592653589793238463/180)) * 97;
	}else if((int)this->angle<=0 && (int)this->angle>=-90 && angle == 270 && (this->x<x) && this->angle!=angle){
		this->angle -= increment;
		this->x = w_x-(sin(this->angle*3.141592653589793238463/180)) * 98;
		this->y = y+(cos(this->angle*3.141592653589793238463/180)) * 97;
	}else if((int)this->angle<=180 && (int)this->angle>=90 && angle == 90 && this->x>x && this->angle!=angle){
		this->angle -= increment;
		this->x = w_x-(sin(this->angle*3.141592653589793238463/180)) * 100;
		this->y = y+(cos(this->angle*3.141592653589793238463/180)) * 97;
	}else if((int)this->angle>=180 && (int)this->angle<=270 && angle == 270 && this->x>x && this->angle!=angle){
		this->angle += increment;
		this->x = w_x+(sin(this->angle*3.141592653589793238463/180)) * 100;
		this->y = y-(cos(this->angle*3.141592653589793238463/180)) * 97;
	}else if((int)this->angle%360==angle){
		if((int)angle%360==0)
			this->x+=increment;
		else if(angle==180)
			this->x-=increment;
		else if(angle==90)
			this->y+=increment;
		else if(angle==270)
			this->y-=increment;
	}
	
	
	sprite.setPosition(this->x, this->y);
	sprite.setRotation(this->angle);
	window.draw(sprite);
	
}

int Waypoint::getIndex(){
	return this->idx; 
}

// The main function
int main()
{
	srand(time(NULL)); //For real randomization 
	sf::RenderWindow window(sf::VideoMode(1195,1195), "Traffic Simulator"); // The window size-name 
	Vehicle car(car1, 118, 218, 270); // The car is started from upper-left corner
	float w_x,w_y;
	
	while (window.isOpen()) //This is the main loop, the simulation should take place within this loop
	{
		 // check all the window's events that were triggered since the last iteration of the loop
		 sf::Event event;
		 while (window.pollEvent(event))
		 {
		 // "close requested" event: we close the window
		 if (event.type == sf::Event::Closed)
			 window.close();
	 	 }
		 //Clear window
		 window.clear(sf::Color::White); 
		
		//Below is the array of every 48 waypoints
		Waypoint arr[48] = {Waypoint(Up,CTL,1,1,0,1,-1,-1), Waypoint(Right,CTL,1,1,1,-1,-1,-1), Waypoint(Right,SH,1,2,0,1,-1,-1), Waypoint(Right,SH,1,2,1,-1,-1,-1),Waypoint(Right,TT,1,3,0,1,2,-1),Waypoint(Down,TT,1,3,1,-1,-1,-1),Waypoint(Right,TT,1,3,2,-1,-1,-1),Waypoint(Right,SH,1,4,0,1,-1,-1), Waypoint(Right,SH,1,4,1,4,-1,-1),Waypoint(Right,CTR,1,5,0,1,-1,-1), Waypoint(Down,CTR,1,5,1,-1,-1,-1),Waypoint(Up,SV,2,1,0,1,-1,-1), Waypoint(Up,SV,2,1,1,-1,-1,-1),Waypoint(Down,SV,2,3,0,1,-1,-1), Waypoint(Down,SV,2,3,1,-1,-1,-1),Waypoint(Down,SV,2,5,0,1,-1,-1), Waypoint(Down,SV,2,5,1,-1,-1,-1),Waypoint(Up,TL,3,1,0,-1,-1,-1),Waypoint(Right,TL,3,1,1,-1,-1,-1),Waypoint(Up,TL,3,1,2,0,1,-1), Waypoint(Right,SH,3,2,0,1,-1,-1), Waypoint(Right,SH,3,2,1,4,-1,-1),Waypoint(Right,C,3,3,0,3,-1,-1), Waypoint(Down,C,3,3,1,3,-1,-1),Waypoint(Left,C,3,3,2,3,-1,-1), Waypoint(Down,C,3,3,3,-1,-1,-1), Waypoint(Left,SH,3,4,0,1,-1,-1), Waypoint(Left,SH,3,4,1,4,-1,-1),Waypoint(Down,TR,3,5,0,1,-1,-1),Waypoint(Left,TR,3,5,1,-1,-1,-1),Waypoint(Up,TR,3,5,2,1,-1,-1),Waypoint(Up,SV,4,1,0,1,-1,-1), Waypoint(Up,SV,4,1,1,-1,-1,-1),Waypoint(Down,SV,4,3,0,1,-1,-1), Waypoint(Down,SV,4,3,1,-1,-1,-1),Waypoint(Up,SV,4,5,0,1,-1,-1), Waypoint(Up,SV,4,5,1,-1,-1,-1),Waypoint(Up,CBL,5,1,0,-1,-1,-1), Waypoint(Left,CBL,5,1,1,0,-1,-1), Waypoint(Left,SH,5,2,0,1,-1,-1), Waypoint(Left,SH,5,2,1,4,-1,-1),Waypoint(Left,TB,5,3,0,-1,-1,-1),Waypoint(Down,TB,5,3,1,0,2,-1),Waypoint(Right,TB,5,3,2,-1,-1,-1), Waypoint(Right,SH,5,4,0,1,-1,-1), Waypoint(Right,SH,5,4,1,4,-1,-1),Waypoint(Right,CBR,5,5,0,1,-1,-1), Waypoint(Up,CBR,5,5,1,-1,-1,-1)};
		
		//Some variables for further usage
		float x,y,x2,y2,dir,next_x,next_y,next_dir;
		int col,row,idx;
		car.getPosition(x,y);
		
		// Every roadtile in our road
		 RoadTile r1(CTL,1,1);
		 RoadTile r2(SH,1,2);
		 RoadTile r3(TT,1,3);
		 RoadTile r4(SH,1,4);
		 RoadTile r5(CTR,1,5); 
		 RoadTile r6(SV,2,1);
		 RoadTile r7(SV,2,3);
		 RoadTile r8(SV,2,5);
		 RoadTile r9(TL,3,1);
		 RoadTile r10(SH,3,2);
		 RoadTile r11(C,3,3);
		 RoadTile r12(SH,3,4);
		 RoadTile r13(TR,3,5);
		 RoadTile r14(SV,4,1);
		 RoadTile r15(SV,4,3);
		 RoadTile r16(SV,4,5);
		 RoadTile r17(CBL,5,1);
		 RoadTile r18(SH,5,2);
		 RoadTile r19(TB,5,3);
		 RoadTile r20(SH,5,4);
		 RoadTile r21(CBR,5,5);
		 
		 // For drawing every roadtile in our road
		 r1.draw(window);
		 r2.draw(window);
		 r3.draw(window);
		 r4.draw(window);
		 r5.draw(window);
		 r6.draw(window);
		 r7.draw(window);
	     r8.draw(window);
	     r9.draw(window);
		 r10.draw(window);
		 r11.draw(window);
		 r12.draw(window);
		 r13.draw(window);
		 r14.draw(window);
		 r15.draw(window);
		 r16.draw(window);
		 r17.draw(window);
		 r18.draw(window);
		 r19.draw(window);
		 r20.draw(window);
		 r21.draw(window);
		
		//For drawing every waypoint in our road
		for(int i=0 ; i<48 ; i++)
		{
			arr[i].draw(window);
		}
		
		//This part finds which waypoint the car stands now for determining local waypoint and use that waypoints functions. 
		for(int j=0; j<48; j++){
			arr[j].getPosition(x2,y2,dir);
			if(x==x2 && y==y2){
				col = (int)(x2/239) + 1; 
				row = (int)(y2/239) + 1;
				idx = arr[j].getNext();
				if(idx<0){
					next_dir = dir;
					break;
				}
				w_x=x2; 
				w_y=y2;
				for(int k=0; k<48; k++){
					int cl,rw;
					float x_,y_,dir_;
					arr[k].getPosition(x_,y_,dir_);
					cl = (int)(x_/239) + 1; 
					rw = (int)(y_/239) + 1;
					if(arr[k].getIndex()==idx && cl==col && rw==row){
						next_x = x_; 
						next_y = y_;
						next_dir = dir_;
						break;
					}
				}
				break;
			}
		}
		// Moving the car
		car.move2(next_x,next_y,next_dir,window,w_x,w_y); 
		 		 
		//Update the display
		window.display();		
	}
	return 0;
}
