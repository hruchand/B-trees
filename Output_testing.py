import re
mainIndex = 0
secIndex = 0

noOfNodes = list()
L=list()
with open('output.txt') as f:
    file = f.read().splitlines()
for line in file:
    arr_2 = file[secIndex]
    arr_2 = (arr_2.replace(" ", ""))
    i = 0
    y = 0
    while i < len(arr_2):
        if arr_2[i] == "|":
            y += 1

            i += 1
        else:
            i += 1
    noOfNodes.append(y)
    secIndex+=1
#print file
for line in file:
    arr_1 = file[mainIndex]

   # print arr_2

           # break

            #break;
    print ("no of nodes is:"), noOfNodes
    #arr_1=(file[mainIndex].replace("|",""))
    #arr_1 = (arr_1.replace(" ",""))
    # print arr_1


    # sorting test
    i = 0
    result =  [int(arr_1) for arr_1 in re.findall(r'\b\d+\b', arr_1)]
    #result = map(int,arr_1)
    print ("the result is" ),result
    if len(result) ==1:
        print ("only one element!! test passed")
    while i < len(result) -1:

        if result[i] <= result[i + 1]:
            print "sorting test passed"
          #  print arr_1[i + 1]

        else:
            print "test failed!"
        i += 1


    # length test
    keys = input( "enter the number of keys you insteted:")
   # print "array length", len(arr_1)
    L.append(len(result))
    print L
    #if len(L) == 1:
    x = L[0]
    #else:
   # print ("lenthh of L"), len(L)
    #x=1
    i=0
    while i < len(L)-1:
    #if L[i+1] != 0:
        x = x + L[i+1]
        i= i+1


   # print "keys is", keys
    #print "x is", x


    if x == keys:
        print ("key check test passed!")
    else:
        print ("key check failed")

    while i < len(noOfNodes)-1:
        if len(result)+1 == noOfNodes[mainIndex+1]:
            print ("node pointer relation test OK")
            i+=1
        else:
            print ("node pointer relation test failed")
            i+=1

    mainIndex +=1
