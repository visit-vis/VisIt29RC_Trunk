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
//                             avtCurveRenderer.C                            //
// ************************************************************************* //

#include "avtCurveRenderer.h"

#include <vtkGeometryFilter.h>
#include <vtkPolyData.h>

#include <avtCallback.h>
#include <DebugStream.h>

// ****************************************************************************
// Constructor:  avtCurveRenderer::avtCurveRenderer
//
// Programmer:  Brad Whitlock
// Creation:    Mon Nov 20 10:44:19 PDT 2006
//
// Modifications:
//
// ****************************************************************************

avtCurveRenderer::avtCurveRenderer() : avtCustomRenderer()
{
    input = 0;
}

// ****************************************************************************
// Destructor:  avtCurveRenderer::~avtCurveRenderer
//
// Programmer:  Brad Whitlock
// Creation:    Mon Nov 20 10:44:19 PDT 2006
//
// Modifications:
//
// ****************************************************************************

avtCurveRenderer::~avtCurveRenderer()
{
    for (std::map<vtkDataSet*,vtkPolyData*>::iterator it = inputMap.begin();
         it != inputMap.end();
         it++)
    {
        it->second->Delete();
    }
    inputMap.clear();
}

// ****************************************************************************
// Method:  avtCurveRenderer::Render
//
// Purpose:
//   Set up things necessary to call the renderer implentation.  Make a new
//   implementation object if things have changed.
//
// Arguments:
//   ds         the dataset to render
//
// Programmer:  Brad Whitlock
// Creation:    Mon Nov 20 10:44:19 PDT 2006
//
// Modifications:
//
// ****************************************************************************

void
avtCurveRenderer::Render(vtkDataSet *ds)
{
    if (ds->GetNumberOfPoints() == 0)
    {
        debug1 << "avtCurveRenderer::Render: The input dataset had no points!" << endl;
        return;
    }

    //
    // For efficiency: if input type is vtkPolyData, there's no 
    // need to pass it through the geometry filter.
    //

    if (!inputMap[ds])
    {
        // we haven't seen this input yet
        if (ds->GetDataObjectType() != VTK_POLY_DATA) 
        {
            vtkGeometryFilter *gf = vtkGeometryFilter::New();
            gf->SetInputData(ds);
            input = vtkPolyData::New();
            gf->SetOutput(input);
            gf->Update();
            gf->Delete();
        }
        else 
        {
            input = (vtkPolyData *)ds;
            input->Register(NULL);
        }

        inputMap[ds] = input;
    }
    else
    {
        // This one is a re-run, so it's safe to re-use whatever
        // polydata we decided to use for it last time.
        input = inputMap[ds];
    }

    //
    // Render the Curves using the derived type's RenderCurves method.
    //
    RenderCurves();
}

// ****************************************************************************
// Method: avtCurveRenderer::ReleaseGraphicsResources
//
// Purpose: 
//   This method is called when the renderer must release its graphical
//   resources.
//
// Programmer: Brad Whitlock
// Creation:   Mon Nov 20 10:44:19 PDT 2006
//
// Modifications:
//   
// ****************************************************************************

void
avtCurveRenderer::ReleaseGraphicsResources()
{
}

// ****************************************************************************
//  Method:  avtCurveRenderer::SetAtts
//
//  Purpose:
//    Set the attributes
//
//  Arguments:
//    a       : the new attributes
//
//  Programmer:  Brad Whitlock
//  Creation:    Mon Nov 20 10:44:19 PDT 2006
//
//  Modifications:
//
// ****************************************************************************

void
avtCurveRenderer::SetAtts(const CurveAttributes &a)
{
    atts = a;
}
