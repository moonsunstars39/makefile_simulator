/*
Author: LiKun Li
Project: mymake.c
Purpose: This program is the first part of the whole program mymake.
*/

//custom header files
#include "AJCLhelper.h"
#include "/home/cs352/spring17/Assignments/proj10/cs352.h"
//Library header files
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char *argv[]) {
	//Pointer Usage
	FILE *File = NULL;
	//Getline Usage
	size_t len = 0;
	char* string = NULL;
	int stringLength = NULL;
	int lineCount = 0;
	//TargetLine Usage
	vNode *Vlist_hd = NULL;
	vNode *curTarget = NULL;
	vNode *InputTarget = NULL;
	vNode *temp = NULL;
	char* target = NULL;
	char* InputName = NULL;
	char* tempString = NULL;
	char* subString1 = NULL;
	char* subString2 = NULL;
	//commandLine usage
	int TStringLength = 0;
	int i = 0;

	//If the input arguments is not 3 then return 1.
	if (argc != 4 || strcmp(argv[1],"-f") != 0) {
		fprintf(stderr, "ERROR: Usage: mymake -f <makefile> <target>\n");
		exit(1);
	}

	else {
		//If the file cannot be open then return 1.
		File = fopen(argv[2], "r");
		if (File == NULL) {
			fprintf(stderr, "ERROR: File: The Makefile is empty!\n");
			exit(1);
		}
		InputName = cs352_strdup(argv[3]);
		if (InputName == NULL) {
			fprintf(stderr, "ERROR: Memory: Cannot allocate memory to InputName!\n");
			free(InputName);
			fclose(File);
			exit(1);
		}
		while (getline(&string, &len, File) != EOF) {
			stringLength = strlen(string);
			//The first line has to be command line. If not, return 1.
			for (i = 0; i < stringLength - 1; i++) {
				if (!isspace(string[i])) {
					break;
				}
			}
			if (i == stringLength - 1) {
				continue;
			}
			lineCount++;
			if (lineCount == 1 && TargetLineDetector(string) == 0) {
				fprintf(stderr, "ERROR: Command without target!\n");
				free(InputName);
				free(string);
				fclose(File);
				exit(1);
			}
			//If the TargetLinDetector return 2, means it has memory problem, then exit with error sign 1.
			else if (TargetLineDetector(string) == 2) {
				free(InputName);
				free(string);
				freeList(Vlist_hd);
				fclose(File);
				exit(1);
			}
			else if (TargetLineDetector(string) == 1) {
				subString1 = cs352_strdup(string);
				//Memory problem, return 1.
				if (subString1 == NULL) {
					fprintf(stderr, "ERROR: Memory: Cannot allocate memory to subString1!\n");
					free(InputName);
					free(string);
					free(subString1);
					freeList(Vlist_hd);
					fclose(File);
					exit(1);
				}
				subString2 = cs352_strdup(string);
				//Memory problem, return 1.
				if (subString2 == NULL) {
					fprintf(stderr, "ERROR: Memory: Cannot allocate memory to subString2!\n");
					free(InputName);
					free(string);
					free(subString1);
					free(subString2);
					freeList(Vlist_hd);
					fclose(File);
					exit(1);
				}
				tempString = strtok(subString1, ":");
				tempString = strtok(tempString, " \t");
				target = cs352_strdup(tempString);
				//Memory problem, return 1.
				if (target == NULL) {
					fprintf(stderr, "ERROR: Memory: Cannot allocate memory to target!\n");
					free(InputName);
					free(string);
					free(subString1);
					free(subString2);
					free(target);
					freeList(Vlist_hd);
					fclose(File);
					exit(1);
				}
				//create the vertix in vNode.
				while (temp != NULL) {
					if (strcmp(temp->target, target) == 0) {
						//If there is a vNode which is a target and exsist,then return 1.
						if (temp->istarget == 1) {
							fprintf(stderr, "ERROR: Repeted Target\n");
							free(InputName);
							free(string);
							free(subString1);
							free(subString2);
							free(target);
							freeList(Vlist_hd);
							fclose(File);
							exit(1);
						}
					}
					temp = temp->next;
				}
				Vlist_hd = addVertix(target, Vlist_hd);
				//Memory problem, return 1.
				if (flagDetector() == 1) {
					free(InputName);
					free(string);
					free(target);
					free(subString1);
					free(subString2);
					freeList(Vlist_hd);
					fclose(File);
					exit(1);
				}
				//printf("TARGET: %s\n", target)
				curTarget = searchNode(target, Vlist_hd);
				temp = Vlist_hd;
				curTarget->istarget = 1;
				//printf("curtarget: %s\n", curTarget->target)
				tempString = strtok(subString2, ":");
				while (tempString) {
					tempString = strtok(NULL, " \t\n");
					if (tempString == NULL) {
						break;
					}
					//printf("TEMPSTRING: %s\n", tempString)
					Vlist_hd = addVertix(tempString, Vlist_hd);
					if (flagDetector() == 1) {
						free(InputName);
						free(string);
						free(target);
						free(subString1);
						free(subString2);
						freeList(Vlist_hd);
						fclose(File);
						exit(1);
					}
					Vlist_hd = createEdge(target, tempString, Vlist_hd);
					if (flagDetector() == 1) {
						free(InputName);
						free(string);
						free(target);
						free(subString1);
						free(subString2);
						freeList(Vlist_hd);
						fclose(File);
						exit(1);
					}
				}
				free(target);
				free(subString1);
				free(subString2);
			}
			//If the commandLineDetector return 1, then the input line is command line.
			else if (CommandLineDetector(string) == 1) {
				cNode *newNode = NULL;
				cNode *temp = NULL;
				char* tempString = cs352_strdup(string);
				if (tempString == NULL) {
					fprintf(stderr, "ERROR: memory: Canno allocate memory to tempString!\n");
					free(tempString);
					free(InputName);
					free(string);
					freeList(Vlist_hd);
					fclose(File);
					exit(1);
				}
				TStringLength = strlen(tempString);
				newNode = cs352_malloc(sizeof(cNode));
				if (newNode == NULL) {
					fprintf(stderr, "ERROR: Cannot allocate memory to newNode in cNode!\n");
					free(newNode);
					free(InputName);
					free(string);
					free(tempString);
					freeList(Vlist_hd);
					fclose(File);
					exit(1);
				}
				memset(newNode, 0, sizeof(cNode));

				for (i = 1; i <= TStringLength; i++) {
					if (tempString[TStringLength - i] == '\n' || tempString[TStringLength - i] == ' ') {
						tempString[TStringLength - i] = '\0';
					}
					else if (tempString[TStringLength - i] != '\n' && tempString[TStringLength - i] != ' ') {
						break;
					}
				}
				memmove(tempString, tempString + 1, TStringLength);
				newNode->command = cs352_strdup(tempString);
				if ((newNode->command) == NULL) {
					fprintf(stderr, "ERROR: Cannot allocate memory newNode->command!\n");
					free(newNode->command);
					free(newNode);
					free(InputName);
					free(string);
					free(tempString);
					freeList(Vlist_hd);
					fclose(File);
					exit(1);
				}
				if (curTarget->cNode == NULL) {
					curTarget->cNode = newNode;
				}
				else {
					temp = curTarget->cNode;
					while (temp->next != NULL) {
						temp = temp->next;
					}
					temp->next = newNode;
				}
				free(tempString);
			}
			//If it's not commandline or targetline, then it's a wrong line, return 1.
			else {
				fprintf(stderr, "Invalid input %s in test_makefile!\n", string);
				free(string);
				free(InputName);
				free(tempString);
				freeList(Vlist_hd);
				fclose(File);
				exit(1);
			}
		}
		//If the input target couldn't be found, return 1.
		if (targetDetector(Vlist_hd, InputName) != 1) {
			fprintf(stderr, "ERROR: TargetDetector: Couldn't find the target %s \n", InputName);
			free(InputName);
			free(string);
			free(tempString);
			freeList(Vlist_hd);
			fclose(File);
			exit(1);
		}
		//printList(Vlist_hd);
		InputTarget = searchNode(InputName, Vlist_hd);
		pathCreate(InputTarget);
		if (flagDetector() == 1) {
			free(InputName);
			free(string);
			freeList(Vlist_hd);
			fclose(File);
			exit(1);
		}
		else if (flagDetector() == 0) {
			printf("%s is up to date.\n", InputName);
		}
		free(InputName);
		free(string);
		freeList(Vlist_hd);
		fclose(File);
	}
	return 0;
}
