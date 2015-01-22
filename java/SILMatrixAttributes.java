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

package llnl.visit;

import java.lang.Integer;
import java.util.Vector;

// ****************************************************************************
// Class: SILMatrixAttributes
//
// Purpose:
//    This class contain the information needed to represent a SIL Matrix.
//
// Notes:      Autogenerated by xml2java.
//
// Programmer: xml2java
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

public class SILMatrixAttributes extends AttributeSubject
{
    private static int SILMatrixAttributes_numAdditionalAtts = 6;

    public SILMatrixAttributes()
    {
        super(SILMatrixAttributes_numAdditionalAtts);

        set1 = new Vector();
        category1 = new String("");
        role1 = 0;
        set2 = new Vector();
        category2 = new String("");
        role2 = 0;
    }

    public SILMatrixAttributes(int nMoreFields)
    {
        super(SILMatrixAttributes_numAdditionalAtts + nMoreFields);

        set1 = new Vector();
        category1 = new String("");
        role1 = 0;
        set2 = new Vector();
        category2 = new String("");
        role2 = 0;
    }

    public SILMatrixAttributes(SILMatrixAttributes obj)
    {
        super(SILMatrixAttributes_numAdditionalAtts);

        int i;

        set1 = new Vector();
        for(i = 0; i < obj.set1.size(); ++i)
        {
            Integer iv = (Integer)obj.set1.elementAt(i);
            set1.addElement(new Integer(iv.intValue()));
        }
        category1 = new String(obj.category1);
        role1 = obj.role1;
        set2 = new Vector();
        for(i = 0; i < obj.set2.size(); ++i)
        {
            Integer iv = (Integer)obj.set2.elementAt(i);
            set2.addElement(new Integer(iv.intValue()));
        }
        category2 = new String(obj.category2);
        role2 = obj.role2;

        SelectAll();
    }

    public int Offset()
    {
        return super.Offset() + super.GetNumAdditionalAttributes();
    }

    public int GetNumAdditionalAttributes()
    {
        return SILMatrixAttributes_numAdditionalAtts;
    }

    public boolean equals(SILMatrixAttributes obj)
    {
        int i;

        // Compare the elements in the set1 vector.
        boolean set1_equal = (obj.set1.size() == set1.size());
        for(i = 0; (i < set1.size()) && set1_equal; ++i)
        {
            // Make references to Integer from Object.
            Integer set11 = (Integer)set1.elementAt(i);
            Integer set12 = (Integer)obj.set1.elementAt(i);
            set1_equal = set11.equals(set12);
        }
        // Compare the elements in the set2 vector.
        boolean set2_equal = (obj.set2.size() == set2.size());
        for(i = 0; (i < set2.size()) && set2_equal; ++i)
        {
            // Make references to Integer from Object.
            Integer set21 = (Integer)set2.elementAt(i);
            Integer set22 = (Integer)obj.set2.elementAt(i);
            set2_equal = set21.equals(set22);
        }
        // Create the return value
        return (set1_equal &&
                (category1.equals(obj.category1)) &&
                (role1 == obj.role1) &&
                set2_equal &&
                (category2.equals(obj.category2)) &&
                (role2 == obj.role2));
    }

    // Property setting methods
    public void SetSet1(Vector set1_)
    {
        set1 = set1_;
        Select(0);
    }

    public void SetCategory1(String category1_)
    {
        category1 = category1_;
        Select(1);
    }

    public void SetRole1(int role1_)
    {
        role1 = role1_;
        Select(2);
    }

    public void SetSet2(Vector set2_)
    {
        set2 = set2_;
        Select(3);
    }

    public void SetCategory2(String category2_)
    {
        category2 = category2_;
        Select(4);
    }

    public void SetRole2(int role2_)
    {
        role2 = role2_;
        Select(5);
    }

    // Property getting methods
    public Vector GetSet1() { return set1; }
    public String GetCategory1() { return category1; }
    public int    GetRole1() { return role1; }
    public Vector GetSet2() { return set2; }
    public String GetCategory2() { return category2; }
    public int    GetRole2() { return role2; }

    // Write and read methods.
    public void WriteAtts(CommunicationBuffer buf)
    {
        if(WriteSelect(0, buf))
            buf.WriteIntVector(set1);
        if(WriteSelect(1, buf))
            buf.WriteString(category1);
        if(WriteSelect(2, buf))
            buf.WriteInt(role1);
        if(WriteSelect(3, buf))
            buf.WriteIntVector(set2);
        if(WriteSelect(4, buf))
            buf.WriteString(category2);
        if(WriteSelect(5, buf))
            buf.WriteInt(role2);
    }

    public void ReadAtts(int index, CommunicationBuffer buf)
    {
        switch(index)
        {
        case 0:
            SetSet1(buf.ReadIntVector());
            break;
        case 1:
            SetCategory1(buf.ReadString());
            break;
        case 2:
            SetRole1(buf.ReadInt());
            break;
        case 3:
            SetSet2(buf.ReadIntVector());
            break;
        case 4:
            SetCategory2(buf.ReadString());
            break;
        case 5:
            SetRole2(buf.ReadInt());
            break;
        }
    }

    public String toString(String indent)
    {
        String str = new String();
        str = str + intVectorToString("set1", set1, indent) + "\n";
        str = str + stringToString("category1", category1, indent) + "\n";
        str = str + intToString("role1", role1, indent) + "\n";
        str = str + intVectorToString("set2", set2, indent) + "\n";
        str = str + stringToString("category2", category2, indent) + "\n";
        str = str + intToString("role2", role2, indent) + "\n";
        return str;
    }


    // Attributes
    private Vector set1; // vector of Integer objects
    private String category1;
    private int    role1;
    private Vector set2; // vector of Integer objects
    private String category2;
    private int    role2;
}
