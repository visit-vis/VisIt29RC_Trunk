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
//                            avtAMRFileFormat.h                             //
// ************************************************************************* //

#ifndef AVT_AMR_FILE_FORMAT_H
#define AVT_AMR_FILE_FORMAT_H
#include <avtSTMDFileFormat.h>

#include <vector>

class AMRreaderInterface;

// ****************************************************************************
//  Class: avtAMRFileFormat
//
//  Purpose:
//      Reads in AMR files as a plugin to VisIt.
//
//  Programmer: hpan -- generated by xml2avt
//  Creation:   Mon Apr 26 10:26:32 PDT 2010
//
//  Modifications:
//    Brad Whitlock, Wed May 28 14:17:40 PDT 2014
//    Make the reader use AMRreaderInterface to read the AMR data. Also tell
//    VisIt that the metadata and SIL vary over time.
//
// ****************************************************************************

class avtAMRFileFormat : public avtSTMDFileFormat
{
public:
    avtAMRFileFormat(const char *);
    virtual           ~avtAMRFileFormat();

    // Tell VisIt that the SIL and Metadata change over time.
    bool                  HasInvariantMetaData(void) const
    {
        return false;
    };
    bool                  HasInvariantSIL(void) const
    {
        return false;
    };

    //
    // This is used to return unconventional data -- ranging from material
    // information to information about block connectivity.
    //
    virtual void         *GetAuxiliaryData(const char *var, int domain,
                                           const char *type, void *args,
                                           DestructorFunction &);

    //
    // If you know the cycle number, overload this function.
    // Otherwise, VisIt will make up a reasonable one for you.
    virtual bool          ReturnsValidCycle() const
    {
        return true;
    };
    virtual int           GetCycle(void);
    virtual bool          ReturnsValidTime() const
    {
        return true;
    };
    virtual double        GetTime(void);

    virtual const char   *GetType(void)
    {
        return "AMR";
    };
    virtual void          FreeUpResources(void);

    virtual vtkDataSet   *GetMesh(int, const char *);
    virtual vtkDataArray *GetVar(int, const char *);
    virtual vtkDataArray *GetVectorVar(int, const char *);

protected:
    AMRreaderInterface   *GetReader(bool heavyweightInit = true);
    virtual void          PopulateDatabaseMetaData(avtDatabaseMetaData *);

    static const std::string amr_name;
    static const std::string intf_name;
    static const std::string invalid_name;

    static std::string composeName( const std::string& m, const std::string& v, const char app='/' );
    static void decomposeName( const std::string& s, std::string& m, std::string& v );

private:
    // DATA MEMBERS
    bool                enableAMR;
    AMRreaderInterface *reader_;
};


#endif