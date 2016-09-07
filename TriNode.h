#ifndef MERC_TRINODE_H
#define MERC_TRINODE_H

typedef struct TriNode
{
	struct TriNode *leftChild;
	struct TriNode *rightChild;
	struct TriNode *leftNeighbor;
	struct TriNode *rightNeighbor;
	struct TriNode *baseNeighbor;
}TriNode;

#endif