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
#ifndef VIEWER_FACTORY_H
#define VIEWER_FACTORY_H
#include <viewercore_exports.h>
#include <ViewerBase.h>

class OperatorPluginManager;
class PlotPluginManager;
class ViewerActionManager;
class ViewerChangeUsername;
class ViewerConnectionPrinter;
class ViewerConnectionProgress;
class ViewerEngineManagerInterface;
class ViewerFileServerInterface;
class ViewerHostProfileSelector;
class ViewerMessaging;
class ViewerWindow;
class VisWindow;

// ****************************************************************************
// Class: ViewerFactory
//
// Purpose:
//   Factory class for various object types.
//
// Notes:    The classes we produce here typically have non-UI and UI variants.
//           This particular factory produces the non-UI versions.
//
// Programmer: Brad Whitlock
// Creation:   Thu Sep  4 00:09:44 PDT 2014
//
// Modifications:
//
// ****************************************************************************

class VIEWERCORE_API ViewerFactory : public ViewerBase
{
public:
    ViewerFactory();
    virtual ~ViewerFactory();

    virtual ViewerWindow        *CreateViewerWindow(int windowIndex);

    virtual VisWindow           *CreateVisWindow();

    virtual ViewerActionManager *CreateActionManager(ViewerWindow *win);

    virtual ViewerMessaging     *CreateViewerMessaging();

    virtual ViewerConnectionProgress *CreateConnectionProgress();

    virtual ViewerConnectionPrinter  *CreateConnectionPrinter();

    virtual ViewerFileServerInterface *CreateFileServerInterface();

    virtual ViewerEngineManagerInterface *CreateEngineManagerInterface();

    virtual ViewerHostProfileSelector *CreateHostProfileSelector();

    virtual ViewerChangeUsername *CreateChangeUsername();

    virtual PlotPluginManager *CreatePlotPluginManager();

    virtual OperatorPluginManager *CreateOperatorPluginManager();

    // Set some method override callbacks.
    void OverrideCreateConnectionPrinter(ViewerConnectionPrinter *(*cb)(void));

protected:
    void SetActionManagerLogic(ViewerActionManager *, ViewerWindow *);

    ViewerConnectionPrinter      *(*override_CreateConnectionPrinter)(void);
};

#endif

