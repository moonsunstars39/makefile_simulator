/**/

//custom header files
#include "AJCLhelper.h"
#include "/home/cs352/spring17/Assignments/proj10/cs352.h"
//library header files
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>

int flag = 0;

/*
Function: int TargetLineDetector(char* string)
Purpose: Identify if the input string is target line.
If it is not, return 0. If it is, return 1. If there are any memory problem, return 2.
*/
int TargetLineDetector(char* string) {
	char* target = NULL;
	char* temptraget = NULL;
	char* tempStr = NULL;

	int targetCount = 0;

	//The line is commmand line
	if (string[0] == '\t') {
		return 0;
	}
	//The line doesn't have colon
	if (ColonDetector(string) != 1) {
		return 0;
	}

	char* tempString1 = cs352_strdup(string);
	if (tempString1 == NULL) {
		fprintf(stderr, "ERROR: Cannot allocate memory to tempString1!\n");
		free(tempString1);
		return 2;
	}
	char* tempString2 = cs352_strdup(string);
	if (tempString2 == NULL) {
		fprintf(stderr, "ERROR: Cannot allocate memory to tempString2!\n");
		free(tempString1);
		free(tempString2);
		return 2;
	}

	target = strtok(tempString1, ":");
	//printf("Target in AJCLhelper: %s\n", target);

	if (target != NULL) {
		targetCount++;
		temptraget = strtok(target, " \t");
		temptraget = strtok(NULL, " \t");
		if (temptraget != NULL) {
			free(tempString1);
			free(tempString2);
			return 0;
		}
	}

	tempStr = strtok(tempString2, ":");
	while (tempStr) {
		tempStr = strtok(NULL, ":");
		if (tempStr != NULL) {
			targetCount++;
		}
	}
	if (targetCount > 2) {
		free(tempString1);
		free(tempString2);
		return 0;
	}
	free(tempString1);
	free(tempString2);
	return 1;
}

/*
Fcuntion: int CommandLineDetector(char* string)
Purpose: Identify if the input string is command line.
If it is, return 1
if it isn't, return 0.
*/
int CommandLineDetector(char* string) {
	if (string[0] != '\t') {
		return 0;
	}
	return 1;
}
/*
Function: ColonDetector
Purpose: Identify if ther input string includes any Colon.
If it does, return 1
if it doesn't, return 0.
*/
int ColonDetector(char *string) {
	int length = strlen(string);
	int i = 0;
	for (i = 0; i < length; i++) {
		if (string[i] == ':') {
			return 1;
		}
	}
	return 0;
}

/*
Function: addVertix(char *name, vNode *list_hd)
Purpose: Input the new vertix with the name char* name.
Error condition: If the vertix has exsisted, then stop inputing the new vertix.
*/
vNode *addVertix(char *name, vNode *list_hd) {
	//printf("ADDNAME: %s\n", name);
	vNode *newNode = NULL;
	vNode *temp = NULL;
	//Loop for checking if the vertix with same name has exsisted.
	temp = list_hd;
	while (temp != NULL) {
		if (strcmp(temp->target, name) == 0) {
			//printf("NAMEEXISTS!\n");
			return list_hd;
		}
		temp = temp->next;
	}
	newNode = cs352_malloc(sizeof(vNode));
	if (newNode == NULL) {
		fprintf(stderr, "ERROR: Cannot allocate memory to newNode in vNode!\n");
		free(newNode);
		flag = 1;
		return list_hd;
	}
	memset(newNode, 0, sizeof(vNode));
	newNode->target = cs352_malloc(strlen(name) + 1);
	if (newNode->target == NULL) {
		fprintf(stderr, "ERROR: Cannot allocate memory to newNode->target in vNode!\n");
		free(newNode->target);
		free(newNode);
		flag = 1;
		return list_hd;
	}
	strcpy(newNode->target, name);
	newNode->visited = 0;
	newNode->completed = 0;
	newNode->istarget = 0;
	//If the vertix linkedlist is empty, then make the newNode become the first node.
	//otherwise, put the newnode at the end of the vertix linkedlist.
	if (list_hd == NULL) {
		//printf("ONHEAD\n");
		list_hd = newNode;
	}
	else {
		newNode->next = list_hd;
		list_hd = newNode;
		//printf("ONTAIL\n");
	}
	return list_hd;
}

/*
Function: vNode *createEdge(char *vName1, char* vName2, vNode *list_hd)
Purpose: create the edge between two vertix, which contains the vName1 and vName2
Error: If cannot find any node the contains the vName1 or vName2, then stop the function.
*/
vNode *createEdge(char *vName1, char* vName2, vNode *list_hd) {
	//printf("EDGE: VNAME1: %s VNAME2:%s\n", vName1,vName2);
	vNode* cur1 = NULL;
	vNode* cur2 = NULL;
	eNode *newNode = NULL;
	eNode *temp = NULL;


	for (cur1 = list_hd; cur1 != NULL; cur1 = cur1->next) {
		if (strcmp(cur1->target, vName1) == 0) {
			for (cur2 = list_hd; cur2 != NULL; cur2 = cur2->next) {
				if (strcmp(cur2->target, vName2) == 0) {
					newNode = (eNode*)cs352_malloc(sizeof(eNode));
					if (newNode == NULL) {
						fprintf(stderr, "ERROR: Cannot malloc memory to newNode in createEdge!\n");
						free(newNode);
						flag = 1;
						return list_hd;
					}
					memset(newNode, 0, sizeof(eNode));

					newNode->vNode = cur2;
					if (cur1->eNode == NULL) {
						cur1->eNode = newNode;
					}
					else {
						temp = cur1->eNode;
						while (temp->next != NULL) {
							//printf("temp->vNode->target = %s\n", temp->vNode->target);
							temp = temp->next;
						}
						temp->next = newNode;
					}
				}
			}
		}
	}
	return list_hd;
}

/*
Function: vNode *searchNode(char* vName, vNode *list_hd)
Purpose: find the vertix with the input string name vName.
*/
vNode *searchNode(char* vName, vNode *list_hd) {
	vNode *cur = NULL;
	//printf("VNAME: %s\n", vName);
	for (cur = list_hd; cur != NULL; cur = cur->next) {
		if (strcmp(cur->target, vName) == 0) {
			break;
			//printf("vName %s Sucess!\n", vName);
		}
	}
	//printf("vName %s Failed!\n", vName);
	return cur;
}

/*
Function: int targetDetector(vNode *list_hd,char *InputName)
Purpose: Scan the whole , and find if the input value exsists.
Return Value: 1 = exsist; 0 = doesn't exsist.
*/
int targetDetector(vNode *list_hd, char *InputName) {
	vNode* Vcur = NULL;
	for (Vcur = list_hd; Vcur != NULL; Vcur = Vcur->next) {
		if (strcmp(Vcur->target, InputName) == 0) {
			if (Vcur->istarget == 1) {
				return 1;
			}
		}
	}
	return 0;
}
/*
Function: int flagDetector()
Purpose: return the flag value from AJCLhelper.c to mymake.c
If the flag = 1, return 1
else, return 0.
*/
int flagDetector() {
	if (flag == 1) {
		return 1;
	}
	if (flag == 2) {
		return 2;
	}
	return 0;
}

/*
Function: int pathPrint(vNode *From, vNode *list_hd)
Purpose: Traverse the whole list and print out the vertix, edge and commands.
the order is: edge->commands->vertix.
*/
int pathPrint(vNode *Node, vNode *list_hd) {
	eNode *temp = NULL;
	cNode *cTemp = NULL;

	if (Node->visited == 1) {
		return 0;
	}
	Node->visited = 1;
	temp = Node->eNode;
	if (temp == NULL) {
		printf("%s\n", Node->target);
		cTemp = Node->cNode;
		while (cTemp != NULL) {
			printf("  %s\n", cTemp->command);
			cTemp = cTemp->next;
		}
		return 0;
	}
	for (temp = Node->eNode; temp != NULL; temp = temp->next) {
		pathPrint(temp->vNode, list_hd);
		if (temp->next == NULL) {
			printf("%s\n", Node->target);
			cTemp = Node->cNode;
			while (cTemp != NULL) {
				printf("  %s\n", cTemp->command);
				cTemp = cTemp->next;
			}
		}
	}
	return 0;
}
/*
Function: void freeList(vNode *Vlist_hd)
Purpose: Free the whole list.
*/
void freeList(vNode *Vlist_hd) {
	vNode *Vcur = NULL;
	eNode *Elist_hd = NULL;
	eNode *Ecur = NULL;
	cNode *Clist_hd = NULL;
	cNode *Ccur = NULL;
	while ((Vcur = Vlist_hd) != NULL) {
		Vlist_hd = Vlist_hd->next;
		if (Vcur->eNode != NULL) {
			Elist_hd = Vcur->eNode;
			while ((Ecur = Elist_hd) != NULL) {
				Elist_hd = Elist_hd->next;
				free(Ecur);
			}
		}
		if (Vcur->cNode != NULL) {
			Clist_hd = Vcur->cNode;
			while ((Ccur = Clist_hd) != NULL) {
				Clist_hd = Clist_hd->next;
				free(Ccur->command);
				free(Ccur);
			}
		}
		free(Vcur->target);
		free(Vcur);
	}
}

int pathCreate(vNode *Node) {
	eNode *temp = NULL;
	eNode *temp2 = NULL;
	cNode *cTemp = NULL;
	struct stat buf;
	struct stat buf2;
	struct stat buf3;
	int RC = 0;
	int DRC = 0;
	int targetOld = 0;
	int returnValue = 0;

	if (Node->visited == 1) {
		return 0;
	}
	Node->visited = 1;
	temp = Node->eNode;
	if (temp == NULL) {
		DRC = stat(Node->target, &buf2);
		if (DRC == -1) {
			fprintf(stderr, "ERROR: The file %s is missing!\n",Node->target);
			flag = 1;
			return 2;
		}
		cTemp = Node->cNode;
		while (cTemp != NULL) {
			printf("%s\n", cTemp->command);
			cTemp = cTemp->next;
		}
		Node->completed = 1;
		return 0;
	}

	for (temp = Node->eNode; temp != NULL; temp = temp->next) {
		returnValue = pathCreate(temp->vNode);
		if (returnValue == 2) {
			return 2;
		}
		if (temp->next == NULL) {
			RC = stat(Node->target, &buf);
			for (temp2 = Node->eNode; temp2 != NULL; temp2 = temp2->next) {
				if (temp2->vNode->completed != 1) {
					fprintf(stderr, "Circular dependency dropped!\n");
				}
			}
			if (RC != -1) {
				for (temp2 = Node->eNode; temp2 != NULL; temp2 = temp2->next) {
					stat(temp2->vNode->target, &buf3);
					if (buf.st_mtime < buf3.st_mtime) {
						targetOld = 1;
					}
				}
			}
			cTemp = Node->cNode;
			if (RC == -1 || targetOld == 1) {
			while (cTemp != NULL) {
				flag = 2;
				printf("%s\n", cTemp->command);
				system(cTemp->command);
				cTemp = cTemp->next;
			}
		 }
			Node->completed = 1;
		}
	}
	return 0;
}
