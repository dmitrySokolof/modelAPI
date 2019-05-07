# 	  PyObject* runPythonFile(char *file_name, char *func_name, char *args[])
#     {
#         PyObject *pName, *pModule, *pFunc;
#         PyObject *pArgs, *pValue;
#         int i;

#         Py_Initialize();
#         /* Add path to your module to sys.path */
#         PyRun_SimpleString("import sys\n"
#                      "sys.path.append(\"/Users/dmitrysokolov/ModelAPI\")\n");

#         pName = PyString_FromString(file_name);
#         /* Error checking of pName left out */

#         pModule = PyImport_Import(pName);
#         Py_DECREF(pName);

#         if (pModule != NULL) 
#         {
#             pFunc = PyObject_GetAttrString(pModule, func_name);
#             /* pFunc is a new reference */

#             if (pFunc && PyCallable_Check(pFunc)) 
#             {
#                 size_t length = sizeof(args)/sizeof(char);
#                 pArgs = PyTuple_New(length);
#                 for (i = 0; i < length; ++i) 
#                 {
#                     pValue = PyInt_FromLong(std::stoi(args[i]));
#                     if (!pValue) 
#                     {
#                         Py_DECREF(pArgs);
#                         Py_DECREF(pModule);
#                         fprintf(stderr, "Cannot convert argument\n");
#                         return nullptr;
#                     }
#                     /* pValue reference stolen here: */
#                     PyTuple_SetItem(pArgs, i, pValue);
#                 }
#                 pValue = PyObject_CallObject(pFunc, pArgs);
#                 Py_DECREF(pArgs);
#                 if (pValue != NULL) 
#                 {
#                     printf("Result of call: %ld\n", PyInt_AsLong(pValue));
#                     Py_DECREF(pValue);
#                 }
#                 else 
#                 {
#                     Py_DECREF(pFunc);
#                     Py_DECREF(pModule);
#                     PyErr_Print();
#                     fprintf(stderr,"Call failed\n");
#                     return nullptr;
#                 }
#             }
#             else 
#             {
#                 if (PyErr_Occurred())
#                     PyErr_Print();
#                 fprintf(stderr, "Cannot find function \"%s\"\n", func_name);
#             }
#             Py_XDECREF(pFunc);
#             Py_DECREF(pModule);
#         }
#         else 
#         {
#             PyErr_Print();
#             fprintf(stderr, "Failed to load \"%s\"\n", file_name);
#             return nullptr;
#         }
#         Py_Finalize();

#         return pValue;
#     }
















# //
# //  vgg-face-api.cpp
# //  
# //
# //  Created by Dmitry Sokolov on 13/04/2019.
# //

# #include "vgg-face-api.h"

# namespace UUUU
# {
#     void runPythonFile(char *programName, char *file_name)
#     {
#         std::cout << programName << std::endl;
#         FILE *fd = std::fopen(file_name, "r");
#         if (fd)
#         {
#             // Py_SetProgramName(programName);
#             Py_Initialize();
#             PyRun_SimpleFileEx(fd, file_name, 1); // last parameter == 1 means to close the file before returning.
#             Py_Finalize();
#             std::fclose(fd);
#         }
#     }
    
#     int isEqualFace(std::string img1, std::string img2)
#     {
#         return 0;
#     }
    
#     void findFaceRect(std::string img)
#     {

#     }
    
#     void cropFace(std::string img)
#     {

#     }
    
#     std::string getName(std::string img)
#     {
#         return img;
#     }
# }


# int main(int argc, char *argv[])
# {
#     char file_name[] = "face_extractor.py";
#     // char func_name[] = "loadFaceDescriptor";
#     // char *args[0];
#     UUUU::runPythonFile(argv[0], file_name);
#     return 0;
# }
