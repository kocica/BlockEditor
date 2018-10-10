/**
 *		@file 		BlockEditorException.hpp
 *		@date 		02/03/2018
 *		@author 	Filip Kocica <xkocic01@fit.vutbr.cz>
 *    @author   Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 *		@brief    Inherited special exception holding error string plus error code
 */

#pragma once

#include <iostream>
#include <exception>

#include "Error.hpp"

///
/// Namespace with implementation of logic of an application
///
namespace BlockEditorLogic
{
  /**
   *  @brief Exception specific for block editor application
   */
  class CBlockEditorException : public std::exception
  {
  public:
      /**
       *  @brief Constructor (C strings).
       *  @param message C-style string error message.
       *                 The string contents are copied upon construction.
       *                 Hence, responsibility for deleting the char* lies
       *                 with the caller.
       */
      explicit CBlockEditorException(const char* message, EErrorCode ec = EErrorCode::E_RUNTIME_ERROR)
        : m_msg{message}
        , m_err{ec}
        {

        }

      /**
       *  @brief Constructor (C++ STL strings).
       *  @param message The error message.
       */
      explicit CBlockEditorException(const std::string& message, EErrorCode ec = EErrorCode::E_RUNTIME_ERROR)
        : m_msg{message}
        , m_err{ec}
        {

        }

      /**
       * @brief Destructor
       * Virtual to allow for subclassing.
       */
      virtual ~CBlockEditorException() throw ()
      {

      }

      /**
       *  @brief Returns a pointer to the (constant) error description.
       *  @return A pointer to a const char*. The underlying memory
       *          is in posession of the Exception object. Callers must
       *          not attempt to free the memory.
       */
      virtual const char * what() const throw ()
      {
         return m_msg.c_str();
      }

      /**
       *  @brief Get function
       *  @return Error code of exception
       */
      EErrorCode getErrCode() const throw ()
      {
        return m_err;
      }

  protected:
      std::string   m_msg; /**< Error message */
      EErrorCode    m_err; /**< Error code */
  };
}