#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
int main()
{
 sf::RenderWindow window(sf::VideoMode(800, 600), "Traffic Simulator");
 sf::Texture texture;
 if (!texture.loadFromFile("images/vehicles/car1.png"))
 {
 cout << "Could not find the image file" << endl;
 }
 sf::Sprite sprite;
 sprite.setTexture(texture);
 int x = 150;
 int y = 150;
 int increment = 1;

 //Normally sprite rotation reference is the sprite's top left corner. To rotate the sprite around its center point, we need to set the
 //rotation origin to the sprite's center. This is how we do it:
 //Get the bounding box coordinates for the sprite
 sf::FloatRect boundingBox = sprite.getGlobalBounds();
 //Set the sprite rotation origin to the center of the bounding box
 sprite.setOrigin(sf::Vector2f(boundingBox.width / 2, boundingBox.height / 2));

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
 //Move car sprite to x,y position
 sprite.setPosition(x, y);

 //Draw the car sprite to screen
 window.draw(sprite);

 //Update the display
 window.display();

 //Change x-coordinate of the car sprite
 x+= increment;
 if (x == 250) {
 //if the car has reached x position of 250, rotate it by 180 degrees (car faces to the left now)
 sprite.setRotation(180);
 //the sprite now moves to the left
 increment = -1;
 }
 else if (x == 150)
 {
 //if sprite has reached back to x position of 150, rotate it back to 0 degrees (car faces to the right now)
 sprite.setRotation(0);
 //the sprite now moves to the right
 increment = 1;
 }

 //Wait 0.01 seconds before looping again
 sf::sleep(sf::seconds(0.01f));
 }
 return 0;
} 
