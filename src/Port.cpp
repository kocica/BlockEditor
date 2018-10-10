/**
 *		@file 		Port.cpp
 *		@date 		02/03/2018
 *		@author 	Filip Kocica <xkocic01@fit.vutbr.cz>
 *    @author   Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 *		@brief    Implementaion of intput/output ports on block and his operations
 */

#include "Port.hpp"

///
/// Namespace with implementation of logic of an application
///
namespace BlockEditorLogic
{
  /**
   * @brief Constructor of the port
   * @param portID idetificator of new port
   */
  CPort::CPort(ID portID) : m_portID{portID}, m_value{0},
      m_bValue{false}
  {

  }

  /**
   * @brief Get function
   * @return Name of type port has
   */
  TypeName CPort::getPortName() const
  {
    return this->m_name;
  }

  /**
   * @brief Get function
   * @return Value of port
   */
  PortValue CPort::getPortValue() const
  {
    return this->m_value;
  }

  /**
   * @brief Get function
   * @return ID of port
   */
  ID CPort::getPortID() const
  {
    return this->m_portID;
  }

  /**
   * @brief Unsets value of port, used after run of scheme
   */
  void CPort::unsetValue()
  {
    this->m_bValue = false;
  }

  /**
   * @brief Set function
   * @param New name of type of port
   */
  void CPort::setPortName(std::string newName)
  {
    this->m_name = newName;
  }

  /**
   * @brief Set function
   * @param New value of port
   */
  void CPort::setPortValue(PortValue pv)
  {
    this->m_bValue = true;
    this->m_value = pv;
  }

  /**
   * @brief Checks if port already has value
   * @return true if has value
   */
  bool CPort::hasValue() const
  {
    return this->m_bValue;
  }
}