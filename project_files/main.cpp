#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

class RoadTile
{
	private:
		float x; //x coordinate of the top left corner of the road tile
		float y; //y coordinate of the top left corner of the road tile
		sf::Texture texture; //tile texture object
		sf::Sprite sprite; //tile sprite object
		
	public : 
		RoadTile(RoadTile type , int row , int col) ;
		
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

int main()
{

	return 0;
}
