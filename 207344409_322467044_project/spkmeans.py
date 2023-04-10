import sys
import math
import os
import numpy as np
import pandas as pd
import random
import mykmeanssp as km
script_dir = os.path.dirname(__file__)


np.random.seed(0)

def Transpose_mat (A, n, m):
        AT = []
        for i in range(m):  
            inner_list = []
            for j in range(n):
                inner_list.append(A[j][i])
            AT.append(inner_list)
        return AT

def readingUserArgs():
    if len(sys.argv)!=4 and len(sys.argv)!= 3:
        print("An Error Has Occurred")
        exit()
    if len(sys.argv) == 4:
        k, goal, file_name = sys.argv[1], sys.argv[2], sys.argv[3]
    else:
        goal, file_name = sys.argv[1], sys.argv[2]
        k = 1; 


    if file_name.endswith('txt'):
        data = np.genfromtxt(file_name,dtype=float,delimiter=",")
    else:
        with open(file_name, 'r', encoding='utf-8-sig') as f: 
            data = np.genfromtxt(f, dtype=float, delimiter=',')
    

    data_df=pd.DataFrame(data)

    rowCount = data_df.shape[0]
    columnCount = data_df.shape[1]
    
    
    if len(sys.argv) == 4:
        if not (k).isnumeric():
            print("Invalid number of clusters!")
            sys.exit()
        k = int(k)
        if (k) % 1 != 0 or k <= 1 or k >= rowCount:
            print("Invalid number of clusters!")
            sys.exit()
    

    x=np.array(data_df)
    data_p= [[x[i][j] for j in range(columnCount)]  for i in range(rowCount)]


    def kFromEigengapHeuristic (eigenvalues):
        max_delta = 0
        eigenvalues = sorted(eigenvalues)
        k = 1
        for i in range(int(len(eigenvalues)/2)):
            delta_i = abs(eigenvalues[i] - eigenvalues[i+1])
            if delta_i > max_delta:
                max_delta = delta_i
                k = i
        return k+1
    
    
    
    
    def printMat(mat):
        for row in mat:
            print(','.join('{:.4f}'.format(v) for v in row))

    def printMatJacobi(mat,n):
        eigenvalues = mat[-1]
        print(','.join('{:.4f}'.format(val) for val in eigenvalues))
        for row in mat[:n]:  
            print(','.join('{:.4f}'.format(v) for v in row))

    
    if goal == "spk":
        wamRes = km.wam(data_p,rowCount,columnCount)
        ddgRes = km.ddg(wamRes,rowCount)
        glRes = km.gl(wamRes, ddgRes,rowCount)
        matRes = km.jacobi(glRes,rowCount)
        eigenvalues = matRes[-1]

        if k==1:
            k = kFromEigengapHeuristic(eigenvalues)
       
        matResTrans = Transpose_mat(matRes, rowCount+1, rowCount)
        matResTrans_sorted = sorted(matResTrans , key=lambda k: k[-1]) 
        U_Trans = matResTrans_sorted[:k] 
        U = [row[:rowCount] for row in U_Trans]
        U = Transpose_mat(U_Trans, k, rowCount)    

        calcKmeans(U, rowCount, k, k)
    
    elif goal == "wam":
        wamRes = km.wam(data_p,rowCount,columnCount)  
        printMat(wamRes)
    elif goal == "ddg":
        wamRes = km.wam(data_p,rowCount,columnCount)
        ddgRes = km.ddg(wamRes,rowCount)
        printMat(ddgRes)
    elif goal == "gl":
        wamRes = km.wam(data_p,rowCount,columnCount)
        ddgRes = km.ddg(wamRes,rowCount)
        glRes = km.gl(wamRes, ddgRes,rowCount)
        printMat(glRes)

    elif goal == "jacobi":
        matRes = km.jacobi(data_p, rowCount)
        printMatJacobi(matRes, rowCount)

    else:
        print("An Error Has Occurred")
        sys.exit()


    
    

def calcKmeans(U, rowCount, columnCount, k):

    def d(dataItem,centroid):
        res = np.sqrt(np.sum((dataItem-centroid) ** 2))
        return res

    iter = 300     
    epsilon = 0    
    data_points = pd.DataFrame(U)

    data_points.columns = [str(i) for i in range(data_points.shape[1])]
    data_points.shape[1]
    randomRow = np.random.choice(data_points.shape[0], 1)[0]
    newCentroid = np.array(data_points.iloc[randomRow])
    
    centroids = np.empty((0,k), int)
    centroids = np.append(centroids, np.array([newCentroid]), axis=0)
    centroids_indices = [randomRow]
    

    while (len(centroids) < k):

        prob = np.zeros(rowCount)
        for i in range(rowCount):
            x_i = data_points.iloc[i].as_matrix()
            minD = d(x_i , centroids[0])
            for centroid in centroids:
                currD = d(x_i , centroid)
                if currD < minD:
                    minD = currD

            prob[i] = minD

        if np.sum(prob) != 0:
            prob = prob / np.sum(prob)


        randomRow = np.random.choice(data_points.shape[0], 1 , p = prob)[0]
        newCentroid = np.array(data_points.iloc[randomRow])
        
        centroids = np.append(centroids, np.array([newCentroid]), axis=0)
        centroids_indices.append(randomRow)


    x = np.array(data_points)
    data_p= [[x[i][j] for j in range(columnCount)]  for i in range(rowCount)]
    cent_p= [[centroids[i][j] for j in range(columnCount)]  for i in range(k)]

    calcCentroids = km.spk(cent_p,data_p,k,rowCount,columnCount)  
    print(','.join(str(v) for v in centroids_indices))
    for centroid in calcCentroids:
        print(','.join('{:.4f}'.format(v) for v in centroid))


readingUserArgs()