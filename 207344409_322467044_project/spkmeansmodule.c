# define PY_SSIZE_T_CLEAN
# include <Python.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include "spkmeans.h"



double** GetCMat_FromPy(PyObject *lst, int n, int m);
PyObject* GetPyList_fromRes_Jacobi(int rows, int columns, double ***data);
PyObject* GetPyList_fromRes(int rows, int columns, double **data);


static PyObject* spk_capi(PyObject *self, PyObject *args)
{
    int i,j;
    PyObject *datalst;
    PyObject *centroidslst;
    int k;
    PyObject* res;
    double **rawres;
    int rowcount;
    int columncount;
    
    if(!PyArg_ParseTuple(args, "OOiii", &centroidslst, &datalst, &k, &rowcount, &columncount)) {
        return NULL; 
    }
    double **data=malloc(rowcount*sizeof(double*)) ;
    if(data == NULL){
        printf("An Error Has Occurred");
        exit(1);
    }
    double **centroids=malloc(k*sizeof(double*)) ;
    if(centroids == NULL){
        printf("An Error Has Occurred");
        exit(1);
    }
    for (i=0;i<rowcount;i++){
        data[i] = malloc(columncount*sizeof(double));
        if(data[i] == NULL){
            printf("An Error Has Occurred");
            exit(1);
        }
        for (j=0;j<columncount;j++){
            data[i][j]= PyFloat_AsDouble(PyList_GetItem(PyList_GetItem(datalst,i),j));
        }
    }
    for (i=0;i<k;i++){
        centroids[i] = malloc(columncount*sizeof(double));
        if(centroids[i] == NULL){
            printf("An Error Has Occurred");
            exit(1);
        }
        for (j=0;j<columncount;j++){
            centroids[i][j]= PyFloat_AsDouble(PyList_GetItem(PyList_GetItem(centroidslst,i),j));
        }
    }
    rawres = spk(centroids,data,k,rowcount,columncount); 
    res= GetPyList_fromRes(k,columncount,rawres);   
     for (i=0;i<rowcount;i++){
        free(data[i]);
        
    }
    for (i=0;i<k;i++){
        free(rawres[i]);

        free(centroids[i]);
    }
    free(data);
    free(centroids);
    free(rawres);
    return res;
}


static PyObject* wam_capi(PyObject *self, PyObject *args)
{
    PyObject *lst;
    double ** datapoints;
    PyObject* res;
    double **rawres;
    int rowcount,columncount;
    
    if(!PyArg_ParseTuple(args, "Oii", &lst, &rowcount, &columncount)) {
        return NULL; 
    }
    datapoints = GetCMat_FromPy(lst,rowcount,columncount);
    rawres = wam(datapoints,rowcount,columncount); 
    res= GetPyList_fromRes(rowcount,rowcount,rawres);
    freeMatrix(rawres, rowcount);
    freeMatrix(datapoints,rowcount);
    return res;
}
static PyObject* ddg_capi(PyObject *self, PyObject *args)
{
    PyObject *lst;
    PyObject* res;
    double **rawres, **wamres;
    int rowcount;
    
    if(!PyArg_ParseTuple(args, "Oi", &lst, &rowcount)) {
        return NULL; 
    }
    wamres = GetCMat_FromPy(lst,rowcount,rowcount);
    rawres = ddg(wamres,rowcount);
    res= GetPyList_fromRes(rowcount,rowcount,rawres);
    freeMatrix(rawres, rowcount);
    freeMatrix(wamres, rowcount);
    return res;
}

static PyObject* gl_capi(PyObject *self, PyObject *args)
{
    PyObject *lst1; 
    PyObject *lst2; 
    PyObject* res;
    double **rawres, **wamres, **ddgres;
    int rowcount;
    
    if(!PyArg_ParseTuple(args, "OOi", &lst1, &lst2, &rowcount)) {
        return NULL; 
    }
    wamres = GetCMat_FromPy(lst1,rowcount,rowcount);
    ddgres = GetCMat_FromPy(lst2,rowcount,rowcount);
    rawres = gl(wamres,ddgres,rowcount);

    res= GetPyList_fromRes(rowcount,rowcount,rawres);
    freeMatrix(rawres, rowcount);
    freeMatrix(wamres, rowcount);
    freeMatrix(ddgres, rowcount);
    return res;
}

static PyObject* jacobi_capi(PyObject *self, PyObject *args)
{
    PyObject *lst;
    double ** datapoints;
    PyObject* res;
    double ***jacobires;
    int n;
    
    if(!PyArg_ParseTuple(args, "Oi", &lst,&n)) {
        return NULL; 
    }
    datapoints = GetCMat_FromPy(lst,n,n);
    jacobires = jacobi(datapoints,n);


    res = GetPyList_fromRes_Jacobi(n,n,jacobires);
    freeMatrix(jacobires[0], n);
    freeMatrix(jacobires[1], n);
    free(jacobires);
    
    return res;
}



double** GetCMat_FromPy(PyObject *lst, int n, int m){
    int i,j;
    double **res= calloc(n, sizeof(double*));
    for (i=0;i<n;i++){
        res[i] = malloc(m*sizeof(double));
        if(res[i] == NULL){
            printf("An Error Has Occurred");
            exit(1);
        }
        for (j=0;j<m;j++){
            res[i][j]= PyFloat_AsDouble(PyList_GetItem(PyList_GetItem(lst,i),j));
        }
    }
    return res;
}

PyObject* GetPyList_fromRes_Jacobi(int rows, int columns, double ***data)
{
    int i,j;
    PyObject* python_val;
    PyObject* inner_list;

    PyObject* python_double;
    python_val = PyList_New(rows+1);
    for (i = 0; i < rows; ++i)
    {
        inner_list=PyList_New(columns);
        for(j = 0; j < columns;j++){
            python_double = Py_BuildValue("d", data[1][i][j]);
            PyList_SetItem(inner_list, j, python_double);
        }
        PyList_SetItem(python_val, i, inner_list);

    }
    inner_list=PyList_New(columns);
    for(j = 0; j<columns;j++){
        python_double = Py_BuildValue("d", data[0][j][j]);
        PyList_SetItem(inner_list, j, python_double);
    }
    PyList_SetItem(python_val, rows, inner_list);
    return python_val;
}

PyObject* GetPyList_fromRes(int rows, int columns, double **data)
{
    int i,j;
    PyObject* python_val;
    PyObject* inner_list;

    PyObject* python_double;
    python_val = PyList_New(rows);
    for (i = 0; i < rows; ++i)
    {
        inner_list=PyList_New(columns);
        for(j = 0; j<columns;j++){
            python_double = Py_BuildValue("d", data[i][j]);
            PyList_SetItem(inner_list, j, python_double);
        }
        PyList_SetItem(python_val, i, inner_list);

    }
    return python_val;
}





static PyMethodDef capiMethods[] = {
    {"wam",                   
      (PyCFunction) wam_capi, 
      METH_VARARGS,         
      PyDoc_STR("returns wam matrix as described in proj. desc")}, 
  
    
    {"ddg",                   
      (PyCFunction) ddg_capi, 
      METH_VARARGS,         
      PyDoc_STR("returns ddg matrix as described in proj. desc")},
    

    {"gl",                   
      (PyCFunction) gl_capi, 
      METH_VARARGS,         
      PyDoc_STR("returns gl matrix as described in proj. desc")}, 
 

    {"jacobi",                   
      (PyCFunction) jacobi_capi, 
      METH_VARARGS,         
      PyDoc_STR("returns jacobi matrix as described in proj. desc where last row is the Eigenvalues and first n rows are the matrix.")}, 
  
    
    {"spk",                   
      (PyCFunction) spk_capi, 
      METH_VARARGS,         
      PyDoc_STR("Kmeans algorithm from HW2")},
    {NULL, NULL, 0, NULL},
};


static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "mykmeanssp", 
    NULL, 
    -1,  
    capiMethods 
};

PyMODINIT_FUNC
PyInit_mykmeanssp(void)
{
    PyObject *m;
    m = PyModule_Create(&moduledef);
    if (!m) {
        return NULL;
    }
    return m;
}