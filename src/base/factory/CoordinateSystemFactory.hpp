//$Id$
//------------------------------------------------------------------------------
//                         CoordinateSystemFactory
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002 - 2022 United States Government as represented by the
// Administrator of the National Aeronautics and Space Administration.
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
// number S-67573-G
//
// Author: Linda Jun
// Created: 2005/01/18
//
/**
 *  This class is the factory class for CoordinateSystems.  
 */
//------------------------------------------------------------------------------
#ifndef CoordinateSystemFactory_hpp
#define CoordinateSystemFactory_hpp

#include "gmatdefs.hpp"
#include "Factory.hpp"
#include "CoordinateSystem.hpp"

class GMAT_API CoordinateSystemFactory : public Factory
{
public:
   CoordinateSystem*  CreateObject(const std::string &ofType,
                                   const std::string &withName = "");
   CoordinateSystem*  CreateCoordinateSystem(const std::string &ofType,
                                             const std::string &withName = "");

   // default constructor
   CoordinateSystemFactory();
   // constructor
   CoordinateSystemFactory(StringArray createList);
   // copy constructor
   CoordinateSystemFactory(const CoordinateSystemFactory& fact);
   // assignment operator
   CoordinateSystemFactory& operator= (const CoordinateSystemFactory& fact);

   // destructor
   ~CoordinateSystemFactory();

protected:
   // protected data

private:
   // private data


};

#endif // CoordinateSystemFactory_hpp




