/* Game code    

--game code accessible functions

--general
void showMessage(int number)						- display the message (number)
void cls()											- clear the screen
void hold(int duration)								- pause for an amount of time
void gotoRoom(int roomNumber)						- move player to room (roomNumber)
void exitGame()										- finish the game

--counter functions
void dec(int counter) 								- decrease the counter by 1. Won't go below 0.
void inc(int counter) 								- increase the counter by 1. Won't go above 255.
int get(int counter) 								- returns the value of the counter.
int isOn(int number) 								- returns false if 0, true if not zero.
void set(int counter,int value)						- sets the counter to the value.
void on(int number)									- sets the counter to 1.
void off(int number)								- sets the counter to 0.

--inv and encumbrance
int objectWeight(int number)						- returns the weight of the object
int objectMessage(int number)						- returns the message number associated with the object number
int objectLocation(int number)						- gets the location id of the object
void setObjectLocation(int number, int location) 	- sets the location id (room number) of the object
int objectHere(int number,int roomNumber)			- checks if the object (number) is in the room (roomNumber) and returns message if it is
int here(int number, int roomNumber)				- returns true if the object (number) is in the room (roomNumber) otherwise returns false
int inInv(int number)								- checks if the object (number) is in the players inv
int available(int number,int roomNumber)			- checks if the object (number) is either in the players inv or the current room
void swapObject(int number1,int number2)			- moves object 1 to the bin (room 0) and object to inv (player location)
void moveObject(int number, int roomNumber)			- moves object (number) to room (roomNumber), seems to be a duplicate of setObjectLocation...
void takeObject(int number)							- check to see if the player can take the object (number), if so move to inv
void dropObject(int number)							- drop object (number)
void showInventory()								- lists objects in inv (player location)
void find(int object)								- get location of object (object) and move player to it

variables

verb		- verb from parser
adverb		- adverb from parser
noun		- noun from parser
noun2		- second noun in sentence
clear		- if clear = 1 call CLS
cw			- current carry weight
exitFlag	- main game loop flag, game continues while =0
room		- current player room number 

main game loop

	1 preRoom()
	2 if in new room show description
	3 highPriority()
	4 parse input (wait for player)
	5 check for moves and move if a valid direction entered - goto to 1
	6 lowPriority()
	7 if not dead or game complete goto 1

*/

enum Counters {
  ISDARK = 1,
    LAMP = 2,
    RATFED = 3,
    LEFTINTHEDARK = 4,
    FIRSTTURN = 5,
    FOUNDKEY = 6,
    DOORLOCKED = 7,
	OILLEFT = 8
};

void initialiseGame() {
  /* set any variables here */

  set(LEFTINTHEDARK, 3);
  createChar();
  setStat(ENC, 0);
  setStat(MAXENC, 110);
  set(OILLEFT, 20);
}


void preRoom() {
  off(ISDARK);

  //set dark for required rooms
  if (room == 2 || room == 3 || room == 6) {
    on(ISDARK);
  }

}

int highPriority() {
  //enum Counters counter;

  if (isOn(ISDARK) == true && isOn(LAMP) == false) {
    dec(LEFTINTHEDARK);
  }
  if (isOn(ISDARK) == true && isOn(LAMP) == false && get(LEFTINTHEDARK) == 1) {
    showMessage(38);
  }
  if (isOn(ISDARK) == true && isOn(LAMP) == false && get(LEFTINTHEDARK) == 0) {
    /*killed by schelob */
    hold(500);
    showMessage(37);
    exitGame();
    return true;
  }
  if (room == 3 && available(2, room) == false && isOn(RATFED) == false) {
    /*killed by the snake*/
    hold(50);
    showMessage(31);
    exitGame();
    return true;
  }
  if (room == 3 && available(2, room) == true && isOn(RATFED) == false) {
    /*snake eats rat */
    hold(50);
    showMessage(39);
    on(RATFED);
    moveObject(2, 0);
    setStat(ENC, getStat(ENC) - objectWeight(2));
    return true;
  }
  if (room == 1 && available(4, room) == true) {
    /* you win, yay */
    showMessage(45);
    exitGame();
    return true;
  }

  return false;
}

int lowPriority() {
	
	//return -1 if nothing happened, e.g. action not possible
	//return 0 if no time taken, e.g. load or save.
	//return 1+ if time taken. Return -1 will be added to the current turn
  if (verb == 14) {
    saveGame();
    return 0;
  }
  if (verb == 15) {
    loadGame("SAVEM.DAT");
    getRoom(room);
    return 0;
  }
  if (verb == 17 && noun == 9) {
    clear = 1;
    return 0;
  }
  if (verb == 18 && noun == 9) {
    clear = 0;
    return 0;
  }
  
  if ((verb ==16 && noun == 1) && (objectHere(5, room)|| objectHere(1, room) )) {
    showMessage(57);
    return 1;	  
  }
  
  if (room == 3 && verb == 2) {
    showMessage(42);
    exitGame();
    return 0;
  }
  if (room == 4 && verb == 16 && noun == 8 && isOn(FOUNDKEY) == false) {
    on(FOUNDKEY);
    showMessage(43);
    moveObject(3, 4);
    return 1;
  }
  if (room == 5 && verb == 19 && available(3, 5) == true && noun == 6 && isOn(DOORLOCKED) == false) {
    on(DOORLOCKED);
    showMessage(44);
    return 1;
  }
  if (room == 5 && verb == 3 && isOn(DOORLOCKED)) {
    gotoRoom(6);
    return 0;
  }
  if (verb == 16) {
    showMessage(41);
    return 1;
  }
  if (verb == 7 && noun == 1 && objectHere(5, room)) {
    takeObject(5);
    return 0;
  }
   
  if (verb == 8 && noun == 1 && inInv(5)) {
    dropObject(5);
    return 0;
  }
  if (verb == 7 && noun < 5) {
    takeObject(noun);
    return 0;
  }
  if (verb == 8 && noun < 5) {
    dropObject(noun);
    return 0;
  }
  if (verb == 9) {
    getRoom(room);
    return 0;
  }
  if (verb == 10) {
    showInventory();
    return 0;
  }
  if (verb == 11) {
    exitFlag = 1;
    return 0;
  }
  if (verb == 17 && noun == 1 && inInv(1)) {
	//lamp on
	if (get(OILLEFT)==0)
	{
		showMessage(56);
		return 0;
	}
	else{
		swapObject(1, 5);
		on(LAMP);
		showMessage(35);
		return 1;		
	}

  }

  if (verb == 20) {
    showMessage(22);
    return 0;
  }

  if (verb == 18 && noun == 1 && inInv(5)) {
	  //lamp off
    swapObject(5, 1);
    off(LAMP);
    showMessage(36);
    return 1;
  }
  if (verb == 21 && available(2, room) && noun == 2) {
	  //you eat the rat
    showMessage(34);
	setStat(ENC, getStat(ENC) - objectWeight(2));
    moveObject(2, 0);
    return 1;
  }

  if (verb == 22) {
    showCharacter();
    return 0;
  }

  return -1;
}

void afterAction()
{
	//check if the lamp is on and if so reduce amount of oil remaining
	if (isOn(LAMP))
	{
		//reduce amount of oil if lamp is on
		dec(OILLEFT);
	}
	
	if (get(OILLEFT)==4 && isOn(LAMP)){
		//flickering
		printf("\n");
		showMessage(54);
	}
	
	if (get(OILLEFT)==0 && isOn(LAMP)){
		//out of oil so switch it off
		printf("\n");		
		showMessage(55);
		swapObject(5, 1);
		off(LAMP);		
	}
	//printf("Oil : %d\n", get(OILLEFT));
	
}