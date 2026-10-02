/* Functions for DOS based file handling
   Will need attention for CPM              */

/*-------------------------- file handling -------------------------*/

int getMetaData(char * filename) {

  int fileNumber, ch, c, i;
  //load data about the game
  FILE * f = fopen(filename, "rb");
  if (!f) {
    printf("File not found %s \n", filename);
    return -1;
  }

  fileNumber = fgetc(f);
  ch = fgetc(f);
  objectFileSize = ch * 256;
  ch = fgetc(f);
  objectFileSize += ch;

  //length of the title
  ch = fgetc(f);

  size_t bytesRead;

  // Read a maximum of STRINGSIZE - 1 elements of size 1 (char)
  bytesRead = fread(gameTitle, 1, STRINGSIZE - 1, f);

  // If z88dk returns EOF (-1) or 0 on error/empty, handle gracefully
  if (bytesRead == (size_t) EOF || bytesRead == 0) {
    gameTitle[0] = '\0';
    return;
  }

  // Safely cap off the string
  gameTitle[bytesRead] = '\0';

  fclose(f);

}

int getSizeOfFile(char * filename) {

  //size is held in the first two bytes
  FILE * f = fopen(filename, "rb");
  if (!f) {
    printf("File not found %s \n", filename);
    return -1;
  }

  int size = 0, ch;
  ch = fgetc(f);
  size = ch * 256;
  ch = fgetc(f);
  size += ch;
  objectFileSize = size;
  fclose(f);
  return size;
}

int getSizeOfFileByBytes(char * filename) {

  long byte_count = 0;
  int ch;

  FILE * f = fopen(filename, "rb");
  if (!f) {
    printf("File not found %s \n", filename);
    return -1;
  }

  // Note: ch must be an int (not a char) to safely match EOF
  while ((ch = fgetc(f)) != EOF) {
    byte_count++;
  }
  return byte_count;
}

long readIndex(int recordNumber, char * fileName, int offSetValue) {
  /* reads an integer from the index file */

  long index;
  int number;

  //printf(fileName);printf("\n");
  //printf("rec n : %d\n",recordNumber);

  FILE * fileptr;
  index = (((long) recordNumber - 1) * 2) + offSetValue;
  number = 0;

  //printf("index %lu\n",index);

  fileptr = fopen(fileName, "rb");
  if (fileptr == NULL) {
    printf("File not found %d", fileName);
    return 0;
  }

  fseek(fileptr, index, SEEK_SET);

  //get first byte
  number = fgetc(fileptr) * 256;
  //get second byte
  number = number + fgetc(fileptr);
  //printf("Pos in file %d\n",number);
  fclose(fileptr);
  return (long) number;

}

long readIndexRoom(int recordNumber, char * fileName) {
  /* reads an integer from the index file */

  long index;
  int number;

  //printf(fileName);printf("\n");
  //printf("rec n : %d\n",recordNumber);

  FILE * fileptr;
  index = (((long) recordNumber - 1) * 2) + 4;
  number = 0;

  //printf("index %lu\n",index);

  fileptr = fopen(fileName, "rb");
  if (fileptr == NULL) {
    printf("File not found %d", fileName);
    return 0;
  }

  fseek(fileptr, index, SEEK_SET);

  //get first byte
  number = fgetc(fileptr) * 256;
  //get second byte
  number = number + fgetc(fileptr);
  //printf("Pos in file %d\n",number);
  fclose(fileptr);
  return (long) number;

}

void loadSystemMessages() {
  long index;
  int i, c, o, l;
  FILE * fileptr;
  fileptr = fopen(messageLoc, "rb");
  //printf("Messages\n ");
  // printf(messageLoc);
  // printf("\n");

  if (fileptr == NULL) {
    printf("Null file pointer in system message");
    return;
  }
  /* get the index of message 19 as this will be the end of message 18 */

  index = readIndex(numSysMessages, (char * ) messageIndexLoc, 5);
  //printf("\nMessage index : %lu\n",index);
  sysMessages = (char * ) malloc(((int) index) * sizeof(char));
  fread(sysMessages, (int) index, 1, fileptr);
  fclose(fileptr);

}

void getMessage(int number) {
  long index;
  FILE * fileptr;
  int msgLen, x, y, num, start, end;

  memset( & message[0], '\0', sizeof(message));

  //printf("System message : %d\n",number);

  if (number < numSysMessages) {
    /* start with first message and step through */
    /* until the correct number is found */

    end = 1;
    for (num = 1; num <= number; num++) {
      msgLen = sysMessages[end]; /* len of current message */
      end = end + msgLen + 2;
    }
    start = end - msgLen;
    end--;

    for (x = 0, y = start - 1; x < msgLen; x++, y++) {
      message[x] = sysMessages[y];
    }
    message[x] = '\0';

  } else {

    printf("Message : %d\n",number);
    
    index = readIndex(number, (char * ) messageIndexLoc, 5);
	printf("Index : %lu\n",index);
	fileptr = fopen(messageLoc, "rb");
	
    fseek(fileptr, index, SEEK_SET);
    fgetc(fileptr);
    msgLen = (int) fgetc(fileptr);
    fread(message, msgLen, 1, fileptr);
    fclose(fileptr);
    reverse(message, 0, msgLen);
    message[msgLen] = '\0';
  }
}

void loadWords(int type) {
  /* file format is:

     first byte = item number
     second byte = 0
     third byte = length of text
     fourth byte onwards = word in reverse order

  */

  FILE * fileptr;
  long filelen;
  int c, i, l, o;
  int ch;

  /* open the object file, jump to the end to get its size*/
  /* and then return to the start*/
  if (type == NOUN) {
    fileptr = fopen(nounFile, "rb");
    //printf("Loading Nouns \n");
    filelen = getSizeOfFile(nounFile);
  }
  if (type == VERB) {
    fileptr = fopen(verbFile, "rb");
    //printf("Loading Verbs \n");
    filelen = getSizeOfFile(verbFile);
  }
  if (type == ADVERB) {
    fileptr = fopen(adverbFile, "rb");
    //printf("Loading Adverbs \n");
    filelen = getSizeOfFile(adverbFile);
  }

  if (fileptr == NULL) {
    printf("Word file not found.");
    return;
  }

  /*resize the objects array*/
  if (type == NOUN) {
    nounNumber = (int) filelen;
    //printf("Nouns : %d", nounNumber);
    nouns = (char * ) malloc(((int) filelen + 3) * sizeof(char));
    /*read in the whole of the file*/
    fread(nouns, (int) filelen, 1, fileptr);

    /*print out the text*/
    c = 0;
    while (c < filelen) {
      i = nouns[c];
      c = c + 2;
      l = nouns[c];
      reverseUpper(nouns, c + 1, l);

      /* step through each character of the word based on length */
      o = 1;
      while (o < l + 1) {
        c++;
        o++;
      }
      putchar('.');
      c++; /*start of next word*/
    }
  }

  if (type == VERB) {
    verbNumber = (int) filelen;
    //printf("verbs : %d", verbNumber);
    verbs = (char * ) malloc(((int) filelen + 3) * sizeof(char));
    /*read in the whole of the file*/
    fread(verbs, (int) filelen, 1, fileptr);

    /*print out the text*/
    c = 0;
    while (c < filelen) {
      i = verbs[c];
      c = c + 2;
      l = verbs[c];
      reverseUpper(verbs, c + 1, l);

      /* step through each character of the word based on length */
      o = 1;
      while (o < l + 1) {
        c++;
        o++;
      }
      putchar('.');
      c++; /*start of next word*/
    }
  }

  if (type == ADVERB) {
    adverbNumber = (int) filelen;
    //printf("adverbs : %d", adverbNumber);
    adverbs = (char * ) malloc(((int) filelen + 3) * sizeof(char));
    /*read in the whole of the file*/
    fread(adverbs, (int) filelen, 1, fileptr);

    /*print out the text*/
    c = 0;
    while (c < filelen) {
      i = adverbs[c];
      c = c + 2;
      l = adverbs[c];
      reverseUpper(adverbs, c + 1, l);

      /* step through each character of the word based on length */
      o = 1;
      while (o < l + 1) {
        c++;
        o++;
      }
      putchar('.');
      c++; /*start of next word*/
    }
  }

  fclose(fileptr);
  //printf("\n");
}

int loadObjects() {
  FILE * fileptr;
  long filelen;
  /*int c,i,o; */
  /* open the object file, jump to the end to get its size*/
  /* and then return to the start*/
  fileptr = fopen(objectLoc, "rb");

  if (fileptr == NULL) {
    printf("Unable to find the objects file.");
    return 1;
  }

  /*resize the objects array*/
  objectList = (char * ) malloc(((int) objectFileSize + 3) * sizeof(char));
  /*read in the whole of the file*/
  fread(objectList, (int) objectFileSize, 1, fileptr);
  fclose(fileptr);
  objn = (int) objectFileSize / 8;
  //objn = 5;

  return 0;
}

void saveGame() {

  /* message contains the save string. e.g. save game01 */
  /* or just save and then save as save-m */

  int i, c, carryWeight, maxEnc;
  FILE * fileptr;
  printf("Saving game...");
  fileptr = fopen("SAVEM.DAT", "w");
  if (fileptr == NULL) {
    printf("Could not save game!\n");
    return;
  }

  carryWeight = getStat(ENC);
  maxEnc = getStat(MAXENC);
  //printf("CW: %d", carryWeight);

  /* save the game variables */
  fwrite( & turn, sizeof(int), 1, fileptr);
  fwrite( & room, sizeof(int), 1, fileptr);
  fwrite( & maxEnc, sizeof(int), 1, fileptr); //max weight
  fwrite( & carryWeight, sizeof(int), 1, fileptr); //current weight
  fwrite( & clear, sizeof(int), 1, fileptr);

  /*save the counter values */
  for (i = 0; i < counterSize; i++) {
    c = ctr[i];

    fputc(c, fileptr);
  }

  /*save objects */
  fwrite(objectList, (objn * 8), 1, fileptr);
  fclose(fileptr);
  printf("\nDone.\n");

}

void loadGame(char * filename) {

  int i;
  unsigned char carryWeight, maxEnc;
  FILE * fileptr;

  if (filename == "NEWGAME.DAT") {
    printf("Start a new game...\n");
  } else {
    printf("Loading game...\n");
  }

  //fileptr = fopen("SAVEM.DAT", "rb");
  fileptr = fopen(filename, "rb");

  if (fileptr == NULL) {
    printf("Could not load game!\n");
    return;
  }

  fread( & turn, sizeof(int), 1, fileptr);
  //printf("Turn : %d\n", turn);

  fread( & room, sizeof(int), 1, fileptr);
  //printf("Room : %d\n", room);

  fread( & maxEnc, sizeof(int), 1, fileptr);

  //printf("Max Enc : %d\n", maxEnc);
  setStat(MAXENC, maxEnc);

  fread( & carryWeight, sizeof(int), 1, fileptr);
  //printf("Carry Weight : %d\n", carryWeight);

  setStat(ENC, carryWeight);
  //setting to 256 for some reason

  fread( & clear, sizeof(int), 1, fileptr);

  /* load counters */
  for (i = 0; i < counterSize; i++) {
    ctr[i] = fgetc(fileptr);
  }

  /* load objects */
  fread(objectList, (objn * 8), 1, fileptr);

  fclose(fileptr);

  printf("\nDone.\n");

}