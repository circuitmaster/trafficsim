INRODUCTION : 

-This project is the second part of the traffic simulation project .  

TEAM MEMBERS : 

-Ali Emre ÖZTAŞ : Algorithm Creating , Coding , Mathematics 

-Mahir DEMİR : Algorithm Creating , Coding , GUI

IMPLEMENTATİON : 

We have implemented two classes . Which are :

--TrafficLight
--TrafficLightGroup

-The positioning of the other cars are random , and the way to place them is the same as first part of the project . First 4 cars are placed to the corners of our map . The other two are placed in the starting and ending points of middle section . 

-The traffic lights are placed by the rule given . In our map , only two roadpieces had multiple inbound traffic , which has 3 -- which has 2 , so we placed 5 traffic lights . Each traffic light had been placed to the right to the inbound waypoint . 

-We have two traffic light groups for 2 roadtiles . One has 3 , other has 2 traffic lights linked to it .

-We connected the traffic lights in the same group via circular link list . We selected had as leftmost light and traversed in the list in clockwise rotatation with simulate function . Some codes we used for this are : 

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

and :

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

Of course we reconciled the waypoints with traffic lights 

-The second important addition is when the light is red , the car is stopping . We did it in for loop . We know which waypoint car is onto . As we have already reconciled the waypoints with traffic lights , we are able to learn the state of light , so in if condition , if it's red , we set the incrementation of car to 0 :

if(arr[j].l->getState()==Red){   //Here it checks the traffic lights state
	car[i].increment=0; 
	break;
}else{
	car[i].increment=1;
}

-As we had added more cars , we put all of the properties of cars into a array

-The third important part is to prevent the cars from collition . We solved this problem by putting a pre-determined distance value between them and setting the back car's inrement to 0 when collision may happen :

if(x[i]==x[j] && abs(y[i]-y[j])<80){
	int k = (y[i]-y[j])/abs(y[i]-y[j]);
	switch((int)next_dir[j]){
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

DISCUSSION : 

-The main difficulity was writing the collision code . In there , there were two hard problems to solve :

--Determining to position of the car which had to be stopped was hard . We spend a lot of time to find which is on the back . We solved it switching in respect to direction .
--When we set the car's speed to 0 , it was indefinitely stooped . We knew it must be stopped but we figured late that it should also be re-drived . 
