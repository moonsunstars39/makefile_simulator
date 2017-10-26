#pragma once
#ifndef _AJCLHELPER_H_INCLUDED_
#define _AJCLHELPER_H_INCLUDED_

/*char *target: the string that being contained by the vertix
int visited: If the node has been visited, visited should be 1
int istarget: if the node is target, then the is target should be 1
eNode *eNode: The edge of vNode
cNode *cNode: The command linked list of vNode.
*/
typedef struct vNode {
	char *target;
	int visited;
	int completed;
	int istarget;
	struct eNode *eNode;
	struct cNode *cNode;
	struct vNode *next;

}vNode;

typedef struct eNode {
	struct eNode *next;
	struct vNode *vNode;
}eNode;

typedef struct cNode {
	char *command;
	struct cNode *next;
}cNode;

//Functions in AJCLhelper.c
int TargetLineDetector(char* string);
int CommandLineDetector(char* string);
int ColonDetector(char *string);
vNode *addVertix(char *name, vNode *list_hd);
vNode *createEdge(char *vName1, char* vName2, vNode *list_hd);
vNode *searchNode(char* vName, vNode *list_hd);
int flagDetector();
int targetDetector(vNode *list_hd, char *InputName);
int pathPrint(vNode *From, vNode *list_hd);
void freeList(vNode *Vlist_hd);
int flagDetector();
int pathCreate(vNode *Node);

#endif
