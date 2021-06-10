INRODUCTION : 

-This project is a traffic simulation . The aim is to learn class implementation of c++ language. 

TEAM MEMBERS : 

-Ali Emre ÖZTAŞ : Algorithm Creating , Coding , Mathematics 

-Mahir DEMİR : Algorithm Creating , Coding , GUI

IMPLEMENTATİON : 

We have implemented three classes . Which are :

--Vehicle
--Waypoint
--Roadtiles

-The positioning of roadtile : We take origin (0,0) as upper left corner . Then we created five rows and columns each have 239 px's . We calculated each position of roadtile with the formula of : x = (row-1)*239 , y = (col-1)*239 . We took row and col numbers and calculated the position in constructor . 

-The positioning of waypoints : We took each roadtile's upper left corner as an origin of waypoints which those waypoints belongs to . Then , in draw function , by using switch - case , we draw each waypoints in predetermined relative coordinates . 

-The movement of car : First , we calculated which waypoint the car is onto now.Then ,we calculated the local next waypoint with GetNext function . Then we have two different movements :

--The non-smooth one : We have 8 different cases (e.g from bottom to right , up to left etc.) , so we changed the car's x or y coordinates according to where is the next waypoint . Then , when the wanted x or y coordinate is reached , car turns according to next waypoint's angle . In this movement , the orbit of movement is rectengular . 

--The smooth one : Likewise the above , we have 8 different cases . For example , in first case (from bottom to right) , We changed the angle from 270 degree to 360 degree . With respect to angle , we changed x and y coordinates according to formula : 

this->x = x+(sin(this->angle*3.141592653589793238463/180)) * 100;
this->y = w_y-(cos(this->angle*3.141592653589793238463/180)) * 97;   

this->x : x coordinate of car 
this->y : y coordinate of car 
x : x coordinate of next waypoint
w_y :  x coordinate of current waypoint

For other cases , the formula is arranged according to positions of current and next waypoints . In this case , the orbit of movement is circular. 

DISCUSSION : 

In the progress of project , we had two main difficulities : 

-First , the part of finding which waypoint car is onto and finding it's next was hard . We solved it by a for loop that compares the positions in each waypoint with the cars position . It was life-saver that we used an array while we were describing waypoints . 

-The second hardship was of course finding the smooth movement . Althrough we had searched it in internet , the formulas we found there couldn't solve our problem . After a few day's
rest and thinking , we created a modification that solves our problem from formulas we found from internet .   



 
 
