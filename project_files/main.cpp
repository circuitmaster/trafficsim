#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

enum tRoadTileType{CBL=1,CBR,CTL,CTR,C,SH,SV,TB,TL,TR,TT};
enum tVehicleType{car1=1, car2, car3, car4, car5, car6};
enum WayPointType{Down=1,Left,Right,Up};

class RoadTile
{
	private:
		float x; //x coordinate of the top left corner of the road tile
		float y; //y coordinate of the top left corner of the road tile
		sf::Texture texture; //tile texture object
		sf::Sprite sprite; //tile sprite object
		
	public: 
		RoadTile(tRoadTileType t, int row , int col);
		void draw(); 
		
};

class Vehicle{
	private:
		float x, y, angle;
		bool origin_set;
		sf::Texture texture; 
		sf::Sprite sprite;
	
	public:
		Vehicle(tVehicleType t, float x, float y, float angle);
		void move(float &x, float &y, float &angle);
};

class Waypoint{
	float x; //Global x coordinate of the waypoint
	float y; //Global y coordinate of the waypoint
	int dir; //direction of the waypoint (one of the 4 available directions)
	int next1; //Global index of the first alternative next waypoint
	int next2; //Global index of the second alternative next waypoint
	int next3; //Global index of the third alternative next waypoint
	sf::Texture texture; //waypoint texture object
	sf::Sprite sprite; //waypoint sprite object
	
	public:
		Waypoint(tWaypointDir dir, tRoadTileType type, int row, int col, int idx, int next1, int next2, int next3);
		int getNext();
		void getPosition(float &x, float &y, float &dir);
		void draw();
};
int main()
{

	return 0;
}
