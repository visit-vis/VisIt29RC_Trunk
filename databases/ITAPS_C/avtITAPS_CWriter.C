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

// ************************************************************************* //
//                            avtITAPS_CWriter.C                             //
// ************************************************************************* //

#include <avtITAPS_CWriter.h>
#include <avtITAPS_CUtility.h>

#include <avtDatabaseMetaData.h>
#include <avtMaterial.h>
#include <avtMetaData.h>
#include <avtOriginatingSource.h>
#include <DBOptionsAttributes.h>
#include <DebugStream.h>
#include <ImproperUseException.h>

#include <iMesh.h>

#include <vtkCell.h>
#include <vtkCellData.h>
#include <vtkCellType.h>
#include <vtkDataArray.h>
#include <vtkDataSet.h>
#include <vtkIntArray.h>
#include <vtkPointData.h>
#include <vtkType.h>

#include <stdlib.h>
#include <vtkDataSetTriangleFilter.h>

#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;
using namespace avtITAPS_CUtility;

// ****************************************************************************
//  Method: avtITAPS_CWriter constructor
//
//  Programmer: Mark C. Miller 
//  Creation:   November 20, 2008 
//
//  Modifications:
//    Mark C. Miller, Wed Jan 14 17:57:46 PST 2009
//    Added some bools control output behavior
//
//    Mark C. Miller, Tue Apr 21 16:05:30 PDT 2009
//    Added addFacesFor3DEnts=true for GRUMMP.
//
//    Mark C. Miller, Thu Sep 23 20:20:43 PDT 2010
//    Save option 'MOAB' no longer recognized.
// ****************************************************************************

avtITAPS_CWriter::avtITAPS_CWriter(DBOptionsAttributes *dbopts)
{
    simplexify = false;
    addFacesFor3DEnts = false;
    preventDupsToiMesh = false;

#if defined(ITAPS_GRUMMP)
    simplexify = true;
    addFacesFor3DEnts = true;
#endif

#if defined(ITAPS_MOAB)
    saveOptions = "";
    formatExtension = "h5m";
#elif defined(ITAPS_GRUMMP)
    formatExtension = ""; // grummp seems to append '.vmesh' always
#elif defined(ITAPS_FMDB)
    formatExtension = "sms";
#else
    formatExtension = "unk";
#endif

    for (int i = 0; dbopts != 0 && i < dbopts->GetNumberOfOptions(); ++i)
    {
        if (dbopts->GetName(i) == "Format")
        {
            switch (dbopts->GetEnum("Format"))
            {
                case 0: break; // default case 
                case 1: saveOptions = "EXODUS"; formatExtension = "exoII"; break;
                case 2: saveOptions = "VTK";    formatExtension = "vtk"; break;
                case 3: saveOptions = "SLAC";   formatExtension = "slac"; break;
                case 4: saveOptions = "GMV";    formatExtension = "gmv"; break;
                case 5: saveOptions = "ANSYS";  formatExtension = "ans"; break;
                case 6: saveOptions = "GMSH";   formatExtension = "gmsh"; break;
                case 7: saveOptions = "STL";    formatExtension = "stl"; break;
            }
        }
        else if (dbopts->GetName(i) == "Simplexify")
        {
            simplexify = dbopts->GetBool("Simplexify");
        }
        else if (dbopts->GetName(i) == "Add faces for 3Dents")
        {
            addFacesFor3DEnts = dbopts->GetBool("Add faces for 3Dents");
        }
        else if (dbopts->GetName(i) == "Prevent duplicates to iMesh")
        {
            preventDupsToiMesh = dbopts->GetBool("Prevent duplicates to iMesh");
        }
    }
}

// ****************************************************************************
//  Method: avtITAPS_CWriter destructor
//
//  Programmer: Mark C. Miller 
//  Creation:   November 20, 2008 
//
//  Modifications:
//    Mark C. Miller, Tue Jan  6 18:50:17 PST 2009
//    Added call to clear static variable holding message counts.
//
// ****************************************************************************

avtITAPS_CWriter::~avtITAPS_CWriter()
{
    messageCounts.clear();
}


// ****************************************************************************
//  Method: avtITAPS_CWriter::OpenFile
//
//  Purpose:
//
//  Programmer: November 20, 2008 
//  Creation:   September 11, 2003
//
// ****************************************************************************

void
avtITAPS_CWriter::OpenFile(const string &stemname, int nb)
{
    stem = stemname;
    nblocks = nb;
    dir ="";
    // find dir if provided
    int idx = stem.rfind("/");
    if ( idx != string::npos )
    {
        int stem_len = stem.size() - (idx+1) ;
        dir  = stem.substr(0,idx+1);
        stem = stem.substr(idx+1,stem_len);
    }
}

// ****************************************************************************
//  Method: avtITAPS_CWriter::WriteHeaders
//
//  Purpose:
//      This will write out the multi-vars for the ITAPS_C constructs.
//
//  Programmer: Mark C. Miller 
//  Creation:   November 20, 2008 
//
//  Modifications:
//    Mark C. Miller, Tue Apr 21 16:06:03 PDT 2009
//    Added code to store spatial/topological dimensions of mesh.
// ****************************************************************************

void
avtITAPS_CWriter::WriteHeaders(const avtDatabaseMetaData *md,
                            vector<string> &scalars, vector<string> &vectors,
                            vector<string> &materials)
{
    const avtMeshMetaData *mmd = md->GetMesh(0);
    spatialDim = mmd->spatialDimension;
    topoDim = mmd->topologicalDimension;
}

// ****************************************************************************
//  Function: compare_ehs
//
//  Purpose: comparison method for entity handles for use in qsort().
//  This assumes iBase_EntityHandle data type supports <, > == operators.
//
//  Programmer: Mark C. Miller, Tue Jan  6 18:51:18 PST 2009
//
//  Modifications:
//    Mark C. Miller, Wed Jan 14 17:58:11 PST 2009
//    Removed conditional compilation bracketing this func.
// ****************************************************************************
static int compare_ehs(const void *a, const void *b)
{
    iBase_EntityHandle *eha = (iBase_EntityHandle *) a;
    iBase_EntityHandle *ehb = (iBase_EntityHandle *) b;
    if (eha < ehb)
        return -1;
    else if (eha > ehb)
        return 1;
    else
        return 0;
}

// ****************************************************************************
//  Function: WriteMesh 
//
//  Purpose: Builds up the mesh description in iMesh. Note, I/O doesn't occur
//  until iMesh_save is called.
//
//  Programmer: Mark C. Miller 
//  Creation:   December 1, 2008 
//
//  Modifications:
//    Mark C. Miller, Tue Jan  6 18:51:58 PST 2009
//    Added tet conversion for GRUMMP. Removed conditional compilation for
//    getDescription calls since those work in most recent GRUMMP.
//
//    Mark C. Miller, Wed Jan 14 17:58:33 PST 2009
//    Added bool arguments to control behavior. Removed conditional
//    compilation of simplexification block and made it run-time conditional.
//    Added code to properly traverse faces of a cell instead of assuming
//    all cells are tets. Expanded the duplicate faces map to include a 4th
//    node id.
//
//    Mark C. Miller, Tue Apr 21 16:06:48 PDT 2009
//    Updated to newest iMesh/iBase specification.
//
//    Mark C. Miller, Thu May 14 21:30:51 PDT 2009
//    Added chunkSet and nblocks arg. Added logic to ONLY create chunk set if
//    there are indeed multiple blocks.
//
//    Jeremy Meredith, Wed Mar 27 15:34:24 EDT 2013
//    Catch iBase_ErrorType, not just iBase_Error.
//    Also, iMesh_getDescription no longer needs an error output argument.
//
// ****************************************************************************
static void
WriteMesh(vtkDataSet *_ds, int nblocks, int chunk,
    iMesh_Instance itapsMesh, iBase_EntitySetHandle rootSet,
    iBase_EntitySetHandle *chunkSet,
    iBase_EntityHandle **pntHdls, iBase_EntityHandle **cellHdls,
    bool simplexify, bool addFacesFor3DEnts, bool preventDupsToiMesh)
{
    int i,j;
    vtkDataSet *ds = _ds;

    *pntHdls = 0;
    *cellHdls = 0;

    if (simplexify)
    {
        debug5 << "Converting mesh to simplexes..." << endl;
        debug5 << "    " << _ds->GetNumberOfPoints() << " points, " << _ds->GetNumberOfCells() << " cells ==> ";
        vtkDataSetTriangleFilter *tf = vtkDataSetTriangleFilter::New();
        tf->SetInputData(_ds);
        tf->Update();
        ds = (vtkDataSet*) tf->GetOutput();
        ds->Register(NULL);
        debug5 << ds->GetNumberOfPoints() << " points, " << ds->GetNumberOfCells() << " cells." << endl;
    }

    try
    {
        // Add the nodes of this mesh as vertices of the iMesh instance.
        // Note that vertices can only ever be created or live in the instance
        // itself and not in entity sets of the instance. Though, I think we
        // can create 'links' to vertices from a given entity set using 'add'
        // method(s). We should really use the 'Arr' versions of this method
        // but initial coding is simplest to use individual entity methods.
        //
        // In fact, it would appear as though everything is, by fiat, created
        // in the iMesh instance itself (root set), and then can be 'moved' by
        // adding to another set and removing from root?
        int npts = ds->GetNumberOfPoints(); 
        iBase_EntityHandle *ptHdls = new iBase_EntityHandle[npts];
        for (i = 0; i < npts; i++)
        {
            double pt[3];
            ds->GetPoint(i, pt);
    
            // create initial Vtx entity
            iMesh_createVtx(itapsMesh, pt[0], pt[1], pt[2], &ptHdls[i], &itapsError);
            if (i<5) CheckITAPSError(itapsMesh, iMesh_createVtx, NoL);
        }
        *pntHdls = ptHdls;
    
        //
        // Create the entity set representing this chunk. GRUMMP will not 
        // permit this call PRIOR to creating ANY entities. So, it had to be
        // moved here from where it was originally at the beginning of the 
        // try... statement.
        //
        *chunkSet = rootSet;
        if (nblocks > 1)
        {
            iMesh_createEntSet(itapsMesh, 0, chunkSet, &itapsError);
            CheckITAPSError(itapsMesh, iMesh_createEntSet, NoL);
    
            // add just created Vtx entites to chunkSet
            iMesh_addEntArrToSet(itapsMesh, ptHdls, npts, *chunkSet, &itapsError);
            CheckITAPSError(itapsMesh, iMesh_addEntArrToSet, NoL);
    
            // remove just created Vtx entities from rootSet, ok?
            //iMesh_rmvEntArrFromSet(itapsMesh, ptHdls, npts, &rootSet, &itapsError);
            //CheckITAPSError(itapsMesh, iMesh_rmvEntArrFromSet, NoL);
        }
    
        int ncells = ds->GetNumberOfCells();
        iBase_EntityHandle *znHdls = new iBase_EntityHandle[ncells];
        map<iBase_EntityHandle, map<iBase_EntityHandle, map<iBase_EntityHandle,
            map<iBase_EntityHandle, iBase_EntityHandle> > > > dupFacesMap;
        for (i = 0; i < ncells; i++)
        {
            vtkCell *theCell = ds->GetCell(i);
    
            int j, status;
            int cellTopo = VTKZoneTypeToITAPSEntityTopology(theCell->GetCellType());

            // We don't need to embue points as entities
            if (cellTopo == iMesh_POINT)
                continue; 

            int ncellPts = theCell->GetNumberOfPoints();
            iBase_EntityHandle *cellPtEnts = new iBase_EntityHandle[ncellPts];
            for (j = 0; j < ncellPts; j++)
                cellPtEnts[j] = ptHdls[theCell->GetPointId(j)];

            iBase_EntityHandle *lo_ents = cellPtEnts;
            int lo_ent_cnt = ncellPts;

            if (addFacesFor3DEnts && (cellTopo == iMesh_TETRAHEDRON ||
                                      cellTopo == iMesh_PYRAMID ||
                                      cellTopo == iMesh_PRISM ||
                                      cellTopo == iMesh_HEXAHEDRON))
            {
                //
                // A few problems here. We may not be handling 2D case correctly.
                // will iMesh_createEnt prevent from creating duplicate entities?
                // For GRUMMP, we can create 3D ents only in terms of 2D faces.
                // However, MOAB permits creation of 3D ents from 0D verts. Finally,
                // in 3D, GRUMMP understans ONLY tets. In 2D, GRUMMP understands
                // tris and quads.
                //
                iBase_EntityHandle faceEnts[6]; // For elem. zoo, max of 6 faces.
                lo_ents = faceEnts;
                lo_ent_cnt = theCell->GetNumberOfFaces();
                for (j = 0; j < lo_ent_cnt; j++)
                {
                    vtkCell *faceCell = theCell->GetFace(j);
                    int faceTopo = VTKZoneTypeToITAPSEntityTopology(faceCell->GetCellType());
                    int k;

                    // These loops populate facePtEnts with 3 or 4 entity handles
                    // for a triangle or quad face, respectively. In the case of
                    // a triangle, we summarily set the last entry to the same as
                    // the first to ensure sorting and map lookup below works. We
                    // keep a copy of faceEntPointsOrig for the call to iMesh but
                    // use facePointEnts (which gets sorted) for duplicate lookups.
                    int nFacePts = faceCell->GetNumberOfPoints();
                    iBase_EntityHandle facePtEnts[4];
                    const int nFacePtsMax = sizeof(facePtEnts)/sizeof(facePtEnts[0]);
                    for (k = 0; k < nFacePts; k++)
                        facePtEnts[k] = ptHdls[faceCell->GetPointId(k)];
                    for (k = nFacePts; k < nFacePtsMax; k++)
                        facePtEnts[k] = ptHdls[faceCell->GetPointId(0)];
                    iBase_EntityHandle facePtEntsOrig[4];
                    memcpy(facePtEntsOrig, facePtEnts, sizeof(facePtEntsOrig));

                    bool newFace = true;
                    if (preventDupsToiMesh)
                    {
                        qsort(facePtEnts, nFacePtsMax, sizeof(facePtEnts[0]), compare_ehs);
                        map<iBase_EntityHandle, map<iBase_EntityHandle, map<iBase_EntityHandle,
                            map<iBase_EntityHandle, iBase_EntityHandle> > > >::iterator f0it =
                            dupFacesMap.find(facePtEnts[0]);
                        if (f0it != dupFacesMap.end())
                        {
                            map<iBase_EntityHandle, map<iBase_EntityHandle,
                                map<iBase_EntityHandle, iBase_EntityHandle> > >::iterator f1it =
                                f0it->second.find(facePtEnts[1]);
                            if (f1it != f0it->second.end())
                            {
                                map<iBase_EntityHandle,
                                    map<iBase_EntityHandle, iBase_EntityHandle> >::iterator f2it =
                                    f1it->second.find(facePtEnts[2]);
                                if (f2it != f1it->second.end())
                                {
                                    map<iBase_EntityHandle, iBase_EntityHandle>::iterator f3it =
                                        f2it->second.find(facePtEnts[3]);
                                    if (f3it != f2it->second.end())
                                    {
                                        newFace = false;
                                        faceEnts[j] = f3it->second;
                                        f2it->second.erase(f3it);
                                        if (f2it->second.size() == 0)
                                        {
                                            f1it->second.erase(f2it);
                                            if (f1it->second.size() == 0)
                                            {
                                                f0it->second.erase(f1it);
                                                if (f0it->second.size() == 0)
                                                    dupFacesMap.erase(f0it);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    } // if (preventDupsToiMesh)

                    if (newFace)
                    { 
                        iMesh_createEnt(itapsMesh, faceTopo, facePtEntsOrig, nFacePts, &faceEnts[j], &status, &itapsError);
                        if (i<5) CheckITAPSError(itapsMesh, iMesh_createEnt, NoL);
                        if (preventDupsToiMesh)
                            dupFacesMap[facePtEnts[0]][facePtEnts[1]][facePtEnts[2]][facePtEnts[3]] = faceEnts[j];
                    }
                }
            }

            iMesh_createEnt(itapsMesh, cellTopo, lo_ents, lo_ent_cnt, &znHdls[i], &status, &itapsError);
            if (i<5) CheckITAPSError(itapsMesh, iMesh_createEnt, NoL);

        }
        *cellHdls = znHdls;
    
        if (nblocks > 1)
        {
            // add just created cell entites to chunkSet
            iMesh_addEntArrToSet(itapsMesh, znHdls, ncells, *chunkSet, &itapsError);
            CheckITAPSError(itapsMesh, iMesh_addEntArrToSet, NoL);
    
            // remove just created cell entities from rootSet, ok?
            //iMesh_rmvEntArrFromSet(itapsMesh, znHdls, ncells, &rootSet, &itapsError);
            //CheckITAPSError(itapsMesh, iMesh_rmvEntArrFromSet, NoL);
        }

    }
    catch (iBase_ErrorType TErr)
    {
        char msg[512];
        char desc[256];
        desc[0] = '\0';
        int tmpError = itapsError;
        iMesh_getDescription(itapsMesh, desc, sizeof(desc));
        SNPRINTF(msg, sizeof(msg), "Encountered ITAPS error (%d) \"%s\""
            "\nUnable to open file!", tmpError, desc);
        if (!avtCallback::IssueWarning(msg))
            cerr << msg << endl;
    }

    // Since we converted to tets, creating a new dataset, we need to delete it.
    if (simplexify)
        ds->Delete();

}

// ****************************************************************************
//  Method: WriteMaterial
//
//  Purpose: Addes material info to the iMesh instance a la MOAB convention. 
//
//  Programmer: Mark C. Miller 
//  Creation:   Thu May 14 21:32:14 PDT 2009
//
//  Modifications:
//    Mark C. Miller, Wed May 20 09:55:31 PDT 2009
//    Made it a method. Added logic to deal with case where materials have
//    to be queried from originatin source.
//
//    Mark C. Miller, Wed May 20 10:48:39 PDT 2009
//    Fixed class name (was avtITAPS_MOABWriter).
// ****************************************************************************

void 
avtITAPS_CWriter::WriteMaterial(vtkCellData *cd, int chunk, iMesh_Instance itapsMesh,
    iBase_EntitySetHandle rootSet, iBase_EntitySetHandle chunkSet,
    iBase_EntityHandle *clHdls)
{

    if (!hasMaterialsInProblem)
        return;

    const int *ia = 0;
    int nzones = 0;

    if (mustGetMaterialsAdditionally)
    {
        avtMaterial *mat = GetInput()->GetOriginatingSource()
                                           ->GetMetaData()->GetMaterial(chunk);
        if (mat == NULL)
        {
            debug1 << "Not able to get requested material" << endl;
            return;
        }

        if (mat->GetMixlen() > 0)
        {
            debug1 << "Cannot handle mixed materials" << endl;
            return;
        }

        nzones = mat->GetNZones();
        ia = mat->GetMatlist();
    }
    else
    {
        vtkDataArray *arr = cd->GetArray("avtSubsets");
        if (arr == NULL)
        {
            debug1 << "Subsets array not available" << endl;
            return;
        }
        if (arr->GetDataType() != VTK_INT)
        {
            debug1 << "Subsets array not of right type" << endl;
            return;
        }

        vtkIntArray *iarr = vtkIntArray::SafeDownCast(arr);
        ia = (int *) iarr->GetVoidPointer(0);
        nzones = iarr->GetNumberOfTuples();
    }

    if (nzones == 0 || ia == 0)
        return;

    // create the tag we'll need
    const char *matTagName = "MATERIAL_SET";
    iBase_TagHandle matTag;
    iMesh_createTag(itapsMesh, matTagName, 1, iBase_INTEGER, &matTag, &itapsError,
        strlen(matTagName)+1);

    // Iterate over zones examining material ids. Whenever we encounter a new
    // material id, create an ent set for it. Map all material ids into the
    // range 0...nmats-1
    int nmats = 0;
    vector<iBase_EntitySetHandle> matSets;
    map<int, int> matValToSetMap;
    for (int i = 0; i < nzones; i++)
    {
        iBase_EntitySetHandle matSet;

        if (matValToSetMap.find(ia[i]) == matValToSetMap.end())
        {
            matValToSetMap[ia[i]] = nmats;

            // Create the new entity set
            iBase_EntitySetHandle newSet;
            iMesh_createEntSet(itapsMesh, 0, &newSet, &itapsError);
            matSets.push_back(newSet);

            // Specify value for material tag
            iMesh_setEntSetIntData(itapsMesh, newSet, matTag, nmats, &itapsError);

            matSet = newSet;
            nmats++;
        }
        else
        {
            matSet = matSets[matValToSetMap[ia[i]]];
        }

        iMesh_addEntToSet(itapsMesh, clHdls[i], matSet, &itapsError);
    }
}

// ****************************************************************************
//  Function: vtkDataTypeToITAPSTagType 
//
//  Programmer: Mark C. Miller 
//  Creation:   December 1, 2008 
//
// ****************************************************************************
static int
vtkDataTypeToITAPSTagType(int t)
{
    switch (t)
    {
        case VTK_CHAR:           
        case VTK_UNSIGNED_CHAR:  return iBase_BYTES;

        case VTK_SHORT:
        case VTK_UNSIGNED_SHORT:
        case VTK_INT:
        case VTK_UNSIGNED_INT:
        case VTK_LONG:
        case VTK_UNSIGNED_LONG:
        case VTK_ID_TYPE:        return iBase_INTEGER;

        case VTK_FLOAT:          
        case VTK_DOUBLE:         return iBase_DOUBLE;
    }
    return iBase_BYTES;
}

// ****************************************************************************
//  Function: ConvertTypeAndStorageOrder
//
//  Programmer: Mark C. Miller 
//  Creation:   December 6, 2008 
//
// ****************************************************************************
template <class iT, class oT>
static void ConvertTypeAndStorageOrder(const iT *const ibuf, int npts, int ncomps, int sorder, oT *obuf)
{
    int i, j;
    if (sorder == iBase_INTERLEAVED)
    {
        for (i = 0; i < npts; i++)
        {
            for (j = 0; j < ncomps; j++)
            {
                obuf[i*ncomps+j] = (oT) ibuf[i*ncomps+j];
            }
        }
    }
    else
    {
        for (j = 0; j < ncomps; j++)
        {
            for (i = 0; i < npts; i++)
            {
                obuf[j*ncomps+i] = (oT) ibuf[i*ncomps+j];
            }
        }
    }
}

// ****************************************************************************
//  Function: ConvertTypeAndStorageOrder
//
//  Programmer: Mark C. Miller 
//  Creation:   December 5, 2008 
//
// ****************************************************************************
template <class T>
static void ConvertTypeAndStorageOrder(vtkDataArray *arr, int sorder, T **buf) 
{
    int npts = arr->GetNumberOfTuples();
    int ncomps = arr->GetNumberOfComponents();
    void *p = arr->GetVoidPointer(0);
    T *newbuf = new T[npts*ncomps];

    switch (arr->GetDataType())
    {
        case VTK_CHAR: ConvertTypeAndStorageOrder((char*) p, npts, ncomps, sorder, newbuf); break;
        case VTK_UNSIGNED_CHAR:ConvertTypeAndStorageOrder((unsigned char*) p, npts, ncomps, sorder, newbuf); break;
        case VTK_SHORT: ConvertTypeAndStorageOrder((short*) p, npts, ncomps, sorder, newbuf); break;
        case VTK_UNSIGNED_SHORT: ConvertTypeAndStorageOrder((unsigned short*) p, npts, ncomps, sorder, newbuf); break;
        case VTK_INT: ConvertTypeAndStorageOrder((int*) p, npts, ncomps, sorder, newbuf); break;
        case VTK_UNSIGNED_INT: ConvertTypeAndStorageOrder((unsigned int*) p, npts, ncomps, sorder, newbuf); break;
        case VTK_LONG: ConvertTypeAndStorageOrder((long*) p, npts, ncomps, sorder, newbuf); break;
        case VTK_UNSIGNED_LONG: ConvertTypeAndStorageOrder((unsigned long*) p, npts, ncomps, sorder, newbuf); break;
        case VTK_ID_TYPE: ConvertTypeAndStorageOrder((vtkIdType*) p, npts, ncomps, sorder, newbuf); break;
        case VTK_FLOAT:  ConvertTypeAndStorageOrder((float*) p, npts, ncomps, sorder, newbuf); break;
        case VTK_DOUBLE: ConvertTypeAndStorageOrder((double*) p, npts, ncomps, sorder, newbuf); break;
    }

    *buf = newbuf;
}

// ****************************************************************************
//  Function: ConvertTypeAndStorageOrder
//
//  Purpose: Iterate over all point and cell variables, skipping VisIt
//  'internal' variables and build up tags on the appropriate entities in
//  iMesh.
//
//  Programmer: Mark C. Miller 
//  Creation:   December 5, 2008 
//
//  Modifications:
//    Mark C. Miller, Tue Jan  6 18:53:09 PST 2009
//    Fixed off by one error in string length passed for array names. It was
//    failing to include the terminating null character. Removed conditional
//    compilation for getDescription as that now works in most recent GRUMMP.
//
//    Mark C. Miller, Tue Apr 21 16:06:48 PDT 2009
//    Updated to newest iMesh/iBase specification.
//
//    Jeremy Meredith, Wed Mar 27 15:34:24 EDT 2013
//    Catch iBase_ErrorType, not just iBase_Error.
//    Also, iMesh_getDescription no longer needs an error output argument.
//
// ****************************************************************************
static void
WriteVariables(vtkDataSet *ds, int chunk,
    iMesh_Instance itapsMesh, iBase_EntitySetHandle rootSet,
    iBase_EntityHandle *ptHdls, iBase_EntityHandle *clHdls)
{
    try
    {
        int sorder;
        iMesh_getDfltStorage(itapsMesh, &sorder, &itapsError);

        for (int pass = 0; pass < 2; pass++)
        {
            vtkPointData *pd = 0;
            vtkCellData *cd = 0;
    
            if (pass == 0)
               pd = ds->GetPointData();
            else
               cd = ds->GetCellData();
    
            iBase_EntityHandle *eHdls = pd ? ptHdls : clHdls;
            int narrays = pd ? pd->GetNumberOfArrays() : cd->GetNumberOfArrays();
            int nents = pd ? ds->GetNumberOfPoints() : ds->GetNumberOfCells();
    
            for (int i = 0 ; i < narrays ; i++)
            {
                vtkDataArray *arr = pd ? pd->GetArray(i) : cd->GetArray(i);
        
                // ignore internal VisIt data
                if (strstr(arr->GetName(), "vtk") != NULL)
                    continue;
                if (strstr(arr->GetName(), "avt") != NULL)
                    continue;
    
                int npts = arr->GetNumberOfTuples();
                int ncomps = arr->GetNumberOfComponents();
                int iType = vtkDataTypeToITAPSTagType(arr->GetDataType());
                iBase_TagHandle varTag;
                iMesh_createTag(itapsMesh, arr->GetName(), ncomps, iType, &varTag,
                    &itapsError, strlen(arr->GetName())+1);
                CheckITAPSError(itapsMesh, iMesh_createTag, NoL);

                switch (iType)
                {
                    case iBase_DOUBLE:
                    {
                        if (arr->GetDataType() == VTK_DOUBLE && sorder == iBase_INTERLEAVED)
                        {
                            // Fastrack because in this case iMesh can digest VTK-native
                            iMesh_setDblArrData(itapsMesh, eHdls, nents, varTag,
                                (double *) arr->GetVoidPointer(0), npts*ncomps, &itapsError);
                            CheckITAPSError(itapsMesh, iMesh_setDblArrData, NoL);
                        }
                        else
                        {
                            double *buf;
                            ConvertTypeAndStorageOrder(arr, sorder, &buf);
                            iMesh_setDblArrData(itapsMesh, eHdls, nents, varTag, buf,
                                npts*ncomps, &itapsError);
                            delete [] buf;
                            CheckITAPSError(itapsMesh, iMesh_setDblArrData, NoL);
                        }
                        break;
                    }
                    case iBase_INTEGER:
                    {
                        if (arr->GetDataType() == VTK_INT && sorder == iBase_INTERLEAVED)
                        {
                            // Fastrack because in this case iMesh can digest VTK-native
                            iMesh_setIntArrData(itapsMesh, eHdls, nents, varTag,
                                (int *) arr->GetVoidPointer(0), npts*ncomps, &itapsError);
                            CheckITAPSError(itapsMesh, iMesh_setIntArrData, NoL);
                        }
                        else
                        {
                            int *buf;
                            ConvertTypeAndStorageOrder(arr, sorder, &buf);
                            iMesh_setIntArrData(itapsMesh, eHdls, nents, varTag, buf,
                                npts*ncomps, &itapsError);
                            delete [] buf;
                            CheckITAPSError(itapsMesh, iMesh_setIntArrData, NoL);
                        }
                        break;
                    }
                    case iBase_BYTES:
                    {
                        if (arr->GetDataType() == VTK_CHAR && sorder == iBase_INTERLEAVED)
                        {
                            // Fastrack because in this case iMesh can digest VTK-native
                            iMesh_setArrData(itapsMesh, eHdls, nents, varTag,
                                (char *) arr->GetVoidPointer(0), npts*ncomps, &itapsError);
                            CheckITAPSError(itapsMesh, iMesh_setArrData, NoL);
                        }
                        else
                        {
                            char *buf;
                            ConvertTypeAndStorageOrder(arr, sorder, &buf);
                            iMesh_setArrData(itapsMesh, eHdls, nents, varTag, buf,
                                npts*ncomps, &itapsError);
                            delete [] buf;
                            CheckITAPSError(itapsMesh, iMesh_setArrData, NoL);
                        }
                        break;
                    }
                }
            }
        }
    }
    catch (iBase_ErrorType TErr)
    {
        char msg[512];
        char desc[256];
        desc[0] = '\0';
        int tmpError = itapsError;
        iMesh_getDescription(itapsMesh, desc, sizeof(desc));
        SNPRINTF(msg, sizeof(msg), "Encountered ITAPS error (%d) \"%s\""
            "\nUnable to open file!", tmpError, desc);
        if (!avtCallback::IssueWarning(msg))
            cerr << msg << endl;
    }
}


// ****************************************************************************
//  Method: avtITAPS_CWriter::WriteChunk
//
//  Purpose:
//      This writes out one chunk of an avtDataset.
//
//  Programmer: Mark C. Miller 
//  Creation:   November 20, 2008 
//
//  Modifications:
//    Mark C. Miller, Tue Jan  6 18:53:09 PST 2009
//    Fixed off by one error in string length passed for array names. It was
//    failing to include the terminating null character. Removed conditional
//    compilation for getDescription as that now works in most recent GRUMMP.
//
//    Mark C. Miller, Wed Jan 14 18:00:42 PST 2009
//    Added bools to call to WriteMesh.
//
//    Mark C. Miller, Tue Apr 21 16:07:33 PDT 2009
//    Added logic to pass "2D" option for GRUMMP.
//
//    Mark C. Miller, Thu May 14 21:30:21 PDT 2009
//    Added material output. Added nblocks/chunkSet args to WriteMesh.
//
//    Jeremy Meredith, Wed Mar 27 15:34:24 EDT 2013
//    Catch iBase_ErrorType, not just iBase_Error.
//    Also, iMesh_getDescription no longer needs an error output argument.
//
// ****************************************************************************

void
avtITAPS_CWriter::WriteChunk(vtkDataSet *ds, int chunk)
{
    int i;

    // What is rule for the null character in specifying length of string
    // What should impl. do with a string that is empty (e.g. "") as opposed
    // to a pointer to 0.
    char const *dummyStr = 0;
#if defined(ITAPS_GRUMMP)
    if (spatialDim == 2)
        dummyStr = "2D";
#endif
    iMesh_Instance itapsMesh;
    iMesh_newMesh(dummyStr, &itapsMesh, &itapsError, dummyStr ? strlen(dummyStr)+1 : 0);
    CheckITAPSError(itapsMesh, iMesh_newMesh, NoL);

    iBase_EntitySetHandle rootSet;
    iMesh_getRootSet(itapsMesh, &rootSet, &itapsError);
    CheckITAPSError(itapsMesh, iMesh_getRootSet, NoL);

    // Create mesh description in iMesh instance
    iBase_EntityHandle *ptHdls = 0, *clHdls = 0;
    iBase_EntitySetHandle chunkSet;
    WriteMesh(ds, nblocks, chunk, itapsMesh, rootSet, &chunkSet, &ptHdls, &clHdls,
        simplexify, addFacesFor3DEnts, preventDupsToiMesh);

    // Create material subsets
    WriteMaterial(ds->GetCellData(), chunk, itapsMesh, rootSet, chunkSet, clHdls);

    // Create variables (tags) in iMesh instance
    WriteVariables(ds, chunk, itapsMesh, rootSet, ptHdls, clHdls);

    if (ptHdls)
        delete [] ptHdls;
    if (clHdls)
        delete [] clHdls;

    // Ok, write the iMesh instance to a file
    try
    {

        // save the file
        string fname = dir + stem;
        char filename[1024];
        if (formatExtension != "")
            sprintf(filename, "%s.%d.%s", fname.c_str(), chunk, formatExtension.c_str());
        else
            sprintf(filename, "%s.%d", fname.c_str(), chunk);
        iMesh_save(itapsMesh, rootSet, filename, saveOptions.c_str(), &itapsError,
            strlen(filename)+1, saveOptions.size());
        CheckITAPSError(itapsMesh, iMesh_save, NoL);

    }
    catch (iBase_ErrorType TErr)
    {
        char msg[512];
        char desc[256];
        desc[0] = '\0';
        int tmpError = itapsError;
        iMesh_getDescription(itapsMesh, desc, sizeof(desc));
        SNPRINTF(msg, sizeof(msg), "Encountered ITAPS error (%d) \"%s\""
            "\nUnable to open file!", tmpError, desc);
        if (!avtCallback::IssueWarning(msg))
            cerr << msg << endl;
    }
}


// ****************************************************************************
//  Method: avtITAPS_CWriter::CloseFile
//
//  Purpose:
//      Closes the file.  This does nothing in this case.
//
//  Programmer: Mark C. Miller 
//  Creation:   November 20, 2008 
//
// ****************************************************************************

void
avtITAPS_CWriter::CloseFile(void)
{
}
