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

#ifndef METRICTHRESHOLDATTRIBUTES_H
#define METRICTHRESHOLDATTRIBUTES_H
#include <string>
#include <AttributeSubject.h>


// ****************************************************************************
// Class: MetricThresholdAttributes
//
// Purpose:
//    This class contains attributes for the MetricThreshold operator.
//
// Notes:      Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

class MetricThresholdAttributes : public AttributeSubject
{
public:
    enum Preset
    {
        None,
        Aspect_Ratio,
        Aspect_Gamma,
        Skew,
        Taper,
        Volume,
        Stretch,
        Diagonal,
        Dimension,
        Oddy,
        Condition,
        Jacobian,
        Scaled_Jacobian,
        Shear,
        Shape,
        Relative_Size,
        Shape_and_Size,
        Area,
        Warpage,
        Smallest_Angle,
        Largest_Angle
    };

    // These constructors are for objects of this class
    MetricThresholdAttributes();
    MetricThresholdAttributes(const MetricThresholdAttributes &obj);
protected:
    // These constructors are for objects derived from this class
    MetricThresholdAttributes(private_tmfs_t tmfs);
    MetricThresholdAttributes(const MetricThresholdAttributes &obj, private_tmfs_t tmfs);
public:
    virtual ~MetricThresholdAttributes();

    virtual MetricThresholdAttributes& operator = (const MetricThresholdAttributes &obj);
    virtual bool operator == (const MetricThresholdAttributes &obj) const;
    virtual bool operator != (const MetricThresholdAttributes &obj) const;
private:
    void Init();
    void Copy(const MetricThresholdAttributes &obj);
public:

    virtual const std::string TypeName() const;
    virtual bool CopyAttributes(const AttributeGroup *);
    virtual AttributeSubject *CreateCompatible(const std::string &) const;
    virtual AttributeSubject *NewInstance(bool) const;

    // Property selection methods
    virtual void SelectAll();

    // Property setting methods
    void SetPreset(Preset preset_);
    void SetHexahedron(bool Hexahedron_);
    void SetHex_lower(double hex_lower_);
    void SetHex_upper(double hex_upper_);
    void SetTetrahedron(bool Tetrahedron_);
    void SetTet_lower(double tet_lower_);
    void SetTet_upper(double tet_upper_);
    void SetWedge(bool Wedge_);
    void SetWed_lower(double wed_lower_);
    void SetWed_upper(double wed_upper_);
    void SetPyramid(bool Pyramid_);
    void SetPyr_lower(double pyr_lower_);
    void SetPyr_upper(double pyr_upper_);
    void SetTriangle(bool Triangle_);
    void SetTri_lower(double tri_lower_);
    void SetTri_upper(double tri_upper_);
    void SetQuad(bool Quad_);
    void SetQuad_lower(double quad_lower_);
    void SetQuad_upper(double quad_upper_);

    // Property getting methods
    Preset GetPreset() const;
    bool   GetHexahedron() const;
    double GetHex_lower() const;
    double GetHex_upper() const;
    bool   GetTetrahedron() const;
    double GetTet_lower() const;
    double GetTet_upper() const;
    bool   GetWedge() const;
    double GetWed_lower() const;
    double GetWed_upper() const;
    bool   GetPyramid() const;
    double GetPyr_lower() const;
    double GetPyr_upper() const;
    bool   GetTriangle() const;
    double GetTri_lower() const;
    double GetTri_upper() const;
    bool   GetQuad() const;
    double GetQuad_lower() const;
    double GetQuad_upper() const;

    // Persistence methods
    virtual bool CreateNode(DataNode *node, bool completeSave, bool forceAdd);
    virtual void SetFromNode(DataNode *node);

    // Enum conversion functions
    static std::string Preset_ToString(Preset);
    static bool Preset_FromString(const std::string &, Preset &);
protected:
    static std::string Preset_ToString(int);
public:

    // Keyframing methods
    virtual std::string               GetFieldName(int index) const;
    virtual AttributeGroup::FieldType GetFieldType(int index) const;
    virtual std::string               GetFieldTypeName(int index) const;
    virtual bool                      FieldsEqual(int index, const AttributeGroup *rhs) const;


    // IDs that can be used to identify fields in case statements
    enum {
        ID_preset = 0,
        ID_Hexahedron,
        ID_hex_lower,
        ID_hex_upper,
        ID_Tetrahedron,
        ID_tet_lower,
        ID_tet_upper,
        ID_Wedge,
        ID_wed_lower,
        ID_wed_upper,
        ID_Pyramid,
        ID_pyr_lower,
        ID_pyr_upper,
        ID_Triangle,
        ID_tri_lower,
        ID_tri_upper,
        ID_Quad,
        ID_quad_lower,
        ID_quad_upper,
        ID__LAST
    };

private:
    int    preset;
    bool   Hexahedron;
    double hex_lower;
    double hex_upper;
    bool   Tetrahedron;
    double tet_lower;
    double tet_upper;
    bool   Wedge;
    double wed_lower;
    double wed_upper;
    bool   Pyramid;
    double pyr_lower;
    double pyr_upper;
    bool   Triangle;
    double tri_lower;
    double tri_upper;
    bool   Quad;
    double quad_lower;
    double quad_upper;

    // Static class format string for type map.
    static const char *TypeMapFormatString;
    static const private_tmfs_t TmfsStruct;
};
#define METRICTHRESHOLDATTRIBUTES_TMFS "ibddbddbddbddbddbdd"

#endif
