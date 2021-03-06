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

// *************************************************************************
//                            avtVistaFileFormat.C
//
//    Mark C. Miller, Tue Oct 21 09:13:48 PDT 2008
//    Modified to use HAVE_LIBHDF5 throughout
// *************************************************************************

// this include directive refernces code provided by Jeff Keasler. I am
// including it using an include directive so that as Jeff updates this
// code, it can be easily incorporated into the plugin 
#include <VisitALE.c>

#include <visit-config.h>

#ifdef HAVE_LIBHDF5
// Define this symbol BEFORE including hdf5.h to indicate the HDF5 code
// in this file uses version 1.6 of the HDF5 API. This is harmless for
// versions of HDF5 before 1.8 and ensures correct compilation with
// version 1.8 and thereafter. When, and if, the HDF5 code in this file
// is explicitly upgraded to the 1.8 API, this symbol should be removed.
#define H5_USE_16_API
#include <hdf5.h>
#include <visit-hdf5.h>
#endif
#include <silo.h>

#include <stdarg.h>
#include <stdio.h>

#include <snprintf.h>

#include <string>
#include <map>

#include <FileFunctions.h>
#include <avtCallback.h>
#include <avtVistaFileFormat.h>
#include <avtVistaAle3dFileFormat.h>
#include <avtVistaDiabloFileFormat.h>

#include <DebugStream.h>

#include <BadIndexException.h>
#include <InvalidFilesException.h>
#include <UnexpectedValueException.h>
#include <ImproperUseException.h>
#include <Utility.h>


using std::string;
using std::map;

avtVistaFileFormat::
VistaTree::VistaTree(const char *buf, size_t size)
{
    theVistaString = new char[size+1];
    memcpy(theVistaString, buf, size);
    theVistaString[size] = '\0';
    VisitParseInternal(theVistaString, &top);
    ExtractAttr(top);
}

avtVistaFileFormat::
VistaTree::~VistaTree()
{
    VisitFreeVistaInfo(top);
    delete [] theVistaString;
}

const void
avtVistaFileFormat::
VistaTree::DumpTree() const
{
    VisitDumpTree(top);
}

const Node *
avtVistaFileFormat::
VistaTree::GetNodeFromPath(const Node *root, const char *path) const
{
    return VisitGetNodeFromPath(root, path);
}

char *
avtVistaFileFormat::
VistaTree::GetPathFromNode(const Node *root, const Node *node) const
{
    return VisitGetPathFromNode(root, node);
}

void
avtVistaFileFormat::
VistaTree::FindNodes(const Node *root, const char *re, Node ***results,
          int *nmatches, RecurseMode rmode) const
{
    VisitFindNodes(root, re, results, nmatches, rmode);
}

const Node *
avtVistaFileFormat::
VistaTree::GetTop() const
{
    return top;
}

const int avtVistaFileFormat::MASTER_FILE_INDEX = 0;
int avtVistaFileFormat::objcnt = 0;

#ifdef HAVE_LIBHDF5
// ****************************************************************************
//  Function:  avtVistaFileFormat::InitializeHDF5
//
//  Purpose:   Initialize interaction with the HDF5 library
//
//  Programmer:  Mark C. Miller 
//  Creation:    February 17, 2004 
//
// ****************************************************************************
static void
InitializeHDF5(void)
{
    debug5 << "Initializing HDF5 Library" << endl;
    H5open();
    H5Eset_auto(NULL, NULL);
}

// ****************************************************************************
//  Function:  avtVistaFileFormat::FinalizeHDF5
//
//  Purpose:   End interaction with the HDF5 library
//
//  Programmer:  Mark C. Miller 
//  Creation:    February 17, 2004 
//
//  Modifications:
//
//    Mark C. Miller, Tue Apr 14 17:21:23 PDT 2009
//    Replaced call to H5close with H5garbage_collect. Calling H5close is 
//    problematic as VisIt may have other plugins instantiated which are
//    still interacting with HDF5. Calling garbage collect routine achieves
//    the desired goal of reducing memory usage without effecting other
//    plugins that might still be using the HDF5 library. 
// ****************************************************************************
static void
FinalizeHDF5(void)
{
    debug5 << "Garbage collecting HDF5 Library" << endl;
    H5garbage_collect();
}
#endif

// ****************************************************************************
//  Function:  avtVistaFileFormat::CreateFileFormatInterface
//
//  Programmer:  Mark C. Miller 
//  Creation:    July 14, 2004 
//
//  Modifications:
//
//    Hank Childs, Fri Jan 28 13:38:43 PST 2005
//    Add CATCH_RETURN.
//
// ****************************************************************************
avtFileFormatInterface *
avtVistaFileFormat::CreateFileFormatInterface(const char *const *list, int nList)
{
    avtFileFormatInterface *ffi = 0;
    avtVistaFileFormat *vff = 0;

    if (list != 0 || nList > 0)
    {
        TRY
        {
            // Try and open the Vista file 
            vff = new avtVistaFileFormat(list[0]);

            if (vff == 0)
                CATCH_RETURN2(1, 0);

            switch (vff->GetFormatType())
            {
                case FTYPE_ALE3D:
                    ffi = avtVistaAle3dFileFormat::
                              CreateInterface(vff, list, nList);
                    break;
                case FTYPE_DIABLO:
                    ffi = avtVistaDiabloFileFormat::
                              CreateInterface(vff, list, nList);
                    break;
#if 0
                case FTYPE_UNKNOWN:
                    ffi = avtVistaUnknownFileFormat::
                        CreateInterface(vff, list, nList);
                    break;
#endif
                // case FTYPE_???: put new file formats here 
                default:
                    break;
            }

#if 0
            //
            // If we didn't succeed above, try the unknown format
            //
            if (ffi == 0)
            {
                ffi = avtVistaUnknownFileFormat::
                    CreateInterface(vff, list, nList);
            }
#endif

        }
        CATCH(VisItException)
        {
            if (vff)
                delete vff;
            RETHROW;
        }
        ENDTRY

    }

    // since the derived class' formats take ownership of the
    // vista file format, we can safely delete it
    if (vff)
        delete vff;

    return ffi;

}

// ****************************************************************************
//  Method: avtVistaFileFormat constructor
//
//  Programmer: Mark C. Miller
//  Creation:   Tue Feb 17 19:19:07 PST 2004
//
//  Modifications:
//
//    Mark C. Miller, Wed May 19 10:56:11 PDT 2004
//    Replaced use of basename/dirname with StringHelper functions
//
//    Mark C. Miller, Tue Oct 26 10:28:36 PDT 2004
//    Added argument for format type
//
//    Mark C. Miller, Fri May 16 15:49:07 PDT 2008
//    Moved code to set file type based on name of writer into the else
//    block of test of writer name for null.
//
// ****************************************************************************

avtVistaFileFormat::avtVistaFileFormat(const char *filename,
                                       VistaFormatType _formatType)
    : avtSTMDFileFormat(&filename, 1),  formatType(_formatType)
{
    wasMorphed = false;

    masterFileName = FileFunctions::Basename(filename);
    masterDirName = FileFunctions::Dirname(filename);

    fileHandles = new void*[MAX_FILES];
    for (int i = 0 ; i < MAX_FILES ; i++)
    {
        fileHandles[i] = 0;
    }

    //
    // Determine file format type and open it 
    //
    DBShowErrors(DB_NONE, 0);
    DBfile *dbfile = DBOpen((char *) filename, DB_UNKNOWN, DB_READ);
    if (dbfile != NULL)
    {
        isSilo = true;

        //
        // keep this file now and set error and precision behavior of silo 
        //
        DBForceSingle(1);
        DBShowErrors(DB_ALL, 0);
        fileHandles[MASTER_FILE_INDEX] = dbfile;
    }
#ifdef HAVE_LIBHDF5
    else
    {
        isSilo = false;

        //
        // Initialize HDF5 library
        //
        if (avtVistaFileFormat::objcnt == 0)
            InitializeHDF5();

        //
        // Open the HDF5 file
        //
        fileHandles[MASTER_FILE_INDEX] = (void *) new hid_t;
        *((hid_t*) fileHandles[MASTER_FILE_INDEX]) = 
            H5Fopen(filenames[MASTER_FILE_INDEX], H5F_ACC_RDONLY, H5P_DEFAULT);
    }
#endif
    RegisterFile(MASTER_FILE_INDEX);

    //
    // Read the name of the code that generated this vista file 
    //
    if (formatType == FTYPE_UNKNOWN)
    {
        writerName = 0;
        ReadDataset(0, "dbtype", 0, 0, (void**) &writerName);
        if (writerName == 0)
        {
            const char *msg = "Unable to determine name of code that generated "
                              "this Vista file. Therefore, VisIt cannot decide "
                              "which Vista file format to use.";
            if (!avtCallback::IssueWarning(msg))
                cerr << msg << endl;
        }
        else
        {
            //
            // Determine format type
            //
            if (strcmp(writerName, "ALE3D") == 0)
                formatType = FTYPE_ALE3D;
            else if (strcmp(writerName, "DIABLO") == 0)
                formatType = FTYPE_DIABLO;
            else
                formatType = FTYPE_UNKNOWN;
        }
    }

    //
    // Read and construct the vista tree
    //
    size_t size = 0;
    char *buf = 0;
    ReadDataset(0, "VisIt", 0, &size, (void**) &buf);
    vTree = new VistaTree(buf, size);
    if (getenv("VISIT_VISTA_DEBUG"))
        vTree->DumpTree();
    delete [] buf;

    //
    // Read the domain to file "part" map
    //
    chunkToFileMap = 0;
    size = 0;
    ReadDataset(0, "domtofilepart", 0, &size, (void**) &chunkToFileMap);
    numChunks = size;

    avtVistaFileFormat::objcnt++;
}

// ****************************************************************************
//  Destructor:  avtVistaFileFormat::~avtVistaFileFormat
//
//  Programmer:  Mark C. Miller 
//  Creation:   Tue Feb 17 19:19:07 PST 2004
//
// ****************************************************************************
avtVistaFileFormat::~avtVistaFileFormat()
{
    if (wasMorphed)
        return;

    FreeUpResources();

    if (writerName)
        delete [] writerName;

    if (fileHandles)
        delete [] fileHandles;

    if (chunkToFileMap)
        delete [] chunkToFileMap;

    avtVistaFileFormat::objcnt--;

    // handle HDF5 library termination on descrution of last instance
#ifdef HAVE_LIBHDF5
    if (!isSilo && (avtVistaFileFormat::objcnt == 0))
        FinalizeHDF5();
#endif

}

// ****************************************************************************
//  Method:  avtVistaFileFormat constructor
//
//  Purpose: Shallow copy various dynamically allocated objects. After calling
//  this method, the derived object that calls it owns all the dynamically
//  allocated stuff. The this object that this method was called on can be
//  destructed without it also destroying the dynamically allocated stuff it
//  once owned.
//
//  The 'this' object is expected to be one of the derived classes of 
//  avtVistaFileFormat.  This method is used in part of the process of
//  constructing a derived vista file format object. The first such derived
//  object constructed is basically morphed from a avtVistaFileFormat object. 
//
//  Programmer:  Mark C. Miller 
//  Creation:    July 15, 2004 
//
// ****************************************************************************
avtVistaFileFormat::avtVistaFileFormat(const char *filename,
    avtVistaFileFormat *fromObj) : avtSTMDFileFormat(&filename, 1)
{
    if (fromObj->wasMorphed)
    {
        EXCEPTION1(ImproperUseException, "file format object is already morphed");
    }

    wasMorphed     = false;

    writerName     = fromObj->writerName;
    formatType     = fromObj->formatType;
    masterFileName = fromObj->masterFileName;
    masterDirName  = fromObj->masterDirName;
    numChunks      = fromObj->numChunks;
    chunkToFileMap = fromObj->chunkToFileMap;
    vTree          = fromObj->vTree;
    fileHandles    = fromObj->fileHandles;
    isSilo         = fromObj->isSilo;

    RegisterFile(MASTER_FILE_INDEX);

    fromObj->wasMorphed = true;

    avtVistaFileFormat::objcnt++;
}


// ****************************************************************************
//  Method: avtVistaFileFormat::OpenFile
//
//  Purpose:
//      Opens a Vista file.
//
//  Arguments:
//      f       The index of the file in our array.
//
//  Returns:    A handle to the open file.
//
//  Programmer: Mark C. Miller
//  Creation:   February 17, 2004
//
// ****************************************************************************

void *
avtVistaFileFormat::OpenFile(int f)
{
    //
    // Make sure this is in range.
    //
    if (f < 0 || f >= nFiles)
    {
        EXCEPTION2(BadIndexException, f, nFiles);
    }

    //
    // Check to see if the file is already open.
    //
    if (fileHandles[f] != 0)
    {
        UsedFile(f);
        return fileHandles[f];
    }

    //
    // Open the Vista file.
    //
    if (isSilo)
    {
        fileHandles[f] = (void *) DBOpen(filenames[f], DB_UNKNOWN, DB_READ);
    }
#ifdef HAVE_LIBHDF5
    else
    {
        fileHandles[f] = (void *) new hid_t;
        *((hid_t*) fileHandles[f]) = H5Fopen(filenames[f],
                                         H5F_ACC_RDONLY, H5P_DEFAULT);
    }
#endif

    //
    // Check to see if we got a valid handle.
    //
    if (fileHandles[f] == 0)
    {
        EXCEPTION1(InvalidFilesException, filenames[f]);
    }

    RegisterFile(f);

    return fileHandles[f];
}


// ****************************************************************************
//  Method: avtVistaFileFormat::OpenFile
//
//  Purpose:
//      Opens a Vista file by name; adds the file name to a list
//      if we haven't heard of it before.
//
//  Programmer: Mark C. Miller 
//  Creation:   February 17, 2004
//
// *****************************************************************************

void *
avtVistaFileFormat::OpenFile(const char *fileName)
{
    //
    // The directory of this file is all relative to the directory of the
    // table of contents.  Reflect that here.
    //
    char name[1024];
    char *tocFile = filenames[0];
    char *thisSlash = tocFile, *lastSlash = tocFile;
    while (thisSlash != NULL)
    {
        lastSlash = thisSlash;
        thisSlash = strstr(lastSlash+1, VISIT_SLASH_STRING);
    }
    if (lastSlash == tocFile)
    {
        strcpy(name, fileName);
    }
    else
    {
        int amount = lastSlash-tocFile+1;
        strncpy(name, tocFile, amount);
        strcpy(name+amount, fileName);
    }

    int fileIndex = -1;
    for (int i = 0 ; i < nFiles ; i++)
    {
        if (strcmp(filenames[i], name) == 0)
        {
            fileIndex = i;
            break;
        }
    }

    if (fileIndex == -1)
    {
        //
        // We have asked for a previously unseen file.  Add it to the list and
        // continue.  AddFile will automatically take care of overflow issues.
        //
        fileIndex = AddFile(name);
    }

    return OpenFile(fileIndex);
}

// ****************************************************************************
//  Method: avtVistaFileFormat::CloseFile
//
//  Purpose:
//      Closes the Vista file.
//
//  Programmer: Mark C. Miller 
//  Creation:   February 17, 2004
//
// ****************************************************************************

void
avtVistaFileFormat::CloseFile(int f)
{
    if (fileHandles[f] != 0)
    {
        debug4 << "Closing Vista file " << filenames[f] << endl;
        if (isSilo)
        {
            DBClose((DBfile*) fileHandles[f]);
        }
#ifdef HAVE_LIBHDF5
        else
        {
            H5Fclose(*((hid_t*) fileHandles[f]));
            delete (hid_t *) fileHandles[f];
        }
#endif
        UnregisterFile(f);
        fileHandles[f] = 0;
    }
}

// ****************************************************************************
//  Method: avtVistaFileFormat::ReadDataset
//
//  Purpose:
//      General ReadDataset that returns any type
//
//  Programmer: Mark C. Miller 
//  Creation:   October 25, 2004
//
// ****************************************************************************
bool
avtVistaFileFormat::ReadDataset(const char *fileName, const char *dsName,
    VistaDataType *dataType, size_t *size, void **buf)
{
    return ReadDataset(fileName, dsName, dataType, size, buf, false);
}

// ****************************************************************************
//  Method: avtVistaFileFormat::ReadDataset
//
//  Purpose:
//      Convenient ReadDataset that always returns floats
//
//  Programmer: Mark C. Miller 
//  Creation:   October 25, 2004
//
// ****************************************************************************
bool
avtVistaFileFormat::ReadDataset(const char *fileName, const char *dsName,
    size_t *size, float **buf)
{
    return ReadDataset(fileName, dsName, 0, size, (void **) buf, true);
}

// ****************************************************************************
//  Method: avtVistaFileFormat::ReadDataset
//
//  Purpose:
//      Reads a named datasets from the file.
//
//  Programmer: Mark C. Miller 
//  Creation:   February 17, 2004
//
//  Modifications:
//
//    Mark C. Miller, Tue Oct 26 10:28:36 PDT 2004
//    Made it private. Added bool to control conversion to float
//
// ****************************************************************************
bool
avtVistaFileFormat::ReadDataset(const char *fileName, const char *dsName,
    VistaDataType *dataType, size_t *size, void **buf, bool convertToFloat)
{
    VistaDataType retType = DTYPE_UNKNOWN; //TODO: check on fix for uninitialized vars
    size_t retSize;
    bool retVal = false;
    void *f;
    
    if (fileName == 0)
        f = OpenFile(MASTER_FILE_INDEX);
    else
        f = OpenFile(fileName);

    if (isSilo)
    {
        DBfile *dbfile = (DBfile*) f;

        if (DBInqVarExists(dbfile, (char *) dsName) == 0)
        {
            retType = DTYPE_UNKNOWN;
            retSize = 0;
        }
        else
        {
            // examine the variable size
            retSize = DBGetVarLength(dbfile, (char *) dsName);

            // examine the variable type
            int type = DBGetVarType(dbfile, (char *) dsName);
            switch (type)
            {
                case DB_CHAR:   retType = DTYPE_CHAR;    break;
                case DB_INT:    retType = DTYPE_INT;     break;
                case DB_FLOAT:  retType = DTYPE_FLOAT;   break;
                case DB_DOUBLE: retType = DTYPE_DOUBLE;  break;
                default:        retType = DTYPE_UNKNOWN; break;
            }

            // override returned type if we're converting to float
            if (convertToFloat)
                retType = DTYPE_FLOAT;

            // read the variable
            if (buf != 0)
            {
                if (*buf == 0)
                {
                    if (convertToFloat)
                        *buf = new float [retSize];
                    else
                        *buf = new char [DBGetVarByteLength(dbfile, (char *) dsName)];

                }
                else
                {
                    if (*size < retSize)
                    {
                        EXCEPTION2(UnexpectedValueException, (int) *size, (int) retSize);
                    }
                }

                // since we're in ForceSingle mode, even doubles get read as floats
                if (convertToFloat == false || type == DB_FLOAT)
                {
                    DBReadVar(dbfile, (char *) dsName, *buf);
                }
                else
                {
                    char *tmpBuf = new char [DBGetVarByteLength(dbfile, (char *) dsName)];
                    DBReadVar(dbfile, (char *) dsName, tmpBuf);
                    
                    float *fptr = (float *) (*buf);
                    switch (type)
                    {
                        case DB_CHAR:
                        {
                            char *cptr = (char *) tmpBuf;
                            for (size_t i = 0; i < retSize; i++)
                                *fptr++ = (float) (*cptr++);
                            break;
                        }
                        case DB_SHORT:
                        {
                            short *sptr = (short *) tmpBuf;
                            for (size_t i = 0; i < retSize; i++)
                                *fptr++ = (float) (*sptr++);
                            break;
                        }
                        case DB_INT:
                        {
                            int *iptr = (int *) tmpBuf;
                            for (size_t i = 0; i < retSize; i++)
                                *fptr++ = (float) (*iptr++);
                            break;
                        }
                        case DB_LONG:
                        {
                            long *lptr = (long *) tmpBuf;
                            for (size_t i = 0; i < retSize; i++)
                                *fptr++ = (float) (*lptr++);
                            break;
                        }
                        case DB_DOUBLE:
                        {
                            double *dptr = (double *) tmpBuf;
                            for (size_t i = 0; i < retSize; i++)
                                *fptr++ = (float) (*dptr++);
                            break;
                        }
                    }

                    delete [] tmpBuf;

                }
            }

            retVal = true;
        }

    }
#ifdef HAVE_LIBHDF5
    else
    {
        // open the dataset
        hid_t hdfFile= *((hid_t *) f);
        hid_t ds = H5Dopen(hdfFile, dsName);

        if (ds < 0)
        {
            retType = DTYPE_UNKNOWN;
            retSize = 0;
        }
        else
        {
            // examine the dataspace
            hid_t space = H5Dget_space(ds);
            int hndims = H5Sget_simple_extent_ndims(space);
            hsize_t *hdims = new hsize_t[hndims];
            hsize_t *max_hdims = new hsize_t[hndims];
            H5Sget_simple_extent_dims(space, hdims, max_hdims);
            retSize = 1;
            int j;
            for (j = 0; j < hndims; j++)
                retSize *= hdims[j];
            delete [] hdims;
            delete [] max_hdims;

            // examine the datatype
            hid_t type = H5Dget_type(ds);
            hid_t memType = H5T_NATIVE_FLOAT; //TODO: check on fix for uninitialized var
            H5T_class_t typeClass = H5Tget_class(type);
            size_t typeSize = H5Tget_size(type);
            if (typeClass == H5T_FLOAT)
            {
                if (typeSize == sizeof(float))
                {
                    retType = DTYPE_FLOAT;
                    memType = H5T_NATIVE_FLOAT;
                }
                else if (typeSize == sizeof(double))
                {
                    retType = DTYPE_DOUBLE;
                    memType = H5T_NATIVE_DOUBLE;
                }
                else
                {
                    retType = DTYPE_UNKNOWN;
                    memType = type;
                }
            }
            else if (typeClass == H5T_INTEGER)
            {
                if (typeSize == sizeof(char))
                {
                    retType = DTYPE_CHAR;
                    memType = H5T_NATIVE_CHAR;
                }
                else if (typeSize == sizeof(int))
                {
                    retType = DTYPE_INT;
                    memType = H5T_NATIVE_INT;
                }
                else
                {
                    retType = DTYPE_UNKNOWN;
                    memType = type;
                }
            }

            // override types if we're supposed to convert to float
            if (convertToFloat)
            {
                memType = H5T_NATIVE_FLOAT;
                retType = DTYPE_FLOAT;
            }

            // do the read
            if (buf != 0)
            {
                if (*buf == 0)
                    *buf = new char [retSize * H5Tget_size(memType)];
                else
                {
                    if (*size < retSize)
                    {
                        EXCEPTION2(UnexpectedValueException, (int) *size, (int) retSize);
                    }
                }
                H5Dread(ds, memType, H5S_ALL, H5S_ALL, H5P_DEFAULT, *buf);
            }

            H5Sclose(space);
            H5Tclose(type);
            H5Dclose(ds);
            retVal = true;
        }
    }
#endif

    // set up the return values
    if (dataType != 0)
        *dataType = retType;
    if (size != 0)
        *size = retSize;
    return retVal;
}


// ****************************************************************************
//  Method: avtVistaFileFormat::FreeUpResources
//
//  Purpose:
//      When VisIt is done focusing on a particular timestep, it asks that
//      timestep to free up any resources (memory, file descriptors) that
//      it has associated with it.  This method is the mechanism for doing
//      that.
//
//  Programmer: Mark C. Miller
//  Creation:   Tue Feb 17 19:19:07 PST 2004
//
// ****************************************************************************

void
avtVistaFileFormat::FreeUpResources(void)
{
    int i;

    for (i = 0 ; i < nFiles; i++)
    {
        CloseFile(i);
    }

    if (vTree != 0)
    {
        delete vTree;
        vTree = 0;
    }

//#warning should we delete other stuff during FreeUpResources

    // we don't finalize HDF5 here because its not clear if/when we might
    // need to re-initialize it, again
}
