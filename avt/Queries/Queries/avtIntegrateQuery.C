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
//                             avtIntegrateQuery.C                           //
// ************************************************************************* //

#include <avtIntegrateQuery.h>

#include <snprintf.h>

#include <string>

// ****************************************************************************
//  Method: avtIntegrateQuery constructor
//
//  Purpose:
//      Defines the constructor.  Note: this should not be inlined in the header
//      because it causes problems for certain compilers.
//
//  Programmer: Hank Childs
//  Creation:   February 5, 2004
//
// ****************************************************************************

avtIntegrateQuery::avtIntegrateQuery()
{
    ;
}


// ****************************************************************************
//  Method: avtIntegrateQuery destructor
//
//  Purpose:
//      Defines the destructor.  Note: this should not be inlined in the header
//      because it causes problems for certain compilers.
//
//  Programmer: Hank Childs
//  Creation:   February 5, 2004
//
// ****************************************************************************

avtIntegrateQuery::~avtIntegrateQuery()
{
    ;
}


// ****************************************************************************
//  Method: avtIntegrateQuery::CurveQuery
//
//  Purpose:
//      Computes the area under the curve -- the integral.
//
//  Programmer:   Hank Childs
//  Creation:     October 3, 2003
//
//  Modifications:
//
//    Hank Childs, Sat Feb 14 09:59:51 PST 2004
//    Fix cut-n-paste bug.
//
// ****************************************************************************

double
avtIntegrateQuery::CurveQuery(int n1, const float *x1, const float *y1)
{
    //
    // Pretty straightforward.  We are going to calculate the integral as
    // the sum of a series of trapezoids.  We are not handling the case
    // where the trapezoid is degenerate (because the function crossed
    // the line y=0).
    //
    double integral = 0;
    for (int i = 0 ; i < n1-1 ; i++)
    {
        double height = x1[i+1] - x1[i];
        double base1 = y1[i] - 0.;
        double base2 = y1[i+1] - 0.;
        integral += (base1+base2)*height / 2.;
    }

    return integral;
}


// ****************************************************************************
//  Method: avtIntegrateQuery::CreateMessage
//
//  Purpose:
//      Creates a message for the integrate query.
//
//  Programmer: Hank Childs
//  Creation:   October 3, 2003
//
//  Modifications:
//
//    Cyrus Harrison, Tue Sep 18 13:45:35 PDT 2007
//    Added support for user settable floating point format string
//
//    Kathleen Biagas, Thu Feb 13 15:04:58 PST 2014
//    Add Xml results.
//
// ****************************************************************************

std::string
avtIntegrateQuery::CreateMessage(double integral)
{
    MapNode result_node;
    result_node["area_under_curve"] = integral;
    SetXmlResult(result_node.ToXML());

    char msg[1024];
    std::string format =  "The area under the curve is " 
                         + queryAtts.GetFloatFormat() + ".";
    SNPRINTF(msg,1024, format.c_str(),integral);
    std::string m = msg;
    return m;
}


