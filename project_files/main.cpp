//This part is calling libraries for the further usage of some methods
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>

#define PI 3.141592653589793238463
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

//Defination of traffic light class
class TrafficLight{
	float x;
	float y;
	float dir;
	tLightState state;
	sf::Texture redTexture;
	sf::Texture greenTexture;
	sf::Sprite sprite;
	public:
		TrafficLight *next;
		TrafficLight(float x, float y, float dir, tLightState state);
		void getPosition(float &x, float&y, float &dir);
		void draw(sf::RenderWindow& window);
		tLightState getState();
		void setState(tLightState state);
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
		TrafficLight *l;
		Waypoint(tWayPointdir dir, tRoadTileType type, int row, int col, int idx, int next1, int next2, int next3,TrafficLight *l);
		int getNext();
		int getIndex();
		void getPosition(float &x, float &y, float &dir);
		void draw(sf::RenderWindow& window);
};

class Vehicle{
	public:
		virtual void move2(sf::RenderWindow& window, Waypoint arr[48]) = 0;
};

//The Car class
class Car : public Vehicle{
	private:
		float next_x, next_y, w_x, w_y; 
		tVehicleType t;
		float x, y, angle;
		bool origin_set;
		sf::Texture texture; 
		sf::Sprite sprite;
	
	public:
		float next_dir;
		int increment;
		Car(tVehicleType t, float x, float y, float angle, int inc);
		void move(float &x, float &y, float &angle, sf::RenderWindow& window);
		void move2(sf::RenderWindow& window, Waypoint arr[48]);
		void getPosition(float &x, float &y);
};

//The Bus class
class Bus : public Vehicle{
	private:
		float next_x, next_y, w_x, w_y; 
		vector<int> stops;
		int currentStop;
		float x, y, angle;
		bool origin_set;
		sf::Texture texture; 
		sf::Sprite sprite;
	
	public:
		float next_dir;
		int increment;
		Bus(float x, float y, float angle, int inc);
		void move2(sf::RenderWindow& window, Waypoint arr[48]);
		void getPosition(float &x, float &y);
		void addStop(int index);
};

class BusStop{
	float x; //x coordinate of the bus stop
    float y; //y coordinate of the bus stop
    float dir; //direction of the bus stop (determines the orientation of the bus stop on the map)
    sf::Texture texture; //texture for the bus stop
    sf::Sprite sprite; //sprite for the bus stop
    public:
    	BusStop(float x, float y, float dir);
    	void getPosition(float &x, float &y, float &dir);
    	void draw(sf::RenderWindow& window);
};



//Defination of traffic light Linked-List class
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

Bus::Bus(float x, float y, float angle, int inc){
	this->x = x;
	this->y = y;
	this->angle = angle;
	this->increment = inc;
	
	//This part is for loading the wanted car type
	string path = "images/vehicles/bus.png";
	
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

BusStop::BusStop(float x, float y, float dir){
	this->x = x;
	this->y = y;
	this->dir = dir;
	string path = "images/busstop/busstop.png";
	if (!this->texture.loadFromFile(path))
	{
		cout << "Could not find the image file" << endl;
	}	
	this->sprite.setTexture(texture);
	sf::FloatRect boundingBox = sprite.getGlobalBounds();
	sprite.setOrigin(sf::Vector2f(boundingBox.width / 2, boundingBox.height / 2)); 
	sprite.setRotation(dir+90);
}


//Constructor of traffic light class
TrafficLight::TrafficLight(float x, float y, float dir, tLightState state){
	string path = "images/trafficlights/red.png";
	string path2 = "images/trafficlights/green.png";
	if (!this->redTexture.loadFromFile(path)||!this->greenTexture.loadFromFile(path2))
	{
		cout << "Could not find the image file" << endl;
	}
	this->x = x;
	this->y = y;
	this->dir = dir;
	this->state=state;
	next = NULL;
}

//Constructor of traffic light group class
TrafficLightGroup::TrafficLightGroup(float duration){
	this->duration = duration;
	head = NULL;
	time = 0;
}

//Traffic light getState function defination
tLightState TrafficLight::getState(){
	return this->state;
}

//Traffic light setState function defination
void TrafficLight::setState(tLightState state){
	this->state = state;
}

// The constructor for roadtile
RoadTile::RoadTile(tRoadTileType t, int row , int col){
	x = (col-1)*239;
	y= (row-1)*239;
	this->t = t;
}

//The constructor for vehicle
Car::Car(tVehicleType t, float x, float y, float angle, int inc){
	this->x = x;
	this->y = y;
	this->angle = angle;
	this->t = t;
	this->increment = inc;
	
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

//Traffic light getPosition function defination
void TrafficLight::getPosition(float &x, float&y, float &dir){
	x = this->x;
	y = this->y;
	dir = this->dir;
}

//Traffic light draw function defination
void TrafficLight::draw(sf::RenderWindow& window){
	if(state == Red){
		this->sprite.setTexture(redTexture);
	}else{
		this->sprite.setTexture(greenTexture);
	}
	 //Move car sprite to x,y position
	this->sprite.setPosition(x, y);
	this->sprite.setRotation(this->dir);
	//Draw the car sprite to screen
	window.draw(this->sprite);
}

//Traffic light Linked-List add function  
void TrafficLightGroup::add(TrafficLight *light){
	TrafficLight *tmp = head;
	if(head==NULL){
		head = light;
		greenLight = head;
		greenLight->setState(Green);
		return;
	}
	while(tmp->next!=NULL){
		tmp = tmp->next;
	}
	tmp->next = light; 
}

//Simulation function for a one group of traffic lights 
void TrafficLightGroup::simulate(float timestep){
	if(time==duration){
		if(greenLight->next==NULL){
			greenLight->setState(Red);
			greenLight=head;
			greenLight->setState(Green);
		}else{
			greenLight->setState(Red);
			greenLight = greenLight->next;
			greenLight->setState(Green);
		}	
		time=0;
	}
	this->time+=timestep;
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

// The constructor for waypoint
Waypoint::Waypoint(tWayPointdir dir, tRoadTileType type, int row, int col, int idx, int next1, int next2, int next3,TrafficLight *l=new TrafficLight(0.0,0.0,0.0,Green)){
	diir = dir;
	this->type = type;
	x = (col-1)*239;
	y = (row-1)*239;
	this->idx = idx;
	this->next1 = next1;
	this->next2 = next2;
	this->next3 = next3;
	this->l=l;
}

// Defination of getposition function for waypoint class
void Waypoint::getPosition(float &x, float &y, float &dir){
	x = this->x;
	y = this->y;
	dir = this->diir;
}

// Defination of getposition function for vehicle class
void Car::getPosition(float &x, float &y){
	x = this->x;
	y = this->y;
}

void BusStop::draw(sf::RenderWindow& window){
	this->sprite.setPosition(x, y);
	window.draw(this->sprite);
}

// Defination of draw function for waypoint class
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
void Car::move(float &x, float &y, float &angle, sf::RenderWindow& window){
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
void Car::move2(sf::RenderWindow& window, Waypoint arr[48]){
	
	float waypoint_x, waypoint_y, waypoint_dir;
	int col, row, idx;
	
	for(int j=0; j<48; j++){
		arr[j].getPosition(waypoint_x, waypoint_y, waypoint_dir);
			if(this->x == waypoint_x && this->y == waypoint_y){
				if(arr[j].l->getState()==Red){   //Here it checks the traffic lights state
					this->increment=0; 
					break;
				}else{
					this->increment=1;
				}
				col = (int)(waypoint_x/239) + 1; 
				row = (int)(waypoint_y/239) + 1;
				idx = arr[j].getNext();
				if(idx<0){
					this->next_dir = waypoint_dir;
					break;
				}
				this->w_x = waypoint_x; 
				this->w_y = waypoint_y;
				for(int k=0; k<48; k++){
					int cl,rw;
					float x_,y_,dir_;
					arr[k].getPosition(x_,y_,dir_);
					cl = (int)(x_/239) + 1; 
					rw = (int)(y_/239) + 1;
					if(arr[k].getIndex()==idx && cl==col && rw==row){
						this->next_x = x_; 
						this->next_y = y_;
						this->next_dir = dir_;
						break;
					}
				}
			}
			
	}
	
	
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
	
	if((int)this->angle>=270 && (int)this->angle<=360 && next_dir==0 && (this->y>next_y) && this->angle!=next_dir){
		this->angle += increment;
		this->x = next_x+(sin(this->angle*PI/180)) * 100;
		this->y = this->w_y-(cos(this->angle*PI/180)) * 97;
	}else if((int)this->angle<=270 && (int)this->angle>=180 && next_dir == 180 && this->y>next_y && this->angle!=next_dir){
		this->angle -= increment;
		this->x = next_x-(sin(this->angle*PI/180)) * 100;
		this->y = this->w_y+(cos(this->angle*PI/180)) * 97;
	}else if((int)this->angle<=90 && (int)this->angle>=0 && next_dir == 0 && this->y<next_y && this->angle!=next_dir){
		this->angle -= increment;
		this->x = next_x-(sin(this->angle*PI/180)) * 100;
		this->y = this->w_y+(cos(this->angle*PI/180)) * 101;
	}else if((int)this->angle>=90 && (int)this->angle<=180 && next_dir == 180 && this->y<next_y && (int)this->angle!=next_dir){
		this->angle += increment;
		this->x = next_x+(sin(this->angle*PI/180)) * 98;
		this->y = this->w_y-(cos(this->angle*PI/180)) * 101;
	}else if((int)this->angle>=0 && (int)this->angle<=90 && next_dir==90 && (this->x<next_x) && this->angle!=next_dir){
		this->angle += increment;
		this->x = this->w_x+(sin(this->angle*PI/180)) * 98;
		this->y = next_y-(cos(this->angle*PI/180)) * 97;
	}else if((int)this->angle<=0 && (int)this->angle>=-90 && next_dir == 270 && (this->x<next_x) && this->angle!=next_dir){
		this->angle -= increment;
		this->x = this->w_x-(sin(this->angle*PI/180)) * 98;
		this->y = next_y+(cos(this->angle*PI/180)) * 97;
	}else if((int)this->angle<=180 && (int)this->angle>=90 && next_dir == 90 && this->x>next_x && this->angle!=next_dir){
		this->angle -= increment;
		this->x = this->w_x-(sin(this->angle*PI/180)) * 100;
		this->y = next_y+(cos(this->angle*PI/180)) * 97;
	}else if((int)this->angle>=180 && (int)this->angle<=270 && next_dir == 270 && this->x>next_x && this->angle!=next_dir){
		this->angle += increment;
		this->x = this->w_x+(sin(this->angle*PI/180)) * 100;
		this->y = next_y-(cos(this->angle*PI/180)) * 97;
	}else if((int)this->angle%360==next_dir){
		if((int)next_dir%360==0)
			this->x+=increment;
		else if(next_dir==180)
			this->x-=increment;
		else if(next_dir==90)
			this->y+=increment;
		else if(next_dir==270)
			this->y-=increment;
	}
	
	
	sprite.setPosition(this->x, this->y);
	sprite.setRotation(this->angle);
	window.draw(sprite);
	
}

void Bus::move2(sf::RenderWindow& window, Waypoint arr[48]){
	float waypoint_x, waypoint_y, waypoint_dir;
	int col, row, idx;
	
	for(int j=0; j<48; j++){
		arr[j].getPosition(waypoint_x, waypoint_y, waypoint_dir);
			if(this->x == waypoint_x && this->y == waypoint_y){
				if(arr[j].l->getState()==Red){   //Here it checks the traffic lights state
					this->increment=0; 
					break;
				}else{
					this->increment=1;
				}
				col = (int)(waypoint_x/239) + 1; 
				row = (int)(waypoint_y/239) + 1;
				idx = arr[j].getNext();
				if(idx<0){
					this->next_dir = waypoint_dir;
					break;
				}
				this->w_x = waypoint_x; 
				this->w_y = waypoint_y;
				for(int k=0; k<48; k++){
					int cl,rw;
					float x_,y_,dir_;
					arr[k].getPosition(x_,y_,dir_);
					cl = (int)(x_/239) + 1; 
					rw = (int)(y_/239) + 1;
					if(arr[k].getIndex()==idx && cl==col && rw==row){
						this->next_x = x_; 
						this->next_y = y_;
						this->next_dir = dir_;
						break;
					}
				}
			}
			
	}
	
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
	
	if((int)this->angle>=270 && (int)this->angle<=360 && next_dir==0 && (this->y>next_y) && this->angle!=next_dir){
		this->angle += increment;
		this->x = next_x+(sin(this->angle*PI/180)) * 100;
		this->y = this->w_y-(cos(this->angle*PI/180)) * 97;
	}else if((int)this->angle<=270 && (int)this->angle>=180 && next_dir == 180 && this->y>next_y && this->angle!=next_dir){
		this->angle -= increment;
		this->x = next_x-(sin(this->angle*PI/180)) * 100;
		this->y = this->w_y+(cos(this->angle*PI/180)) * 97;
	}else if((int)this->angle<=90 && (int)this->angle>=0 && next_dir == 0 && this->y<next_y && this->angle!=next_dir){
		this->angle -= increment;
		this->x = next_x-(sin(this->angle*PI/180)) * 100;
		this->y = this->w_y+(cos(this->angle*PI/180)) * 101;
	}else if((int)this->angle>=90 && (int)this->angle<=180 && next_dir == 180 && this->y<next_y && (int)this->angle!=next_dir){
		this->angle += increment;
		this->x = next_x+(sin(this->angle*PI/180)) * 98;
		this->y = this->w_y-(cos(this->angle*PI/180)) * 101;
	}else if((int)this->angle>=0 && (int)this->angle<=90 && next_dir==90 && (this->x<next_x) && this->angle!=next_dir){
		this->angle += increment;
		this->x = this->w_x+(sin(this->angle*PI/180)) * 98;
		this->y = next_y-(cos(this->angle*PI/180)) * 97;
	}else if((int)this->angle<=0 && (int)this->angle>=-90 && next_dir == 270 && (this->x<next_x) && this->angle!=next_dir){
		this->angle -= increment;
		this->x = this->w_x-(sin(this->angle*PI/180)) * 98;
		this->y = next_y+(cos(this->angle*PI/180)) * 97;
	}else if((int)this->angle<=180 && (int)this->angle>=90 && next_dir == 90 && this->x>next_x && this->angle!=next_dir){
		this->angle -= increment;
		this->x = this->w_x-(sin(this->angle*PI/180)) * 100;
		this->y = next_y+(cos(this->angle*PI/180)) * 97;
	}else if((int)this->angle>=180 && (int)this->angle<=270 && next_dir == 270 && this->x>next_x && this->angle!=next_dir){
		this->angle += increment;
		this->x = this->w_x+(sin(this->angle*PI/180)) * 100;
		this->y = next_y-(cos(this->angle*PI/180)) * 97;
	}else if((int)this->angle%360==next_dir){
		if((int)next_dir%360==0)
			this->x+=increment;
		else if(next_dir==180)
			this->x-=increment;
		else if(next_dir==90)
			this->y+=increment;
		else if(next_dir==270)
			this->y-=increment;
	}
	
	
	sprite.setPosition(this->x, this->y);
	sprite.setRotation(this->angle);
	window.draw(sprite);
}

void Bus::getPosition(float &x, float &y){
	x = this->x;
	y = this->y;
}

void Bus::addStop(int index){
	this->stops.push_back(index);
}

int Waypoint::getIndex(){
	return this->idx; 
}

// The main function
int main()
{
	srand(time(NULL)); //For real randomization 
	sf::RenderWindow window(sf::VideoMode(1195,1195), "Traffic Simulator"); // The window size-name 
	Car car[6] = {Car(car1, 118, 218, 270, 1), Car(car2, 4*239+20, 121, 0, 1), Car(car3, 118, 4*239+20, 270, 1), Car(car4, 4*239+118, 4*239+20, 270, 1), Car(car5, 218, 2*239+121, 0, 1), Car(car6, 4*239+20, 2*239+121, 180, 1)};
	Bus buses[2] = {Bus(239*2+20, 121, 0, 1), Bus(239*2+20, 239*4+121, 180, 1)};
	BusStop stops[8] = {BusStop(2*239+218,121,0),BusStop(4*239+118, 239+20, 90),BusStop(2*239+218, 2*239+121, 180),BusStop(2*239+118,3*239+172,90),BusStop(118,3*239+172,270),BusStop(118,218,270),BusStop(2*239+118,218,90),BusStop(4*239+118,4*239+20,270)};
	float w_x[6],w_y[6];
	TrafficLight* l1 = new TrafficLight(530,647,90,Red); //middle left
	TrafficLight* l2 = new TrafficLight(547,532,180,Red); //middle top
	TrafficLight* l3 = new TrafficLight(662,552,270,Red); //middle right
	TrafficLight* l4 = new TrafficLight(1025,532,180,Red); //right top 
	TrafficLight* l5 = new TrafficLight(1125,655,0,Red); //right down
	
	TrafficLightGroup g1(200);
	TrafficLightGroup g2(200);
	
	g1.add(l1);
	g1.add(l2);
	g1.add(l3);
	g2.add(l4);
	g2.add(l5);
	
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
		Waypoint arr[48] = {Waypoint(Up,CTL,1,1,0,1,-1,-1), Waypoint(Right,CTL,1,1,1,-1,-1,-1), Waypoint(Right,SH,1,2,0,1,-1,-1), Waypoint(Right,SH,1,2,1,-1,-1,-1),Waypoint(Right,TT,1,3,0,1,2,-1),Waypoint(Down,TT,1,3,1,-1,-1,-1),Waypoint(Right,TT,1,3,2,-1,-1,-1),Waypoint(Right,SH,1,4,0,1,-1,-1), Waypoint(Right,SH,1,4,1,4,-1,-1),Waypoint(Right,CTR,1,5,0,1,-1,-1), Waypoint(Down,CTR,1,5,1,-1,-1,-1),Waypoint(Up,SV,2,1,0,1,-1,-1), Waypoint(Up,SV,2,1,1,-1,-1,-1),Waypoint(Down,SV,2,3,0,1,-1,-1), Waypoint(Down,SV,2,3,1,-1,-1,-1),Waypoint(Down,SV,2,5,0,1,-1,-1), Waypoint(Down,SV,2,5,1,-1,-1,-1),Waypoint(Up,TL,3,1,0,-1,-1,-1),Waypoint(Right,TL,3,1,1,-1,-1,-1),Waypoint(Up,TL,3,1,2,0,1,-1), Waypoint(Right,SH,3,2,0,1,-1,-1), Waypoint(Right,SH,3,2,1,4,-1,-1),Waypoint(Right,C,3,3,0,3,-1,-1,l1), Waypoint(Down,C,3,3,1,3,-1,-1,l2),Waypoint(Left,C,3,3,2,3,-1,-1,l3), Waypoint(Down,C,3,3,3,-1,-1,-1), Waypoint(Left,SH,3,4,0,1,-1,-1), Waypoint(Left,SH,3,4,1,4,-1,-1),Waypoint(Down,TR,3,5,0,1,-1,-1,l4),Waypoint(Left,TR,3,5,1,-1,-1,-1),Waypoint(Up,TR,3,5,2,1,-1,-1,l5),Waypoint(Up,SV,4,1,0,1,-1,-1), Waypoint(Up,SV,4,1,1,-1,-1,-1),Waypoint(Down,SV,4,3,0,1,-1,-1), Waypoint(Down,SV,4,3,1,-1,-1,-1),Waypoint(Up,SV,4,5,0,1,-1,-1), Waypoint(Up,SV,4,5,1,-1,-1,-1),Waypoint(Up,CBL,5,1,0,-1,-1,-1), Waypoint(Left,CBL,5,1,1,0,-1,-1), Waypoint(Left,SH,5,2,0,1,-1,-1), Waypoint(Left,SH,5,2,1,4,-1,-1),Waypoint(Left,TB,5,3,0,-1,-1,-1),Waypoint(Down,TB,5,3,1,0,2,-1),Waypoint(Right,TB,5,3,2,-1,-1,-1), Waypoint(Right,SH,5,4,0,1,-1,-1), Waypoint(Right,SH,5,4,1,4,-1,-1),Waypoint(Right,CBR,5,5,0,1,-1,-1), Waypoint(Up,CBR,5,5,1,-1,-1,-1)};
		
		//Some variables for further usage
		float x[6],y[6],x2,y2,dir,next_x[6],next_y[6],next_dir[6], bx[2], by[2];
		int col,row,idx;
		for(int i=0;i<6;i++){
			car[i].getPosition(x[i],y[i]);
			car[i].increment=1;
		}
		for(int i=0;i<2;i++){
			buses[i].getPosition(bx[i],by[i]);
			buses[i].increment=1;
		}

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
		 // For drawing every traffic light in our road
		 
		 l1->draw(window);
		 l2->draw(window);
		 l3->draw(window);
		 l4->draw(window);
		 l5->draw(window);
		
		//For drawing every waypoint in our road
		for(int i=0 ; i<48 ; i++)
		{
			arr[i].draw(window);
		}
		
		for(int i=0 ; i<8 ; i++)
		{
			stops[i].draw(window);
		}
		
		//This part finds which waypoint the car stands now for determining local waypoint and use that waypoints functions.(For first part)
		//The speed reduction part is also added for the state of the traffic light in second part
		
		
		//This part is checking if there are any collision between the cars , and if any , reduces the speed of the behind to zero
		for(int i=0;i<6;i++){
			for(int j=i+1;j<6;j++){
				if(x[i]==x[j] && abs(y[i]-y[j])<80){
					int k = (y[i]-y[j])/abs(y[i]-y[j]);
					switch((int)car[j].next_dir){
						case Up:
							if(k==-1)k=j;
							else k=i;
							break;
						case Down:
							if(k==-1)k=i;
							else k=j;
							break;
						default:
							break;
					}
					car[k].increment = 0;	
				}else if(y[i]==y[j] && abs(x[i]-x[j])<80){
					int k = (x[i]-x[j])/abs(x[i]-x[j]);
					switch((int)car[j].next_dir){
						case Left:
							if(k==-1)k=j;
							else k=i;
							break;
						case Right:
							if(k==-1)k=i;
							else k=j;
							break;
						default:
							break;
					}
					car[k].increment = 0;	
				}
			}
			for(int k=0; k<2; k++){
				if(x[i]==bx[k] && abs(y[i]-by[k])<80){
					int kk = (y[i]-by[k])/abs(y[i]-by[k]);
					switch((int)buses[k].next_dir){
						case Up:
							cout << "UP" << endl;
							if(kk==-1){
								buses[k].increment=0;
							}
							else{
								car[i].increment = 0;
							}
							break;
						case Down:
							cout << "DOWN" << endl;
							if(kk==-1){
								car[i].increment = 0;	
							}
							else{
								buses[k].increment=0;
							}
							break;
						default:
							break;
					}
				}else if(y[i]==by[k] && abs(x[i]-bx[k])<80){
					int kk = (x[i]-bx[k])/abs(x[i]-bx[k]);
					switch((int)buses[k].next_dir){
						case Left:
							cout << "LEFT" << endl;
							if(kk==-1){
								buses[k].increment=0;
							}
							else{
								car[i].increment = 0;	
							}
							break;
						case Right:
							cout << "RIGHT" << endl;
							if(kk==-1){
								car[i].increment = 0;	
							}
							else{
								buses[k].increment=0;
							}
							break;
						default:
							break;
					}
				}
			}
		}
		
		if(bx[0]==bx[1] && abs(y[0]-by[1])<80){
					int kk = (by[0]-by[1])/abs(by[0]-by[1]);
					switch((int)buses[1].next_dir){
						case Up:
							cout << "UP" << endl;
							if(kk==-1){
								buses[1].increment=0;
							}
							else{
								buses[0].increment = 0;
							}
							break;
						case Down:
							cout << "DOWN" << endl;
							if(kk==-1){
								buses[0].increment = 0;	
							}
							else{
								buses[1].increment=0;
							}
							break;
						default:
							break;
					}
		}else if(y[0]==by[1] && abs(x[0]-bx[1])<80){
					int kk = (bx[0]-bx[1])/abs(bx[0]-bx[1]);
					switch((int)buses[1].next_dir){
						case Left:
							cout << "LEFT" << endl;
							if(kk==-1){
								buses[1].increment=0;
							}
							else{
								buses[0].increment = 0;	
							}
							break;
						case Right:
							cout << "RIGHT" << endl;
							if(kk==-1){
								buses[0].increment = 0;	
							}
							else{
								buses[1].increment=0;
							}
							break;
						default:
							break;
					}
		}
		// Moving the cars
		//Simulating the traffic lights 
		g1.simulate(1);
		g2.simulate(1);
		for(int i=0;i<6;i++){
			car[i].move2(window, arr);
		}
		buses[0].move2(window, arr);
		buses[1].move2(window, arr);	 
		//Update the display
		window.display();		
	}
	return 0;
}
