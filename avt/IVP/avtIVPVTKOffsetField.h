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
//                              avtIVPVTKOffsetField.h                             //
// ************************************************************************* //

#ifndef AVT_IVPVTKOFFSETFIELD_H
#define AVT_IVPVTKOFFSETFIELD_H

#include <vtkCell.h>

#include <avtIVPVTKField.h>
#include <avtCellLocator.h>
#include <ivp_exports.h>

#include <vector>

class vtkDataSet;
class vtkDataArray;

// ****************************************************************************
//  Class:  avtIVPVTKOffsetField
//
//  Purpose:
//    A wrapper class to allow the use of vtkDataSets as IVP fields for 
//    streamline integration. Uses vtkInterpolatedVelocityField on top of 
//    the supplied vtkDataSet. Fields are allowed to have offsets, i.e. 
//    each component can have a different stagger.
//
//  Programmer:  Alexander Pletzer
//  Creation:    Thu Mar  1 12:01:37 MST 2012
//
//  Modifications:
//
//
// ****************************************************************************

class IVP_API avtIVPVTKOffsetField: public avtIVPVTKField
{
  public:
    avtIVPVTKOffsetField( vtkDataSet* dataset, 
                          avtCellLocator* locator );
    ~avtIVPVTKOffsetField();

    virtual avtVector operator()( const double &t, const avtVector &p ) const;
    virtual Result    operator()(const double& t,
                                 const avtVector &pt,
                                 avtVector &retV) const;

    virtual bool      FindValue(vtkDataArray *vectorData, avtVector &vel) const;

    void SetNodeOffsets( const std::vector<avtVector>& offsets );

  protected:

    avtVector GetPositionCorrection( size_t compIndex ) const;

    std::vector<avtVector> nodeOffsets;

  private:
};

#endif