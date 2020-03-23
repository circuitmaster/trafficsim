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
		void move(float &x, float &y, float &angle);
};

class Waypoint{
	tWayPointdir diir;
	float x; //Global x coordinate of the waypoint
	float y; //Global y coordinate of the waypoint
	int dir; //direction of the waypoint (one of the 4 available directions)
	int next1; //Global index of the first alternative next waypoint
	int next2; //Global index of the second alternative next waypoint
	int next3; //Global index of the third alternative next waypoint
	sf::Texture texture; //waypoint texture object
	sf::Sprite sprite; //waypoint sprite object
	
	public:
		Waypoint(tWayPointdir dir, tRoadTileType type, int row, int col, int idx, int next1, int next2, int next3);
		int getNext();
		void getPosition(float &x, float &y, float &dir);
		void draw();
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
			path+="corner-cross.png";
			break;
		case SH:
			path+="corner-straight-horizontal.png";
			break;
		case SV:
			path+="corner-straight-vertical.png";
			break;
		case TB:
			path+="corner-t-bottom.png";
			break;
		case TL:
			path+="corner-t-left.png";
			break;
		case TR:
			path+="corner-t-right.png";
			break;
		case TT:
			path+="corner-t-top.png";
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

int main()
{
	sf::RenderWindow window(sf::VideoMode(1195,1195), "Traffic Simulator");
 	

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

		 RoadTile rt(CTL,1,1);
		 RoadTile r2(CTR,1,5); 
		 RoadTile r3(CBR,5,5);
		 RoadTile r4(CBL,5,1);
		 
		 rt.draw(window);
		 r2.draw(window);
		 r3.draw(window);
		 r4.draw(window);
		 		 
		//Update the display
		window.display();		
	}
	return 0;
}
