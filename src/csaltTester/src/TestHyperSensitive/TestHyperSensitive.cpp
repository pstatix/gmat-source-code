//------------------------------------------------------------------------------
//                         TestHyperSensitive
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Author: Wendy Shoan
// Created: 2016.09.21
//
/**
 * Test driver for HyperSensitive optimization problem, etc. classes.
 */
//------------------------------------------------------------------------------
// global igrid iGfun jGvar traj
#include <iostream>
#include <string>
#include <ctime>
#include "gmatdefs.hpp"
#include "GmatConstants.hpp"
#include "Linear.hpp"
#include "Rvector.hpp"
#include "Rvector3.hpp"
#include "Rmatrix.hpp"
#include "Rmatrix33.hpp"
//#include "TestOutput.hpp"
#include "MessageInterface.hpp"
#include "ConsoleMessageReceiver.hpp"
#include "FileManager.hpp"
#include "TimeTypes.hpp"
#include "FunctionOutputData.hpp"
#include "PathFunctionContainer.hpp"
#include "PointFunctionContainer.hpp"
#include "UserFunctionManager.hpp"
#include "UserPointFunctionManager.hpp"
#include "UserFunctionProperties.hpp"
#include "ImplicitRKPhase.hpp"
#include "RadauPhase.hpp"
#include "Trajectory.hpp"
//#include "Phase.hpp"
#include "HyperSensitivePointObject.hpp"
#include "HyperSensitivePathObject.hpp"
#include "boost/config.hpp"  // BOOST


static const Real INF = std::numeric_limits<Real>::infinity();

using namespace std;
using namespace GmatMathConstants;

Real GetMaxError(const Rvector &vec)
{
   Real max = -999.99;
   for (Integer ii = 0; ii < vec.GetSize(); ii++)
      if (vec(ii) > max) max = vec(ii);
   return max;
}

Real GetMaxError(const Rmatrix &mat)
{
   Real max = -999.99;
   Integer r, c;
   mat.GetSize(r,c);
   for (Integer ii = 0; ii < r; ii++)
      for (Integer jj = 0; jj < c; jj++)
         if (mat(ii,jj) > max) max = mat(ii,jj);
   return max;
}

//------------------------------------------------------------------------------
// int main(int argc, char *argv[])
//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
   std::string outFormat = "%16.9f ";
   Real        tolerance = 1e-15;
   
   ConsoleMessageReceiver *consoleMsg = ConsoleMessageReceiver::Instance();
   MessageInterface::SetMessageReceiver(consoleMsg);
   std::string outPath = "./";
   MessageInterface::SetLogFile(outPath + "GmatLog.txt");
   MessageInterface::ShowMessage("%s\n",
                                 GmatTimeUtil::FormatCurrentTime().c_str());
   
   // Set global format setting
   GmatGlobal *global = GmatGlobal::Instance();
   global->SetActualFormat(false, false, 16, 1, false);
   
   char *buffer = NULL;
   buffer = getenv("OS");
   if (buffer  != NULL)
   {
      MessageInterface::ShowMessage("Current OS is %s\n", buffer);
   }
   else
   {
      MessageInterface::ShowMessage("Buffer is NULL\n");
   }
   
   MessageInterface::ShowMessage("*** START TEST ***\n");
   MessageInterface::ShowMessage("*** TESTing HyperSensitive optimization problem ***\n");
   
   try
   {
      clock_t t0 = clock();

      // ---------------------------------------------------------------------
      // ==========================================================================
      // =====  Define Properties for the Trajectory
      // Create trajectory and configure user function names
      Trajectory *traj                = new Trajectory();
      // Create path and point objects
      MessageInterface::ShowMessage("*** TEST *** creating path and point objects\n");
      HyperSensitivePathObject *pathFunctionObject  =
                                       new HyperSensitivePathObject();
      HyperSensitivePointObject *pointFunctionObject  =
                                        new HyperSensitivePointObject();
      
      // not doing plotting for now
      //std::string plotFunctionName   = "HyperSensitivePlotFunction";
      //bool    showPlot               = false;
      //Integer plotUpdateRate         = 3;
      Real    costLowerBound         = 0.0;
      Real    costUpperBound         = INF;
      Integer maxMeshRefinementCount = 15;

      // Create trajectory
      MessageInterface::ShowMessage("*** TEST *** creating trajectory\n");
      traj->SetUserPathFunction(pathFunctionObject);
      traj->SetUserPointFunction(pointFunctionObject);
      traj->SetCostLowerBound(costLowerBound);
      traj->SetCostUpperBound(costUpperBound);
      traj->SetMaxMeshRefinementCount(maxMeshRefinementCount);

      // ==========================================================================
      // =====  Define Properties for Phases
      // ==========================================================================
      // Create phases
      MessageInterface::ShowMessage("*** TEST *** creating phase\n");

      // method = 1 RK, 2 for Radau
      Integer method = 2;
      IntegerArray meshIntervalNumPoints;
      Rvector meshIntervalFractions;

      Phase *phase1;
      if (method == 1)
      {
         RadauPhase *phasetmp = new RadauPhase();
         phase1 = phasetmp;
         meshIntervalFractions.SetSize(5);
         meshIntervalFractions(0) = -1.0;
         meshIntervalFractions(1) = -0.8;
         meshIntervalFractions(2) = 0.0;
         meshIntervalFractions(3) = 0.8;
         meshIntervalFractions(4) = 1.0;
         meshIntervalNumPoints.push_back(12);
         meshIntervalNumPoints.push_back(5);
         meshIntervalNumPoints.push_back(5);
         meshIntervalNumPoints.push_back(12);
      }
      else
      {
         ImplicitRKPhase *phasetmp = new ImplicitRKPhase();
         phasetmp->SetTranscription("RungeKutta8");
         phase1 = phasetmp;                  
         meshIntervalFractions.SetSize(5);
         meshIntervalFractions(0) = 0.0;
         meshIntervalFractions(1) = 0.2;
         meshIntervalFractions(2) = 0.5;
         meshIntervalFractions(3) = 0.8;
         meshIntervalFractions(4) = 1.0;
         meshIntervalNumPoints.push_back(7);
         meshIntervalNumPoints.push_back(4);
         meshIntervalNumPoints.push_back(4);
         meshIntervalNumPoints.push_back(7);
         /*
         meshIntervalFractions.SetSize(5);
         meshIntervalFractions(0) = 0.0;
         meshIntervalFractions(1) = 0.2;
         meshIntervalFractions(2) = 0.5;
         meshIntervalFractions(3) = 0.8;
         meshIntervalFractions(4) = 1.0;
         meshIntervalNumPoints.push_back(15);
         meshIntervalNumPoints.push_back(7);
         meshIntervalNumPoints.push_back(7);
         meshIntervalNumPoints.push_back(15);
         */
         
      }
      std::string initialGuessMode = "LinearUnityControl";

      
//      Rvector meshIntervalFractions(5, -1.0, -0.8, 0.0, 0.8, 1.0);
//      IntegerArray meshIntervalNumPoints;
//      meshIntervalNumPoints.push_back(15);
//      meshIntervalNumPoints.push_back(10);
//      meshIntervalNumPoints.push_back(10);
//      meshIntervalNumPoints.push_back(15);
      // Set time properties
      Real timeLowerBound   = 0.0;
      Real timeUpperBound   = 10000.0;
      Real initialGuessTime = 0.0;
      Real finalGuessTime   = 10000.0;
      // Set state properties
      Integer numStateVars              = 1;
      Rvector stateLowerBound(1, -50.0);
      Rvector stateUpperBound(1, 50.0);
      Rvector initialGuessState(1, 1.0);
      Rvector finalGuessState(1, 1.0);
      // Set control properties
      Integer numControlVars  = 1;
      Rvector controlUpperBound(1,  50.0);
      Rvector controlLowerBound(1, -50.0);

      phase1->SetInitialGuessMode(initialGuessMode);

      phase1->SetNumStateVars(numStateVars);
      phase1->SetNumControlVars(numControlVars);
      phase1->SetMeshIntervalFractions(meshIntervalFractions);
      phase1->SetMeshIntervalNumPoints(meshIntervalNumPoints);
      phase1->SetStateLowerBound(stateLowerBound);
      phase1->SetStateUpperBound(stateUpperBound);
      phase1->SetStateInitialGuess(initialGuessState);
      phase1->SetStateFinalGuess(finalGuessState);
      phase1->SetTimeLowerBound(timeLowerBound);
      phase1->SetTimeUpperBound(timeUpperBound);
      phase1->SetTimeInitialGuess(initialGuessTime);
      phase1->SetTimeFinalGuess(finalGuessTime);
      phase1->SetControlLowerBound(controlLowerBound);
      phase1->SetControlUpperBound(controlUpperBound);


      // =========================================================================
      // =====  Define the linkage configuration and optimize
      // =========================================================================

      // Add phases to Trajectory
      MessageInterface::ShowMessage("*** TEST *** adding Phase list to Trajectory\n");

      std::vector<Phase*> pList;
      pList.push_back(phase1);
      traj->SetPhaseList(pList);
      
      MessageInterface::ShowMessage("*** TEST *** initializing the Trajectory\n");
      // Initialize the Trajectory
      traj->Initialize();
//      %traj.linkageConfig{1} = {phase1,phase2};
//      [z,F,xmul,Fmul] = traj.Optimize();
      
      MessageInterface::ShowMessage("*** TEST *** setting up the call to Optimize!!\n");
      
      Rvector  dv2      = traj->GetDecisionVector();
      Rvector  C        = traj->GetCostConstraintFunctions(); // nonDim);
      
      RSMatrix conSp    = phase1->GetConSparsityPattern();
//      MessageInterface::ShowMessage("*** TEST *** Con sparsity pattern from Phase1:\n");
//      Integer rJac = conSp.size1();
//      Integer cJac = conSp.size2();
//      for (Integer cc = 0; cc < cJac; cc++)
//      {
//         for (Integer rr = 0; rr < rJac; rr++)
//         {
//            Real jacTmp = conSp(rr,cc);
//            if (jacTmp != 0.0)
//            MessageInterface::ShowMessage(" (%d, %d)   %12.10f\n", rr+1, cc+1, jacTmp);
//         }
//      }
      
// ------------------ Optimizing -----------------------------------------------
// TRY it first without optimizing
      Rvector z = dv2;
      Rvector F(C.GetSize());
      Rvector xmul(dv2.GetSize());
      Rvector Fmul(C.GetSize());
      MessageInterface::ShowMessage("*** TEST *** Optimizing!!\n");
      traj->Optimize(z, F, xmul, Fmul);

      MessageInterface::ShowMessage("*** TEST *** z:\n%s\n",
                                    z.ToString(12).c_str());
      MessageInterface::ShowMessage("*** TEST *** F:\n%s\n",
                                    F.ToString(12).c_str());
      MessageInterface::ShowMessage("*** TEST *** xmul:\n%s\n",
                                    xmul.ToString(12).c_str());
      MessageInterface::ShowMessage("*** TEST *** Fmul:\n%s\n",
                                    Fmul.ToString(12).c_str());
      MessageInterface::ShowMessage("*** TEST *** Optimization complete!!\n");
// ------------------ Optimizing -----------------------------------------------

      Rvector dvP1 = phase1->GetDecVector();
      MessageInterface::ShowMessage("*** TEST *** dvP1:\n%s\n",
                                    dvP1.ToString(12).c_str());
      
      // Interpolate solution
      Rvector timeVector = phase1->GetTimeVector();
      DecisionVector *dv = phase1->GetDecisionVector();
      Rmatrix stateSol   = dv->GetStateArray();
      Rmatrix controlSol = dv->GetControlArray();

      Real timeSpent = ((Real) (clock() - t0)) / CLOCKS_PER_SEC;
      MessageInterface::ShowMessage(">>>>>>>>>> CLOCK TIME (sec) for entire test = %12.10f\n", timeSpent);

      cout << endl;
      cout << "Hit enter to end" << endl;
      cin.get();
      
      MessageInterface::ShowMessage("*** END TEST ***\n");
   }
   catch (BaseException &be)
   {
      MessageInterface::ShowMessage("Exception caught: %s\n", be.GetFullMessage().c_str());
   }
   
}
