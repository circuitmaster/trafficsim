#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

enum tRoadTileType{CBL=1,CBR,CTL,CTR,C,SH,SV,TB,TL,TR,TT};
enum tVehicleType{car1=1, car2, car3, car4, car5, car6};
enum tWayPointdir{Down=1,Left,Right,Up};

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
};

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
		void getPosition(float &x, float &y, float &dir);
		void draw(sf::RenderWindow& window);
};

RoadTile::RoadTile(tRoadTileType t, int row , int col){
	x = (col-1)*239;
	y= (row-1)*239;
	this->t = t;
}

Vehicle::Vehicle(tVehicleType t, float x, float y, float angle){
	this->x = x;
	this->y = y;
	this->angle = angle;
	this->t = t;
	
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

void Waypoint::getPosition(float &x, float &y, float &dir){
	x = this->x;
	y = this->y;
	dir = 0;
}

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
	
	switch(type){
 		case CBL:
 			
 			break;
 		case CBR:
 			
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
			
			break;
		case C:
			
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
			
			break;
		case TB:
			
			break;
		case TL:
			
			break;
		case TR:
			
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

void Vehicle::move(float &x, float &y, float &angle, sf::RenderWindow& window){
	int increment=1;
	
	switch((int)this->angle){
		case 0:
			if((angle == 90 || angle == 270) && this->x >= x){
				break;
			}
			this->x+=increment;
			break;
		case 90:
			if((angle == 0 || angle == 180) && this->y >= y){
				break;
			}
			this->y+=increment;
			break;
		case 180:
			if((angle == 90 || angle == 270) && this->x <= x){
				break;
			}
			this->x-=increment;
			break;
		case 270:
			if((angle == 0 || angle == 180) && this->y <= y){
				break;
			}
			this->y-=increment;
			break;
		default:
			break;
	}
	
	if(angle>this->angle){
		this->angle += increment;
	}else{
		this->angle -= increment;
	}
	
	sprite.setPosition(this->x, this->y);
	sprite.setRotation(this->angle);
	window.draw(sprite);
	
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(1195,1195), "Traffic Simulator");
	Vehicle car(car1, 118, 218, 270);
	

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
		
		Waypoint arr[7] = {Waypoint(Up,CTL,1,1,0,1,-1,-1), Waypoint(Right,CTL,1,1,1,-1,-1,-1), Waypoint(Right,SH,1,2,0,1,-1,-1), Waypoint(Right,SH,1,2,1,4,-1,-1),Waypoint(Right,TT,1,3,0,1,2,-1),Waypoint(Down,TT,1,3,1,-1,-1,-1),Waypoint(Right,TT,1,3,2,-1,-1,-1)};
		
		float x,y,angle;
		arr[1].getPosition(x,y,angle);
		
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
		
		for(int i=0 ; i<7 ; i++)
		{
			arr[i].draw(window);
		}
		
		car.move(x,y,angle,window); 
		 		 
		//Update the display
		window.display();		
	}
	return 0;
}
