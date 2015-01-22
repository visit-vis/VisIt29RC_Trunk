// ***************************************************************************
//
// Copyright (c) 2000 - 2015, Lawrence Livermore National Security, LLC
// Produced at the Lawrence Livermore National Laboratory
// LLNL-CODE-442911
// All rights reserved.
//
// This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
// full copyright notice is contained in the file COPYRIGHT located at the root
// of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
//
// Redistribution  and  use  in  source  and  binary  forms,  with  or  without
// modification, are permitted provided that the following conditions are met:
//
//  - Redistributions of  source code must  retain the above  copyright notice,
//    this list of conditions and the disclaimer below.
//  - Redistributions in binary form must reproduce the above copyright notice,
//    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
//    documentation and/or other materials provided with the distribution.
//  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
//    be used to endorse or promote products derived from this software without
//    specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
// ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
// LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
// DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
// SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
// CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
// LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
// OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// ***************************************************************************

package llnl.visit.plots;

import llnl.visit.AttributeSubject;
import llnl.visit.CommunicationBuffer;
import llnl.visit.Plugin;
import llnl.visit.ColorControlPointList;
import java.lang.Byte;
import java.util.Vector;
import llnl.visit.ColorAttribute;
import llnl.visit.ColorAttributeList;
import java.lang.Integer;

// ****************************************************************************
// Class: WellBoreAttributes
//
// Purpose:
//    This class contains the plot attributes for the well bore plot.
//
// Notes:      Autogenerated by xml2java.
//
// Programmer: xml2java
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

public class WellBoreAttributes extends AttributeSubject implements Plugin
{
    private static int WellBoreAttributes_numAdditionalAtts = 19;

    // Enum values
    public final static int WELLRENDERINGMODE_LINES = 0;
    public final static int WELLRENDERINGMODE_CYLINDERS = 1;

    public final static int DETAILLEVEL_LOW = 0;
    public final static int DETAILLEVEL_MEDIUM = 1;
    public final static int DETAILLEVEL_HIGH = 2;
    public final static int DETAILLEVEL_SUPER = 3;

    public final static int COLORINGMETHOD_COLORBYSINGLECOLOR = 0;
    public final static int COLORINGMETHOD_COLORBYMULTIPLECOLORS = 1;
    public final static int COLORINGMETHOD_COLORBYCOLORTABLE = 2;

    public final static int WELLANNOTATION_NONE = 0;
    public final static int WELLANNOTATION_STEMONLY = 1;
    public final static int WELLANNOTATION_NAMEONLY = 2;
    public final static int WELLANNOTATION_STEMANDNAME = 3;


    public WellBoreAttributes()
    {
        super(WellBoreAttributes_numAdditionalAtts);

        defaultPalette = new ColorControlPointList();
        changedColors = new Vector();
        colorType = COLORINGMETHOD_COLORBYMULTIPLECOLORS;
        colorTableName = new String("Default");
        invertColorTable = false;
        singleColor = new ColorAttribute(255, 0, 0);
        multiColor = new ColorAttributeList();
        drawWellsAs = WELLRENDERINGMODE_CYLINDERS;
        wellCylinderQuality = DETAILLEVEL_MEDIUM;
        wellRadius = 0.12f;
        wellLineWidth = 0;
        wellLineStyle = 0;
        wellAnnotation = WELLANNOTATION_STEMANDNAME;
        wellStemHeight = 10f;
        wellNameScale = 0.2f;
        legendFlag = true;
        nWellBores = 0;
        wellBores = new Vector();
        wellNames = new Vector();
    }

    public WellBoreAttributes(int nMoreFields)
    {
        super(WellBoreAttributes_numAdditionalAtts + nMoreFields);

        defaultPalette = new ColorControlPointList();
        changedColors = new Vector();
        colorType = COLORINGMETHOD_COLORBYMULTIPLECOLORS;
        colorTableName = new String("Default");
        invertColorTable = false;
        singleColor = new ColorAttribute(255, 0, 0);
        multiColor = new ColorAttributeList();
        drawWellsAs = WELLRENDERINGMODE_CYLINDERS;
        wellCylinderQuality = DETAILLEVEL_MEDIUM;
        wellRadius = 0.12f;
        wellLineWidth = 0;
        wellLineStyle = 0;
        wellAnnotation = WELLANNOTATION_STEMANDNAME;
        wellStemHeight = 10f;
        wellNameScale = 0.2f;
        legendFlag = true;
        nWellBores = 0;
        wellBores = new Vector();
        wellNames = new Vector();
    }

    public WellBoreAttributes(WellBoreAttributes obj)
    {
        super(WellBoreAttributes_numAdditionalAtts);

        int i;

        defaultPalette = new ColorControlPointList(obj.defaultPalette);
        changedColors = new Vector(obj.changedColors.size());
        for(i = 0; i < obj.changedColors.size(); ++i)
        {
            Byte bv = (Byte)obj.changedColors.elementAt(i);
            changedColors.addElement(new Byte(bv.byteValue()));
        }

        colorType = obj.colorType;
        colorTableName = new String(obj.colorTableName);
        invertColorTable = obj.invertColorTable;
        singleColor = new ColorAttribute(obj.singleColor);
        multiColor = new ColorAttributeList(obj.multiColor);
        drawWellsAs = obj.drawWellsAs;
        wellCylinderQuality = obj.wellCylinderQuality;
        wellRadius = obj.wellRadius;
        wellLineWidth = obj.wellLineWidth;
        wellLineStyle = obj.wellLineStyle;
        wellAnnotation = obj.wellAnnotation;
        wellStemHeight = obj.wellStemHeight;
        wellNameScale = obj.wellNameScale;
        legendFlag = obj.legendFlag;
        nWellBores = obj.nWellBores;
        wellBores = new Vector();
        for(i = 0; i < obj.wellBores.size(); ++i)
        {
            Integer iv = (Integer)obj.wellBores.elementAt(i);
            wellBores.addElement(new Integer(iv.intValue()));
        }
        wellNames = new Vector(obj.wellNames.size());
        for(i = 0; i < obj.wellNames.size(); ++i)
            wellNames.addElement(new String((String)obj.wellNames.elementAt(i)));


        SelectAll();
    }

    public int Offset()
    {
        return super.Offset() + super.GetNumAdditionalAttributes();
    }

    public int GetNumAdditionalAttributes()
    {
        return WellBoreAttributes_numAdditionalAtts;
    }

    public boolean equals(WellBoreAttributes obj)
    {
        int i;

        // Compare the elements in the wellBores vector.
        boolean wellBores_equal = (obj.wellBores.size() == wellBores.size());
        for(i = 0; (i < wellBores.size()) && wellBores_equal; ++i)
        {
            // Make references to Integer from Object.
            Integer wellBores1 = (Integer)wellBores.elementAt(i);
            Integer wellBores2 = (Integer)obj.wellBores.elementAt(i);
            wellBores_equal = wellBores1.equals(wellBores2);
        }
        // Compare the elements in the wellNames vector.
        boolean wellNames_equal = (obj.wellNames.size() == wellNames.size());
        for(i = 0; (i < wellNames.size()) && wellNames_equal; ++i)
        {
            // Make references to String from Object.
            String wellNames1 = (String)wellNames.elementAt(i);
            String wellNames2 = (String)obj.wellNames.elementAt(i);
            wellNames_equal = wellNames1.equals(wellNames2);
        }
        // Create the return value
        return (true /* can ignore defaultPalette */ &&
                true /* can ignore changedColors */ &&
                (colorType == obj.colorType) &&
                (colorTableName.equals(obj.colorTableName)) &&
                (invertColorTable == obj.invertColorTable) &&
                (singleColor == obj.singleColor) &&
                (multiColor.equals(obj.multiColor)) &&
                (drawWellsAs == obj.drawWellsAs) &&
                (wellCylinderQuality == obj.wellCylinderQuality) &&
                (wellRadius == obj.wellRadius) &&
                (wellLineWidth == obj.wellLineWidth) &&
                (wellLineStyle == obj.wellLineStyle) &&
                (wellAnnotation == obj.wellAnnotation) &&
                (wellStemHeight == obj.wellStemHeight) &&
                (wellNameScale == obj.wellNameScale) &&
                (legendFlag == obj.legendFlag) &&
                (nWellBores == obj.nWellBores) &&
                wellBores_equal &&
                wellNames_equal);
    }

    public String GetName() { return "WellBore"; }
    public String GetVersion() { return "1.0"; }

    // Property setting methods
    public void SetDefaultPalette(ColorControlPointList defaultPalette_)
    {
        defaultPalette = defaultPalette_;
        Select(0);
    }

    public void SetChangedColors(Vector changedColors_)
    {
        changedColors = changedColors_;
        Select(1);
    }

    public void SetColorType(int colorType_)
    {
        colorType = colorType_;
        Select(2);
    }

    public void SetColorTableName(String colorTableName_)
    {
        colorTableName = colorTableName_;
        Select(3);
    }

    public void SetInvertColorTable(boolean invertColorTable_)
    {
        invertColorTable = invertColorTable_;
        Select(4);
    }

    public void SetSingleColor(ColorAttribute singleColor_)
    {
        singleColor = singleColor_;
        Select(5);
    }

    public void SetMultiColor(ColorAttributeList multiColor_)
    {
        multiColor = multiColor_;
        Select(6);
    }

    public void SetDrawWellsAs(int drawWellsAs_)
    {
        drawWellsAs = drawWellsAs_;
        Select(7);
    }

    public void SetWellCylinderQuality(int wellCylinderQuality_)
    {
        wellCylinderQuality = wellCylinderQuality_;
        Select(8);
    }

    public void SetWellRadius(float wellRadius_)
    {
        wellRadius = wellRadius_;
        Select(9);
    }

    public void SetWellLineWidth(int wellLineWidth_)
    {
        wellLineWidth = wellLineWidth_;
        Select(10);
    }

    public void SetWellLineStyle(int wellLineStyle_)
    {
        wellLineStyle = wellLineStyle_;
        Select(11);
    }

    public void SetWellAnnotation(int wellAnnotation_)
    {
        wellAnnotation = wellAnnotation_;
        Select(12);
    }

    public void SetWellStemHeight(float wellStemHeight_)
    {
        wellStemHeight = wellStemHeight_;
        Select(13);
    }

    public void SetWellNameScale(float wellNameScale_)
    {
        wellNameScale = wellNameScale_;
        Select(14);
    }

    public void SetLegendFlag(boolean legendFlag_)
    {
        legendFlag = legendFlag_;
        Select(15);
    }

    public void SetNWellBores(int nWellBores_)
    {
        nWellBores = nWellBores_;
        Select(16);
    }

    public void SetWellBores(Vector wellBores_)
    {
        wellBores = wellBores_;
        Select(17);
    }

    public void SetWellNames(Vector wellNames_)
    {
        wellNames = wellNames_;
        Select(18);
    }

    // Property getting methods
    public ColorControlPointList GetDefaultPalette() { return defaultPalette; }
    public Vector                GetChangedColors() { return changedColors; }
    public int                   GetColorType() { return colorType; }
    public String                GetColorTableName() { return colorTableName; }
    public boolean               GetInvertColorTable() { return invertColorTable; }
    public ColorAttribute        GetSingleColor() { return singleColor; }
    public ColorAttributeList    GetMultiColor() { return multiColor; }
    public int                   GetDrawWellsAs() { return drawWellsAs; }
    public int                   GetWellCylinderQuality() { return wellCylinderQuality; }
    public float                 GetWellRadius() { return wellRadius; }
    public int                   GetWellLineWidth() { return wellLineWidth; }
    public int                   GetWellLineStyle() { return wellLineStyle; }
    public int                   GetWellAnnotation() { return wellAnnotation; }
    public float                 GetWellStemHeight() { return wellStemHeight; }
    public float                 GetWellNameScale() { return wellNameScale; }
    public boolean               GetLegendFlag() { return legendFlag; }
    public int                   GetNWellBores() { return nWellBores; }
    public Vector                GetWellBores() { return wellBores; }
    public Vector                GetWellNames() { return wellNames; }

    // Write and read methods.
    public void WriteAtts(CommunicationBuffer buf)
    {
        if(WriteSelect(0, buf))
            defaultPalette.Write(buf);
        if(WriteSelect(1, buf))
            buf.WriteByteVector(changedColors);
        if(WriteSelect(2, buf))
            buf.WriteInt(colorType);
        if(WriteSelect(3, buf))
            buf.WriteString(colorTableName);
        if(WriteSelect(4, buf))
            buf.WriteBool(invertColorTable);
        if(WriteSelect(5, buf))
            singleColor.Write(buf);
        if(WriteSelect(6, buf))
            multiColor.Write(buf);
        if(WriteSelect(7, buf))
            buf.WriteInt(drawWellsAs);
        if(WriteSelect(8, buf))
            buf.WriteInt(wellCylinderQuality);
        if(WriteSelect(9, buf))
            buf.WriteFloat(wellRadius);
        if(WriteSelect(10, buf))
            buf.WriteInt(wellLineWidth);
        if(WriteSelect(11, buf))
            buf.WriteInt(wellLineStyle);
        if(WriteSelect(12, buf))
            buf.WriteInt(wellAnnotation);
        if(WriteSelect(13, buf))
            buf.WriteFloat(wellStemHeight);
        if(WriteSelect(14, buf))
            buf.WriteFloat(wellNameScale);
        if(WriteSelect(15, buf))
            buf.WriteBool(legendFlag);
        if(WriteSelect(16, buf))
            buf.WriteInt(nWellBores);
        if(WriteSelect(17, buf))
            buf.WriteIntVector(wellBores);
        if(WriteSelect(18, buf))
            buf.WriteStringVector(wellNames);
    }

    public void ReadAtts(int index, CommunicationBuffer buf)
    {
        switch(index)
        {
        case 0:
            defaultPalette.Read(buf);
            Select(0);
            break;
        case 1:
            SetChangedColors(buf.ReadByteVector());
            break;
        case 2:
            SetColorType(buf.ReadInt());
            break;
        case 3:
            SetColorTableName(buf.ReadString());
            break;
        case 4:
            SetInvertColorTable(buf.ReadBool());
            break;
        case 5:
            singleColor.Read(buf);
            Select(5);
            break;
        case 6:
            multiColor.Read(buf);
            Select(6);
            break;
        case 7:
            SetDrawWellsAs(buf.ReadInt());
            break;
        case 8:
            SetWellCylinderQuality(buf.ReadInt());
            break;
        case 9:
            SetWellRadius(buf.ReadFloat());
            break;
        case 10:
            SetWellLineWidth(buf.ReadInt());
            break;
        case 11:
            SetWellLineStyle(buf.ReadInt());
            break;
        case 12:
            SetWellAnnotation(buf.ReadInt());
            break;
        case 13:
            SetWellStemHeight(buf.ReadFloat());
            break;
        case 14:
            SetWellNameScale(buf.ReadFloat());
            break;
        case 15:
            SetLegendFlag(buf.ReadBool());
            break;
        case 16:
            SetNWellBores(buf.ReadInt());
            break;
        case 17:
            SetWellBores(buf.ReadIntVector());
            break;
        case 18:
            SetWellNames(buf.ReadStringVector());
            break;
        }
    }

    public String toString(String indent)
    {
        String str = new String();
        str = str + indent + "defaultPalette = {\n" + defaultPalette.toString(indent + "    ") + indent + "}\n";
        str = str + ucharVectorToString("changedColors", changedColors, indent) + "\n";
        str = str + indent + "colorType = ";
        if(colorType == COLORINGMETHOD_COLORBYSINGLECOLOR)
            str = str + "COLORINGMETHOD_COLORBYSINGLECOLOR";
        if(colorType == COLORINGMETHOD_COLORBYMULTIPLECOLORS)
            str = str + "COLORINGMETHOD_COLORBYMULTIPLECOLORS";
        if(colorType == COLORINGMETHOD_COLORBYCOLORTABLE)
            str = str + "COLORINGMETHOD_COLORBYCOLORTABLE";
        str = str + "\n";
        str = str + stringToString("colorTableName", colorTableName, indent) + "\n";
        str = str + boolToString("invertColorTable", invertColorTable, indent) + "\n";
        str = str + indent + "singleColor = {" + singleColor.Red() + ", " + singleColor.Green() + ", " + singleColor.Blue() + ", " + singleColor.Alpha() + "}\n";
        str = str + indent + "multiColor = {\n" + multiColor.toString(indent + "    ") + indent + "}\n";
        str = str + indent + "drawWellsAs = ";
        if(drawWellsAs == WELLRENDERINGMODE_LINES)
            str = str + "WELLRENDERINGMODE_LINES";
        if(drawWellsAs == WELLRENDERINGMODE_CYLINDERS)
            str = str + "WELLRENDERINGMODE_CYLINDERS";
        str = str + "\n";
        str = str + indent + "wellCylinderQuality = ";
        if(wellCylinderQuality == DETAILLEVEL_LOW)
            str = str + "DETAILLEVEL_LOW";
        if(wellCylinderQuality == DETAILLEVEL_MEDIUM)
            str = str + "DETAILLEVEL_MEDIUM";
        if(wellCylinderQuality == DETAILLEVEL_HIGH)
            str = str + "DETAILLEVEL_HIGH";
        if(wellCylinderQuality == DETAILLEVEL_SUPER)
            str = str + "DETAILLEVEL_SUPER";
        str = str + "\n";
        str = str + floatToString("wellRadius", wellRadius, indent) + "\n";
        str = str + intToString("wellLineWidth", wellLineWidth, indent) + "\n";
        str = str + intToString("wellLineStyle", wellLineStyle, indent) + "\n";
        str = str + indent + "wellAnnotation = ";
        if(wellAnnotation == WELLANNOTATION_NONE)
            str = str + "WELLANNOTATION_NONE";
        if(wellAnnotation == WELLANNOTATION_STEMONLY)
            str = str + "WELLANNOTATION_STEMONLY";
        if(wellAnnotation == WELLANNOTATION_NAMEONLY)
            str = str + "WELLANNOTATION_NAMEONLY";
        if(wellAnnotation == WELLANNOTATION_STEMANDNAME)
            str = str + "WELLANNOTATION_STEMANDNAME";
        str = str + "\n";
        str = str + floatToString("wellStemHeight", wellStemHeight, indent) + "\n";
        str = str + floatToString("wellNameScale", wellNameScale, indent) + "\n";
        str = str + boolToString("legendFlag", legendFlag, indent) + "\n";
        str = str + intToString("nWellBores", nWellBores, indent) + "\n";
        str = str + intVectorToString("wellBores", wellBores, indent) + "\n";
        str = str + stringVectorToString("wellNames", wellNames, indent) + "\n";
        return str;
    }


    // Attributes
    private ColorControlPointList defaultPalette;
    private Vector                changedColors; // vector of Byte objects
    private int                   colorType;
    private String                colorTableName;
    private boolean               invertColorTable;
    private ColorAttribute        singleColor;
    private ColorAttributeList    multiColor;
    private int                   drawWellsAs;
    private int                   wellCylinderQuality;
    private float                 wellRadius;
    private int                   wellLineWidth;
    private int                   wellLineStyle;
    private int                   wellAnnotation;
    private float                 wellStemHeight;
    private float                 wellNameScale;
    private boolean               legendFlag;
    private int                   nWellBores;
    private Vector                wellBores; // vector of Integer objects
    private Vector                wellNames; // vector of String objects
}
