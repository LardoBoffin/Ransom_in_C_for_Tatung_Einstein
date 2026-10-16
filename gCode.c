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
int available(room,int number,int roomNumber)			- checks if the object (number) is either in the players inv or the current room
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
nounTwo		- second noun in sentence
clear		- if clear = 1 call CLS
cw			- current carry weight
exitFlag	- main game loop flag, game continues while =0
room		- current player room number 

main game loop

	1 beforeRoomDescription() - fires before description of room shown
	2 if in new room show description
	3 beforePlayerInput()
	4 parse input (wait for player)
	5 check for moves and move if a valid direction entered - goto to 1
	6 afterPlayerInput()
	7 if a valid action then increment turn counter and call afterAction()
	8 if not dead or game complete goto 1

*/

enum Counters {
    DARK = 1,
    LIGHT = 2,
    SOCEITY = 3,
    SKILL = 4,
    MUTATION = 5,
    CLONE = 6,
    LATECTR = 7,
	SUCCESS = 8,
	BRIEFING = 9,
	TREES = 10,
    PLASMA = 11,
    ROPE=12,
    BOATREADY=13,
    CUPBOARDFOUND=14,
    CUPBOARDOPEN=15,
    BEENOUTSIDE=16,
    TELEPORTERPATCHED=17,
    GPSON=18,
    OFFICESPOTTED=19,
    FORESTMAZE=20,
    ROPETIED=21,
    BOATPOSITION=22,
    SIERRACLUB=23,
    ALCOVE=24,
    TURRETDEAD=25,
    DOORLOCKED=26,
    BOTDEAD=27,
    MAZEMESSAGESHOWN=28,
    SIERRATHISCLONE=29,
    PATCHFOUND=30,
    CURRENTSTAGE=31,
    GPSTAKEN=32,
    COORDSENTERED=33,
    MENUPOSITION=34,
    ROOMTOTELEPORT=35,
    GPSENTERED=36,
    DISTANCEOFFSET=37,
    DIRECTIONOFFSET=38,
    DISCUSED=39,
    CHIPFOUND=40,
    SCORE1=41,
    SCORE2=42,
    SCIENCEDOOROPEN=43,
    RUBBLECLEARED=44,
    RND1=45,
    RND2=46,
    RND3=47,
    RND4=48,
    RND5=49

};

void stg(int number) {
 int s = get(CURRENTSTAGE);
 if (s > 7 && number==7){
    return;
 }else{
    set(CURRENTSTAGE,number);
 }
}

void moveAll(){
printf("Move all called\n");

}

int rn(int max) {
  /* return a random number between 1 and max */
  int r;
  r = rand();
  r = (r % max) + 1;
  return r;
}

void setAchievements(int number) {
  /* set the achievement (number) */

}

void WaitForKey() {
    printf("\nPress any key to continue...");
  while (getk() == 0) { }
}

void die() {
  /* player dies */
  inc(6);
  if (get(CLONE)>8){
        showMessage(76);
        exitFlag=1;
  }
  printf("\n");
  WaitForKey(); 
  gotoRoom(1);
}



int rm10(void)
{
    int ST = get(CURRENTSTAGE);

    if (verb == 20 && ST == 1) {
        showMessage(128);
        showMessage(183);
        showMessage(184);
        showMessage(129);
        return 1;
    }

    if (verb == 20 && ST == 2) {
        showMessage(128);
        showMessage(185);
        return 1;
    }

    if (verb == 20 && ST == 3) {
        showMessage(128);
        showMessage(189);
        return 1;
    }

    if (verb == 20 && ST == 4) {
        showMessage(128);
        showMessage(186);
        return 1;
    }

    if (verb == 20 && ST == 5) {
        showMessage(128);
        showMessage(187);
        return 1;
    }

    if (verb == 20 && ST == 6) {
        showMessage(128);
        showMessage(190);
        showMessage(191);
        return 1;
    }

    if (verb == 20 && ST == 7) {
        showMessage(128);
        showMessage(192);
        return 1;
    }

    if (verb == 20 && ST > 7) {
        if (get(MUTATION) != 2) {
            showMessage(128);
            showMessage(193);
            hold(100);
            showMessage(200);
            setAchievements(5);
            die();
            return 1;
        }
    }

    if (verb == 20 && ST > 7) {
        showMessage(128);
        showMessage(193);
        showMessage(201);
        return 1;
    }

    return 0;
}


int rm11(void)
{
    if (verb == 20 && (noun == 16 || noun == 38)) {
        if (get(SKILL) == 1 && !isOn(TELEPORTERPATCHED)) {
            showMessage(98);
            return 1;
        }
    }

    if ((verb == 41 || verb == 36) && noun == 39) {
        if (get(SKILL) == 1 && isOn(PATCHFOUND) && !isOn(TELEPORTERPATCHED)) {
            on(TELEPORTERPATCHED);
            on(COORDSENTERED);
            showMessage(209);
            return 1;
        }
    }

    if ((verb == 47 || verb == 33) && noun == 42) {
        if (isOn(COORDSENTERED)) {
            showMessage(215);
            hold(200);
            putchar('\n');
            showMessage(218);
            putchar('\n');
            putchar('\n');

            room = 48;
            lastRoom = room;
            getRoom(room);

            return 1;
        }
    }

    if ((verb == 47 || verb == 33) && noun == 42) {
        if (!isOn(COORDSENTERED)) {
            showMessage(242);
            return 1;
        }
    }

    if (verb == 20 && (noun == 16 || noun == 38)) {
        showMessage(97);
        return 1;
    }

    if ((verb == 41 || verb == 36) && noun == 39) {
        if (get(SKILL) == 1 && isOn(PATCHFOUND) && isOn(TELEPORTERPATCHED)) {
            showMessage(219);
            return 1;
        }
    }

    return 0;
}


void l1(void)
{
    showMessage(204);
    putchar('\n');
    showMessage(205);
    putchar('\n');
    showMessage(206);
    putchar('\n');
    showMessage(207);
}


void l2(void)
{
    showMessage(210);
    putchar('\n');
    showMessage(211);
    putchar('\n');
    showMessage(212);
    putchar('\n');
    showMessage(207);
}


int rm21(void)
{
    if ((!isOn(GPSON) || !available(room,1)) && !isOn(FORESTMAZE) && verb < 11) {
        lastRoom = 17;
        moveAll();
        room = 21;
        return 1;
    }

    if (verb == 3 && room == 21) {
        room = 22;
        return 1;
    }

    if (verb == 4 && room == 21) {
        room = 17;
        return 1;
    }

    return 0;
}


int rm22(void)
{
    if (verb == 29 && noun == 3) {
        if (available(room,3)) {
            if (get(SKILL) == 3) {
                set(ROPETIED, 2);
                showMessage(138);
                moveObject(3, 22);
                return 1;
            }
        }
    }

    if (verb == 29 && noun == 3) {
        if (available(room,3)) {
            if (get(SKILL) != 3) {
                on(ROPETIED);
                showMessage(139);
                moveObject(3, 22);
                return 1;
            }
        }
    }

    if (!isOn(BOATREADY) && verb == 22 && noun == 3 &&
        available(room,3) && !isOn(ROPETIED)) {
        showMessage(137);
        setAchievements(16);
        moveObject(3, 0);
        return 1;
    }

    if (!isOn(BOATREADY) && verb == 22 && noun == 3 &&
        available(room,3) && get(MUTATION) == 4 && get(ROPETIED) == 1) {
        showMessage(140);
        setAchievements(16);
        moveObject(3, 0);
        return 1;
    }

    if (!isOn(BOATREADY) && verb == 22 && noun == 3 &&
        available(room,3) && get(MUTATION) == 4 && get(ROPETIED) == 2) {
        showMessage(135);
        on(BOATREADY);
        stg(3);
        set(BOATPOSITION, 22);
        moveObject(3, 0);
        return 1;
    }

    if (!isOn(BOATREADY) && verb == 22 && noun == 3 && get(MUTATION) < 4) {
        showMessage(136);
        off(BOATREADY);
        return 1;
    }

    if (!isOn(BOATREADY) && verb == 3) {
        showMessage(94);
        setAchievements(9);
        putchar('\n');
        die();
        return 1;
    }

    if (isOn(BOATREADY) && verb == 3) {// && get(BOATPOSITION) == 22
        gotoRoom(49);
        return 1;
    }

    return 0;
}



void rndshowMessage(){
    int r;
    printf("\n");
    showMessage(19);
    r=rn(10);
    showMessage(r+19);
    setAchievements(r+19);
    showMessage(72);
    printf("\n");
    //LOCAL R%:PROCshowMessage(19):R%=FNrn(10):PROCshowMessage(R%+19):PROCsetAchievements(19+R%):PROCshowMessage(72):PRINT:PRINT
}

int rndEnc(void){
    int ENC = 0;

    if (isOn(49) || rn(100) > 6) {
        return false;
    }

    do {
        ENC = rn(4);
        if (isOn(44 + ENC)) {
            ENC = -1;
        }
    } while (ENC <= 0);

    showMessage(266 + ENC);
    on(44 + ENC);
    if (isOn(45) && isOn(46) && isOn(47) && isOn(48)) {
        on(49);
    }

    putchar('\n');
    if (ENC == 1) {
        if (available(room,1)) {
            showMessage(275);
        } else {
            showMessage(276);
        }
        setAchievements(30);
        return false;
    }

    if (ENC == 2) {
        setAchievements(31);
        return false;
    }

    if (ENC == 3 && get(SKILL) == 2 && inInv(1)) {
        showMessage(271);
        putchar('\n');
        setAchievements(32);
        return false;
    }

    if (ENC == 3 && !inInv(1)) {
        showMessage(277);
        showMessage(278);
        setAchievements(18);
        setAchievements(32);
        die();
        return true;
    }

    if (ENC == 3 && inInv(1) && get(SKILL) != 2) {
        showMessage(272);
        showMessage(278);
        setAchievements(32);
        die();
        return true;
    }

    if (get(SKILL) == 2 && inInv(1)) {
        showMessage(274);
        setAchievements(33);
        die();
        return true;
    }

    if (get(SKILL) != 2 && inInv(1)) {
        showMessage(273);
        setAchievements(33);
        putchar('\n');
        return false;
    }

    showMessage(277);
    showMessage(279);
    setAchievements(33);
    return false;
}

void rmshowMessage(void) {
if (room==4) {
    if (isOn(CUPBOARDFOUND)) {
        showMessage(95);
    }
}
if (room==7) {
    if (! isOn(PLASMA)) {
        showMessage(83);
    }
}
if (room==8) {
    if (get(MUTATION)==1){
        on(OFFICESPOTTED);
    }
    if (isOn(OFFICESPOTTED)) {
        showMessage(127);
    }
}
if (room==12) {
    if (get(SOCEITY)==4) {
        showMessage(82);
    }
}
if (room==13) {
    if (get(SOCEITY)==4 || get(SKILL)==3) {
        showMessage(92);
    }
}
if (room==14) {
    if ((get(SOCEITY)==4 || get(SKILL)==3) && !isOn(ROPE)) {
        showMessage(93);
    }
}
if (room==21) {
    if (isOn(GPSON) && !isOn(FORESTMAZE) && available(room,1)) {
        on(FORESTMAZE);
        showMessage(171);
    }
}
if (room==21) {
    if (isOn(FORESTMAZE)) {
        showMessage(166);
    }
}
if (room==22) {
    if (isOn(BOATREADY) && get(BOATPOSITION)==22) {
        showMessage(90);
    }
}
if (room==22) {
    if (! isOn(BOATREADY) && get(BOATPOSITION)==27) {
        showMessage(91);
    }
}
if (room==27) {
    if (get(BOATPOSITION)==27) {
        showMessage(90);
    }
}
if (room==28) {
    if (get(BOATPOSITION)==22) {
        showMessage(90);
    }
}
if (room==39) {
    if (get(DOORLOCKED)==1) {
        showMessage(165);
    }
}
if (room==43) {
    if (get(BOTDEAD)==1) {
        showMessage(173);
    }
}
if (room==48) {
    if (get(SOCEITY)==3) {
        showMessage(248);
    }
}
    return;
}


void win(void) {
    int S = 0, L = 0;
    L=get(CLONE);
    cls();
if (L==8) {
    S=10;
}
if (L==7) {
    S=50;
}
if (L==6) {
    S=100;
}
if (L==5) {
    S=200;
}
if (L==8) {
    showMessage(243);
} else {
    showMessage(241);
}
if (isOn(41)) {
    S=S+5;
}
if (isOn(42)) {
    S=S+10;
}
if (turn<200) {
    setAchievements(19);
}
if (turn>300) {
    setAchievements(34);
}
    putchar('\n');
    showMessage(11);
    printf("%d", S);
    showMessage(12);
    printf("%d", turn);
    showMessage(17);
    showAchievements();
    return;
}

void initialiseGame() {
  /* set any variables here */

  createChar();
  setStat(ENC, 0);
  setStat(MAXENC, 110);
  on(CLONE);
  set(BOATPOSITION,27);

}


void beforeRoomDescription() {
    set(DARK,0);
    if (room==27 && lastRoom==26) {
        set(BOATPOSITION,27);
    }
    if (room==22 && lastRoom==49) {
        set(BOATPOSITION,22);
    }
    if (room==19) {
        on(DARK);
    }
        return;
}

int beforePlayerInput() {
  //event fires before player input

  rmshowMessage();

if (room==13 || room==16 || room==17 || room==18 || room==29 || room==30 || room==32 || room==33) {
    if (rndEnc()) {
        return true;
    }
}
if (room==4) {
    if (! isOn(BRIEFING) && !isOn(LATECTR)) {
        on(LATECTR);
        showMessage(77);
    }
}
if (isOn(LATECTR) && !isOn(BRIEFING)) {
    inc(7);
}
if (room==5) {
    if (get(LATECTR)<4 && !isOn(BRIEFING)) {
        on(BRIEFING);
        inc(31);
        showMessage(78);
        putchar('\n');
        showMessage(79);
        putchar('\n');
        showMessage(80);
        putchar('\n');
        showMessage(81);
        return true;
    }
}
if (room==5) {
    if (isOn(SUCCESS)) {
        win();
        exitFlag=1;
        return true;
    }
}
if (room==5) {
    if (get(LATECTR)>3 && !isOn(BRIEFING)) {
        off(LATECTR);
        showMessage(71);
        showMessage(75);
        putchar('\n');
        rndshowMessage();
        die();
        return true;
    }
}
if (room==7) {
    if (available(room,13) && !isOn(SUCCESS)) {
        on(SUCCESS);
        return true;
    }
}
if (room==7) {
    if (isOn(BEENOUTSIDE) && !isOn(SUCCESS)) {
        showMessage(74);
        putchar('\n');
        rndshowMessage();
        die();
        return true;
    }
}
if (room==19) {
    if (! isOn(LIGHT) && rn(100)>=70) {
        showMessage(157);
        setAchievements(10);
        die();
        return true;
    }
}
if (room==21) {
    if (! isOn(GPSON) && !isOn(FORESTMAZE) && rn(100)>=90) {
        showMessage(114);
        setAchievements(4);
        die();
        putchar('\n');
        return true;
    }
}
if (room==28) {
    if (get(SIERRACLUB)==0 && get(SOCEITY)<4) {
        showMessage(150);
        showMessage(151);
        die();
        return true;
    }
}
if (room==28) {
    if (get(SOCEITY)==4 && !isOn(SIERRATHISCLONE)) {
        on(SIERRACLUB);
        on(SIERRATHISCLONE);
        stg(4);
        showMessage(152);
        return false;
    }
}
if (room==28) {
    if (isOn(SIERRACLUB) && get(SOCEITY)<4 && !isOn(SIERRATHISCLONE)) {
        on(SIERRATHISCLONE);
        showMessage(153);
        return false;
    }
}
if (room==25) {
    setAchievements(9);
    die();
    return true;
}
if (room==31) {
    setAchievements(12);
    die();
    return true;
}
if (room==36 && !isOn(TURRETDEAD)) {
    showMessage(159);
    die();
    return true;
}
if (room==43) {
    if (! available(room,9) && !isOn(BOTDEAD)) {
        showMessage(172);
        die();
        return true;
    }
}
if (room==43) {
    if (available(room,9) && !isOn(BOTDEAD)) {
        showMessage(175);
        return false;
    }
}
    return false;
}

int afterPlayerInput() {	
	//return -1 if nothing happened, e.g. action not possible
	//return 0 if no time taken, e.g. load or save.
	//return 1+ if time taken. Return will be added to the current turn (a slow action could take 2 or more turns)
	
 if (verb==21 && noun==32) {
    set(SOCEITY,4);
    on(SKILL);
    on(MUTATION);
    on(BRIEFING);
    on(PLASMA);
    gotoRoom(4);
    return true;
}
if (room==1 && verb==21 && noun>15 && noun<20) {
    off(BEENOUTSIDE);
    set(SOCEITY,noun-15);
    gotoRoom(2);
    return true;
}
if (room==2 && verb==21 && noun>19 && noun<24) {
    set(SKILL,noun-19);
    gotoRoom(3);
    return true;
}
if (room==3 && verb==21 && noun>23 && noun<28) {
    set(MUTATION,noun-23);
    gotoRoom(4);
    return true;
}
if (room==4 && verb==20) {
    if (isOn(CUPBOARDOPEN)) {
        showMessage(95);
        return true;
    }
}
if (room==4 && verb==20) {
    if (! isOn(CUPBOARDOPEN)) {
        on(CUPBOARDOPEN);
        showMessage(96);
        showMessage(179);
        moveObject(4,4);
        return true;
    }
}
if (room==6 && verb==20 && noun==0) {
    showMessage(233);
    return true;
}
if (room==6 && (verb==46 || verb==33) && noun==11) {
    if (available(room,11) && !isOn(DISCUSED)) {
        on(DISCUSED);
        moveObject(12,6);
        moveObject(11,0);
        showMessage(238);
        return true;
    }
}
if (room==7 && verb==24 && noun==30 && adverb==1 && nounTwo==1) {
    if (available(room,1) && !isOn(PLASMA) && get(MUTATION)==2) {
        showMessage(86);
        setAchievements(1);
        return true;
    }
}
if (room==7 && verb==24 && noun==30 && adverb==0 && nounTwo==0) {
    if (!isOn(PLASMA)) {
        showMessage(84);
        rndshowMessage();
        die();
        return true;
    }
}
if (room==7 && verb==24 && noun==30 && adverb==1 && nounTwo==1) {
    if (available(room,1) && !isOn(PLASMA) && get(MUTATION)!=2) {
        showMessage(85);
        on(PLASMA);
        stg(2);
        rndshowMessage();
        die();
        return true;
    }
}
if (room==7 && verb==3 && isOn(PLASMA)) {
    gotoRoom(12);
    on(BEENOUTSIDE);
    return true;
}
if (room==8 && verb==20) {
    if (! isOn(OFFICESPOTTED) && get(MUTATION)==1) {
        on(OFFICESPOTTED);
        showMessage(127);
        return true;
    }
}
if (room==8 && verb==2) {
    if (isOn(OFFICESPOTTED)) {
        gotoRoom(10);
        return true;
    }
}
if (room==10) {
    if (rm10()) {
        return true;
    }
}
if (room==11) {
    if (rm11()) {
        return true;
    }
}
if (room==13 && (verb==5 || verb==26)) {
    if (get(SOCEITY)==4 || get(SKILL)==3) {
        gotoRoom(14);
        return true;
    }
}
if (room==14 && verb==11 && noun==28) {
    showMessage(188);
    return true;
}
if (room==14 && verb==27 && noun==28) {
    if ((get(SOCEITY)==4 || get(SKILL)==3) && available(room,4) && !isOn(ROPE)) {
        moveObject(3,14);
        on(ROPE);
        showMessage(132);
        return true;
    }
}
if (room==19 && verb==20) {
    if (isOn(LIGHT)) {
        on(ALCOVE);
        showMessage(156);
        return true;
    }
}
if (room==19 && verb==4) {
    if (isOn(LIGHT) && isOn(ALCOVE)) {
        gotoRoom(20);
        return true;
    }
}
if (room==21) {
    if (rm21()) {
        return true;
    }
}
if (room==27 && verb==3) {
    if (get(BOATPOSITION)==22) {
        showMessage(94);
        setAchievements(9);
        die();
        return true;
    }
}
if (room==27 && verb==3) {
    if (get(BOATPOSITION)==27) {
        gotoRoom(26);
        return true;
    }
}
if (room==22) {
    if (rm22()) {
        return true;
    }
}
if (room==39 && verb==2) {
    if (get(DOORLOCKED)==0) {
        gotoRoom(38);
        return true;
    }
}
if (room==39) {
    if (!isOn(TURRETDEAD) && verb==34) {
        showMessage(160);
        showMessage(161);
        on(DOORLOCKED);
        return true;
    }
}
if (room==39) {
    if (!isOn(TURRETDEAD) && verb==33 && noun==26 && get(MUTATION)==3) {
        showMessage(160);
        showMessage(162);
        on(TURRETDEAD);
        stg(5);
        die();
        return true;
    }
}
if (room==40 && verb==20 && noun==0) {
    if (available(room,1) && isOn(GPSON)) {
        showMessage(195);
        putchar('\n');
        showMessage(196);
        showMessage(75);
        setAchievements(3);
        putchar('\n');
        rndshowMessage();
        die();
        return true;
    }
}
if (room==40 && verb==20 && noun==0) {
    showMessage(195);
    return true;
}
if (room==41 && verb==20 && noun==0) {
    if ((get(SOCEITY)==1 || get(SKILL)==1) && !isOn(PATCHFOUND) && available(room,12)) {
        showMessage(197);
        showMessage(198);
        on(PATCHFOUND);
        stg(8);
        return true;
    }
}
if (room==41 && verb==20 && noun==0) {
    if ((get(SOCEITY)>1 && get(SKILL)>1)) {
        showMessage(199);
        return true;
    }
}
if (room==43 && verb==24) {
    if (available(room,1) && (get(SKILL)==2 || get(SOCEITY)==2 || get(MUTATION)==1)) {
        showMessage(176);
        on(BOTDEAD);
        stg(6);
        return true;
    }
}
if (room==43 && verb==24) {
    if (available(room,1)) {
        showMessage(177);
        die();
        return true;
    }
}
if (room==44 && verb==20 && noun==0) {
    if (get(SKILL)>1) {
        showMessage(254);
        showMessage(255);
        showMessage(256);
        setAchievements(13);
        die();
        return true;
    }
}
if (room==44 && verb==20 && noun==0) {
    if (get(SKILL)==1) {
        showMessage(257);
        showMessage(258);
        setAchievements(14);
        return true;
    }
}
if (room==45 && verb==33 && noun==5) {
    if (available(room,5) && !isOn(44) && available(room,6)) {
        if (get(SOCEITY)==3){
            showMessage(262);
            setAchievements(8);
            on(44);
            return true;
        }
        else
        {
            showMessage(328);
            return true;
        }
    }
}
if (room==45 && verb==2 && !isOn(44)) {
    showMessage(246);
    return true;
}
if (room==45 && verb==2 && isOn(44)) {
    gotoRoom(46);
    return true;
}
if (room==47 && (noun==37 || noun==34 || nounTwo==37 || nounTwo==34) && (verb==37 || verb==11 || verb==18)) {
    if (available(room,1) && available(room,12) && isOn(GPSON)) {
        on(GPSTAKEN);
        showMessage(202);
        stg(9);
        set(ROOMTOTELEPORT,room);
        return true;
    }
}
if (room==47 && verb==3) {
    if (! isOn(43)) {
        showMessage(249);
        return true;
    }
}
if (room==47 && verb==3) {
    if (isOn(48)) {
        gotoRoom(48);
        return true;
    }
}
if (room==47 && verb==33) {
    if (available(room,7) && !isOn(42)) {
        showMessage(265);
        return true;
    }
}
if (room==47 && verb==33) {
    if (available(room,7) && isOn(42)) {
        showMessage(266);
        setAchievements(7);
        return true;
    }
}
if (room==48 && verb==20 && noun==0) {
    if (! isOn(40)) {
        showMessage(247);
        moveObject(13,48);
        return true;
    }
}
if (room==48 && verb==4) {
    if (! isOn(43)) {
        showMessage(249);
        return true;
    }
}
if (room==48 && verb==4) {
    gotoRoom(47);
    return true;
}
if (room==48 && verb==33 && noun==6) {
    if (available(room,6) && get(SOCEITY)==3) {
        showMessage(250);
        showMessage(251);
        showMessage(252);
        showMessage(253);
        on(43);
        return true;
    }
}
if (room==50 && verb==20) {
    showMessage(263);
    return true;
}
if (room==50 && verb==33 && noun==7 && available(room,7)) {
    showMessage(264);
    on(42);
    stg(7);
    return true;
}
if (verb==11 && noun<16) {
    takeObject(noun);
    return true;
}
if (verb==11 && noun==16) {
    takeObject(12);
    return true;
}
if (verb==12 && noun<16) {
    dropObject(noun);
    return true;
}
if (verb==13) {
    getRoom(room);
    lastRoom=room;
    return true;
}
if (verb==14) {
    showInventory();
    return true;
}
if (verb==18) {
    saveGame();
    return true;
}
if (verb==19) {
    loadGame("SAVEM.DAT");
    return true;
}
if (verb==20 && noun>15 && noun<28) {
    showMessage(noun+99);
    return true;
}
if (verb==20 && noun==2) {
    if (available(room,2)) {
        showMessage(87);
        moveObject(2,6);
        setAchievements(2);
        die();
        return true;
    }
}
if (verb==20 && noun==1) {
    if (available(room,1) && isOn(GPSON)) {
        showMessage(111);
        return true;
    }
}
if (verb==20 && noun==1) {
    if (available(room,1)) {
        showMessage(110);
        return true;
    }
}
if (verb==20 && noun==3) {
    if (available(room,3)) {
        showMessage(113);
        return true;
    }
}
if (verb==20 && noun==4) {
    if (available(room,4)) {
        showMessage(170);
        return true;
    }
}
if (verb==20 && noun==5) {
    if (available(room,5) && get(SOCEITY)==3) {
        showMessage(169);
        return true;
    }
}
if (verb==20 && noun==5) {
    if (available(room,5) && get(SOCEITY)!=3) {
        showMessage(168);
        return true;
    }
}
if (verb==20 && noun==6) {
    if (available(room,6)) {
        showMessage(167);
        return true;
    }
}
if (verb==20 && noun==7) {
    if (available(room,7)) {
        showMessage(261);
        return true;
    }
}
if (verb==20 && noun==9) {
    if (available(room,9)) {
        showMessage(174);
        return true;
    }
}
if (verb==20 && noun==11) {
    if (available(room,11)) {
        showMessage(234);
        return true;
    }
}
if (verb==20 && noun==12) {
    if (available(room,12)) {
        showMessage(237);
        return true;
    }
}
if (verb==20 && noun==13) {
    if (available(room,13)) {
        showMessage(240);
        return true;
    }
}
if (verb==20) {
    showMessage(178);
    return true;
}
if (verb==22 && noun==2) {
    if (available(room,2)) {
        showMessage(88);
        moveObject(2,room);
        return true;
    }
}
if (verb==22 && noun==4) {
    showMessage(113);
    return true;
}
if (verb==23 && noun==29) {
    if (available(room,2)) {
        showMessage(89);
        moveObject(2,6);
        die();
        return true;
    }
}
if (verb==28 && noun==34 && nounTwo==35 && available(room,1)) {
    on(GPSON);
    showMessage(16);
    return true;
}
if (verb==28 && noun==34 && nounTwo==36 && available(room,1)) {
    off(GPSON);
    showMessage(16);
    return true;
}
if (verb==30) {
    showMessage(140+rn(8));
    return true;
}
if (verb==31 && noun==6) {
    if (available(room,6) && !isOn(LIGHT)) {
        on(LIGHT);
        showMessage(154);
        return true;
    }
}
if (verb==32 && noun==6) {
    if (available(room,6) && isOn(LIGHT)) {
        off(LIGHT);
        showMessage(155);
        return true;
    }
}
if (verb==33 && noun==26) {
    if (isOn(GPSON) && available(room,1)) {
        showMessage(163);
        showMessage(164);
        setAchievements(15);
        die();
        return true;
    }
}
if (verb==33 && noun==26) {
    if (! isOn(GPSON)) {
        showMessage(163);
        return true;
    }
}
if (verb==35 && room<4) {
    showMessage(317);
    return true;
}
if (verb==35 && room>3) {
    showMessage(180);
    showMessage(114+get(SOCEITY));
    putchar('\n');
    showMessage(181);
    showMessage(118+get(SKILL));
    putchar('\n');
    showMessage(182);
    showMessage(122+get(MUTATION));
    return true;
}
if (verb==48 && noun==10) {
    if (available(room,10)) {
        moveObject(10,0);
        showMessage(259);
        showMessage(260);
        setAchievements(6);
        die();
        return true;
    }
}
if (verb==49) {
    showAchievements();
    return true;
}
    return -1;
}

void afterAction()
{
		
}