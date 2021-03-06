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

#include "BitUtils.h"

// ----------------------------------------------------------------------------
//  Programmer: Jeremy Meredith
//  Date:       August 21, 2003
// ----------------------------------------------------------------------------

const unsigned char nBitsSet[256] = {
    0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
    1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
    1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
    2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
    1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
    2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
    2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
    3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8
};

const signed char bitsSet[256][8] = {
    {-1, -1, -1, -1, -1, -1, -1, -1},
    { 0, -1, -1, -1, -1, -1, -1, -1},
    { 1, -1, -1, -1, -1, -1, -1, -1},
    { 0,  1, -1, -1, -1, -1, -1, -1},
    { 2, -1, -1, -1, -1, -1, -1, -1},
    { 0,  2, -1, -1, -1, -1, -1, -1},
    { 1,  2, -1, -1, -1, -1, -1, -1},
    { 0,  1,  2, -1, -1, -1, -1, -1},
    { 3, -1, -1, -1, -1, -1, -1, -1},
    { 0,  3, -1, -1, -1, -1, -1, -1},
    { 1,  3, -1, -1, -1, -1, -1, -1},
    { 0,  1,  3, -1, -1, -1, -1, -1},
    { 2,  3, -1, -1, -1, -1, -1, -1},
    { 0,  2,  3, -1, -1, -1, -1, -1},
    { 1,  2,  3, -1, -1, -1, -1, -1},
    { 0,  1,  2,  3, -1, -1, -1, -1},
    { 4, -1, -1, -1, -1, -1, -1, -1},
    { 0,  4, -1, -1, -1, -1, -1, -1},
    { 1,  4, -1, -1, -1, -1, -1, -1},
    { 0,  1,  4, -1, -1, -1, -1, -1},
    { 2,  4, -1, -1, -1, -1, -1, -1},
    { 0,  2,  4, -1, -1, -1, -1, -1},
    { 1,  2,  4, -1, -1, -1, -1, -1},
    { 0,  1,  2,  4, -1, -1, -1, -1},
    { 3,  4, -1, -1, -1, -1, -1, -1},
    { 0,  3,  4, -1, -1, -1, -1, -1},
    { 1,  3,  4, -1, -1, -1, -1, -1},
    { 0,  1,  3,  4, -1, -1, -1, -1},
    { 2,  3,  4, -1, -1, -1, -1, -1},
    { 0,  2,  3,  4, -1, -1, -1, -1},
    { 1,  2,  3,  4, -1, -1, -1, -1},
    { 0,  1,  2,  3,  4, -1, -1, -1},
    { 5, -1, -1, -1, -1, -1, -1, -1},
    { 0,  5, -1, -1, -1, -1, -1, -1},
    { 1,  5, -1, -1, -1, -1, -1, -1},
    { 0,  1,  5, -1, -1, -1, -1, -1},
    { 2,  5, -1, -1, -1, -1, -1, -1},
    { 0,  2,  5, -1, -1, -1, -1, -1},
    { 1,  2,  5, -1, -1, -1, -1, -1},
    { 0,  1,  2,  5, -1, -1, -1, -1},
    { 3,  5, -1, -1, -1, -1, -1, -1},
    { 0,  3,  5, -1, -1, -1, -1, -1},
    { 1,  3,  5, -1, -1, -1, -1, -1},
    { 0,  1,  3,  5, -1, -1, -1, -1},
    { 2,  3,  5, -1, -1, -1, -1, -1},
    { 0,  2,  3,  5, -1, -1, -1, -1},
    { 1,  2,  3,  5, -1, -1, -1, -1},
    { 0,  1,  2,  3,  5, -1, -1, -1},
    { 4,  5, -1, -1, -1, -1, -1, -1},
    { 0,  4,  5, -1, -1, -1, -1, -1},
    { 1,  4,  5, -1, -1, -1, -1, -1},
    { 0,  1,  4,  5, -1, -1, -1, -1},
    { 2,  4,  5, -1, -1, -1, -1, -1},
    { 0,  2,  4,  5, -1, -1, -1, -1},
    { 1,  2,  4,  5, -1, -1, -1, -1},
    { 0,  1,  2,  4,  5, -1, -1, -1},
    { 3,  4,  5, -1, -1, -1, -1, -1},
    { 0,  3,  4,  5, -1, -1, -1, -1},
    { 1,  3,  4,  5, -1, -1, -1, -1},
    { 0,  1,  3,  4,  5, -1, -1, -1},
    { 2,  3,  4,  5, -1, -1, -1, -1},
    { 0,  2,  3,  4,  5, -1, -1, -1},
    { 1,  2,  3,  4,  5, -1, -1, -1},
    { 0,  1,  2,  3,  4,  5, -1, -1},
    { 6, -1, -1, -1, -1, -1, -1, -1},
    { 0,  6, -1, -1, -1, -1, -1, -1},
    { 1,  6, -1, -1, -1, -1, -1, -1},
    { 0,  1,  6, -1, -1, -1, -1, -1},
    { 2,  6, -1, -1, -1, -1, -1, -1},
    { 0,  2,  6, -1, -1, -1, -1, -1},
    { 1,  2,  6, -1, -1, -1, -1, -1},
    { 0,  1,  2,  6, -1, -1, -1, -1},
    { 3,  6, -1, -1, -1, -1, -1, -1},
    { 0,  3,  6, -1, -1, -1, -1, -1},
    { 1,  3,  6, -1, -1, -1, -1, -1},
    { 0,  1,  3,  6, -1, -1, -1, -1},
    { 2,  3,  6, -1, -1, -1, -1, -1},
    { 0,  2,  3,  6, -1, -1, -1, -1},
    { 1,  2,  3,  6, -1, -1, -1, -1},
    { 0,  1,  2,  3,  6, -1, -1, -1},
    { 4,  6, -1, -1, -1, -1, -1, -1},
    { 0,  4,  6, -1, -1, -1, -1, -1},
    { 1,  4,  6, -1, -1, -1, -1, -1},
    { 0,  1,  4,  6, -1, -1, -1, -1},
    { 2,  4,  6, -1, -1, -1, -1, -1},
    { 0,  2,  4,  6, -1, -1, -1, -1},
    { 1,  2,  4,  6, -1, -1, -1, -1},
    { 0,  1,  2,  4,  6, -1, -1, -1},
    { 3,  4,  6, -1, -1, -1, -1, -1},
    { 0,  3,  4,  6, -1, -1, -1, -1},
    { 1,  3,  4,  6, -1, -1, -1, -1},
    { 0,  1,  3,  4,  6, -1, -1, -1},
    { 2,  3,  4,  6, -1, -1, -1, -1},
    { 0,  2,  3,  4,  6, -1, -1, -1},
    { 1,  2,  3,  4,  6, -1, -1, -1},
    { 0,  1,  2,  3,  4,  6, -1, -1},
    { 5,  6, -1, -1, -1, -1, -1, -1},
    { 0,  5,  6, -1, -1, -1, -1, -1},
    { 1,  5,  6, -1, -1, -1, -1, -1},
    { 0,  1,  5,  6, -1, -1, -1, -1},
    { 2,  5,  6, -1, -1, -1, -1, -1},
    { 0,  2,  5,  6, -1, -1, -1, -1},
    { 1,  2,  5,  6, -1, -1, -1, -1},
    { 0,  1,  2,  5,  6, -1, -1, -1},
    { 3,  5,  6, -1, -1, -1, -1, -1},
    { 0,  3,  5,  6, -1, -1, -1, -1},
    { 1,  3,  5,  6, -1, -1, -1, -1},
    { 0,  1,  3,  5,  6, -1, -1, -1},
    { 2,  3,  5,  6, -1, -1, -1, -1},
    { 0,  2,  3,  5,  6, -1, -1, -1},
    { 1,  2,  3,  5,  6, -1, -1, -1},
    { 0,  1,  2,  3,  5,  6, -1, -1},
    { 4,  5,  6, -1, -1, -1, -1, -1},
    { 0,  4,  5,  6, -1, -1, -1, -1},
    { 1,  4,  5,  6, -1, -1, -1, -1},
    { 0,  1,  4,  5,  6, -1, -1, -1},
    { 2,  4,  5,  6, -1, -1, -1, -1},
    { 0,  2,  4,  5,  6, -1, -1, -1},
    { 1,  2,  4,  5,  6, -1, -1, -1},
    { 0,  1,  2,  4,  5,  6, -1, -1},
    { 3,  4,  5,  6, -1, -1, -1, -1},
    { 0,  3,  4,  5,  6, -1, -1, -1},
    { 1,  3,  4,  5,  6, -1, -1, -1},
    { 0,  1,  3,  4,  5,  6, -1, -1},
    { 2,  3,  4,  5,  6, -1, -1, -1},
    { 0,  2,  3,  4,  5,  6, -1, -1},
    { 1,  2,  3,  4,  5,  6, -1, -1},
    { 0,  1,  2,  3,  4,  5,  6, -1},
    { 7, -1, -1, -1, -1, -1, -1, -1},
    { 0,  7, -1, -1, -1, -1, -1, -1},
    { 1,  7, -1, -1, -1, -1, -1, -1},
    { 0,  1,  7, -1, -1, -1, -1, -1},
    { 2,  7, -1, -1, -1, -1, -1, -1},
    { 0,  2,  7, -1, -1, -1, -1, -1},
    { 1,  2,  7, -1, -1, -1, -1, -1},
    { 0,  1,  2,  7, -1, -1, -1, -1},
    { 3,  7, -1, -1, -1, -1, -1, -1},
    { 0,  3,  7, -1, -1, -1, -1, -1},
    { 1,  3,  7, -1, -1, -1, -1, -1},
    { 0,  1,  3,  7, -1, -1, -1, -1},
    { 2,  3,  7, -1, -1, -1, -1, -1},
    { 0,  2,  3,  7, -1, -1, -1, -1},
    { 1,  2,  3,  7, -1, -1, -1, -1},
    { 0,  1,  2,  3,  7, -1, -1, -1},
    { 4,  7, -1, -1, -1, -1, -1, -1},
    { 0,  4,  7, -1, -1, -1, -1, -1},
    { 1,  4,  7, -1, -1, -1, -1, -1},
    { 0,  1,  4,  7, -1, -1, -1, -1},
    { 2,  4,  7, -1, -1, -1, -1, -1},
    { 0,  2,  4,  7, -1, -1, -1, -1},
    { 1,  2,  4,  7, -1, -1, -1, -1},
    { 0,  1,  2,  4,  7, -1, -1, -1},
    { 3,  4,  7, -1, -1, -1, -1, -1},
    { 0,  3,  4,  7, -1, -1, -1, -1},
    { 1,  3,  4,  7, -1, -1, -1, -1},
    { 0,  1,  3,  4,  7, -1, -1, -1},
    { 2,  3,  4,  7, -1, -1, -1, -1},
    { 0,  2,  3,  4,  7, -1, -1, -1},
    { 1,  2,  3,  4,  7, -1, -1, -1},
    { 0,  1,  2,  3,  4,  7, -1, -1},
    { 5,  7, -1, -1, -1, -1, -1, -1},
    { 0,  5,  7, -1, -1, -1, -1, -1},
    { 1,  5,  7, -1, -1, -1, -1, -1},
    { 0,  1,  5,  7, -1, -1, -1, -1},
    { 2,  5,  7, -1, -1, -1, -1, -1},
    { 0,  2,  5,  7, -1, -1, -1, -1},
    { 1,  2,  5,  7, -1, -1, -1, -1},
    { 0,  1,  2,  5,  7, -1, -1, -1},
    { 3,  5,  7, -1, -1, -1, -1, -1},
    { 0,  3,  5,  7, -1, -1, -1, -1},
    { 1,  3,  5,  7, -1, -1, -1, -1},
    { 0,  1,  3,  5,  7, -1, -1, -1},
    { 2,  3,  5,  7, -1, -1, -1, -1},
    { 0,  2,  3,  5,  7, -1, -1, -1},
    { 1,  2,  3,  5,  7, -1, -1, -1},
    { 0,  1,  2,  3,  5,  7, -1, -1},
    { 4,  5,  7, -1, -1, -1, -1, -1},
    { 0,  4,  5,  7, -1, -1, -1, -1},
    { 1,  4,  5,  7, -1, -1, -1, -1},
    { 0,  1,  4,  5,  7, -1, -1, -1},
    { 2,  4,  5,  7, -1, -1, -1, -1},
    { 0,  2,  4,  5,  7, -1, -1, -1},
    { 1,  2,  4,  5,  7, -1, -1, -1},
    { 0,  1,  2,  4,  5,  7, -1, -1},
    { 3,  4,  5,  7, -1, -1, -1, -1},
    { 0,  3,  4,  5,  7, -1, -1, -1},
    { 1,  3,  4,  5,  7, -1, -1, -1},
    { 0,  1,  3,  4,  5,  7, -1, -1},
    { 2,  3,  4,  5,  7, -1, -1, -1},
    { 0,  2,  3,  4,  5,  7, -1, -1},
    { 1,  2,  3,  4,  5,  7, -1, -1},
    { 0,  1,  2,  3,  4,  5,  7, -1},
    { 6,  7, -1, -1, -1, -1, -1, -1},
    { 0,  6,  7, -1, -1, -1, -1, -1},
    { 1,  6,  7, -1, -1, -1, -1, -1},
    { 0,  1,  6,  7, -1, -1, -1, -1},
    { 2,  6,  7, -1, -1, -1, -1, -1},
    { 0,  2,  6,  7, -1, -1, -1, -1},
    { 1,  2,  6,  7, -1, -1, -1, -1},
    { 0,  1,  2,  6,  7, -1, -1, -1},
    { 3,  6,  7, -1, -1, -1, -1, -1},
    { 0,  3,  6,  7, -1, -1, -1, -1},
    { 1,  3,  6,  7, -1, -1, -1, -1},
    { 0,  1,  3,  6,  7, -1, -1, -1},
    { 2,  3,  6,  7, -1, -1, -1, -1},
    { 0,  2,  3,  6,  7, -1, -1, -1},
    { 1,  2,  3,  6,  7, -1, -1, -1},
    { 0,  1,  2,  3,  6,  7, -1, -1},
    { 4,  6,  7, -1, -1, -1, -1, -1},
    { 0,  4,  6,  7, -1, -1, -1, -1},
    { 1,  4,  6,  7, -1, -1, -1, -1},
    { 0,  1,  4,  6,  7, -1, -1, -1},
    { 2,  4,  6,  7, -1, -1, -1, -1},
    { 0,  2,  4,  6,  7, -1, -1, -1},
    { 1,  2,  4,  6,  7, -1, -1, -1},
    { 0,  1,  2,  4,  6,  7, -1, -1},
    { 3,  4,  6,  7, -1, -1, -1, -1},
    { 0,  3,  4,  6,  7, -1, -1, -1},
    { 1,  3,  4,  6,  7, -1, -1, -1},
    { 0,  1,  3,  4,  6,  7, -1, -1},
    { 2,  3,  4,  6,  7, -1, -1, -1},
    { 0,  2,  3,  4,  6,  7, -1, -1},
    { 1,  2,  3,  4,  6,  7, -1, -1},
    { 0,  1,  2,  3,  4,  6,  7, -1},
    { 5,  6,  7, -1, -1, -1, -1, -1},
    { 0,  5,  6,  7, -1, -1, -1, -1},
    { 1,  5,  6,  7, -1, -1, -1, -1},
    { 0,  1,  5,  6,  7, -1, -1, -1},
    { 2,  5,  6,  7, -1, -1, -1, -1},
    { 0,  2,  5,  6,  7, -1, -1, -1},
    { 1,  2,  5,  6,  7, -1, -1, -1},
    { 0,  1,  2,  5,  6,  7, -1, -1},
    { 3,  5,  6,  7, -1, -1, -1, -1},
    { 0,  3,  5,  6,  7, -1, -1, -1},
    { 1,  3,  5,  6,  7, -1, -1, -1},
    { 0,  1,  3,  5,  6,  7, -1, -1},
    { 2,  3,  5,  6,  7, -1, -1, -1},
    { 0,  2,  3,  5,  6,  7, -1, -1},
    { 1,  2,  3,  5,  6,  7, -1, -1},
    { 0,  1,  2,  3,  5,  6,  7, -1},
    { 4,  5,  6,  7, -1, -1, -1, -1},
    { 0,  4,  5,  6,  7, -1, -1, -1},
    { 1,  4,  5,  6,  7, -1, -1, -1},
    { 0,  1,  4,  5,  6,  7, -1, -1},
    { 2,  4,  5,  6,  7, -1, -1, -1},
    { 0,  2,  4,  5,  6,  7, -1, -1},
    { 1,  2,  4,  5,  6,  7, -1, -1},
    { 0,  1,  2,  4,  5,  6,  7, -1},
    { 3,  4,  5,  6,  7, -1, -1, -1},
    { 0,  3,  4,  5,  6,  7, -1, -1},
    { 1,  3,  4,  5,  6,  7, -1, -1},
    { 0,  1,  3,  4,  5,  6,  7, -1},
    { 2,  3,  4,  5,  6,  7, -1, -1},
    { 0,  2,  3,  4,  5,  6,  7, -1},
    { 1,  2,  3,  4,  5,  6,  7, -1},
    { 0,  1,  2,  3,  4,  5,  6,  7}
};

const signed char bitPos[256][8] = {
    {-1, -1, -1, -1, -1, -1, -1, -1},
    { 0, -1, -1, -1, -1, -1, -1, -1},
    {-1,  0, -1, -1, -1, -1, -1, -1},
    { 0,  1, -1, -1, -1, -1, -1, -1},
    {-1, -1,  0, -1, -1, -1, -1, -1},
    { 0, -1,  1, -1, -1, -1, -1, -1},
    {-1,  0,  1, -1, -1, -1, -1, -1},
    { 0,  1,  2, -1, -1, -1, -1, -1},
    {-1, -1, -1,  0, -1, -1, -1, -1},
    { 0, -1, -1,  1, -1, -1, -1, -1},
    {-1,  0, -1,  1, -1, -1, -1, -1},
    { 0,  1, -1,  2, -1, -1, -1, -1},
    {-1, -1,  0,  1, -1, -1, -1, -1},
    { 0, -1,  1,  2, -1, -1, -1, -1},
    {-1,  0,  1,  2, -1, -1, -1, -1},
    { 0,  1,  2,  3, -1, -1, -1, -1},
    {-1, -1, -1, -1,  0, -1, -1, -1},
    { 0, -1, -1, -1,  1, -1, -1, -1},
    {-1,  0, -1, -1,  1, -1, -1, -1},
    { 0,  1, -1, -1,  2, -1, -1, -1},
    {-1, -1,  0, -1,  1, -1, -1, -1},
    { 0, -1,  1, -1,  2, -1, -1, -1},
    {-1,  0,  1, -1,  2, -1, -1, -1},
    { 0,  1,  2, -1,  3, -1, -1, -1},
    {-1, -1, -1,  0,  1, -1, -1, -1},
    { 0, -1, -1,  1,  2, -1, -1, -1},
    {-1,  0, -1,  1,  2, -1, -1, -1},
    { 0,  1, -1,  2,  3, -1, -1, -1},
    {-1, -1,  0,  1,  2, -1, -1, -1},
    { 0, -1,  1,  2,  3, -1, -1, -1},
    {-1,  0,  1,  2,  3, -1, -1, -1},
    { 0,  1,  2,  3,  4, -1, -1, -1},
    {-1, -1, -1, -1, -1,  0, -1, -1},
    { 0, -1, -1, -1, -1,  1, -1, -1},
    {-1,  0, -1, -1, -1,  1, -1, -1},
    { 0,  1, -1, -1, -1,  2, -1, -1},
    {-1, -1,  0, -1, -1,  1, -1, -1},
    { 0, -1,  1, -1, -1,  2, -1, -1},
    {-1,  0,  1, -1, -1,  2, -1, -1},
    { 0,  1,  2, -1, -1,  3, -1, -1},
    {-1, -1, -1,  0, -1,  1, -1, -1},
    { 0, -1, -1,  1, -1,  2, -1, -1},
    {-1,  0, -1,  1, -1,  2, -1, -1},
    { 0,  1, -1,  2, -1,  3, -1, -1},
    {-1, -1,  0,  1, -1,  2, -1, -1},
    { 0, -1,  1,  2, -1,  3, -1, -1},
    {-1,  0,  1,  2, -1,  3, -1, -1},
    { 0,  1,  2,  3, -1,  4, -1, -1},
    {-1, -1, -1, -1,  0,  1, -1, -1},
    { 0, -1, -1, -1,  1,  2, -1, -1},
    {-1,  0, -1, -1,  1,  2, -1, -1},
    { 0,  1, -1, -1,  2,  3, -1, -1},
    {-1, -1,  0, -1,  1,  2, -1, -1},
    { 0, -1,  1, -1,  2,  3, -1, -1},
    {-1,  0,  1, -1,  2,  3, -1, -1},
    { 0,  1,  2, -1,  3,  4, -1, -1},
    {-1, -1, -1,  0,  1,  2, -1, -1},
    { 0, -1, -1,  1,  2,  3, -1, -1},
    {-1,  0, -1,  1,  2,  3, -1, -1},
    { 0,  1, -1,  2,  3,  4, -1, -1},
    {-1, -1,  0,  1,  2,  3, -1, -1},
    { 0, -1,  1,  2,  3,  4, -1, -1},
    {-1,  0,  1,  2,  3,  4, -1, -1},
    { 0,  1,  2,  3,  4,  5, -1, -1},
    {-1, -1, -1, -1, -1, -1,  0, -1},
    { 0, -1, -1, -1, -1, -1,  1, -1},
    {-1,  0, -1, -1, -1, -1,  1, -1},
    { 0,  1, -1, -1, -1, -1,  2, -1},
    {-1, -1,  0, -1, -1, -1,  1, -1},
    { 0, -1,  1, -1, -1, -1,  2, -1},
    {-1,  0,  1, -1, -1, -1,  2, -1},
    { 0,  1,  2, -1, -1, -1,  3, -1},
    {-1, -1, -1,  0, -1, -1,  1, -1},
    { 0, -1, -1,  1, -1, -1,  2, -1},
    {-1,  0, -1,  1, -1, -1,  2, -1},
    { 0,  1, -1,  2, -1, -1,  3, -1},
    {-1, -1,  0,  1, -1, -1,  2, -1},
    { 0, -1,  1,  2, -1, -1,  3, -1},
    {-1,  0,  1,  2, -1, -1,  3, -1},
    { 0,  1,  2,  3, -1, -1,  4, -1},
    {-1, -1, -1, -1,  0, -1,  1, -1},
    { 0, -1, -1, -1,  1, -1,  2, -1},
    {-1,  0, -1, -1,  1, -1,  2, -1},
    { 0,  1, -1, -1,  2, -1,  3, -1},
    {-1, -1,  0, -1,  1, -1,  2, -1},
    { 0, -1,  1, -1,  2, -1,  3, -1},
    {-1,  0,  1, -1,  2, -1,  3, -1},
    { 0,  1,  2, -1,  3, -1,  4, -1},
    {-1, -1, -1,  0,  1, -1,  2, -1},
    { 0, -1, -1,  1,  2, -1,  3, -1},
    {-1,  0, -1,  1,  2, -1,  3, -1},
    { 0,  1, -1,  2,  3, -1,  4, -1},
    {-1, -1,  0,  1,  2, -1,  3, -1},
    { 0, -1,  1,  2,  3, -1,  4, -1},
    {-1,  0,  1,  2,  3, -1,  4, -1},
    { 0,  1,  2,  3,  4, -1,  5, -1},
    {-1, -1, -1, -1, -1,  0,  1, -1},
    { 0, -1, -1, -1, -1,  1,  2, -1},
    {-1,  0, -1, -1, -1,  1,  2, -1},
    { 0,  1, -1, -1, -1,  2,  3, -1},
    {-1, -1,  0, -1, -1,  1,  2, -1},
    { 0, -1,  1, -1, -1,  2,  3, -1},
    {-1,  0,  1, -1, -1,  2,  3, -1},
    { 0,  1,  2, -1, -1,  3,  4, -1},
    {-1, -1, -1,  0, -1,  1,  2, -1},
    { 0, -1, -1,  1, -1,  2,  3, -1},
    {-1,  0, -1,  1, -1,  2,  3, -1},
    { 0,  1, -1,  2, -1,  3,  4, -1},
    {-1, -1,  0,  1, -1,  2,  3, -1},
    { 0, -1,  1,  2, -1,  3,  4, -1},
    {-1,  0,  1,  2, -1,  3,  4, -1},
    { 0,  1,  2,  3, -1,  4,  5, -1},
    {-1, -1, -1, -1,  0,  1,  2, -1},
    { 0, -1, -1, -1,  1,  2,  3, -1},
    {-1,  0, -1, -1,  1,  2,  3, -1},
    { 0,  1, -1, -1,  2,  3,  4, -1},
    {-1, -1,  0, -1,  1,  2,  3, -1},
    { 0, -1,  1, -1,  2,  3,  4, -1},
    {-1,  0,  1, -1,  2,  3,  4, -1},
    { 0,  1,  2, -1,  3,  4,  5, -1},
    {-1, -1, -1,  0,  1,  2,  3, -1},
    { 0, -1, -1,  1,  2,  3,  4, -1},
    {-1,  0, -1,  1,  2,  3,  4, -1},
    { 0,  1, -1,  2,  3,  4,  5, -1},
    {-1, -1,  0,  1,  2,  3,  4, -1},
    { 0, -1,  1,  2,  3,  4,  5, -1},
    {-1,  0,  1,  2,  3,  4,  5, -1},
    { 0,  1,  2,  3,  4,  5,  6, -1},
    {-1, -1, -1, -1, -1, -1, -1,  0},
    { 0, -1, -1, -1, -1, -1, -1,  1},
    {-1,  0, -1, -1, -1, -1, -1,  1},
    { 0,  1, -1, -1, -1, -1, -1,  2},
    {-1, -1,  0, -1, -1, -1, -1,  1},
    { 0, -1,  1, -1, -1, -1, -1,  2},
    {-1,  0,  1, -1, -1, -1, -1,  2},
    { 0,  1,  2, -1, -1, -1, -1,  3},
    {-1, -1, -1,  0, -1, -1, -1,  1},
    { 0, -1, -1,  1, -1, -1, -1,  2},
    {-1,  0, -1,  1, -1, -1, -1,  2},
    { 0,  1, -1,  2, -1, -1, -1,  3},
    {-1, -1,  0,  1, -1, -1, -1,  2},
    { 0, -1,  1,  2, -1, -1, -1,  3},
    {-1,  0,  1,  2, -1, -1, -1,  3},
    { 0,  1,  2,  3, -1, -1, -1,  4},
    {-1, -1, -1, -1,  0, -1, -1,  1},
    { 0, -1, -1, -1,  1, -1, -1,  2},
    {-1,  0, -1, -1,  1, -1, -1,  2},
    { 0,  1, -1, -1,  2, -1, -1,  3},
    {-1, -1,  0, -1,  1, -1, -1,  2},
    { 0, -1,  1, -1,  2, -1, -1,  3},
    {-1,  0,  1, -1,  2, -1, -1,  3},
    { 0,  1,  2, -1,  3, -1, -1,  4},
    {-1, -1, -1,  0,  1, -1, -1,  2},
    { 0, -1, -1,  1,  2, -1, -1,  3},
    {-1,  0, -1,  1,  2, -1, -1,  3},
    { 0,  1, -1,  2,  3, -1, -1,  4},
    {-1, -1,  0,  1,  2, -1, -1,  3},
    { 0, -1,  1,  2,  3, -1, -1,  4},
    {-1,  0,  1,  2,  3, -1, -1,  4},
    { 0,  1,  2,  3,  4, -1, -1,  5},
    {-1, -1, -1, -1, -1,  0, -1,  1},
    { 0, -1, -1, -1, -1,  1, -1,  2},
    {-1,  0, -1, -1, -1,  1, -1,  2},
    { 0,  1, -1, -1, -1,  2, -1,  3},
    {-1, -1,  0, -1, -1,  1, -1,  2},
    { 0, -1,  1, -1, -1,  2, -1,  3},
    {-1,  0,  1, -1, -1,  2, -1,  3},
    { 0,  1,  2, -1, -1,  3, -1,  4},
    {-1, -1, -1,  0, -1,  1, -1,  2},
    { 0, -1, -1,  1, -1,  2, -1,  3},
    {-1,  0, -1,  1, -1,  2, -1,  3},
    { 0,  1, -1,  2, -1,  3, -1,  4},
    {-1, -1,  0,  1, -1,  2, -1,  3},
    { 0, -1,  1,  2, -1,  3, -1,  4},
    {-1,  0,  1,  2, -1,  3, -1,  4},
    { 0,  1,  2,  3, -1,  4, -1,  5},
    {-1, -1, -1, -1,  0,  1, -1,  2},
    { 0, -1, -1, -1,  1,  2, -1,  3},
    {-1,  0, -1, -1,  1,  2, -1,  3},
    { 0,  1, -1, -1,  2,  3, -1,  4},
    {-1, -1,  0, -1,  1,  2, -1,  3},
    { 0, -1,  1, -1,  2,  3, -1,  4},
    {-1,  0,  1, -1,  2,  3, -1,  4},
    { 0,  1,  2, -1,  3,  4, -1,  5},
    {-1, -1, -1,  0,  1,  2, -1,  3},
    { 0, -1, -1,  1,  2,  3, -1,  4},
    {-1,  0, -1,  1,  2,  3, -1,  4},
    { 0,  1, -1,  2,  3,  4, -1,  5},
    {-1, -1,  0,  1,  2,  3, -1,  4},
    { 0, -1,  1,  2,  3,  4, -1,  5},
    {-1,  0,  1,  2,  3,  4, -1,  5},
    { 0,  1,  2,  3,  4,  5, -1,  6},
    {-1, -1, -1, -1, -1, -1,  0,  1},
    { 0, -1, -1, -1, -1, -1,  1,  2},
    {-1,  0, -1, -1, -1, -1,  1,  2},
    { 0,  1, -1, -1, -1, -1,  2,  3},
    {-1, -1,  0, -1, -1, -1,  1,  2},
    { 0, -1,  1, -1, -1, -1,  2,  3},
    {-1,  0,  1, -1, -1, -1,  2,  3},
    { 0,  1,  2, -1, -1, -1,  3,  4},
    {-1, -1, -1,  0, -1, -1,  1,  2},
    { 0, -1, -1,  1, -1, -1,  2,  3},
    {-1,  0, -1,  1, -1, -1,  2,  3},
    { 0,  1, -1,  2, -1, -1,  3,  4},
    {-1, -1,  0,  1, -1, -1,  2,  3},
    { 0, -1,  1,  2, -1, -1,  3,  4},
    {-1,  0,  1,  2, -1, -1,  3,  4},
    { 0,  1,  2,  3, -1, -1,  4,  5},
    {-1, -1, -1, -1,  0, -1,  1,  2},
    { 0, -1, -1, -1,  1, -1,  2,  3},
    {-1,  0, -1, -1,  1, -1,  2,  3},
    { 0,  1, -1, -1,  2, -1,  3,  4},
    {-1, -1,  0, -1,  1, -1,  2,  3},
    { 0, -1,  1, -1,  2, -1,  3,  4},
    {-1,  0,  1, -1,  2, -1,  3,  4},
    { 0,  1,  2, -1,  3, -1,  4,  5},
    {-1, -1, -1,  0,  1, -1,  2,  3},
    { 0, -1, -1,  1,  2, -1,  3,  4},
    {-1,  0, -1,  1,  2, -1,  3,  4},
    { 0,  1, -1,  2,  3, -1,  4,  5},
    {-1, -1,  0,  1,  2, -1,  3,  4},
    { 0, -1,  1,  2,  3, -1,  4,  5},
    {-1,  0,  1,  2,  3, -1,  4,  5},
    { 0,  1,  2,  3,  4, -1,  5,  6},
    {-1, -1, -1, -1, -1,  0,  1,  2},
    { 0, -1, -1, -1, -1,  1,  2,  3},
    {-1,  0, -1, -1, -1,  1,  2,  3},
    { 0,  1, -1, -1, -1,  2,  3,  4},
    {-1, -1,  0, -1, -1,  1,  2,  3},
    { 0, -1,  1, -1, -1,  2,  3,  4},
    {-1,  0,  1, -1, -1,  2,  3,  4},
    { 0,  1,  2, -1, -1,  3,  4,  5},
    {-1, -1, -1,  0, -1,  1,  2,  3},
    { 0, -1, -1,  1, -1,  2,  3,  4},
    {-1,  0, -1,  1, -1,  2,  3,  4},
    { 0,  1, -1,  2, -1,  3,  4,  5},
    {-1, -1,  0,  1, -1,  2,  3,  4},
    { 0, -1,  1,  2, -1,  3,  4,  5},
    {-1,  0,  1,  2, -1,  3,  4,  5},
    { 0,  1,  2,  3, -1,  4,  5,  6},
    {-1, -1, -1, -1,  0,  1,  2,  3},
    { 0, -1, -1, -1,  1,  2,  3,  4},
    {-1,  0, -1, -1,  1,  2,  3,  4},
    { 0,  1, -1, -1,  2,  3,  4,  5},
    {-1, -1,  0, -1,  1,  2,  3,  4},
    { 0, -1,  1, -1,  2,  3,  4,  5},
    {-1,  0,  1, -1,  2,  3,  4,  5},
    { 0,  1,  2, -1,  3,  4,  5,  6},
    {-1, -1, -1,  0,  1,  2,  3,  4},
    { 0, -1, -1,  1,  2,  3,  4,  5},
    {-1,  0, -1,  1,  2,  3,  4,  5},
    { 0,  1, -1,  2,  3,  4,  5,  6},
    {-1, -1,  0,  1,  2,  3,  4,  5},
    { 0, -1,  1,  2,  3,  4,  5,  6},
    {-1,  0,  1,  2,  3,  4,  5,  6},
    { 0,  1,  2,  3,  4,  5,  6,  7}
};

