
#include <iostream>
#include <queue>
#include <fstream>
#include "ArgumentManager.h"
using namespace std;

struct node{
  node* left;
  node* right;
  int data;
  int level;

  node(int dat) {
    data = dat;
    left = NULL; 
    right = NULL;
    
  }
};

class avlTree{
private:
  node* root;

public:
  avlTree() {root = NULL;}

  node* getRoot(){ return root; }

  int getHeight(node* cu){
    if(cu ==NULL) 
      return 0;
    int lH = getHeight(cu->left);
    int rH = getHeight(cu->right);
    
    if(lH > rH)
      return ++lH;
    else
      return ++rH;
  }

  node* insertNode(node* cu, int dat){
    if(cu == NULL) {
      cu = new node(dat);

      if(root == NULL) 
        root = cu;
    }
      
    else if(dat > cu->data) 
      cu->right = insertNode(cu->right, dat);

    else if(dat < cu->data)
      cu->left = insertNode(cu->left, dat);
      
    else if(cu->data == dat) 
      return cu;

    cu = balanceFac(cu); 
    return cu;
  }

  node* removeNode(node* cu, int dat){
    if(cu ==NULL) 
      return NULL;

    if(cu->data == dat) {
      if(cu->left == NULL && cu->right == NULL) { // leaf node
        if(cu == root)
          root = NULL;

        delete cu;
        return NULL;
      }

    else if(cu->right != NULL)
      cu = swapMinSuc(cu->right, cu, cu);

    else if(cu->right == NULL)
      cu = swapMaxPred(cu->left, cu, cu);
    }

    else if(dat > cu->data)
      cu->right = removeNode(cu->right, dat);
    else if(dat < cu->data)
      cu->left = removeNode(cu->left, dat);

    cu = balanceFac(cu);
    return cu;
  }

  node* singleLeftRotate(node* rootV1){
    node* newRoot = rootV1->right;
    rootV1->right = newRoot->left;
    newRoot->left = rootV1;

    if(rootV1 == root) 
      root = newRoot;
    return newRoot;
  }

  node* singleRightRotate(node* rootV1){
    node *newRoot = rootV1->left;
    rootV1->left = newRoot->right;
    newRoot->right = rootV1;

    if(rootV1 == root) 
      root = newRoot;
    
    return newRoot;
  }

  node* swapMinSuc(node* cu, node* orig, node *parent){
      if(cu->left == NULL){
        if(orig != parent) {
          parent->left = cu->right;
          cu->right = orig->right;
        }

        cu->left = orig->left;
        if(orig==root)
          root = cu;

        delete orig;
        return cu;
    }
        node *t1 = swapMinSuc(cu->left, orig, cu);
        return t1;
  }

  node* swapMaxPred(node* cu, node* orig, node *parent){
    if(cu->right == NULL) {
      if(orig!=parent) {
        parent->right = cu->left;
        cu->left = orig->left;
      }
      cu->right = orig->right;
      if(root==orig)
        root = cu;

      delete orig;
      return cu;
    }
    node *t1 = swapMaxPred(cu->right, orig, cu);
    return t1;
  }

  node* balanceFac(node *cu) {
    if(cu == NULL) 
      return NULL;

    int lH = getHeight(cu->left);
    int rH = getHeight(cu->right);
    int lRight = 0;
    int lLeft = 0;
    int rLeft = 0;
    int rRight = 0;

    if(lH - rH >1) {// left bigger than right
      if(cu->left->right !=NULL)
        lRight = getHeight(cu->left->right);
      if(cu->left->left != NULL)
        lLeft = getHeight(cu->left->left); 
    
      if(lRight > lLeft) {
        cu->left = singleLeftRotate(cu->left);
        cu = singleRightRotate(cu);
      }
      else
        cu = singleRightRotate(cu);
    }
    else if (lH - rH < -1) {
      if(cu->right->left !=NULL)
        rLeft = getHeight(cu->right->left);
      if(cu->right->right !=NULL)
        rRight = getHeight(cu->right->right);

      if(rLeft > rRight) {
        cu->right = singleRightRotate(cu->right);
        cu = singleLeftRotate(cu);
      }
      else 
        cu= singleLeftRotate(cu);
    }
    return cu;
  }

  void setLevel(int lev, node *cu){
    if(cu != NULL){
      cu->level = lev;
      setLevel(lev+1, cu->left);
      setLevel(lev+1, cu->right);
    }
  }


  
};

int main(int argc, char* argv[]){
  ArgumentManager am(argc, argv);
  ifstream inputFile(am.get("input"));
  ofstream outputFile(am.get("output"));
  //ifstream inputFile("input1.txt");
  //ofstream output("output1.txt");
  
  avlTree plant;
  string tStr;
  int count = 0;
  node* tempN;
  queue<node*> q;

  while(!inputFile.fail()){
    if(tStr.find("Insert") != -1)
      plant.insertNode(plant.getRoot(), stoi(tStr.substr(7, tStr.size()-8)));
  
    if(tStr.find("Remove") != -1)
      plant.removeNode(plant.getRoot(), stoi(tStr.substr(7, tStr.size()-8)));
  
    inputFile >> tStr;
  }
  
    plant.setLevel(0, plant.getRoot());
    q.push(plant.getRoot());  
    count = 0;
    while(!q.empty()){
      tempN = q.front();
      q.pop();

      if(tempN != NULL){
        q.push(tempN->left);
        q.push(tempN->right);
        
        if(tempN->level != count){
          outputFile << endl;
          count = tempN->level;
        }
        outputFile << tempN->data << " ";
      }     
    }
} 