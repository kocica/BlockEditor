/**
 *		@file 		Error.hpp
 *		@date 		01/03/2018
 *		@author 	Filip Kocica <xkocic01@fit.vutbr.cz>
 *    @author   Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 *		@brief    Error codes of an application
 */

#pragma once

#include <iostream>

namespace BlockEditorLogic
{
  /**
   * @brief Enumator of error codes
   */
  enum class EErrorCode
  {
    //E_UNKNOWN       = -1,

    E_RUNTIME_ERROR = -2,

    E_UI_CYCLE      = -3, /**< There is a cycle in the scheme */
    E_UI_NOT_CON    = -4, /**< Blocks hasnt all input port/output port */
    E_UI_BAD_TYPES  = -5, /**< Types mismatch (eg. connected blocks with different types) */
    E_UI_BAD_FILE   = -6,

    E_INTERN        = -7,  /**< Inside error, cause by wrong call, shouldnt happen in UI, mainly for debug purposes */
  };

  /*
  TODO udelat pole erroru s indexi od enum classu
  */
}