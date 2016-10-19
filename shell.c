void Print(char buff);
void decodeInput(char buffer);
int CalcSecNo(int a, int b);
void printChar(char ch);
void clear(char*,int);

main(){
	char buffer[80];
	Print("Welcome to Tokyo OS");
	Print("\r\n");
	Print("Shell-> ");

	while(1){

		interrupt(0x21,1,buffer,0,0);
		Print("\r\n");
		Print("Shell-> ");
		decodeInput(buffer);
		clear(buffer,80);

	}
}

void decodeInput(char* buff){
	char inputBuff[80];
	char inputDataBuff[512];
	int inputDataBuffSingleLineCount = 0;
	char fileName[7];
//	char type[4];
//	char run[3];
//	char dirBuff[512];
	char fileBuff[13312];
	char fileName1[7];
	char fileName2[7];

	int BoolFlag = 1;
	int bufferCount = 0;

	//fydeto  fl create bas
	int indexIn, index;
	// end
	int end;
	int i = 0;
	int lock = 0;

	int fileLen = 0;
	int sectors;
	// create input
	int input = 1;
	int inputCount = 0 ;

	char b[512];

	//create
	int inputDataBuffLength=0;

	while(BoolFlag){
		if (buff[bufferCount] == 0x0 || buff[bufferCount] == "\0"){

			BoolFlag = 0;
		}
		else{
			bufferCount++;
		}
	}
	for (indexIn = 0; indexIn <80; indexIn++){

		if(buff[indexIn] != 0x0 && buff[indexIn] != '\r' && buff[indexIn] != '\n' && buff[indexIn] != '-' && buff[indexIn] != '>'){
			break;
		}
	}
	for (end=indexIn;end<80;end++){

		if(buff[end] == '\r'){
			break;
		}
	}

	if (end - indexIn < 1){
		Print("Too short!");
	}

	// view

	else if (buff[indexIn]=='v' && buff[indexIn+1]=='i' && buff[indexIn+2]=='e' && buff[indexIn+3]=='w' ){

		indexIn = indexIn + 5;

		for(i=0;i<6;i++){
			fileName[i] = buff[indexIn+i];
		}
		fileName[6] = "\0";

		interrupt(0x21, 3, fileName, fileBuff, 0); /*read the file into buffer*/
		interrupt(0x21, 0, fileBuff, 0, 0); /*print out the file*/

	}

	//end of view


	// exexute


	// end of exexute

	// delete
	else if (buff[indexIn]=='d' && buff[indexIn+1]=='e' && buff[indexIn+2]=='l' && buff[indexIn+3]=='e'  && buff[indexIn+4]=='t' && buff[indexIn+5]=='e' ){

		indexIn = indexIn + 7;

		for(i=0;i<6;i++){
			fileName[i] = buff[indexIn+i];
		}
		fileName[6] = "\0";

		interrupt(0x21, 7, fileName, 0 , 0); /*delete  the file */
			interrupt(0x21, 0," deleted successfully \0", 0 , 0);


	}

	// end opof delete
	else if (buff[indexIn]=='e' && buff[indexIn+1]=='x' && buff[indexIn+2]=='e' &&  buff[indexIn+3]=='c' && buff[indexIn+4]=='u' && buff[indexIn+5]=='t' && buff[indexIn+6]=='e'){

		indexIn = indexIn + 8;

		for(i=0;i<6;i++){
			fileName[i] = buff[indexIn+i];
		}
		fileName[6] = "\0";

		interrupt(0x21, 4, fileName, 0x2000, 0);

	}
	else if (buff[indexIn]=='c' && buff[indexIn+1]=='o' && buff[indexIn+2]=='p' && buff[indexIn+3]=='y'){
		/* Get the file names Split at space char */

		index = indexIn+5;
		i = 0;
		while(buff[index] != 0x20 && buff[index] != 0x0){
			fileName1[i] = buff[index];
			index++;
			i++;
		}
		fileName1[6] = '\0';

		i = 0;
		index++;
		while(buff[index] != 0x20 && buff[index] != 0x0){
			fileName2[i] = buff[index];
			index++;
			i++;
		}
		fileName2[6] = '\0';
		/* Have the file names. Read in filename1 */
		interrupt(0x21,3,fileName1,fileBuff,0);
		/* Figure out how many sectors we need */

		while(fileBuff[fileLen] != 0x0){
			fileLen++;
		}
		sectors = CalcSecNo(fileLen,512);
		/* Write fileBuff (with fileName1 inside of it) to fileName2 */

		interrupt(0x21,8,fileName2,fileBuff,sectors+1);

	}
// dir


// end of dir

// create file



//  end of create  file

	else if (buff[indexIn]=='d' && buff[indexIn+1]=='i' && buff[indexIn+2]=='r'){
		interrupt(0x21,9,0,0,0);
		Print("Shell-> ");

	}
	else if (buff[indexIn]=='c' && buff[indexIn+1]=='r' && buff[indexIn+2]=='e' && buff[indexIn+3]=='a'&& buff[indexIn+4]=='t' && buff[indexIn+5]=='e'){
		indexIn = indexIn + 7;
		Print("Enter File Data: \r\n");
		Print("\t-:");
		for(i=0;i<6;i++){
			fileName[i] = buff[indexIn+i];
		}
		fileName[6] = "\0";

		while(input || inputCount < 13312){
			/* Get user input */
			clear(inputBuff,80);

			interrupt(0x21,1,inputBuff,0,0);
			Print("\r\n");
			Print("\t-:");

			if (inputBuff[2] == 0x0){
				Print("END OF FILE");
				Print("\r\n");
				Print("Shell-> ");
				input = 0;
				break;
			}
			else{

				for(i=0;i<80;i++){
					if(inputBuff[i] == 0x0 || inputBuff[i] == '\0'||inputBuff[i]=='\r'||inputBuff[i]=='\n'||inputBuff[i]=='\t'){
						inputBuff[i] = 0x20;
					}
					inputDataBuff[i+80*inputDataBuffSingleLineCount] = inputBuff[i];
					inputCount++;
				}
				inputDataBuffLength++;
				inputDataBuffSingleLineCount++;
			}
		}
		sectors = CalcSecNo(inputDataBuffLength/512);
		/* Now create the file */
		interrupt(0x21,8,fileName,inputDataBuff,inputDataBuffLength);

	}
	else{
		Print("Bad Command!");
		Print("\r\n");
		Print("Shell-> ");
	}


}
int CalcSecNo(int a, int b){
    int q = 0;
    while(q*b <=a){
        q = q+1;
    }
    return q-1;

}
void Print(char* buff){


	interrupt(0x21,0,buff,0,0);

}
void clear(char* buff, int len){
	int i;
	for(i=0;i<len;i++){
		buff[i] = 0x0;
	}
}
void printChar(char ch){
	char* chars[2];
	chars[0] = ch;
	chars[1] = '\0';
	Print(chars);
}
