typedef struct LPair {
  int d_key;
  int d_tid;       /* tuple ID */
};

/* a B+-Tree internal node of 64 bytes.
   corresponds to a cache line size of 64 bytes.
   We can store a maximum of 7 keys and 8 child pointers in each node. */
typedef struct BPLNODE64 {
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
typedef struct CSBINODE64 {
  int    d_num;
  void*  d_firstChild;       //pointer to the first child in a node group
  int    d_keyList[14];
//public:
  //int operator == (const CSBINODE64& node) {
    //if (d_num!=node.d_num)
      //return 0;
    //for (int i=0; i<d_num; i++)
      //if (d_keyList[i]!=node.d_keyList[i])
        //return 0;
    //return 1;
  //}
};
int array[] = {2,3,5,7,12,13,16,19,20,22,24,25,27,30,31,33,36,39};
struct LPair* a;
struct CSBINODE64* root;




void main(){
    int i =0;
    int iUpper = 2;
    int lUpper = 2;
    int count =(int)( sizeof(array) / sizeof(array[0]));
    printf ("count is %d\n",count);
    a = (struct LPair*)malloc(sizeof(struct LPair)*count);
    for(i =0;i<count;i++){
        a[i].d_key = array[i];
        a[i].d_tid = array[i];
        printf("%d\t",a[i].d_key);
    }
   csbBulkLoad64(count,a,iUpper,lUpper);
   // printf ("%d\n",root->d_keyList[2]);





}



// bulk load a CSB+-Tree
// n: size of the sorted array
// a: sorted leaf array
// iUpper: maximum number of keys for each internal node duing bulkload.
// lUpper: maximum number of keys for each leaf node duing bulkload.
// Note: iUpper has to be less than 14.
void csbBulkLoad64(int n, struct LPair* a, int iUpper, int lUpper) {
  struct BPLNODE64 *lcurr, *start, *lprev;
  struct CSBINODE64 *iLow, *iHigh, *iHighStart, *iLowStart;
  int temp_key;
  void* temp_child;
  int i, j, nLeaf, nHigh, nLow, remainder;

  // first step, populate all the leaf nodes
  nLeaf=(n+lUpper-1)/lUpper;
  lcurr=(struct BPLNODE64*) malloc(sizeof(struct BPLNODE64)*nLeaf);
  lcurr->d_flag=0;
  lcurr->d_num=0;
  lcurr->d_prev=0;
  start=lcurr;

  for (i=0; i<n; i++) {
    if (lcurr->d_num >= lUpper) { // at the beginning of a new node

      lprev=lcurr;
      lcurr++;
      lcurr->d_flag=0;
      lcurr->d_num=0;
      lcurr->d_prev=lprev;
      //ASSERT(lprev);
      lprev->d_next=lcurr;
    }
    lcurr->d_entry[lcurr->d_num]=a[i];
    lcurr->d_num++;
    //printf("lcurr first is %d \n",lcurr->d_entry[0].d_key);
    //printf("lcurr sec is %d \n",lcurr->d_entry[1].d_key);
  }

  lcurr->d_next=0;


  // second step, build the internal nodes, level by level.
  // we can put IUpper keys and IUpper+1 children (implicit) per node
  nHigh=(nLeaf+iUpper)/(iUpper+1);
  remainder=nLeaf%(iUpper+1);
  iHigh=(struct CSBINODE64*) malloc(sizeof(struct CSBINODE64)*nHigh);
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

  }
  else if (remainder>1) {
    iHigh->d_firstChild=lcurr;
    for (i=0; i<remainder; i++) {
      iHigh->d_keyList[i]=lcurr->d_entry[lcurr->d_num-1].d_key;
      lcurr++;
    }
    iHigh->d_num=remainder-1;
  }
  //ASSERT((lcurr-nLeaf) == start);

  while (nHigh>1) {
    nLow=nHigh;
    iLow=iHighStart;
    iLowStart=iLow;
    nHigh=(nLow+iUpper)/(iUpper+1);
    remainder=nLow%(iUpper+1);
    iHigh=(struct CSBINODE64*) malloc(sizeof(struct CSBINODE64)*nHigh);
    //iLow=(struct CSBINODE64*) malloc(sizeof(struct CSBINODE64)*nLow);
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

      iHigh++;
    }
    if (remainder==1) { //this is a special case, we have to borrow a key from the left node
      iHigh->d_keyList[0]=(iHigh-1)->d_keyList[iUpper];
      (iHigh-1)->d_num--;
      iHigh->d_num=1;
      iHigh->d_firstChild=iLow-1;
      iLow++;

    }
    else if (remainder>1) {
      iHigh->d_firstChild=iLow;
      for (i=0; i<remainder; i++) {
	//ASSERT(iLow->d_num<14);
	iHigh->d_keyList[i]=iLow->d_keyList[iLow->d_num];
	iLow++;
      }
      iHigh->d_num=remainder-1;

    }
   //nHigh--;
    //ASSERT((iLow-nLow) == iLowStart);
  }

  root=iHighStart;

printf ("I am \n%d\n",iHighStart->d_keyList[0]);
}


