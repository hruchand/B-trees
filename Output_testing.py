import re
mainIndex = 0
secIndex = 0
noOfNodes = list()
L=list()
# open the desired file
with open('output.txt') as f:
    file = f.read().splitlines()
# loop to get the number of "|" so that we can know the number of nodes in a particular level
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
# loop to run three tests
# 1. Sorting Test
# 2. No of keys check
# 3. Key pointer relationship test
for line in file:
    # Sorting test
    arr_1 = file[mainIndex]
    i = 0
    result =  [int(arr_1) for arr_1 in re.findall(r'\b\d+\b', arr_1)]
    if len(result) ==1:
        print ("Only one element. Sorting test: OK")
    while i < len(result) -1:
        if result[i] <= result[i + 1]:
            print "Sorting test: OK"
        else:
            print "Sorting test: FAILED"
        i += 1
    # No of keys Check
    keys = input( "Enter the number of keys inserted:")
    L.append(len(result))
    x = L[0]
    i=0
    while i < len(L)-1:
        x = x + L[i+1]
        i= i+1
    if x == keys:
        print ("key check test: OK!")
    else:
        print ("key check test: FAILED")

    #Node pointer relationship test
    while i < len(noOfNodes)-1:
        if len(result)+1 == noOfNodes[mainIndex+1]:
            print ("Node pointer relationship test: OK")
            i+=1
        else:
            print ("Node pointer relationship test: FAILED")
            i+=1
    mainIndex +=1
