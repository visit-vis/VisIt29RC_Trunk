/*****************************************************************************
*
* Copyright (c) 2000 - 2015, Lawrence Livermore National Security, LLC
* Produced at the Lawrence Livermore National Laboratory
* LLNL-CODE-442911
* All rights reserved.
*
* This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
* full copyright notice is contained in the file COPYRIGHT located at the root
* of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
*
* Redistribution  and  use  in  source  and  binary  forms,  with  or  without
* modification, are permitted provided that the following conditions are met:
*
*  - Redistributions of  source code must  retain the above  copyright notice,
*    this list of conditions and the disclaimer below.
*  - Redistributions in binary form must reproduce the above copyright notice,
*    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
*    documentation and/or other materials provided with the distribution.
*  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
*    be used to endorse or promote products derived from this software without
*    specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
* ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
* LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
* DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
* SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
* CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
* LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
* OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*
*****************************************************************************/

#include <PyColorAttribute.h>
#include <ObserverToCallback.h>
#include <stdio.h>
#include <snprintf.h>

// ****************************************************************************
// Module: PyColorAttribute
//
// Purpose: 
//   This class contains RGBA color information
//
// Note:       Autogenerated by xml2python. Do not modify by hand!
//
// Programmer: xml2python
// Creation:   omitted
//
// ****************************************************************************

//
// This struct contains the Python type information and a ColorAttribute.
//
struct ColorAttributeObject
{
    PyObject_HEAD
    ColorAttribute *data;
    bool        owns;
    PyObject   *parent;
};

//
// Internal prototypes
//
static PyObject *NewColorAttribute(int);

std::string
PyColorAttribute_ToString(const ColorAttribute *atts, const char *prefix)
{
    std::string str;
    char tmpStr[1000];

    {   const unsigned char *color = atts->GetColor();
        SNPRINTF(tmpStr, 1000, "%scolor = (", prefix);
        str += tmpStr;
        for(int i = 0; i < 4; ++i)
        {
            SNPRINTF(tmpStr, 1000, "%d", int(color[i]));
            str += tmpStr;
            if(i < 3)
            {
                SNPRINTF(tmpStr, 1000, ", ");
                str += tmpStr;
            }
        }
        SNPRINTF(tmpStr, 1000, ")\n");
        str += tmpStr;
    }
    return str;
}

static PyObject *
ColorAttribute_Notify(PyObject *self, PyObject *args)
{
    ColorAttributeObject *obj = (ColorAttributeObject *)self;
    obj->data->Notify();
    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
ColorAttribute_SetColor(PyObject *self, PyObject *args)
{
    ColorAttributeObject *obj = (ColorAttributeObject *)self;

    unsigned char *cvals = obj->data->GetColor();
    if(!PyArg_ParseTuple(args, "cccc", &cvals[0], &cvals[1], &cvals[2], &cvals[3]))
    {
        PyObject     *tuple;
        if(!PyArg_ParseTuple(args, "O", &tuple))
            return NULL;

        if(PyTuple_Check(tuple))
        {
            if(PyTuple_Size(tuple) != 4)
                return NULL;

            PyErr_Clear();
            for(int i = 0; i < PyTuple_Size(tuple); ++i)
            {
                int c;
                PyObject *item = PyTuple_GET_ITEM(tuple, i);
                if(PyFloat_Check(item))
                    c = int(PyFloat_AS_DOUBLE(item));
                else if(PyInt_Check(item))
                    c = int(PyInt_AS_LONG(item));
                else if(PyLong_Check(item))
                    c = int(PyLong_AsDouble(item));
                else
                    c = 0;

                if(c < 0) c = 0;
                if(c > 255) c = 255;
                cvals[i] = (unsigned char)(c);
            }
        }
        else
            return NULL;
    }

    // Mark the color in the object as modified.
    obj->data->SelectColor();

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
ColorAttribute_GetColor(PyObject *self, PyObject *args)
{
    ColorAttributeObject *obj = (ColorAttributeObject *)self;
    // Allocate a tuple the with enough entries to hold the color.
    PyObject *retval = PyTuple_New(4);
    const unsigned char *color = obj->data->GetColor();
    for(int i = 0; i < 4; ++i)
        PyTuple_SET_ITEM(retval, i, PyInt_FromLong(long(color[i])));
    return retval;
}



PyMethodDef PyColorAttribute_methods[COLORATTRIBUTE_NMETH] = {
    {"Notify", ColorAttribute_Notify, METH_VARARGS},
    {"SetColor", ColorAttribute_SetColor, METH_VARARGS},
    {"GetColor", ColorAttribute_GetColor, METH_VARARGS},
    {NULL, NULL}
};

//
// Type functions
//

static void
ColorAttribute_dealloc(PyObject *v)
{
   ColorAttributeObject *obj = (ColorAttributeObject *)v;
   if(obj->parent != 0)
       Py_DECREF(obj->parent);
   if(obj->owns)
       delete obj->data;
}

static int
ColorAttribute_compare(PyObject *v, PyObject *w)
{
    ColorAttribute *a = ((ColorAttributeObject *)v)->data;
    ColorAttribute *b = ((ColorAttributeObject *)w)->data;
    return (*a == *b) ? 0 : -1;
}

PyObject *
PyColorAttribute_getattr(PyObject *self, char *name)
{
    if(strcmp(name, "color") == 0)
        return ColorAttribute_GetColor(self, NULL);

    return Py_FindMethod(PyColorAttribute_methods, self, name);
}

int
PyColorAttribute_setattr(PyObject *self, char *name, PyObject *args)
{
    // Create a tuple to contain the arguments since all of the Set
    // functions expect a tuple.
    PyObject *tuple = PyTuple_New(1);
    PyTuple_SET_ITEM(tuple, 0, args);
    Py_INCREF(args);
    PyObject *obj = NULL;

    if(strcmp(name, "color") == 0)
        obj = ColorAttribute_SetColor(self, tuple);

    if(obj != NULL)
        Py_DECREF(obj);

    Py_DECREF(tuple);
    if( obj == NULL)
        PyErr_Format(PyExc_RuntimeError, "Unable to set unknown attribute: '%s'", name);
    return (obj != NULL) ? 0 : -1;
}

static int
ColorAttribute_print(PyObject *v, FILE *fp, int flags)
{
    ColorAttributeObject *obj = (ColorAttributeObject *)v;
    fprintf(fp, "%s", PyColorAttribute_ToString(obj->data, "").c_str());
    return 0;
}

PyObject *
ColorAttribute_str(PyObject *v)
{
    ColorAttributeObject *obj = (ColorAttributeObject *)v;
    return PyString_FromString(PyColorAttribute_ToString(obj->data,"").c_str());
}

//
// The doc string for the class.
//
#if PY_MAJOR_VERSION > 2 || (PY_MAJOR_VERSION == 2 && PY_MINOR_VERSION >= 5)
static const char *ColorAttribute_Purpose = "This class contains RGBA color information";
#else
static char *ColorAttribute_Purpose = "This class contains RGBA color information";
#endif

//
// The type description structure
//
static PyTypeObject ColorAttributeType =
{
    //
    // Type header
    //
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                   // ob_size
    "ColorAttribute",                    // tp_name
    sizeof(ColorAttributeObject),        // tp_basicsize
    0,                                   // tp_itemsize
    //
    // Standard methods
    //
    (destructor)ColorAttribute_dealloc,  // tp_dealloc
    (printfunc)ColorAttribute_print,     // tp_print
    (getattrfunc)PyColorAttribute_getattr, // tp_getattr
    (setattrfunc)PyColorAttribute_setattr, // tp_setattr
    (cmpfunc)ColorAttribute_compare,     // tp_compare
    (reprfunc)0,                         // tp_repr
    //
    // Type categories
    //
    0,                                   // tp_as_number
    0,                                   // tp_as_sequence
    0,                                   // tp_as_mapping
    //
    // More methods
    //
    0,                                   // tp_hash
    0,                                   // tp_call
    (reprfunc)ColorAttribute_str,        // tp_str
    0,                                   // tp_getattro
    0,                                   // tp_setattro
    0,                                   // tp_as_buffer
    Py_TPFLAGS_CHECKTYPES,               // tp_flags
    ColorAttribute_Purpose,              // tp_doc
    0,                                   // tp_traverse
    0,                                   // tp_clear
    0,                                   // tp_richcompare
    0                                    // tp_weaklistoffset
};

//
// Helper functions for object allocation.
//

static ColorAttribute *defaultAtts = 0;
static ColorAttribute *currentAtts = 0;

static PyObject *
NewColorAttribute(int useCurrent)
{
    ColorAttributeObject *newObject;
    newObject = PyObject_NEW(ColorAttributeObject, &ColorAttributeType);
    if(newObject == NULL)
        return NULL;
    if(useCurrent && currentAtts != 0)
        newObject->data = new ColorAttribute(*currentAtts);
    else if(defaultAtts != 0)
        newObject->data = new ColorAttribute(*defaultAtts);
    else
        newObject->data = new ColorAttribute;
    newObject->owns = true;
    newObject->parent = 0;
    return (PyObject *)newObject;
}

static PyObject *
WrapColorAttribute(const ColorAttribute *attr)
{
    ColorAttributeObject *newObject;
    newObject = PyObject_NEW(ColorAttributeObject, &ColorAttributeType);
    if(newObject == NULL)
        return NULL;
    newObject->data = (ColorAttribute *)attr;
    newObject->owns = false;
    newObject->parent = 0;
    return (PyObject *)newObject;
}

///////////////////////////////////////////////////////////////////////////////
//
// Interface that is exposed to the VisIt module.
//
///////////////////////////////////////////////////////////////////////////////

PyObject *
ColorAttribute_new(PyObject *self, PyObject *args)
{
    int useCurrent = 0;
    if (!PyArg_ParseTuple(args, "i", &useCurrent))
    {
        if (!PyArg_ParseTuple(args, ""))
            return NULL;
        else
            PyErr_Clear();
    }

    return (PyObject *)NewColorAttribute(useCurrent);
}

//
// Plugin method table. These methods are added to the visitmodule's methods.
//
static PyMethodDef ColorAttributeMethods[] = {
    {"ColorAttribute", ColorAttribute_new, METH_VARARGS},
    {NULL,      NULL}        /* Sentinel */
};

static Observer *ColorAttributeObserver = 0;

std::string
PyColorAttribute_GetLogString()
{
    std::string s("ColorAttribute = ColorAttribute()\n");
    if(currentAtts != 0)
        s += PyColorAttribute_ToString(currentAtts, "ColorAttribute.");
    return s;
}

static void
PyColorAttribute_CallLogRoutine(Subject *subj, void *data)
{
    typedef void (*logCallback)(const std::string &);
    logCallback cb = (logCallback)data;

    if(cb != 0)
    {
        std::string s("ColorAttribute = ColorAttribute()\n");
        s += PyColorAttribute_ToString(currentAtts, "ColorAttribute.");
        cb(s);
    }
}

void
PyColorAttribute_StartUp(ColorAttribute *subj, void *data)
{
    if(subj == 0)
        return;

    currentAtts = subj;
    PyColorAttribute_SetDefaults(subj);

    //
    // Create the observer that will be notified when the attributes change.
    //
    if(ColorAttributeObserver == 0)
    {
        ColorAttributeObserver = new ObserverToCallback(subj,
            PyColorAttribute_CallLogRoutine, (void *)data);
    }

}

void
PyColorAttribute_CloseDown()
{
    delete defaultAtts;
    defaultAtts = 0;
    delete ColorAttributeObserver;
    ColorAttributeObserver = 0;
}

PyMethodDef *
PyColorAttribute_GetMethodTable(int *nMethods)
{
    *nMethods = 1;
    return ColorAttributeMethods;
}

bool
PyColorAttribute_Check(PyObject *obj)
{
    return (obj->ob_type == &ColorAttributeType);
}

ColorAttribute *
PyColorAttribute_FromPyObject(PyObject *obj)
{
    ColorAttributeObject *obj2 = (ColorAttributeObject *)obj;
    return obj2->data;
}

PyObject *
PyColorAttribute_New()
{
    return NewColorAttribute(0);
}

PyObject *
PyColorAttribute_Wrap(const ColorAttribute *attr)
{
    return WrapColorAttribute(attr);
}

void
PyColorAttribute_SetParent(PyObject *obj, PyObject *parent)
{
    ColorAttributeObject *obj2 = (ColorAttributeObject *)obj;
    obj2->parent = parent;
}

void
PyColorAttribute_SetDefaults(const ColorAttribute *atts)
{
    if(defaultAtts)
        delete defaultAtts;

    defaultAtts = new ColorAttribute(*atts);
}

