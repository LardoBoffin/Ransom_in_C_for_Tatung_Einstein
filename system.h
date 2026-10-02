/* Main System header */

#define true 1
#define false 0
#define counterSize 256
#define true 1
#define false 0
#define STRINGSIZE 256
#define WORDSIZE 50
#define YouAreCarrying 1
#define WhatNow 2
#define YouCant 3
#define Pardon 4
#define YouDontHaveIt 8
#define TooHeavy 10
#define ItIsDark 13
#define CantFindThat 14
#define YouCanAlsoSee 15
#define Okay 16
#define Nothing 18
/*#define numSysMessages 45*/
#define BytesPerIndex 5
#define XPOS 0
#define YPOS 1
#define MODE 7
#define modeZero 80
#define modeSeven 39
#define VERB 1
#define NOUN 2
#define ADVERB 3



extern int debug,enc,turn,room,andActive,clear;
extern unsigned char *objectList;
extern char andMessage[STRINGSIZE];
extern int playerLocation,cw,exitFlag;
extern const char messageLoc[];
extern const char messageIndexLoc[];
extern const char nounFile[];
extern const char verbFile[];
extern const char adverbFile[];
extern const char roomLoc[];
extern const char objectLoc[];
extern const char roomIndexLoc[];
extern int screenWidth,nounNumber,verb,noun,verbNumber,adverbNumber,objn;
extern char *nouns,*verbs,*adverbs;
extern unsigned char *sysMessages;
extern char message[STRINGSIZE];
extern int numSysMessages;
extern int objectFileSize;
extern char gameTitle[STRINGSIZE];

extern enum Stats {
	STR = 1,
    CON = 2,
    INT = 3,
    POW = 4,
    DEX = 5,
    CHA = 6,
    LCK = 7,
    LVL = 8,
    HP = 9,
    HPBASE = 10,
    HPADJUSTED = 11,
    MP = 12,
    MPBASE = 13,
    MPADJUSTED = 14,
    HUNGER = 15,
    THIRST = 16,
    COLD = 17,
    WET = 18,
    TIRED = 19,
    ENC = 20,
    MAXENC = 21
};



/* Main functions */
int getSizeOfFile(char *);
void saveGame(void);
void loadGame(char *);
void showMessage(int);
void reverse(char *, int, int);
void reverseUpper(char *, int, int);
void moveCursor(int,int);
int again(void);
void listObjects(int,int);
void roomMessage(int);
int objectLocation(int);
void reverse(char *,int,int);
void reverseUpper(char *,int,int);
void toUpperCase(char *);
void cls(void);
void hold(int);
int checkMove(void);
void initialiseGame(void);
void exitGame(void);
void preRoom(void);
void roomMessage(int );
int highPriority(void);
int lowPriority(void);
void showMessage(int);
void gotoRoom(int);
void takeObject(int);
void dropObject(int);
void showInventory(void);
void getRoom(int);
/* character related functions*/
int getStat(int);
void setStat(int,int);
void showCharacter(void);
void createChar(void);
int getXP(void);
void increaseXP(int);
void afterAction(void);

#include "system.c"
#include "gCode.h"

