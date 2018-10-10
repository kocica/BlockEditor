/**
 *		@file 		TypeName.hpp
 *		@date 		12/04/2018
 *		@author 	Filip Kocica <xkocic01@fit.vutbr.cz>
 *    @author   Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 *		@brief    Type of data on port/block
 */

#pragma once

#include <iostream>

///
/// Namespace with implementation of logic of an application
///
namespace BlockEditorLogic
{
  std::string index2typename(int); // TODO remove or change

  const std::string TN_FLOAT   = "FLT";          /**< TODO */
  const std::string TN_INTEGER = "INT";          /**< TODO */
  const std::string TN_HEXA    = "HEX";          /**< TODO */
  const std::string TN_INPUT   = "TN_INPUT";     /**< For input blocks only, dont use in GUI */
}