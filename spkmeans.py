import sys
import math
import os
import numpy as np
import pandas as pd
import random
import mykmeanssp as km
script_dir = os.path.dirname(__file__)


np.random.seed(0)
#path1 = "C:/Users/Erezd/OneDrive/Desktop/input_2_db_1.txt"
#path2 = "C:/Users/Erezd/OneDrive/Desktop/input_2_db_2.txt"


def readingUserArgs():
    if len(sys.argv)!=4 and len(sys.argv)!= 3:
        print("An Error Has Occurred")
        exit()
    if len(sys.argv) == 4:
        k, goal, file_name = sys.argv[1], sys.argv[2], sys.argv[3]
    else:
        goal, file_name = sys.argv[1], sys.argv[2]
        k = 1; #defult - need uristic  


    if file_name.endswith('txt'):
        data=np.genfromtxt(file_name,dtype=float,delimiter=",")
    else:
        with open(file_name, 'r', encoding='utf-8-sig') as f: 
            data = np.genfromtxt(f, dtype=float, delimiter=',')
    
    data_df=pd.DataFrame(data)
    data_df.columns = [str(i) for i in range(data_df.shape[1])]
    data_df = data_df.sort_values('0')

    rowCount = data_df.shape[0]
    columnCount = data_df.shape[1]

    #------
    if len(sys.argv) == 4:
        if not (k).isnumeric():
            print("Invalid number of clusters!")
            sys.exit()
        k = int(k)
        if (k) % 1 != 0 or k <= 1 or k >= rowCount:
            print("Invalid number of clusters!")
            sys.exit()
    

    x=np.array(data_df)
    data_p= [[x[i][j] for j in range(1,columnCount)]  for i in range(rowCount)]


    def kFromEigengapHeuristic (eigenvalues):
        max_delta = 0
        k = 1
        for i in range(len(eigenvalues)/2):
            delta_i = abs(eigenvalues[i] - eigenvalues[i+1])
            if delta_i > max_delta:
                max_delta = delta_i
                k = i
        return k
    
    
    def Transpose (A, n, m):
        AT = []
        inner_list = []
        for i in range(m):
            for j in range(n):
                inner_list.append(A[j][i])
                AT.append(inner_list)
        return AT
    


    if goal == "spk":
        print("todo")

    elif goal == "wam":
        wamRes = km.wam(data_p,rowCount,columnCount-1)
        printMat(wamRes)
    elif goal == "ddg":
        ddgRes = km.ddg(data_p,rowCount,columnCount-1)
        printMat(ddgRes)
    elif goal == "gl":
        glRes = km.gl(data_p,rowCount,columnCount-1)
        printMat(glRes)

    
    elif goal == "jacobi":
        matRes = km.jacobi(data_p,rowCount,columnCount-1)
        eigenvalues = matRes[rowCount-1]
        if k==1:
            k = kFromEigengapHeuristic(eigenvalues)
        
        matResTrans = Transpose(matRes, rowCount+1, columnCount)
        matResTrans_sorted = sorted(matRes , key=lambda k: k[rowCount])  # sort by the last element - eigenvalues
        U_Trans = matResTrans_sorted[:k]
        U = Transpose(U_Trans, k, rowCount+1)

        # remove last element in U in each row
        U = [row[:rowCount] for row in U]
        # send U to kmeans++ 
        #####
        #U_df = pd.DataFrame(U)
        calcKmeans(U, rowCount, k, k)

    else:
        print("An Error Has Occurred")
        sys.exit()

    def printMat(mat):
        for row in mat:
            print(','.join('{:.4f}'.format(v) for v in row))




def calcKmeans(U, rowCount, columnCount, k):

    def d(dataItem,centroid):
        dataItem = dataItem[1:]
        centroid = centroid[1:]
        res = np.sqrt(np.sum((dataItem-centroid) ** 2))
        return res

    iter = 300     # not sure
    epsilon = 0
    data_points = pd.DataFrame(U)


    randomRow = np.random.choice(data_points['0'], 1)[0]
    newCentroid = np.array(data_points[data_points['0']==randomRow])
    centroids_indices = [int(newCentroid[0][0])]
    centroids = np.copy(newCentroid)

    while (len(centroids) < k):

        prob = np.zeros(rowCount)
        for i in range(rowCount):
            x_i = data_points.iloc[i].to_numpy()
            minD = d(x_i , centroids[0])
            for centroid in centroids:
                currD = d(x_i , centroid)
                if currD < minD:
                    minD = currD

            prob[i] = minD

        if np.sum(prob) != 0:
            prob = prob / np.sum(prob)


        randomRow = np.random.choice(data_points['0'], 1 , p = prob)[0]
        newCentroid = np.array(data_points[data_points['0']==randomRow])

        centroids = np.append(centroids,newCentroid, axis=0)
        centroids_indices.append(int(newCentroid[0][0]))


    # print(centroids)
    x=np.array(data_points)
    data_p= [[x[i][j] for j in range(1,columnCount)]  for i in range(rowCount)]
    cent_p= [[centroids[i][j] for j in range(1,columnCount)]  for i in range(k)]

    calcCentroids = km.fit(cent_p,data_p,k,iter,epsilon,rowCount,columnCount-1)  #change to spk - not fit 
    print(','.join(str(v) for v in centroids_indices))
    for centroid in calcCentroids:
        print(','.join('{:.4f}'.format(v) for v in centroid))


    