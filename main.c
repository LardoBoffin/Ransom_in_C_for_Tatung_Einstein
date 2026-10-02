/*>c.ransom
 */

/* Lardo Boffin */
/* Version alpha  */

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <time.h>

#include <ctype.h>

#include "tatung.h"

#include "system.h"

#include "fileIO.h"

#include "char.h"


const char messageLoc[] = "DATAM.DAT";
const char nounFile[] = "DATAN.DAT";
const char verbFile[] = "DATAV.DAT";
const char adverbFile[] = "DATAA.DAT";
const char roomLoc[] = "DATAR.DAT";
const char objectLoc[] = "DATAO.DAT";
const char messageIndexLoc[] = "IDXM.DAT";
const char roomIndexLoc[] = "IDXR.DAT";

int noun, noun2, oldNoun, verb, adverb, object, turn, room, objn, achn, doneFlag;
int nounNumber, verbNumber, adverbNumber, randomNumber, exitFlag, andActive;
int lastRoom, playerLocation, clear, numLinks, screenWidth, objectFileSize;
char * objectList, links[36], * ach, * nouns, * verbs, * adverbs, * sysMessages;
char message[STRINGSIZE], word[WORDSIZE], andMessage[STRINGSIZE], gameTitle[STRINGSIZE];
int debug;
int numSysMessages;

unsigned char get_universal_cpm_x(void) {
    int row = 0;
    int col = 0;

    // 1. Send the standard ANSI interrogation sequence to the console
    printf("\x1b[6n"); 
    
    // 2. CP/M will pipe the response back into your program's input stream.
    // The sequence arrives formatted exactly as: ESC [ Row ; Col R
    scanf("\x1b[%d;%dR", &row, &col); 

    // ANSI columns are 1-indexed (1-40 or 1-80). 
    // Subtract 1 to match C's standard 0-indexed column system.
    return (unsigned char)(col - 1);
}

void takeObject(int number) {
  int objWeight;
  objWeight = objectWeight(number);
  if (debug == true) {
    printf("Object num %5d, weight %5d\n", number, objWeight);
    printf("Current ENC : %d\n", getStat(ENC));
    printf("MAX ENC : %d\n", getStat(MAXENC));
  }

  if (objectHere(number, room) == 0) {
    showMessage(CantFindThat);
    andActive = false;
    andMessage[0] = '\0';
    return;
  }

  if (getStat(ENC) + objectWeight(number) > getStat(MAXENC)) {
    showMessage(TooHeavy);
    andActive = false;
    andMessage[0] = '\0';
    return;
  }

  setObjectLocation(number, playerLocation);
  showMessage(16);
  setTurn(1);
  setStat(ENC, getStat(ENC) + objWeight);
  if (debug == true) {
    printf("Max enc %5d, carried %5d\n", getStat(MAXENC), getStat(ENC));
  }
}

void dropObject(int number) {
  /*drop object if we have it*/
  int cw;

  if (objectHere(number, playerLocation) == 0) {
    showMessage(YouDontHaveIt);
    andActive = false;
    andMessage[0] = '\0';
    return;
  }
  setObjectLocation(number, room);
  showMessage(Okay);
  setTurn(1);

  cw = getStat(ENC) - objectWeight(number);
  if (cw < 0) {
    cw = 0;
  }
  setStat(ENC, cw);
  //printf("Max enc %5d, carried %5d\n", getStat(MAXENC), getStat(ENC));
}

/*----------------- display functions -------------------*/

void showMessage(int number) {
  /* load and display a message*/

  int x, xpos, slen, wl, printLoop;

  getMessage(number);
  slen = strlen(message);
  xpos = pos(); /*position across screen */

  x = 0;

  while (x < slen) {
    wl = 0;

    /* get the next word (or end of string)*/
    while (message[x + wl] != ' ' && x + wl < slen) {
      wl++;
    }

    if (xpos + wl > screenWidth) {
      putchar('\n');
      xpos = 0;
    }

    /* print out the word */
    for (printLoop = 0; printLoop < wl; printLoop++, x++) {
      putchar(message[x]);
    }

    xpos = xpos + wl + 1; /* +1 for the space */

    if (xpos < screenWidth && xpos > 0) /* no space at end or start of line*/ {
      putchar(' ');
    } else {
      xpos--;
    }
    x++;
  }

  message[0] = '\0';

}

void listObjects(int roomNumber, int messageNumber) {
  /*lists all objects at a location */
  /*this can also include the inventory */

  int x, message, numHere;
  if (objn == 0) return;
  numHere = 0;

  /*check for objects at location */
  for (x = 1; x <= objn; x++) {
    message = objectHere(x, roomNumber);
    if (message >> 0) {
      numHere++;
    }
  }
  if (numHere == 0 && messageNumber >> 1) {
    putchar('\n');
    return;
  }
  if (numHere == 0 && messageNumber == 1) {
    showMessage(YouAreCarrying);
    showMessage(Nothing);
    return;
  }
  putchar('\n');
  if (messageNumber >> 0) {
    showMessage(messageNumber);
    putchar('\n');
  }

  /*print objects at location */
  for (x = 1; x <= objn; x++) {
    message = objectHere(x, roomNumber);
    if (message >> 0) {
      showMessage(message);
      putchar('\n');
    }
  }

}

void showInventory() {
  listObjects(playerLocation, 1);
}

/*------------------ room code ---------------*/
void gotoRoom(int roomNumber) {
  room = roomNumber;
}

void find(int object) {
  /*find an object an object and jump to it */
  int loc;
  loc = objectLocation(object);
  gotoRoom(loc);
}

void getRoom(int roomNumber) {
  int dark;
  long index;
  FILE * fileptr;
  int num, message, x;

  dark = false;

  if (isOn(1) == true && isOn(2) == false) {
    dark = true;
    showMessage(ItIsDark);
  }

  fileptr = fopen(roomLoc, "rb");
  index = readIndex(roomNumber - 1, (char * ) roomIndexLoc, 4);


  fseek(fileptr, index, SEEK_SET);

  //get environmental byte
  x = (int) fgetc(fileptr);
  
  /*deal with messages*/
  num = (int) fgetc(fileptr);
  //printf("Num message : %d\n",num);

  for (x = 0; x < num; x++) {
    message = (int) fgetc(fileptr) * 256;
    message = message + (int) fgetc(fileptr);
    if (dark == false) {
      showMessage(message);
    }
  }

  /* deal with links (if any) */
  memset( & links[0], 0, sizeof(links)); /* zero out the links*/
  num = (int) fgetc(fileptr);
  if (num >> 0) {
    for (x = 0; x < (num * 3); x++) {
      links[x] = fgetc(fileptr);
    }
    numLinks = num;
  }

  fclose(fileptr);

  if (dark == false) {
    //roomMessage(roomNumber);
    listObjects(roomNumber, YouCanAlsoSee);
  }
}

/*------------- parser --------------------------*/
int parseWord(int type) {
  /* compare the value of word[] to the appropriate data list */
  /* byte 0 is return number */
  /* next byte is 0 */
  /* next byte is length of word */
  /* next bytes are word */

  char * list;
  int listLen, x, retVal, wordLen, testLen, listLoop;
  retVal = 0;

  if (strncmp("IT", word, 2) == 0 && oldNoun > 0) {
    noun = oldNoun;
    return noun;
  }

  if (type == VERB) {
    list = & * verbs;
    listLen = verbNumber;
  }

  if (type == NOUN) {
    list = & * nouns;
    listLen = nounNumber;
  }

  if (type == ADVERB) {
    list = & * adverbs;
    listLen = adverbNumber;
  }

  wordLen = strlen(word);

  for (x = 0; x < listLen; x++) {
    retVal = list[x];
    x = x + 2;
    testLen = list[x];
    if (testLen == wordLen) {
      x++; /*first letter of word in list */
      listLoop = 0;

      while (listLoop < wordLen) {
        if (list[x + listLoop] != word[listLoop]) {
          retVal = 0;
          listLoop = wordLen + 1;
          x--;
        } /* drop out of loop */
        listLoop++;
      }
      if (listLoop == wordLen) return retVal;
    }
    x = x + testLen;

  }

  return 0;
}

void parseText() {
  int slen, gotText, x, wl, printLoop, parsed, andPos, i, andWasActive;

  /* if andActive is true then this is the second half of a sentence */
  /* e.g. take lamp and light it */

  verb = 0;
  noun = 0;
  adverb = 0;
  noun2 = 0;
  andWasActive = false;

  if (andActive == false) {
    /* step through the text entered and break down into words */
    if (pos() > 0) {
      putchar('\n');
    }

    showMessage(WhatNow);
    gotText = 0;
    while (gotText == 0) {
      putchar('>');
      if (fgets(message, 255, stdin) == NULL) {
        puts("Error getting command line.");
        return;
      }
      slen = strlen(message);
      if (message[slen - 1] == '\n') {
        message[slen - 1] = '\0';
      }
      if (! * message) {
        puts("Please type something!");
      } else {
        gotText = 1;
      }
    }

    toUpperCase(message);

  } else {
    slen = strlen(andMessage);
    /*printf("\nand message %d",slen);*/
    for (i = 0; i < slen; i++) {
      message[i] = andMessage[i];
    }
    message[i] = '\0';
    /*putchar('\n');putchar('(');
    printf("%s",message);putchar(')');putchar('\n'); */
    slen = i + 1;
    andWasActive = true;
    /*printf("\new message %d",slen);*/
  }

  /*check for " AND " */
  andPos = strstr(message, " AND ") - message;

  if (andPos > 0) {
    /* AND is present with spaces before and after */
    andPos = andPos + 5;
    /*printf("position of AND is %d",andPos);*/
    /* copy rest of string into andMessage and set flag */

    for (i = andPos, x = 0; i < slen - 1; i++, x++) {
      andMessage[x] = message[i];
    }
    andMessage[x] = '\0';
    message[andPos - 4] = '\0';

    slen = andPos - 4;
    andActive = true;
  } else {
    andActive = false;
  }

  if (andWasActive == true) {
    putchar('\n');
    printf(">%s", message);
    putchar('\n');
  }

  /* check for *cat or *. and call directly */
  if (strncmp("*.", message, 2) == 0 || strncmp("*C", message, 2) == 0) {
    oscli("*CAT");
    doneFlag = true;
    return;
  }

  if (strncmp("DEBUG", message, 5) == 0) {
    debug = true;
    return;
  }

  /*break text into individual words*/
  x = 0;

  while (x < slen - 1) {
    wl = 0;
    while (message[x + wl] != ' ' && x + wl < slen - 1) {
      word[wl] = message[x + wl];
      wl++;
    }
    word[wl] = '\0';

    for (printLoop = 0; printLoop < wl; printLoop++, x++);

    x++; /*skip the space we have just ignored*/

    parsed = false;

    /*pass the word into the appropriate decoder */
    if (verb == 0) {
      verb = parseWord(VERB);
      if (verb > 0) parsed = true;
    }

    if (noun == 0 && parsed == false) {
      noun = parseWord(NOUN);
      if (noun > 0) {
        parsed = true;
        oldNoun = noun;
      }
    }

    if (adverb == 0 && parsed == false) {
      adverb = parseWord(ADVERB);
      if (adverb > 0) parsed = true;
    }

    if (noun2 == 0 && parsed == false) {
      noun2 = parseWord(NOUN);
    }

  }
  if (debug == true) {
    printf("Verb %2d", verb);
    printf("  Noun %2d", noun);
    printf("  Old Noun %2d", oldNoun);
    printf("  Adverb %2d", adverb);
    printf("  Noun Two %2d\n", noun2);
  }
}

/*-------------- main loop -------------*/
int main() {
  int anothergame;

  //printf("\016"); // clear the screen
  
  debug = false;
  srand(time(NULL));
  turn = 0, anothergame = 1;

  mode(MODE);

  getMetaData("META.DAT");

  printf("Loading data.");
  if (loadObjects() > 0) {
    return 0;
  };

  loadWords(NOUN);
  loadWords(VERB);
  loadWords(ADVERB);
  numSysMessages = getSizeOfFile((char * )
    "IDXM.DAT");
  loadSystemMessages();

  //printf("\016"); // clear the screen
  //printf("\x10");
  //printf("\x14");
  mode(MODE);
  screenWidth = 39;

  while (anothergame == 1) {

    memset( & ctr[0], 0, sizeof(ctr)); /*initialise the counter array to 0*/

    /*base variable values*/
    lastRoom = 0;
    turn = 0;
    playerLocation = 10000;
    puts(gameTitle);
    puts("\n");

    exitFlag = false;
    room = 1;
    clear = false;
    andActive = false;
    initialiseGame();

    while (exitFlag == 0) {
      doneFlag = 0;
      randomNumber = rand() % 101;
      preRoom();

      if (room != lastRoom) {
		//if you moved to a new room
        lastRoom = room;
        cls();
        getRoom(room);
        setTurn(1);
      }

      /* check for instant death stuff */
      doneFlag = highPriority();

      if (exitFlag == 0 && doneFlag == 0) {
        parseText();
      }
      if (verb == 0 && exitFlag == 0 && doneFlag == 0) {
		  //does not increment the current turn
        showMessage(Pardon);
        andActive = false;
        andMessage[0] = '\0';
      } else {
        if (exitFlag == 0 && doneFlag == 0) {
          doneFlag = checkMove();
		  //no need to increment the turn as this will be handled in if (room != lastRoom)
        }
        if (exitFlag == 0 && doneFlag == 0) {
          doneFlag = lowPriority();
		  if (doneFlag > 0)
		  {
			setTurn(doneFlag);
		  }
		  if (doneFlag >-1) {doneFlag=1;}else{doneFlag=0;}

        }
      }

      if (doneFlag == 0 && verb > 0) {
        showMessage(YouCant);
        andActive = false;
        andMessage[0] = '\0';
      }

    }

    anothergame = again();

  }

  printf("\nThanks for playing...");
  return (0);
}

/*------------------- game code ---------------*/



void exitGame() {
  exitFlag = 1;
  if (pos() > 0) {
    putchar('\n');
  }
}

int checkMove() {
  int i;

  /* if there are no links then drop out */
  if (numLinks == 0) {
    return 0;
  }

  for (i = 0; i < (numLinks * 3); i++) {
    if (links[i] == verb) {
      room = (links[i + 1] * 256) + links[i + 2];
      return room;
    } else {
      i = i + 2;
    }
  }

  putchar('\n');
  return 0;
}