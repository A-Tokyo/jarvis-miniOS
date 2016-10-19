void printString(char*);
void printStringHelper(char*, int);
char* readString(char* );
void handleInterrupt21(int, int , int , int);
void readSector(char* , int );
void readFile(char* , char* );
void terminate();
 void writeSector(char* , int );
void deleteFile(char* );
void writeFile(char* , char* , int );
void ListFile();

void main(){

    // char line[80];
  	// printString("Enter a line: \0");
  	// readString(line);
  	// printString("   ");
  	// printString(line);
// shell
    int i;
  	char buff[13312];
  	char b[512];
  	makeInterrupt21();

  	interrupt(0x21, 4, "shell\0", 0x2000, 0);

	// char line[80];
  //  char buffer[13312] ;/*this is the maximum size of a file*/
	// makeInterrupt21();
	//  interrupt(0x21,1,line,0,0);
	// makeInterrupt21();
	// interrupt(0x21, 3, "messag\0", buffer, 0); /*read the file into buffer*/
	// interrupt(0x21, 0, buffer, 0, 0); /*print out the file*/



// 	interrupt(0x21,0,line,0,0);
// 	char line[80];
// 	printString("Enter a line: \0");
// 	readString(line);
// 	printString("   ");
// 	printString(line);
//
// char buffer[13312] ;  /*this is the maximum size of a file*/
// makeInterrupt21();
// interrupt(0x21, 3, "messag\0", buffer, 0); /*read the file into buffer*/
// interrupt(0x21, 0, buffer, 0, 0); /*print out the file*/


	// char buffer[13312];
  //   makeInterrupt21();
  //   interrupt(0x21, 7, "messag\0", 0, 0); //delete messag
  //   interrupt(0x21, 3, "messag\0", buffer, 0); // try to read messag
  //     interrupt(0x21, 0, buffer, 0, 0); //print out the contents of buffer


    //  char buffer[512];
    //  readSector(buffer, 30);
    //  printString(buffer);
		//

    // writeFile
    // int i=0;
    // char buffer1[4096];
    // char buffer2[4096];
    // buffer2[0]='h';
    // buffer2[1]='e';
    // buffer2[2]='l';
    // buffer2[3]='l';
    // buffer2[4]='o';
    // for(i=5; i<4096; i++) buffer2[i]=0x0;
    // makeInterrupt21();
    // interrupt(0x21,8, "testW\0", buffer2, 1); //write file testW
    // interrupt(0x21,3, "testW\0", buffer1, 0); //read file testW
    // interrupt(0x21,0, buffer1, 0, 0); // print out contents of testW

     while(1); // hang up
}

void printString(char* Array){
	printStringHelper(Array, 0);
}

void printStringHelper(char* Array, int offset){
	if (*(Array + offset) == '\0')
      return;
	interrupt(0x10, (0xE)*256 + *(Array + offset), 0x0, 0x0, 0x0);
	printStringHelper(Array,offset+1);
}


char* readString(char* l){
	int count = 0 ;
	char* input = interrupt(0x16,0x0,0x0,0x0,0x0);
	while (input != 0Xd){
		interrupt(0X10, 0xE * 256 + input,0x0,0x0, 0x0);
		if (input != 0x8) {
			l[count++] = input;
		}
		else {
			interrupt(0x10, 0xE*256+'\0',0x0,0x0,0x0);
			interrupt(0x10, 0xE*256+0x8,0x0,0x0,0x0);
			count--;
		}

		input = interrupt(0x16,0x0,0x0,0x0,0x0);
	}
	if(input == 0xd){
		char Feed = 0xa;
		char end = 0x0;
		l[count] = Feed;
		l[count+1] = end;
	}
}

void executeProgram(char* name, int segment){
	int i;
	int address;
	char readingBuffer[13312];

	readFile(name, readingBuffer);

	for (i=0; i<13312;i++){
		putInMemory(segment,i,readingBuffer[i]);
	}

	launchProgram(segment);

}

void handleInterrupt21(int ax, int bx, int cx, int dx){

if (ax == 0)
	printString(bx);
		else
			if (ax==1)
				readString(bx);
				else
				if (ax ==2)
					readSector(bx, cx);
				else if (ax == 3)
					readFile(bx, cx);
          else if (ax == 4){
			executeProgram(bx,cx);
		}
					else if(ax==5)
						terminate() ;
 						else if (ax==6)
							writeSector(bx,cx);//part 4-
							else if (ax==7)
							   deleteFile(bx);
                 else if (ax==8)
                 writeFile(bx,cx,dx);
                 else if (ax == 9)
			                ListFile();
							     else
 						  	       printString("value of AX is wrong/0");

}

void readSector(char* buffer, int sector){

	int relativesector=modulus(sector,18) + 1;
	int head =modulus(divide( sector, 18 ) , 2) ;
	int track = divide( sector , 36 ) ;
	int AX=2*256 + 1;
	int BX=buffer ;
	int CH =track;
	int CL=relativesector ;
	int DH=head;
	int DL=0;
	int CX=CH*256+CL ;
	int DX=DH*256+DL ;


	//call interupt
	interrupt(0x13,AX, BX, CX,DX);
}

int divide(int x, int y){
	int result=0;
	while(x >= y){
		x-=y;
		result=result+1;
	}
	return result;
}

int modulus(int x, int y){
	while(x >= y){
		x-=y;
	}
	return x;
}

// MS3 from here part 1-
void readFile(char* file, char* buffer){
	int index = 0;
	char dir[512];// ba3ml copy mn el directory 3shan a3rf a2ra manno.
	readSector(dir, 2);

	while(index < 512)
	{
		int j = index;
		int k = 0;
		int found = 1;
		for (; index < j + 6; index++, k++)
		{
			if (file[k] == '\0')
			{
				index = j + 6;
				break;
			}
			if (file[k] != dir[index])
				found = 0;
		}

		if (found)
		{
			int buffIndex = 0;
			int j = index;
			for (; index < j + 26; index++)
			{
				int i = 0;
				char tempBuffer[512];
				if (dir[index] == 0)
					break;

				readSector(tempBuffer, dir[index]);
				for (; i < 512; i++, buffIndex++)
					buffer[buffIndex] = tempBuffer[i];
			}

			return;
		}

		index += 26;
	}
  printString("File Not Found!");
	return;
}
// part 3-
void terminate(){
  char shell[6];
  	shell[0] = 's';
  	shell[1] = 'h';
  	shell[2] = 'e';
  	shell[3] = 'l';
  	shell[4] = 'l';
  	shell[5] = 0x0;
  	interrupt(0x21,4,shell,0x2000,0);

}


void writeSector(char* buffer, int sector){

	int relativesector=modulus(sector,18) + 1;
	int head =modulus(divide( sector, 18 ) , 2) ;
	int track = divide( sector , 36 ) ;
	int AX=3*256 + 1;
	int BX=buffer ;
	int CH =track;
	int CL=relativesector ;
	int DH=head;
	int DL=0;
	int CX=CH*256+CL ;
	int DX=DH*256+DL ;


	//call interupt

	interrupt(0x13,AX, BX, CX,DX);
}
//part 5-
void deleteFile(char* filename){
  int generalpointer = 0 ;
  int rowPointer = 0 ;
  int mapPointer = 0 ;
  int filePointer = 0 ;
  int    mapValue = 0  ;
  int found = 0 ;
	char dir[512];
  char map[512];// hna fe l asami
	readSector(dir, 2);// ba3ml copy mn el directory 3shan a3rf a2ra manno.
	readSector(map, 1);

  while(generalpointer < 512)
  {
// neshofo found wla la2
for (; generalpointer < rowPointer+ 6; generalpointer++ , filePointer++) {
  if(filename[filePointer] == '\0' && dir[generalpointer] == '\0'){
found =1 ;
  }

    if(dir[generalpointer] != filename[filePointer] ){

    break ;

    }

  }

  if(generalpointer == rowPointer +6) {found = 1 ;

  }

if(found) break ;
filePointer = 0 ;
rowPointer = rowPointer + 32 ;
generalpointer = rowPointer ;

  }
if(found){
  dir[rowPointer] = 0x00 ;
  generalpointer = rowPointer + 6 ;
for (;generalpointer < rowPointer  +  32 ; generalpointer++) {
   if(dir[generalpointer] == 0x00) break ;
      mapValue = dir[generalpointer] ;
  map[mapValue] = 0x00 ;
}

}

	writeSector(dir, 2);
	writeSector(map, 1);
	return;
}


  void writeFile(char* name, char* buffer, int secNum) {
  int generalpointer = 0;
  int bufferMaxSize  = 13312;
  int rowPointer =0;
  int mapPointer = 0 ;
  int secCounter = 0 ;
  char partOfBuffer[512];
  int fileNamePointer= 0  ;
  int bufferPointer = 0 ;
  int partOfBufferPointer = 0 ;
  int freeSector = 0 ;
	char dir[512];
  char map[512];// hna fe l asami
  char test[512] ;
  char debug[100] ;
	readSector(dir, 2);// ba3ml copy mn el directory 3shan a3rf a2ra manno.
	readSector(map, 1);
// loop in dir
    while(rowPointer < 512)	{
// search for free space in dir  by search for zero first index
if(dir[rowPointer] == '\0' ){
break ;

}
rowPointer = rowPointer + 32 ;
generalpointer = rowPointer ;
  	}
    for (; generalpointer < rowPointer+6; generalpointer++  ) {
      if(generalpointer != 0 )dir[generalpointer] = '\0';
    for (; generalpointer < rowPointer+6; generalpointer++ , fileNamePointer++ ) {
             if(name[fileNamePointer] == '\0') {break ; }
               dir[generalpointer] = name[fileNamePointer];
    }


    }

    generalpointer = rowPointer + 6 ;
// if(secNum == 0)
//  printString("0");
//   else if (secNum == 1) printString("1");
//   else if (secNum == 2) printString("2");
//   else if (secNum == 3) printString("3");
//   else if (secNum == 4) printString("4");
//   else if (secNum == 5) printString("5");
//   else if (secNum == 6) printString("6");
//   else if (secNum == 7) printString("7");
//   else if (secNum == 8) printString("8");
//   else if (secNum == 9) printString("9");
//   else if (secNum == 10) printString("10");
//   else if (secNum == 11) printString("11");
//   else if (secNum == 12) printString("12");
//   else if (secNum == 13) printString("13");
//   else if (secNum == 14) printString("14");
//   else if (secNum == 15) printString("15");
//   else printString("out of range ");


    if(secNum < 27){
      for (; mapPointer < 512; mapPointer++) {
        if(map[mapPointer] == '\0'){
  //  printString(" i found  space : ") ;
          freeSector= freeSector + 1 ;
        }

      }
      mapPointer = 0 ;

      if(secNum > freeSector){

      //  if(freeSector == 503)  printString("510\n");

        printString("NO enough sector for file\n");
    return ;

      }


for (;secCounter < secNum && (generalpointer< rowPointer + 32) ; secCounter++ , generalpointer++) {

      for (; mapPointer < 512   ; mapPointer++ ) {
         if(map[mapPointer] == '\0'){
          // printString("found free");
           map[mapPointer] = 0xff;
          dir[generalpointer]= mapPointer;

          for (; bufferPointer < 512; bufferPointer++) {
             partOfBuffer[partOfBufferPointer] =  buffer[bufferPointer];
          }
          writeSector(partOfBuffer,mapPointer) ;
           partOfBufferPointer = 0 ;
           break ;
         }

      }


    }  } else {

      			printString("Not enough space in directory entry for file\n");
      			return;

    }

//printString("here");
for (; generalpointer < rowPointer + 32 ; generalpointer++ ){

  dir[generalpointer] = 0x00 ;
}

//handle law l map mliana 2aw law l dir msh mkfya

  	writeSector(dir, 2);
  	writeSector(map, 1);
  	return;

}
// try to get files listed
void ListFile(){
	char buff[512];
	char dirBuff[512];
	int i,j;
	int index = 0;
	for(i=0;i<512;i++){
		buff[i] = 0x0;
		dirBuff[i]=0x0;
	}
	/* Read in the directory sector */
    readSector(dirBuff, 2);
	/* Look through the directory for files */
	for(i=0;i<16;i++){
		 /* If there is a file at the location */
		if (dirBuff[32*i] != 0x0){
			/* Check the first characters */
			for (j=0; j < 6; j++){
				buff[index] = dirBuff[j+32*i];
				index++;
			}
			/* Add some formatting */
			buff[index] = '\r';
			index++;
			buff[index] = '\n';
			index++;
		}
	}
	for(i=0;i<512;i++){
		printChar(buff[i]);
	}
	return;

}
