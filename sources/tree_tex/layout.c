/*
Author: Anthony Bloesch
Title: layout.c

Copyright (C) Anthony Bloesch 1991

Purpose: Allow n-ary trees with arbitrary nodes to be aestheticaly
         layed out.
*/

#include "layout.h"
#include <stddef.h>
#include <stdlib.h>

typedef struct edge {
/* The outline of a tree.
   INV: all i in [yPosition, last(offset)] * Valid(offset[i])
*/

  unsigned yPosition; /* The first value of <offset> that is valid. */
  int      offset[1]; /* Variable sized array */
} edge;

static
edge *newEdge(unsigned height)
/* Return a new tree outline of height <height>.

  Pre:
  Post:
*/

{
  edge *result;
  
  result = (edge *)malloc((size_t)(sizeof(edge)+
                                   height*sizeof(int)-sizeof(int)));

  if ((edge *)NULL != result)
    result->yPosition = height;

  return result;

} /* newEdge */

int addBranch(Tree      branch,
              TreeNode *node)
{
  TreeNode **newBranch;

  node->nrBranches++;

  if ((Tree *)NULL == node->branches) /* No existing branches. */
    node->branches = (TreeNode **)malloc(
                       (size_t)(node->nrBranches*sizeof(TreeNode *)));

  else { /* Existing branches. */
    newBranch = (TreeNode **)realloc(node->branches,
                  (size_t)(node->nrBranches*sizeof(TreeNode *)));

    if (newBranch != node->branches)
      (void)free(node->branches);

    else
      node->branches = newBranch;
  } /* if */

  if ((Tree *)NULL != node->branches)
    node->branches[node->nrBranches-1] = branch;

  return (Tree *)NULL != node->branches;

} /* addBranch */

void deleteNode(TreeNode *node)
{
  if ((Tree)NULL != node) {
    (void)free((char *)node->name);
    (void)free((char *)node->parent);
    (void)free((char *)node->branches);
    (void)free((char *)node);
  } /* if */
} /* deleteNode */

void deleteTree(Tree tree)
{
  postorderTraverse(tree, deleteNode);

} /* deleteTree */

Tree newTree(char     *nodeName,
             char     *parentName,
             unsigned height,
             unsigned width,
             unsigned depth)
{
  Tree result;

  result = (Tree)malloc(sizeof(TreeNode));

  if (NULL != result) {
    result->name = nodeName;
    result->parent = parentName;
    result->height = height;
    result->width = width;
    result->depth = depth;
    result->x = 0;
    result->y = 0;
    result->nrBranches = 0;
    result->branches = (Tree *)NULL;
  } /* if */

  return result;
} /* newTree */

void inorderTraverse(Tree          tree,
                     NodeFunction *nodeFunction)
{
  unsigned i;

  if ((Tree)NULL != tree) {
    for(i = 0; i < tree->nrBranches; i++) {
      postorderTraverse(tree->branches[i], nodeFunction);
      nodeFunction(tree);
    } /* for */
  } /* if */
} /* inorderTraverse */

void postorderTraverse(Tree          tree,
                       NodeFunction *nodeFunction)
{
  unsigned i;

  if (tree != NULL) {
    for(i = 0; i < tree->nrBranches; i++)
      postorderTraverse(tree->branches[i], nodeFunction);

    nodeFunction(tree);
  } /* if */
} /* postorderTraverse */

void preorderTraverse(Tree          tree,
                      NodeFunction *nodeFunction)
{
  unsigned i;

  if(tree != (Tree)NULL) {
    nodeFunction(tree);

    for(i = 0; i < tree->nrBranches; i++)
      preorderTraverse(tree->branches[i], nodeFunction);
  } /* if */
} /* preorderTraverse */

static
int intMax(int x,
           int y)
/* Return the maximun of <x> and <y>.

   Pre:
   Post: intMax' = max(x, y)
*/
{
  if (x > y)
    return x;

  else
    return y;

}  /* intMax */

static
int intMin(int x,
           int y)
/* Return the maximun of <x> and <y>.

   Pre:
   Post: intMax' = max(x, y)
*/
{
  if (x < y)
    return x;

  else
    return y;

}  /* intMin */

static
unsigned unsignedMax(unsigned x,
                     unsigned y)
/* Return the maximun of <x> and <y>.

   Pre:
   Post: unsignedMax' = max(x, y)
*/
{
  if (x > y)
    return x;

  else
    return y;

}  /* unsignedMax */

static
unsigned unsignedMin(unsigned x,
                     unsigned y)
/* Return the maximun of <x> and <y>.

   Pre:
   Post: unsignedMax' = max(x, y)
*/
{
  if (x < y)
    return x;

  else
    return y;

}  /* unsignedMin */

static
void doShapeTree(
  Tree        tree,
  unsigned    height,
  unsigned    yPosition,
  edge      **left,
  edge      **right,
  unsigned    xSeparation,
  unsigned    ySeparation)
/* Aestheticaly layout <tree> using Bloesch's SPE algorithm.  Assume the
   tree has height <height> in rasters its top edge is at <yPosition> and
   the x & y node separations are to be <xSeparation> and <ySeparation>.
   Return the left and right outlines of the layed out tree in <left> and
   <right>.

   Pre: tree != NULL &
        height(tree) = height
   Post: leftOutline(tree) = left &
         rightOutline(tree) = right
*/
{
  int        centre;        /* The x position of the root node. */
  unsigned   i;
  unsigned   j;
  edge     **leftOutline;	/* The left outlines of the subbranches */
  int        overlap;	/* The amount of overlap between two branches. */
  edge     **rightOutline;	/* The right outlines of the subbranches */

  if (tree->nrBranches == 0) { /* No children. */
    *left  = newEdge(height);
    *right = newEdge(height);
    (*left)->yPosition = yPosition+tree->height-1;
    (*right)->yPosition = yPosition+tree->height-1; }

  else { /* Children. */
    leftOutline = (edge **)malloc((size_t)(tree->nrBranches*sizeof(edge *)));
    rightOutline = (edge **)malloc((size_t)(tree->nrBranches*sizeof(edge *)));
    
    for (i = 0; i < tree->nrBranches; i++)
      doShapeTree(tree->branches[i], height,
                  (int)(yPosition+tree->height+ySeparation),
                  &leftOutline[i], &rightOutline[i],
                  xSeparation, ySeparation);

    /* Set up left and right. */
    *left = leftOutline[0];
    *right = rightOutline[0];   

    /* Position branches relative to the left branch. */
    tree->branches[0]->x = 0;
    for (i = 0; i < tree->nrBranches - 1; i++) {
      /* Calculate maximum overlap. */
      overlap = 0;
      for (j = yPosition+tree->height+ySeparation;
           j <= intMin(leftOutline[i+1]->yPosition, (*right)->yPosition);
           j++)
        overlap = intMax(overlap, leftOutline[i+1]->offset[j] + 
                                  (*right)->offset[j]);

      /* Push branches apart. */
      tree->branches[i+1]->x = overlap+xSeparation;

      /* Adjust left outline. */
      for (j = (*left)->yPosition+1; j <= leftOutline[i+1]->yPosition; j++)
        (*left)->offset[j] = leftOutline[i+1]->offset[j] -
                                tree->branches[i+1]->x;
      (*left)->yPosition = unsignedMax((*left)->yPosition,
                                       leftOutline[i+1]->yPosition);

      /* Adjust right outline */
      for (j = yPosition; j <= rightOutline[i+1]->yPosition; j++)
        (*right)->offset[j] = rightOutline[i+1]->offset[j] +
                                tree->branches[i+1]->x;
      (*right)->yPosition = unsignedMax((*right)->yPosition,
                                        rightOutline[i+1]->yPosition);
    } /* for */
   
    if (tree->nrBranches > 1) {
      /* Position branches relative to the centre. */
      centre = tree->branches[tree->nrBranches-1]->x/2;

      for (i = 0; i < tree->nrBranches; i++)
        tree->branches[i]->x -= centre;

      for (i = yPosition; i <= (*left)->yPosition; i++)
        (*left)->offset[i] += centre;

      for (i = yPosition; i <= (*right)->yPosition; i++)
        (*right)->offset[i] -= centre;  
     } /* if */

    /* Free the old outlines. */
    for (i = 1; i < tree->nrBranches; i++) {
      (void)free((char *)leftOutline[i]);
      (void)free((char *)rightOutline[i]);
    } /* for */
  } /* if */

  /* Add node to tree outline. */
  for (i = yPosition - ySeparation; i < yPosition+tree->height; i++) {
    (*left)->offset[i] = tree->width/2;
    (*right)->offset[i] = (tree->width+1)/2;
  } /* for */
  
  tree->y = yPosition;

} /* doShapeTree */

static
int treeHeight(Tree     tree,
               unsigned ySeparation)
/* Return the height of <tree> assuming the y separation is <ySeparation>.

   Pre:
   Post:
*/
{
  unsigned height = 0;
  unsigned i;
  unsigned newHeight;

  if (tree != (Tree)NULL) {
    for (i = 0; i < tree->nrBranches; i++) {
      newHeight = treeHeight(tree->branches[i], ySeparation);
      if (newHeight > height)
        height = newHeight;
    } /* for */
    height += ySeparation+tree->height;
  } /* if */

  return height;

} /* treeHeight */

void shapeTree(Tree      tree,
               unsigned *height,
               unsigned *leftWidth,
               unsigned *rightWidth,
               unsigned  xSeparation,
               unsigned  ySeparation)
{
  unsigned  i;
  edge     *left;  /* Left outline of tree. */
  edge     *right; /* Right outline of tree. */

  *height = treeHeight(tree, ySeparation);

  doShapeTree(tree, *height, ySeparation, &left, &right,
              xSeparation, ySeparation);

  *leftWidth = 0;
  *rightWidth = 0;
  for (i = 0; i < *height; i++) {
    *leftWidth = (unsigned)intMax((int)*leftWidth, left->offset[i]);
    *rightWidth = (unsigned)intMax((int)*rightWidth, right->offset[i]);
  } /* for */

  (void)free((char *)left);
  (void)free((char *)right);

} /* shapeTree */
