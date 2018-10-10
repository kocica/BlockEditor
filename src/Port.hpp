/**
 *		@file 		Port.hpp
 *		@date 		02/03/2018
 *		@author 	Filip Kocica <xkocic01@fit.vutbr.cz>
 *    @author   Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 *		@brief    Implementaion of intput/output ports on block and his operations
 */

#pragma once

#include <iostream>

#include "TypeName.hpp"

///
/// Namespace with implementation of logic of an application
///
namespace BlockEditorLogic
{
  /**
   * @brief Which port it is
   */
  enum class Ports
  {
    P_INPUT1 = 1,
    P_INPUT2 = 2,
    P_OUTPUT = 3
  };

  /// Alliases
  using PortValue = double;
  using TypeName  = std::string;
  using ID        = unsigned int;

  ///
  /// Each port has type, which consists of the name and value of type double
  ///
  class CPort
  {
  public:
    CPort() = delete;
    CPort(ID);
    ~CPort() = default;

    ID          getPortID() const;
    TypeName    getPortName() const;
    PortValue   getPortValue() const;
    bool        hasValue() const;

    void        unsetValue();
    void        setPortName(std::string);
    void        setPortValue(PortValue);

  protected:

  private:
    ID                m_portID;      /**< Unique ID of port */
    TypeName          m_name;        /**< Name of port */
    PortValue         m_value;       /**< Port value */
    bool              m_bValue;      /**< Does port have value ? */
  };
}