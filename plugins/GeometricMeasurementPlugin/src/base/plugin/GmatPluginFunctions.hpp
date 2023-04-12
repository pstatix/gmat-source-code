//$Id: GmatPluginFunctions.hpp 1398 2011-04-21 20:39:37Z  $
//------------------------------------------------------------------------------
//                            GmatPluginFunctions
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002 - 2022 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); 
// You may not use this file except in compliance with the License. 
// You may obtain a copy of the License at:
// http://www.apache.org/licenses/LICENSE-2.0. 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
// express or implied.   See the License for the specific language
// governing permissions and limitations under the License.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under contract
// number NNG06CA54C
//
// Author: Darrel J. Conway, Thinking Systems, Inc.
// Created: 2009/08/03
//
/**
 * Definition for library code interfaces.
 */
//------------------------------------------------------------------------------

#ifndef GmatPluginFunctions_hpp
#define GmatPluginFunctions_hpp

#include "geometricmeasurement_defs.hpp"
#include "Factory.hpp"
#include "TriggerManager.hpp"

class MessageReceiver;

extern "C"
{
   Integer          GEOMETRICMEAS_API GetFactoryCount();
   Factory          GEOMETRICMEAS_API *GetFactoryPointer(Integer index);
   void             GEOMETRICMEAS_API SetMessageReceiver(MessageReceiver* mr);
};


#endif /* GmatPluginFunctions_hpp */


/**
 * \mainpage API for the GMAT Geometric Measurement Plugin
 *
 * \section intro_sec Introduction
 *
 * To be written
 *
 */
