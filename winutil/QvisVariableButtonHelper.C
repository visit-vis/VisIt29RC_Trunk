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

#include <QvisVariableButtonHelper.h>
#include <QvisVariableButton.h>

// ****************************************************************************
// Method: QvisVariableButtonHelper::QvisVariableButtonHelper
//
// Purpose: 
//   Constructor for the QvisVariableButtonHelper class.
//
// Programmer: Brad Whitlock
// Creation:   Thu Dec 9 16:35:42 PST 2004
//
// Modifications:
//   Brad Whitlock, Fri May  9 13:31:33 PDT 2008
//   Qt 4.
//
// ****************************************************************************

QvisVariableButtonHelper::QvisVariableButtonHelper(QObject *parent) : QObject(parent)
{
    btn = 0;
}

// ****************************************************************************
// Method: QvisVariableButtonHelper::~QvisVariableButtonHelper
//
// Purpose: 
//   Destructor for the QvisVariableButtonHelper class.
//
// Programmer: Brad Whitlock
// Creation:   Thu Dec 9 16:35:42 PST 2004
//
// Modifications:
//   
// ****************************************************************************

QvisVariableButtonHelper::~QvisVariableButtonHelper()
{
}

// ****************************************************************************
// Method: QvisVariableButtonHelper::setButton
//
// Purpose: 
//   Sets the button to use to call the changeVariable method.
//
// Arguments:
//   b : The variable button to use.
//
// Programmer: Brad Whitlock
// Creation:   Thu Dec 9 16:36:16 PST 2004
//
// Modifications:
//   
// ****************************************************************************

void
QvisVariableButtonHelper::setButton(QvisBaseVariableButton *b)
{
    btn = b;
}

// ****************************************************************************
// Method: QvisVariableButtonHelper::activated
//
// Purpose: 
//   This is a Qt slot function that is called for all variable menus.
//
// Programmer: Brad Whitlock
// Creation:   Thu Dec 9 16:36:53 PST 2004
//
// Modifications:
//   
// ****************************************************************************

void
QvisVariableButtonHelper::activated(int val, const QString &sval)
{
    if(btn)
        btn->changeVariable(val, sval);
}
