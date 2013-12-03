/*
Author: Anthony Bloesch
Title: layout.h

Copyright (C) Anthony Bloesch 1991

Purpose: Allow n-ary trees with arbitrary nodes to be aestheticaly
         layed out.
*/

#ifndef _LAYOUT_H
#define _LAYOUT_H

typedef struct TreeNode {
  char             *name;
  char             *parent;
  unsigned          height;
  unsigned          width;
  unsigned          depth;
  int               x;
  int               y;
  unsigned	    nrBranches;
  struct TreeNode **branches;} TreeNode;
typedef TreeNode *Tree;
typedef void      NodeFunction(TreeNode *node); 

extern
int addBranch(Tree      branch,
              TreeNode *node);

extern
void deleteNode(TreeNode *node);

extern
void deleteTree(Tree tree);
/* Recursively deallocate the storage used by tree. 

   Pre: Valid(tree)
   Post: Invalid(tree)
*/

extern
Tree newTree(char     *nodeName,
             char     *parentName,
             unsigned  height,
             unsigned  width,
             unsigned  depth);
/* Return a new tree initialized to the specified height and width with the
   branches initialized to NULL.

   Pre:
   Post: newTree'->height = height &
         newTree'->width  = width &
         All i 0<=i<NR_BRANCHES -> newTree'->branches[i]
*/

extern
void shapeTree(Tree       tree,
               unsigned  *height,
               unsigned  *leftWidth,
               unsigned  *rightWidth,
               unsigned   xSeparation,
               unsigned   ySeparation);

extern
void inorderTraverse(Tree          tree,
                     NodeFunction *nodeFunction);

extern
void postorderTraverse(Tree          tree,
                       NodeFunction *nodeFunction);

extern
void preorderTraverse(Tree          tree,
                      NodeFunction *nodeFunction);

#endif /* _LAYOUT_H */
