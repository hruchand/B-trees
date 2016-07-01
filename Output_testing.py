mainIndex = 0

L=list()
with open('output.txt') as f:
    file = f.read().splitlines()

#print file
for line in file:
    arr_1 = file[mainIndex]
    arr_1=(file[mainIndex].replace("|",""))
    arr_1 = (arr_1.replace(" ",""))
   # print arr_1


    # sorting test
    i = 0
    if len(arr_1) ==1:
        print ("only one element!! test passed")
    while i < len(arr_1) -1:

        if arr_1[i] <= arr_1[i + 1]:
            print "sorting test passed"
          #  print arr_1[i + 1]

        else:
            print "test failed!"
        i += 1


    # length test
    keys = input( "enter the number of keys you insteted:")
   # print "array length", len(arr_1)
    L.append(len(arr_1))
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
    mainIndex +=1

    if x == keys:
        print ("key check test passed!")
    else:
        print ("key check failed")




