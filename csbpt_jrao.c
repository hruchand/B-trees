#ifndef PRODUCT
#define ASSERT(x) assert(x);
#else
#define ASSERT(x) ((void)0)
#endif /*PRODUCT*/




#include <stdio.h>
#include<stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


struct IPair {
  int d_key;
  void* d_child;
};

/* a B+-Tree internal node of 64 bytes.
   corresponds to a cache line size of 64 bytes.
   We can store a maximum of 7 keys and 8 child pointers in each node. */
struct BPINODE64 {
	struct IPair d_entry[8];
    /* d_entry[0].d_key is used to store the number of used keys in this node*/
};

/* one B+-Tree leaf entry */
struct LPair {
  int d_key;
  int d_tid;       /* tuple ID */
};

/* a B+-Tree internal node of 64 bytes.
   corresponds to a cache line size of 64 bytes.
   We can store a maximum of 7 keys and 8 child pointers in each node. */
struct BPLNODE64 {
  int        d_num;       /* number of keys in the node */
  void*      d_flag;      /* this pointer is always set to null and is used to distinguish
			     between and internal node and a leaf node */
  struct BPLNODE64* d_prev;      /* backward and forward pointers */
  struct BPLNODE64* d_next;
  struct LPair d_entry[6];       /* <key, TID> pairs */
};

/* a CSB+-Tree internal node of 64 bytes.
   corresponds to a cache line size of 64 bytes.
   We put all the child nodes of any given node continuously in a node group and
   store explicitly only the pointer to the first node in the node group.
   We can store a maximum of 14 keys in each node.
   Each node has a maximum of 15 implicit child nodes.
*/
struct CSBINODE64 {
  int    d_num;
  void*  d_firstChild;       //pointer to the first child in a node group
  int    d_keyList[14];


};

void*          g_free=0;
int            g_expand=0;
int            g_split=0;
//Stat           g_stat_rec;
int            g_space_used=0;
struct BPINODE64*     g_bp_root64;
struct CSBINODE64*    g_csb_root64;
struct GCSBINODE64_2* g_gcsb_root64_2;
struct GCSBINODE64_3* g_gcsb_root64_3;
char*          g_pool_start;
char*          g_pool_curr;
char*          g_pool_end;


void csbBulkLoad64(int n,struct LPair* a, int iUpper, int lUpper);

 void main()
{
	struct LPair* temmp = NULL;
	printf("we are ON");
	csbBulkLoad64(5,temmp,7,7);
 return 0;
}











void csbBulkLoad64(int n,struct LPair* a, int iUpper, int lUpper) {
	struct BPLNODE64 *lcurr, *start, *lprev;
	struct  CSBINODE64 *iLow, *iHigh, *iHighStart, *iLowStart;
  int temp_key;
  void* temp_child;
  int i, j, nLeaf, nHigh, nLow, remainder;

  // first step, populate all the leaf nodes
  nLeaf=(n+lUpper-1)/lUpper;
  lcurr = (struct CSBINODE64*) malloc (sizeof(struct BPLNODE64)*nLeaf);
  lcurr->d_flag=0;
  lcurr->d_num=0;
  lcurr->d_prev=0;
  start=lcurr;

  for (i=0; i<n; i++) {
    if (lcurr->d_num >= lUpper) { // at the beginning of a new node
#ifdef FIX_HARDCODE
      // fill the empty slots with MAX_KEY
      for (j=lcurr->d_num; j<6; j++)
	lcurr->d_entry[j].d_key=MAX_KEY;
#endif
      lprev=lcurr;
      lcurr++;
      lcurr->d_flag=0;
      lcurr->d_num=0;
      lcurr->d_prev=lprev;
   //   ASSERT(lprev);
      lprev->d_next=lcurr;
    }
    lcurr->d_entry[lcurr->d_num]=a[i];
    lcurr->d_num++;
  }
  lcurr->d_next=0;
#ifdef FIX_HARDCODE
      // fill the empty slots with MAX_KEY
      for (j=lcurr->d_num; j<6; j++)
	lcurr->d_entry[j].d_key=MAX_KEY;
#endif

  // second step, build the internal nodes, level by level.
  // we can put IUpper keys and IUpper+1 children (implicit) per node
  nHigh=(nLeaf+iUpper)/(iUpper+1);
  remainder=nLeaf%(iUpper+1);
  iHigh=(struct CSBINODE64*) malloc (sizeof(struct CSBINODE64)*nHigh);
  iHigh->d_num=0;
  iHigh->d_firstChild=start;
  iHighStart=iHigh;
  lcurr=start;
  for (i=0; i<((remainder==0)?nHigh:(nHigh-1)); i++) {
    iHigh->d_num=iUpper;
    iHigh->d_firstChild=lcurr;
    for (j=0; j<iUpper+1; j++) {
      iHigh->d_keyList[j]=lcurr->d_entry[lcurr->d_num-1].d_key;
      lcurr++;
    }
#ifdef FIX_HARDCODE
    for (j=iUpper+1; j<14; j++)
      iHigh->d_keyList[j]=MAX_KEY;
#endif
    iHigh++;
  }
  if (remainder==1) {
    //this is a special case, we have to borrow a key from the left node if there is one
    //leaf node remaining.
    iHigh->d_keyList[0]=(iHigh-1)->d_keyList[iUpper];
    (iHigh-1)->d_num--;
    iHigh->d_num=1;
    iHigh->d_firstChild=lcurr-1;
    lcurr++;
#ifdef FIX_HARDCODE
    for (j=1; j<14; j++)
      iHigh->d_keyList[j]=MAX_KEY;
    iHigh++;
#endif
  }
  else if (remainder>1) {
    iHigh->d_firstChild=lcurr;
    for (i=0; i<remainder; i++) {
      iHigh->d_keyList[i]=lcurr->d_entry[lcurr->d_num-1].d_key;
      lcurr++;
    }
    iHigh->d_num=remainder-1;
#ifdef FIX_HARDCODE
    for (j=remainder; j<14; j++)
      iHigh->d_keyList[j]=MAX_KEY;
    iHigh++;
#endif
  }
#ifdef FIX_HARDCODE
  (iHigh-1)->d_keyList[(iHigh-1)->d_num]=MAX_KEY;
#endif
 // ASSERT((lcurr-nLeaf) == start);

  while (nHigh>1) {
    nLow=nHigh;
    iLow=iHighStart;
    iLowStart=iLow;
    nHigh=(nLow+iUpper)/(iUpper+1);
    remainder=nLow%(iUpper+1);
    iHigh=(struct CSBINODE64*) malloc (sizeof(struct CSBINODE64)*nHigh);
    iHigh->d_num=0;
    iHigh->d_firstChild=iLow;
    iHighStart=iHigh;
    for (i=0; i<((remainder==0)?nHigh:(nHigh-1)); i++) {
      iHigh->d_num=iUpper;
      iHigh->d_firstChild=iLow;
      for (j=0; j<iUpper+1; j++) {
	//ASSERT(iLow->d_num<14);
	iHigh->d_keyList[j]=iLow->d_keyList[iLow->d_num];
	iLow++;
      }
#ifdef FIX_HARDCODE
      for (j=iUpper+1; j<14; j++)
	iHigh->d_keyList[j]=MAX_KEY;
#endif
      iHigh++;
    }
    if (remainder==1) { //this is a special case, we have to borrow a key from the left node
      iHigh->d_keyList[0]=(iHigh-1)->d_keyList[iUpper];
      (iHigh-1)->d_num--;
      iHigh->d_num=1;
      iHigh->d_firstChild=iLow-1;
      iLow++;
#ifdef FIX_HARDCODE
      for (j=1; j<14; j++)
	iHigh->d_keyList[j]=MAX_KEY;
      iHigh++;
#endif
    }
    else if (remainder>1) {
      iHigh->d_firstChild=iLow;
      for (i=0; i<remainder; i++) {
	//ASSERT(iLow->d_num<14);
	iHigh->d_keyList[i]=iLow->d_keyList[iLow->d_num];
	iLow++;
      }
      iHigh->d_num=remainder-1;
#ifdef FIX_HARDCODE
      for (j=remainder; j<14; j++)
	iHigh->d_keyList[j]=MAX_KEY;
      iHigh++;
#endif
    }
#ifdef FIX_HARDCODE
    (iHigh-1)->d_keyList[(iHigh-1)->d_num]=MAX_KEY;
#endif
  //  ASSERT((iLow-nLow) == iLowStart);
  }

  g_csb_root64=iHighStart;
}
