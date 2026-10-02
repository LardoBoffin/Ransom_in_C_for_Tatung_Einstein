/* Main system functions */

/* Variables */
char ctr[counterSize];

/* Code */

/*------------ counter based functions-------------------*/

void dec(int counter) {
  int value;
  value = (int) ctr[counter];
  if (value > 0) {
    value--;
    ctr[counter] = value;
  }
}

void inc(int counter) {
  int value;
  value = (int) ctr[counter];
  if (value < 256) {
    value++;
    ctr[counter] = value;
  }
}

int get(int counter) {
  return (int) ctr[counter];
  if (debug == true) {
    printf("Counter %5d is %5d", counter, ctr[counter]);
  }
}

int isOn(int number) {
  int value;
  if (debug == true) {
    value = ctr[number];
    printf("\nCtr %5d value is %5d\n", number, value);
  }

  if (ctr[number] == 0) {
    return false;
  }
  return true;
}

void set(int counter, int value) {
  ctr[counter] = value;
}

void on(int number) {
  ctr[number] = 1;
}

void off(int number) {
  ctr[number] = 0;
}

/*-------------------- object based functions ---------------*/

int setTurn(int number) {
  turn+=number;
  printf("\nTurn : %d\n",turn);
  afterAction();
  return turn;
}

int objectWeight(int number) {
  /* returns the weight of the object */
  int wgh;
  number--;
  wgh = (number * 8) + 5;
  number = objectList[wgh];
  return number;
}

int objectMessage(int number) {
  /* returns the message number for the object */
  int loc;
  loc = (number * 8) + 1;
  number = objectList[loc] * 256;
  number = number + objectList[loc + 1];
  return number;
}

int objectLocation(int number) {
  /* returns which room the object is in*/
  int loc;
  loc = (number * 8) + 3;
  number = objectList[loc] * 256;
  number = number + objectList[loc + 1];
  return number;
}

void setObjectLocation(int number, int location) {
  /*sets the location of the object */
  int loc;
  number--;
  loc = (number * 8) + 3;
  objectList[loc] = (location >> 8) & 0xff; /*msb*/
  objectList[loc + 1] = (location >> 0) & 0xff; /*lsb*/
}

int objectHere(int number, int roomNumber) {
  /* check to see if the object is here, if so return the message number */
  int location, message;
  number--;
  location = objectLocation(number);
  if (location == roomNumber) {
    message = objectMessage(number);
    return message;
  }
  return 0;
}

int here(int number, int roomNumber) {
  if (objectHere(number, roomNumber) > 0) {
    return true;
  }
  return false;
}

int inInv(int number) {
  if (objectHere(number, playerLocation) > 0) {
    return true;
  }
  return false;
}

int available(int number, int roomNumber) {
  if (here(number, roomNumber) == true || inInv(number) == true) {
    return true;
  }
  return false;
}

void swapObject(int number1, int number2) {
  /*move object 1 from inv to bin */
  /*move object 2 to inv*/
  setObjectLocation(number1, 0);
  setObjectLocation(number2, playerLocation);
}

void moveObject(int number, int roomNumber) {
  /* move object to location */
  setObjectLocation(number, roomNumber);
}

/*--------------------- generics -------------------*/

char getSingleChar() {
  /* prompt for a single char and ensure anything else is trimmed off */
  char answer, c;

  answer = getchar();
  while ((c = getchar()) != '\n' && c != EOF);
  return (answer);
}

int again(void) {
  char answer;

  printf("\nAnother game? (Y/N)");
  answer = getSingleChar();

  if (answer == 'y' || answer == 'Y') {
	  loadGame("NEWGAME.DAT");
    return true;
  }
  return false;

}

void reverse(char * s, int start, int length) {
  int c, i, j;

  for (i = 0, j = length - 1; i < j; i++, j--) {
    c = s[start + i];
    s[start + i] = s[start + j];
    s[start + j] = c;
  }
}

void reverseUpper(char * s, int start, int length) {
  int c, i, j;

  for (i = 0, j = length; i < j; i++) {
    s[start + i] = toupper(s[start + i]);
  }

  for (i = 0, j = length - 1; i < j; i++, j--) {
    c = s[start + i];
    s[start + i] = s[start + j];
    s[start + j] = c;
  }

}

void toUpperCase(char * s) {
  /* step through each character converting to upper case */

  int x;
  x = 0;
  while (s[x] != '\0') {
    s[x] = toupper(s[x]);
    x++;
  }
}

void cls() {
  if (clear == 1) {
    mode(MODE);
  }
}

void hold(int duration) {
  int p, t;
  for (p = 0; p < duration * 1000; p++) {
    t = p * 10;
    t = p / 10;
  }
}