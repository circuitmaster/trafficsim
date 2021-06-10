INRODUCTION : 

-This project is the third part of the traffic simulation project .  

TEAM MEMBERS : 

-Ali Emre ÖZTAŞ : Algorithm Creating , Coding , Mathematics 

-Mahir DEMİR : Algorithm Creating , Coding , GUI

IMPLEMENTATİON : 

We have implemented four classes :

--Vehicle
--Car
--Bus
--BusStop

-From the previous part of the project , we had vehicle class . We tranformed it to abstract class and made its function virtual too . 

-We inherited the car and bus classes from vehicle . We inherited only move2 function . 

-In bus class , aside from car , there is a vector object which holds indexes of BusStops . Also we hold current index in the class too . 

-For BusStop , we made eight of them in an array . Each bus contains five of them and two of them are common between two buses . This array is circular , meaning when the last stop is reached , the counter resets to first stop . 

-Also , in previous part we had the algorithm for finding next waypoint not in the move function but in main . For purposes of abstract class , we transferred that part to move function . 

-The main difference is in the move2 function . In car , the next waypoint was chosen randomly . However , in buses function we can not do it . To move it between BusStop to BusStop , in for loop , we checked distance from each next waypoint to next BusStop . Then , we took the minimum one and the buses goes that way . In this piece of code it can be seen :

float min = 10000; 
for(int k=0; k<48; k++){
int cl,rw;
float x_,y_,dir_;
arr[k].getPosition(x_,y_,dir_);	
cl = (int)(x_/239) + 1; 
rw = (int)(y_/239) + 1;
if(idx == 3 && arr[k].getIndex() == 3 && cl==col && rw==row){
this->next_x = x_; 
this->next_y = y_;
this->next_dir = dir_;
break;
}
else if(arr[k].getIndex()!=arr[j].getIndex() && cl==col && rw==row){
if(abs(x_ - stopx) + abs(y_ - stopy) < min){	
this->next_x = x_; 
this->next_y = y_;
this->next_dir = dir_;
min = abs(x_ - stopx) + abs(y_ - stopy);
}
}	
}

-Also , we had improved the collision detector for detecting between bus-bus and car-bus . 

DISCUSSION : 

-Ablsoultely the hardest part was transferring our next waypoint finding algorithm inside the move function . We almost changed everything . 

-In previous parts , after a certain time , the texture memory was filled . We didn't undersood the problem until we realized we had our draw function's texture load part was in while loop . We solved this sneaky problem by transferring it to the constructors . It had dominating effect in the speed of simulation . 







-
	

