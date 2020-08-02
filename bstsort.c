#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Title: Assignment 1
 * Semester: COP4338 - Spring 2021
 * Author Megan Jane Thompson
 *
 * I affirm that the revisions of this program is entirely
 * my own work and none of it is the work of any other person.
 *
 * This program extracts flag values and accordingly creates/prints
 * a Binary Search Tree.
 *
 */


/*****bstNode Structure******/
typedef struct bstNode{
	char *keyString;
	struct bstNode *left;
	struct bstNode *right;
	int occurCount;
}bstNode;


bstNode *nodePtr = NULL;        //global nodePtr


/******compare if cflag is present******/
int compareSensitive(char *key1, char *key2){
	int i = 0;
	while((i < strlen(key1)) && (i < strlen(key2))){     //compares case sensitive
		if (key1[i] < key2[i]){
			return -1;
		}
		else if(key1[i] > key2[i]){
			return 1;
	 	}
		else{
			++i;
		}
	}
	return 0;
}


/******compare if cflag is not present******/
int compareInsensitive(char *key1, char *key2){
	int i = 0;
	while((i < strlen(key1)) && (i < strlen(key2))){     //compares case insensitive
		if (tolower(key1[i]) < tolower(key2[i])){
			return -1;
		}
		else if(tolower(key1[i]) > tolower(key2[i])){
			return 1;
	 	}
		else{
			++i;
		}
	}
	return 0;
}


/******Insert New Node into BST******/
bstNode* addNode(char *fileLine){
	bstNode *newNode = (bstNode*)calloc(1, sizeof(bstNode));
	//failure allocating memory
	if(newNode == NULL){
		printf("Error allocating memory for newNode\n");
		exit(1);
	}
	//success allocating memory
	else{
		newNode->keyString = fileLine;           //sets key
		newNode->left = NULL;
		newNode->right = NULL;
		newNode->occurCount = 1;
		return newNode;
	}
}


/******Insert Node into BST******/
bstNode* insertBSTNode(bstNode *theNode, char *fileLine, int cflag){
	//empty tree, add root node
	if (nodePtr == NULL){
		nodePtr = addNode(fileLine);
		return nodePtr;
	}
	else if(theNode == NULL){
		theNode = addNode(fileLine);
		return theNode;
	}
	else{
		//if cflag is present, compare strings case sensitive
		if(cflag == 1){
			if(compareSensitive(theNode->keyString, fileLine) < 0){
				theNode->right = insertBSTNode(theNode->right, fileLine, cflag);
				return theNode;
			}
			else if(compareSensitive(theNode->keyString, fileLine) > 0){
				theNode->left = insertBSTNode(theNode->left, fileLine, cflag);
				return theNode;
			}
			else if(compareSensitive(theNode->keyString, fileLine) == 0){
				theNode->occurCount += 1;
			}
			else{
				return theNode;
			}
		}
		//if cflag is not present, compare strings case insensitive
		else{
			if(compareInsensitive(theNode->keyString, fileLine) < 0){
				theNode->right = insertBSTNode(theNode->right, fileLine, cflag);
				return theNode;
			}
			else if(compareInsensitive(theNode->keyString, fileLine) > 0){
				theNode->left = insertBSTNode(theNode->left, fileLine, cflag);
				return theNode;
			}
			else{
				theNode->occurCount += 1;
			}
		}
	}
	return theNode;
}


/******Print BST to output file if oflag is present******/
void printToFile(bstNode *theNode, FILE *foutptr){
	if (theNode != NULL) {                       //recursively calls and prints to file in order
		printToFile(theNode->left, foutptr);
		fprintf(foutptr, "%d: %s", theNode->occurCount, theNode->keyString);
		printToFile(theNode->right, foutptr);
	}
}


/******Print BST to screen if oflag is not present******/
void printToScreen(bstNode *theNode){
	if (theNode != NULL) {                       //recursively calls and prints to screen in order
		printToScreen(theNode->left);
		printf("%d: %s", theNode->occurCount, theNode->keyString);  //print the node
		printToScreen(theNode->right);
	}
}


/******Frees bstNode Pointers******/
void freeBST(bstNode *theNode){
	if (theNode != NULL) {                       //frees nodePtr and keyString pointers
		freeBST(theNode->right);
		freeBST(theNode->left);
		free(theNode->keyString);
		free(theNode);
	}
}


/********MAIN********/
int main(int argc, char ** argv){
	extern char *optarg;
	extern int optind;
	int c, err=0, cflag=0, oflag=0;
	int maxLength = 100;
	char *outFileName;
	char *inFileName;
    FILE *finptr;
    FILE *foutptr;
	static char usage[] = "usage: bstsort [-c] [-o output_file_name] [input_file_name]\n";

	if(argc < 1){
		printf("Try running the program with some arguments\n\t%sSomething like:\texample -c inFile.txt\n\t\texample -c -o outFile.txt\n", usage);
		exit(1);
	}
	/*set the flags per command line prompt*/
	while ((c = getopt(argc, argv, "co:")) > 0){
		switch (c) {
		case 'c':
			cflag = 1;
			break;
		case 'o':
			oflag = 1;
			outFileName = (char*)calloc(strlen(optarg)+1, sizeof(char));
			//failure allocating memory
			if (outFileName == NULL){
				printf("Error allocating memory for outFileName\n");
				exit(1);
			}
			//success allocating memory
			else{
				strcpy(outFileName, optarg);                //obtains the output file's filename
				strcat(outFileName, "\0");
			}
			break;
		case '?':
			err = 1;
			break;
		}
	}

	/*checks for errors*/
	if (err) {                                              //general error
		fprintf(stderr, "%s %s %s", "General Error.\n", usage, argv[0]);
		exit(1);
	}


	/*processes argument after the command-line options, may contain an input file name*/
	if (optind < argc){
		inFileName = (char*)calloc(strlen((argv[optind]))+1, sizeof(char));     //gets input file name
		//failure allocating memory
		if (inFileName == NULL){
			printf("Error allocating memory for inFileName\n");
			exit(1);
		}
		//success allocating memory
		else{
			strcpy(inFileName, argv[optind]);
			finptr = fopen(inFileName, "r");                //opens input file to read data
			//failure to open inFileName
			if (finptr == NULL){
				printf("Error opening %s.", inFileName);
				printf(" Please be sure file name is spelled correctly and/or file extension is included.\n");
				exit(1);
			}
			//successful opening inFileName
			else{
				while(!feof(finptr)){
					char *fileLine = (char*)calloc(maxLength, sizeof(char));
					//failure allocating memory
					if (fileLine == NULL){
						printf("Error allocating memory for file line of text from input file\n");
						exit(1);
					}
					//success allocating memory
					else{
						fgets(fileLine, maxLength, finptr);
						insertBSTNode(nodePtr, fileLine, cflag);    //call to insert bstNode
					}
				}
			}
		}
	}
	/*input file not include, use user text*/
	else {
		printf("No input file was provided, please type in lines of text and click enter twice to submit\n");
		int loop = 1;
		while(loop){
			char *userText = (char*)malloc(maxLength * sizeof(char));
			//failure allocating memory
			if (userText == NULL){
				printf("Error allocating memory for userText\n");
				exit(1);
			}
			//success allocating memory
			else{
				fgets(userText, maxLength, stdin);
				if(userText[0] == '\n' || feof(stdin)){                            //ends loop, user text insertion is complete
					loop = 0;
				}
				else{
					insertBSTNode(nodePtr, userText, cflag);        //call to insert bstNode
				}
			}
		}
	}


	//if oflag is present: write bst in output file; else: print to screen
	if (oflag == 1){
		foutptr = fopen(outFileName, "w");                  //opens file to write data
		//failure to create outFileName
		if (foutptr == NULL){
			printf("Error creating %s\n", outFileName);
			exit(1);
		}
		//success creating outFileName
		else{
			printToFile(nodePtr, foutptr);
		}
		fclose(foutptr);                                    //closes and saves file
	}
	else{
		printToScreen(nodePtr);
	}
	freeBST(nodePtr);                                       //free nodePtr
	exit(0);
}

