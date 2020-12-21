/**
 * File processing, 2020
 * AVL.cpp
 * implementation of AVL tree
 */
#include <stdio.h>
#include <iostream>
#include <algorithm>
using namespace std;
/**
 * Node represents a single node in AVL tree.
 */
typedef struct Node {
  int         key, bf;
  struct Node *left, *right;
} Node;
typedef Node* Tree;
/**
 * getNode returns a new node.
 * @return a new node
 */
Node* getNode() {
	Node* tempNode = new Node;
	tempNode->left = nullptr;
	tempNode->right = nullptr;
	tempNode->bf = 0;
	tempNode->key = 0;
	return tempNode;
}
int height(Tree T)
{
	if (T == nullptr)
		return 0;

	return 1 + max(height(T->left), height(T->right));
}

Node* maxNode(Tree* T)
{
	if ((*T)->right == nullptr)
		return *T;
	maxNode(&(*T)->right);
}

Node* minNode(Tree* T)
{
	if ((*T)->left == nullptr)
		return *T;
	minNode(&(*T)->left);
}

int noNodes(Tree* T)
{
	if (*T == nullptr)
		return 0;
	return 1 + noNodes(&(*T)->left) + noNodes(&(*T)->right);
}
/**
 * updateBF updates balancing factors of nodes in T
 * and finds the unbalanced node nearest to y.
 * @param T: an AVL tree
 * @param y: the inserted/deleted node
 * @param x: the unbalanced node nearest to y
 * @param p: parent node of x
 */
void updateBF(Tree* T, Node* y, Node** x, Node** p) {
	if (*T == nullptr)
		return;
	if (T == &y)
		return;

	updateBF(&(*T)->left, y, x, p);
	updateBF(&(*T)->right, y, x, p);
	(*T)->bf = height((*T)->left) - height((*T)->right);

	if ((*T)->bf > 1 || (*T)->bf < -1)
	{
		if(*x == nullptr)
			*x = *T;
	}
	if ((*T)->left != NULL)
	{
		if ((*T)->left->bf > 1 || (*T)->left->bf < -1)
			if(*p == nullptr)
				*p = *T;
	}
	if ((*T)->right != NULL)
	{
		if ((*T)->right->bf > 1 || (*T)->right->bf < -1)
			if(*p == nullptr)
				*p = *T;
	}
}
/**
 * rotateLL implements LL rotation in subtree rooted with x.
 * @param T: an AVL tree
 * @param x: root node of subtree
 * @param p: parent node of x
 */
void rotateLL(Tree *T, Node *x, Node *p) {
	Node* tempNode = x->left;
	x->left = tempNode->right;
	tempNode->right = x;

	if (p == nullptr)
		*T = tempNode;
	else if (p->left == x)
		p->left = tempNode;
	else
		p->right = tempNode;
}


/**
 * rotateRR implements RR rotation in subtree rooted with x.
 * @param T: an AVL tree
 * @param x: root node of subtree
 * @param p: parent node of x
 */
void rotateRR(Tree *T, Node *x, Node *p) {
	Node* tempNode = x->right;
	x->right = tempNode->left;
	tempNode->left = x;

	if (p == nullptr)
		*T = tempNode;
	else if (p->left == x)
		p->left = tempNode;
	else
		p->right = tempNode;
}

/**
 * rotateLR implements LR rotation in subtree rooted with x.
 * @param T: an AVL tree
 * @param x: root node of subtree
 * @param p: parent node of x
 */

void rotateLR(Tree* T, Node* x, Node* p) {
	rotateRR(T, x->left, x);
	rotateLL(T, x, p);
}

/**
 * rotateRL implements RL rotation in subtree rooted with x.
 * @param T: an AVL tree
 * @param x: root node of subtree
 * @param p: parent node of x
 */
void rotateRL(Tree* T, Node* x, Node* p)
{
	rotateLL(T, x->right, x);
	rotateRR(T, x, p);
}

/**
 * insertBST inserts newKey into T
 * and returns the inserted node.
 * @param T: a binary search tree
 * @param newKey: a key to insert
 * @return the inserted node
 */
Node *insertBST(Tree *T, int newKey) {
	Node* tempNode = getNode();
	tempNode->key = newKey;

	if (*T == NULL)
	{
		*T = tempNode;
		return *T;
	}

	if ((*T)->key > newKey)
	{
		if ((*T)->left == nullptr)
		{
			(*T)->left = tempNode;
			return (*T)->left;
		}

		T = &((*T)->left);
		insertBST(T, newKey);
	}
	else if ((*T)->key < newKey)
	{
		if ((*T)->right == nullptr)
		{
			(*T)->right = tempNode;
			return (*T)->right;
		}

		T = &((*T)->right);
		insertBST(T, newKey);
	}
} 

/**
 * insertAVL inserts newKey into T.
 * @param T: an AVL tree
 * @param newKey: a key to insert
 */
void insertAVL(Tree* T, int newKey) {
	Node *rtnNode = getNode();
	rtnNode = insertBST(T, newKey);
	
	Node* x = NULL;
	Node* p = NULL;
	updateBF(T, rtnNode, &x, &p);

	if (x == NULL)
	{
		cout << "NO" ;
		return;
	}

	if (x->bf > 1)
	{
		if (x->left->bf < 0)
		{
			rotateLR(T, x, p);
			cout << "LR";
		}
		else {
			rotateLL(T, x, p);
			cout << "LL";
		}
	}
	else {
		if (0 < x->right->bf)
		{
			rotateRL(T, x, p);
			cout << "RL";
		}
		else {
			rotateRR(T, x, p);
			cout << "RR";
		}
	}

	updateBF(T, rtnNode, NULL, NULL);
	return;
}
void deleteAVL(Tree* T, int deleteKey);
/**
 * deleteBST deletes deleteKey from T
 * and returns the parent node of the deleted node.
 * @param T: a binary search tree
 * @param deleteKey: a key to delete
 * @return the parent node of the deleted node
 */
Node *deleteBST(Tree *T, int deleteKey) {
	if (*T == nullptr)
		return NULL;

	Node* parentNode = getNode();
	Node* targetNode = getNode();
	Node* returnNode = getNode();
	targetNode = *T;
	
	while (targetNode->key != deleteKey)
	{
		if (targetNode->key > deleteKey)
		{
			parentNode = targetNode;
			targetNode = targetNode->left;
		}
		else if (targetNode->key < deleteKey)
		{
			parentNode = targetNode;
			targetNode = targetNode->right;
		}
	}

	// 자식이 0개
	if (targetNode->left == nullptr && targetNode->right == nullptr)
	{
		if (parentNode->left == targetNode)
			parentNode->left = nullptr;
		else if (parentNode->right == targetNode)
			parentNode->right = nullptr;
		else
			*T = nullptr;
		return parentNode;
	}
	// 자식이 2개
	else if (targetNode->left != nullptr && targetNode->right != nullptr)
	{
		// 왼쪽의 높이가 큰 경우
		if (height(targetNode->left) > height(targetNode->right))
		{
			Node* tempNode = getNode();
			tempNode = maxNode(&targetNode->left);
			returnNode = deleteBST(T, tempNode->key);

			targetNode->key = tempNode->key;
		}
		// 오른쪽의 높이가 큰 경우
		else if (height(targetNode->left) < height(targetNode->right))
		{
			Node* tempNode = getNode();
			tempNode = minNode(&targetNode->right);
			returnNode = deleteBST(T, tempNode->key);

			targetNode->key = tempNode->key;
		}
		// 높이가 같은 경우에는 노드의 갯수 비교
		else
		{
			//왼쪽의 노드 개수가 많은 경우
			if (noNodes(&targetNode->left) >= noNodes(&targetNode->right))
			{
				Node* tempNode = getNode();
				tempNode = maxNode(&targetNode->left);
				returnNode = deleteBST(T, tempNode->key);

				targetNode->key = tempNode->key;
			}
			//오른쪽의 노드 개수가 많은 경우
			else
			{
				Node* tempNode = getNode();
				tempNode = minNode(&targetNode->right);
				returnNode = deleteBST(T, tempNode->key);

				targetNode->key = tempNode->key;
			}
		}
	}
	// 자식이 1개
	else
	{
		if (targetNode->left != nullptr)
		{
			if (parentNode->left == targetNode)
				parentNode->left = targetNode->left;
			else
				parentNode->right = targetNode->left;
		}
		else 
		{
			if (parentNode->left == targetNode)
				parentNode->left = targetNode->right;
			else
				parentNode->right = targetNode->right;
		}
		return parentNode;
	}
}

/**
 * deleteAVL deletes deleteKey from T.
 * @param T: an AVL tree
 * @param deleteKey: a key to delete
 */
void deleteAVL(Tree *T, int deleteKey) {
	Node* rtnNode = getNode();
	rtnNode = deleteBST(T, deleteKey);

	Node* x = NULL;
	Node* p = NULL;
	updateBF(T, rtnNode, &x, &p);

	if (x == NULL)
	{
		cout << "NO";
		return;
	}

	if (x->bf > 1)
	{
		if (x->left->bf < 0)
		{
			rotateLR(T, x, p);
			cout << "LR";
		}
		else {
			rotateLL(T, x, p);
			cout << "LL";
		}
	}
	else {
		if (0 < x->right->bf)
		{
			rotateRL(T, x, p);
			cout << "RL";
		}
		else {
			rotateRR(T, x, p);
			cout << "RR";
		}
	}

	updateBF(T, rtnNode, NULL, NULL);
	return;
}

/**
 * inorderAVL implements inorder traversal in T.
 * @param T: an AVL tree
 */
void inorderAVL(Tree T) {
	if (T == nullptr)
		return;
	inorderAVL(T->left);
	cout << T->key <<" " ;
	inorderAVL(T->right);
}

int main() {
  /* Do not modify the code below */

  int testcase[] = { 40, 11, 77, 33, 20, 90, 99, 70, 88, 80, 66, 10, 22, 30, 44, 55, 50, 60, 25, 49 };

  Tree T = NULL;

  // insertion
  for (int i=0; i<20; i++) { printf("%d ", testcase[i]); insertAVL(&T, testcase[i]); printf(" : "); inorderAVL(T); printf("\n"); }

  ////// deletion
  for (int i=0; i<20; i++) { printf("%d ", testcase[i]); deleteAVL(&T, testcase[i]); printf(" : "); inorderAVL(T); printf("\n"); }

  //T = NULL;

  //// reinsertion
  for (int i=0; i<20; i++) { printf("%d ", testcase[i]); insertAVL(&T, testcase[i]); printf(" : "); inorderAVL(T); printf("\n"); }

  //// redeletion
  for (int i=19; 0<=i; i--) { printf("%d ", testcase[i]); deleteAVL(&T, testcase[i]); printf(" : "); inorderAVL(T); printf("\n"); }
}