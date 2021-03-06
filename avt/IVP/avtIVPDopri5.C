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
//                              avtIVPDopri5.C                               //
// ************************************************************************* //

#include <avtIVPDopri5.h>
#include <avtIVPStateHelper.h>
#include <DebugStream.h>

#include <algorithm>
#include <limits>
#include <cmath>
#include <float.h>

// some constants for the Dormand-Prince RK scheme
static const double safe = 0.9;
static const double epsilon = std::numeric_limits<double>::epsilon();
static const double facl = 0.2;
static const double facr = 10.0;
static const double beta = 0.04;
static const unsigned int nstiff = 100;

static const double a21=0.2, a31=3.0/40.0, a32=9.0/40.0, a41=44.0/45.0,
                    a42=-56.0/15.0, a43=32.0/9.0, a51=19372.0/6561.0, 
                    a52=-25360.0/2187.0,
                    a53=64448.0/6561.0, a54=-212.0/729.0, a61=9017.0/3168.0, 
                    a62=-355.0/33.0, a63=46732.0/5247.0, a64=49.0/176.0, 
                    a65=-5103.0/18656.0,
                    a71=35.0/384.0, a73=500.0/1113.0, a74=125.0/192.0, 
                    a75=-2187.0/6784.0, a76=11.0/84.0;

static const double c2=0.2, c3=0.3, c4=0.8, c5=8.0/9.0;

static const double d1=-12715105075.0/11282082432.0,
                    d3=87487479700.0/32700410799.0,
                    d4=-10690763975.0/1880347072.0, 
                    d5=701980252875.0/199316789632.0,
                    d6=-1453857185.0/822651844.0, d7=69997945.0/29380423.0;

static const double e1=71.0/57600.0, e3=-71.0/16695.0, e4=71.0/1920.0, 
                    e5=-17253.0/339200.0, e6=22.0/525.0, e7=-1.0/40.0;

// helper function
// returns a with the same sign as b
static inline double sign( const double& a, const double& b )
{
    return (b > 0.0) ? std::abs(a) : -std::abs(a);
}


// ****************************************************************************
//  Method: avtIVPDopri5 constructor
//
//  Programmer: Christoph Garth
//  Creation:   February 25, 2008
//
//  Modifications:
//
//    Dave Pugmire, Tue Aug 19, 17:38:03 EDT 2008
//    Chagned how distanced based termination is computed.
//
// ****************************************************************************

avtIVPDopri5::avtIVPDopri5()
{
     // set (somewhat) reasonable defaults
     reltol = 1e-8;
     abstol = 1e-6;
     
     h_max = 0.0;
     nonsti = 0;
}


// ****************************************************************************
//  Method: avtIVPDopri5 constructor
//
//  Programmer: Christoph Garth
//  Creation:   February 25, 2008
//
//  Modifications:
//
//    Dave Pugmire, Tue Aug 19, 17:38:03 EDT 2008
//    Chagned how distanced based termination is computed.
//
//    Dave Pugmire, Tue Dec  1 11:50:18 EST 2009
//    Switch from avtVec to avtVector.
//
// ****************************************************************************

avtIVPDopri5::avtIVPDopri5( const double& t_start, const avtVector& y_start )
{
    // set (somewhat) reasonable defaults
    reltol = 1e-8;
    abstol = 1e-6;
    h_max = 0.0;

    Reset(t_start, y_start);
    nonsti = 0;
}


// ****************************************************************************
//  Method: avtIVPDopri5 destructor
//
//  Programmer: Christoph Garth
//  Creation:   February 25, 2008
//
// ****************************************************************************

avtIVPDopri5::~avtIVPDopri5()
{
    //debug1 << "delete avtIVPDopri5\n";
}


// ****************************************************************************
//  Method: avtIVPDopri5::Reset
//
//  Purpose:
//      Resets data members.  Called by the constructors.
//
//  Programmer: Christoph Garth
//  Creation:   February 25, 2008
//
//  Modifications:
//    Dave Pugmire, Mon Feb 23, 09:11:34 EST 2009
//    Reworked the termination code. Added a type enum and value. Made num steps
//    a termination criterion.
//
//    Dave Pugmire, Tue Dec  1 11:50:18 EST 2009
//    Switch from avtVec to avtVector.
//  
// ****************************************************************************

void 
avtIVPDopri5::Reset(const double& t_start,
                    const avtVector& y_start,
                    const avtVector& v_start)
{
    h = h_init = 0.0;
    
    n_accepted = n_rejected = n_steps = n_eval = 0;
    
    facold = 1e-4;
    hlamb  = 0.0;
    iasti  = 0;
    
    t = t_start;
    numStep = 0;
    yCur = y_start;
    k1 = avtVector(0,0,0);
}


// ****************************************************************************
//  Method: avtIVPDopri5::SetTolerances
//
//  Purpose:
//      Sets the relative and absolute tolerances.
//
//  Programmer: Christoph Garth
//  Creation:   February 25, 2008
//
// ****************************************************************************

void
avtIVPDopri5::SetTolerances(const double& relt, const double& abst)
{
    reltol = std::abs(relt);
    abstol = std::max(std::abs(abst), epsilon);
}


// ****************************************************************************
//  Method: avtIVPDopri5::GuessInitialStep
//
//  Purpose:
//      Guesses the initial step size.
//
//  Programmer: Christoph Garth
//  Creation:   February 25, 2008
//
//  Modifications:
//    Kathleen Bonnell, Thu Aug  7, 08:29:42 PDT 2008
//    Changed for loops to use size_t to eliminate signed/unsigned int 
//    comparison warnings.
//
//    Dave Pugmire, Tue Aug 19, 17:38:03 EDT 2008
//    Consider a negative stepsize.
//
//    Hank Childs, Thu Apr  2 16:40:08 PDT 2009
//    Incorporate fix from Christoph to do a better job guessing the
//    initial step size.
//
//    Dave Pugmire, Tue Dec  1 11:50:18 EST 2009
//    Switch from avtVec to avtVector.
//
// ****************************************************************************

double 
avtIVPDopri5::GuessInitialStep(const avtIVPField* field, 
                               const double& h_max, 
                               const double& t_max)
{
    double t_local = GetLocalTime();

    // make a local copy since we may need to modify it
    double local_h_max = h_max;
    double direction = sign(1.0, t_max - t_local);
        
    // loop until an estimate succeeds
    while( true )
    {
        double sk, sqr;
        double dnf = 0.0;
        double dny = 0.0;

        double h;

        for(size_t i=0 ; i < 3; i++) 
        {
            sk = abstol + reltol * std::abs(yCur[i]);
            sqr = k1[i] / sk;
            dnf += sqr * sqr;
            sqr = yCur[i] / sk;
            dny += sqr * sqr;
        }

        if( (dnf <= 1.0e-10) || (dny <= 1.0e-10) ) 
            h = 1.0e-6;
        else 
            h = sqrt( dny/dnf ) * 0.01;

        h = std::min( h, local_h_max );
        h = sign( h, direction );

        // perform an explicit Euler step
        avtVector k2, k3 = yCur + h * k1;
        if ((*field)(t_local+h, k3, k2) != avtIVPField::OK)
        {
            // Somehow we couldn't evaluate one of the points we need for the
            // starting estimate. The above code adheres to the h_max that is
            // passed in - let's reduce that and try again.
            // (we're really using local_h_max since h_max is const double&)

            // (Oh, and if local_h_max is zero or infinite, let's set it to unit
            // length (= direction))
            if( local_h_max == 0.0 ||
                local_h_max == std::numeric_limits<double>::infinity() )
                local_h_max = direction;
            else
                local_h_max /= 2;

            // if local_h_max is smaller then epsilon, we stop trying
            // return that back to Step() which will then fail with a
            // stepsize underflow
            if( local_h_max < std::numeric_limits<double>::epsilon() )
                return local_h_max;

            continue;
        }


        n_eval++;

        // estimate the second derivative of the solution
        double der2 = 0.0;

        for( size_t i=0; i < 3; i++) 
        {
            sk = abstol + reltol * std::abs( yCur[i] );
            sqr = ( k2[i] - k1[i] ) / sk;
            der2 += sqr*sqr;
        }

        der2 = sqrt( der2 ) / h;

        // step size is computed such that
        // h**(1/5) * max( norm(k1), norm(der2) ) = 0.01
        double der12 = std::max( std::abs(der2), sqrt(dnf) );
        double h1;

        if( der12 <= 1.0e-15 ) 
            h1 = std::max( 1.0e-6, std::abs(h)*1.0e-3 );
        else 
            h1 = pow( 0.01/der12, 0.2 );

        h = std::min( fabs(100.0*h), std::min( h1, local_h_max ) );
        h = sign( h, direction );

        return h;
    }
}


// ****************************************************************************
//  Method: avtIVPDopri5::Step
//
//  Purpose:
//      Take a step and return the result.
//
//  Programmer: Christoph Garth
//  Creation:   February 25, 2008
//
//  Modifications:
//    Kathleen Bonnell, Thu Aug  7, 08:29:42 PDT 2008
//    Changed for loops to use size_t to eliminate signed/unsigned int 
//    comparison warnings.
//
//    Dave Pugmire, Wed Aug 13 10:58:32 EDT 2008
//    Store the velocity with each step.
//
//    Dave Pugmire, Tue Aug 19, 17:38:03 EDT 2008
//    Consider a negative stepsize. Change how distance termination is computed.
//
//    Dave Pugmire, Mon Feb 23, 09:11:34 EST 2009
//    Reworked the termination code. Added a type enum and value. Made num steps
//    a termination criterion.    
//
//    Hank Childs, Fri Apr 10 23:31:22 CDT 2009
//    Put if statements in front of debug's.  The generation of strings to
//    output to debug was doubling the total integration time.
//
//    Mark C. Miller, Wed Apr 22 13:48:13 PDT 2009
//    Changed interface to DebugStream to obtain current debug level.
//
//    Dave Pugmire, Tue Aug 11 10:25:45 EDT 2009
//    Add new termination criterion: Number of intersections with an object.
//
//    Dave Pugmire, Tue Dec  1 11:50:18 EST 2009
//    Switch from avtVec to avtVector.
//
//    Dave Pugmire, Tue Feb 23 09:42:25 EST 2010
//    Set the velStart/velEnd direction based on integration direction.
//
//    Hank Childs, David Camp, and Christoph Garth, Fri Jul 23 14:31:57 PDT 2010
//    Make sure we don't exceed the max step size.
//
//    Hank Childs and Christoph Garth, Thu Oct 21 14:34:37 PDT 2010
//    Fix problem where stepsize can creep to infinity if no max step size is
//    set.
//
//    David Camp, Mon Mar  5 09:48:43 PST 2012
//    Changed optimation, reduce the number of compares and memory copies.
//
// ****************************************************************************

avtIVPSolver::Result 
avtIVPDopri5::Step(avtIVPField* field, double t_max,
                   avtIVPStep* ivpstep) 
{    
    double t_local = GetLocalTime();

    const double direction = sign( 1.0, t_max - t_local );
    avtIVPField::Result fieldResult;

    // compute maximum stepsize
    double local_h_max = h_max;

    if( local_h_max == 0.0 )
        local_h_max = std::abs( t_max - t_local );

    // compute k1 to ensure first-same-as-last principle, 
    // maybe also needed for hinit())
    if( n_steps == 0 )
    {
        if ((fieldResult = (*field)( t_local, yCur, k1 )) != avtIVPField::OK)
            return ConvertResult(fieldResult);
        n_eval++;
    }

    // determine stepsize (user-specified or educated guess)
    if( h == 0.0 )
    {
        if( h_init == 0.0 )
        {
            h = GuessInitialStep( field, local_h_max, t_max );
        }
        else
        {
            h = h_init;
        }
    }
    else
    {   
        h = sign( h, direction );
    }

    bool reject = false;
   
    // integration step loop, will exit after successful step
    while( true )
    {
        bool last = false;
        avtVector y_new, y_stiff;

        // stepsize underflow?
        if( 0.1*std::abs(h) <= std::abs(t)*epsilon ) 
        {
            if (DebugStream::Level5())
            {
                debug5 << "\tavtIVPDopri5::Step(): exiting at t = " 
                       << t << ", step size too small (h = " << h << ")\n";
            }
            return avtIVPSolver::STEPSIZE_UNDERFLOW;
        }

        // Check to make sure we don't exceed the max step.
        if( h_max != 0.0 && std::abs(h) > std::abs(h_max) )
            h = sign( h_max, h );

        // do not run past integration end
        if( (t_local + 1.01*h - t_max) * direction > 0.0 ) 
        {
            last = true;
            h = t_max - t_local;
        }

        n_steps++;

        if (DebugStream::Level5())
        {
            debug5 << "\tavtIVPDopri5::Step(): t = " << t << ", y = " << yCur 
                   << ", h = " << h << ", t+h = " << t+h << '\n';
        }

        avtVector k2, k3, k4, k5, k6, k7;

        // perform stages
        y_new = yCur + h*a21*k1;
        if ((fieldResult = (*field)( t_local+c2*h, y_new, k2 )) != avtIVPField::OK)
            return ConvertResult(fieldResult);

        y_new = yCur + h * ( a31*k1 + a32*k2 );
        if ((fieldResult = (*field)( t_local+c3*h, y_new, k3 )) != avtIVPField::OK)
            return ConvertResult(fieldResult);
        
        y_new = yCur + h * ( a41*k1 + a42*k2 + a43*k3 );
        if ((fieldResult = (*field)( t_local+c4*h, y_new, k4 )) != avtIVPField::OK)
            return ConvertResult(fieldResult);
        
        y_new = yCur + h * ( a51*k1 + a52*k2 + a53*k3 + a54*k4 );
        if ((fieldResult = (*field)( t_local+c5*h, y_new, k5 )) != avtIVPField::OK)
            return ConvertResult(fieldResult);

        y_stiff = y_new = yCur + h * (a61*k1 + a62*k2 + a63*k3 + a64*k4 + a65*k5);
        if ((fieldResult = (*field)( t_local+h, y_new, k6 )) != avtIVPField::OK)
            return ConvertResult(fieldResult);
        
        y_new = yCur + h * (a71*k1 + a73*k3 + a74*k4 + a75*k5 + a76*k6 );
        if ((fieldResult = (*field)( t_local+h, y_new, k7 )) != avtIVPField::OK)
            return ConvertResult(fieldResult);

        n_eval += 6;

        double err = 0.0, h_new, fac11;

        // error estimation
        avtVector ee = h * ( e1*k1 + e3*k3 + e4*k4 + e5*k5 + e6*k6 + e7*k7 );
        double sk, sqr;
            
        for( size_t i=0; i<3; i++ ) 
        {
            sk = abstol + reltol * std::max(std::abs(yCur[i]), std::abs(y_new[i]));
            sqr = ee[i]/sk;
            err += sqr*sqr;
        }
            
        err = sqrt(err / 3.0);
            
        // compute next potential stepsize
        fac11 = pow( err, 0.2 - beta*0.75 );
            
        // Lund-stabilization
        double fac = fac11 / pow( facold, beta );
            
        // we require facl <= h_new/h <= facr
        fac = std::max( 1.0/facr, std::min( 1.0/facl, fac/safe ) );
            
        h_new = h / fac;

        if( h_new > std::numeric_limits<double>::max() )
            h_new = std::numeric_limits<double>::max();
        
        if( h_new < -std::numeric_limits<double>::max() )
            h_new = std::numeric_limits<double>::max();

        if( err <= 1.0 ) 
        {
            // step accepted
            facold = std::max( err, 1.0e-4 );
            n_accepted++;

            // stiffness detection
            if( !(n_accepted % nstiff) || (iasti > 0) ) 
            {
                double stnum = 0.0, stden = 0.0, sqr;

                for( size_t i=0; i < 3; i++ ) 
                {
                    sqr = k7[i] - k6[i];
                    stnum += sqr * sqr;
                    sqr = y_new[i] - y_stiff[i];
                    stden += sqr * sqr;
                }
                
                if( stden > 0.0 ) 
                    hlamb = h * sqrt( stnum/stden );
            
                if( fabs(hlamb) > 3.25 ) 
                {
                    nonsti = 0;
                    iasti++;
                
                    if( iasti == 15 ) 
                    {
                        if (DebugStream::Level5()) 
                        {
                            debug5 << "\tavtIVPDopri5::Step(): exiting at t = " 
                                   << t << ", problem seems stiff (y = " << yCur
                                   << ")\n";
                        }
                        return avtIVPSolver::STIFFNESS_DETECTED;
                    }
                }
                else 
                {
                    nonsti++;
                    if( nonsti == 6 ) 
                        iasti = 0;
                }
            }
                
            // --- step looks ok - prepare for return
            if( reject ) 
                h_new = direction * std::min( std::abs(h_new), std::abs(h) );

            // fill in step
            // make interpolation polynomial
            if( ivpstep )
            {
              // Convert and save the position.
                ivpstep->resize(5);

                if( convertToCartesian )
                {
                  (*ivpstep)[0] = field->ConvertToCartesian( yCur );
                  (*ivpstep)[1] = field->ConvertToCartesian( yCur + (h*k1/4.) );
                  (*ivpstep)[2] = field->ConvertToCartesian( (yCur + y_new)/2 +
                                                          h*( (d1+1)*k1 +
                                                              d3*k3 + d4*k4 +
                                                              d5*k5 + d6*k6 +
                                                              (d7-1)*k7 )/6. );
                  (*ivpstep)[3] = field->ConvertToCartesian( y_new - h*k7/4 );
                  (*ivpstep)[4] = field->ConvertToCartesian( y_new );
                }
                else
                {
                  (*ivpstep)[0] = yCur;
                  (*ivpstep)[1] = yCur + (h*k1/4.);
                  (*ivpstep)[2] = (yCur + y_new)/2 + h*( (d1+1)*k1 + d3*k3 + d4*k4 
                                                      + d5*k5 + d6*k6 + (d7-1)*k7 )/6.;
                  (*ivpstep)[3] = y_new - h*k7/4;
                  (*ivpstep)[4] = y_new;
                }

                ivpstep->t0 = t;
                ivpstep->t1 = t + h;
            }
            
            // update internal state
            // first-same-as-last for k1
            k1 = k7;

            // Update for the next step.
            numStep++;

            yCur = y_new;
            t = t+h;

            if( period && last )
              t += FLT_EPSILON;

            // Set the step size on sucessful step.
            h = h_new;

            return last ? avtIVPSolver::TERMINATE : avtIVPSolver::OK;
        }
        else 
        {
            // step rejected
            h_new = h / std::min( 1.0/facl, fac11/safe );
            reject = true;
            
            if( n_accepted >= 1 ) 
                n_rejected++;

            // Update the step size to the new step size.
            h = h_new;
            
            if (DebugStream::Level5())
            {
                debug5 << "\tavtIVPDopri5::Step(): step rejected, retry with h = "
                       << h << '\n';
            }
        }
    }
}


// ****************************************************************************
//  Method: avtIVPDopri5::AcceptStateVisitor
//
//  Purpose:
//      Loads the state into the state helper.
//
//  Programmer: Christoph Garth
//  Creation:   February 25, 2008
//
// ****************************************************************************

void
avtIVPDopri5::AcceptStateVisitor(avtIVPStateHelper& aiss)
{
    aiss.Accept(numStep)
        .Accept(reltol)
        .Accept(abstol)
        .Accept(h)
        .Accept(h_max)
        .Accept(h_init)
        .Accept(t)
        .Accept(facold)
        .Accept(hlamb)
        .Accept(n_accepted)
        .Accept(n_rejected)
        .Accept(n_steps)
        .Accept(n_eval)
        .Accept(iasti)
        .Accept(nonsti)
        .Accept(yCur)
        .Accept(k1);
}
