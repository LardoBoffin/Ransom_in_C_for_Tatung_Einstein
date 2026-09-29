/* character data and functions */
#define numOfChars 21

/*

STR
CON
INT
POW
DEX
CHA
LCK
LVL
HP
MP

storage locations

belt = 4 small pouches, start with 2.
backpack straps = 4 small pouches. Start with 0.
leg pouches = 2 small pouches. Start with 0.

backpack = carry weight

*/

unsigned char statistics[numOfChars];

static int XP;

int getStat(int stat) {
  return statistics[stat];
}

void setStat(int stat, int value) {
  //printf("Set stat num :%d val : %d",stat, value);
  statistics[stat] = value;
}

int getXP() {
  return XP;
}

void increaseXP(int number) {
  XP += number;
}

void showCharacter() {
  cls();
  printf("\n");
  printf("STR %2d, CON %2d, INT %2d, POW %2d, DEX %2d, CHA %2d, LCK %2d\n", getStat(STR), getStat(CON), getStat(INT), getStat(POW), getStat(DEX), getStat(CHA), getStat(LCK));
  printf("LVL %2d\n", getStat(LVL));
  printf("XP  %2d\n", getXP());
  printf("HP  %2d/%2d,MP %2d/%2d\n", getStat(HP), getStat(HPADJUSTED), getStat(MP), getStat(MPADJUSTED));
  printf("ENC %2d/%2d", getStat(ENC), getStat(MAXENC));
  printf("\n");
  showInventory();
}

void createChar() {
  int maxEnc;

  setStat(STR, 11);

  maxEnc = (int) getStat(STR) * 10;

  setStat(CON, 14);
  setStat(INT, 19);
  setStat(POW, 18);
  setStat(DEX, 14);
  setStat(CHA, 19);
  setStat(LCK, 12);
  setStat(LVL, 1);
  setStat(HP, getStat(CON));
  setStat(HPADJUSTED, getStat(HP));
  setStat(MP, getStat(POW));
  setStat(MPADJUSTED, getStat(MP));
  setStat(HUNGER, 0);
  setStat(THIRST, 0);
  setStat(WET, 0);
  setStat(COLD, 0);
  setStat(TIRED, 0);
  setStat(ENC, 0);
  setStat(MAXENC, maxEnc);
  XP = 0;

}