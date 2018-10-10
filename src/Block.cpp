/**
 *		@file 		Block.cpp
 *		@date 		01/03/2018
 *		@author 	Filip Kocica <xkocic01@fit.vutbr.cz>
 *    @author   Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 *		@brief 		Implementation of Block used in blockedior
 */

#include "Block.hpp"

///
/// Namespace with implementation of logic of an application
///
namespace BlockEditorLogic
{
  /**
   * @brief Constructor of block, inits block
   */
  CBlock::CBlock(ID blockID, EBlockType blockType, TypeName tn) :
    m_blockID{blockID}, m_bt{blockType}, m_x{0}, m_y{0}, m_name{tn}
  {
    m_inputPort1 = nullptr;
    m_inputPort2 = nullptr;
    m_outputPort = nullptr;
  }

  /**
   * @brief Constructor used in loadScheme
   */
   CBlock::CBlock(ID ID, EBlockType bt, int x, int y,
     PortValue pv, CPort *p1, CPort *p2,  CPort *p3, TypeName tn) :
     m_blockID{ID}, m_bt{bt}, m_x{x}, m_y{y}, m_value{pv},
     m_inputPort1{p1}, m_inputPort2{p2}, m_outputPort{p3}, m_name{tn}
     {

     }

  /**
   * @brief Destructor of block, removes ports
   */
  CBlock::~CBlock()
  {
    // Depends if output port of block was already removed in GUI
    // delete m_outputPort;
  }

  /**
   * @brief Overloaded ostream operator used for saving scheme to file
   * @param os Opened file desc
   * @param block Block to write into file
   * @return ostream, to make proper overload
   */
  std::ostream& operator<<(std::ostream& os, const CBlock& block)
  {
    os << "Type:"        << block.m_bt << std::endl;
    os << "ID:"          << block.m_blockID << std::endl;
    os << "Position X:"  << block.m_x << std::endl;
    os << "Position Y:"  << block.m_y << std::endl;
    os << "Type name:" << block.m_name << std::endl;
    os << "Input value:" << ((block.getType() == BT_INPUT) ?
          std::to_string(block.getValue()) : "None") << std::endl;
    os << "Input 1 ID:"  << ((block.hasPort(Ports::P_INPUT1)) ?
          std::to_string(block.getPortID(Ports::P_INPUT1)) :
          "None") <<  std::endl;
    os << "Input 2 ID:"  << ((block.hasPort(Ports::P_INPUT2)) ?
          std::to_string(block.getPortID(Ports::P_INPUT2)) :
          "None") <<  std::endl;
    os << "Output ID:"   << ((block.hasPort(Ports::P_OUTPUT)) ?
          std::to_string(block.getPortID(Ports::P_OUTPUT)) :
          "None") <<  std::endl;
    return os;
  }

   /**
    * @brief Function returns one of ports
    * @param whichPort input/output port
    * @return pointer to selected port
    */
   CPort* CBlock::getPort(Ports whichPort) const
   {
     switch (whichPort)
     {
       case Ports::P_INPUT1:
         return m_inputPort1;
       case Ports::P_INPUT2:
         return m_inputPort2;
       case Ports::P_OUTPUT:
         return m_outputPort;
     }
   }

   /**
    * @brief Get function
    * @return Name of type of block
    */
   TypeName	CBlock::getTypeName() const
   {
     return this->m_name;
   }

   /**
    * @brief Function performs operation due to type of block on two operands
    * @param pv1 Operand #1
    * @param pv2 Operand #2
    * @return Result of operation
    */
   PortValue CBlock::performOperation(PortValue&& pv1, PortValue&& pv2)
   {
     switch(this->m_bt)
     {
       case BT_ADD:
         return pv1 + pv2;
       case BT_SUB:
         return pv1 - pv2;
       case BT_MUL:
         return pv1 * pv2;
       case BT_DIV:
         return pv1 / pv2;
       case BT_POW:
          return pow(pv1, pv2);
        default:
          throw CBlockEditorException("Unknown type of block", EErrorCode::E_INTERN);
     }
   }

  /**
   * @brief Get function
   * @return ID of block
   */
  ID CBlock::getID() const
  {
    return this->m_blockID;
  }

  /**
   * @brief Get function
   * @return Type of block
   */
  EBlockType CBlock::getType() const
  {
    return this->m_bt;
  }

  /**
   * @brief Get function
   * @return Position of the block
   */
  std::pair<int, int> CBlock::getPosition() const
  {
    return std::move(std::pair<int, int>(m_x, m_y));
  }

  /**
   * @brief Set function
   * @param Coords of block
   */
   void CBlock::setPosition(std::pair<int, int> coords)
   {
     this->m_x = coords.first;
     this->m_y = coords.second;
   }

  /**
   * @brief Creates output port
   * @return New port
   */
  CPort* CBlock::addOutputPort(ID portID)
  {
    CPort *port = new CPort(portID);
    m_outputPort = port;
    return port;
  }

  /**
   * @brief Inits input block value
   * @param value Input value to assign
   */
  void CBlock::setInputValue(PortValue value)
  {
    this->m_value = value;
  }

  /**
   * @brief Get function
   * @return Value of input block
   */
  PortValue CBlock::getValue() const
  {
    return this->m_value;
  }

  /**
   * @brief Assigns input port
   * @param whichPort Which of the input ports
   * @param port Port to assign
   */
  void CBlock::addInputPort(Ports whichPort, CPort* port)
  {
    if (whichPort == Ports::P_INPUT1)
    {
      m_inputPort1 = port;
    }
    else
    {
      m_inputPort2 = port;
    }
  }

  /**
   * @brief Removes port between two blocks
   * @param whichPort Which of the input ports
   */
  void CBlock::removePort(Ports whichPort)
  {
    switch (whichPort)
    {
      case Ports::P_INPUT1:
        m_inputPort1 = nullptr;
        break;
      case Ports::P_INPUT2:
        m_inputPort2 = nullptr;
        break;
      case Ports::P_OUTPUT:
        delete m_outputPort;
        m_outputPort = nullptr;
        break;
    }
  }

  /**
   * @brief Unsets output port bcs of double free
   */
  void CBlock::setPort(Ports whichPort, void *ptr)
  {
    switch (whichPort)
    {
      case Ports::P_INPUT1:
        m_inputPort1 = static_cast<CPort*>(ptr);
        break;
      case Ports::P_INPUT2:
        m_inputPort2 = static_cast<CPort*>(ptr);
        break;
      case Ports::P_OUTPUT:
        m_outputPort = static_cast<CPort*>(ptr);
        break;
    }
  }

  /**
   * @brief Checks if the block already has specified port
   * @param whichPort Which of the input ports
   * @return Yes or not
   */
  bool CBlock::hasPort(Ports whichPort) const
  {
    switch (whichPort)
    {
      case Ports::P_INPUT1:
        if (m_inputPort1 == nullptr)
          return false;
        return true;
      case Ports::P_INPUT2:
        if (m_inputPort2 == nullptr)
          return false;
        return true;
      case Ports::P_OUTPUT:
        if (m_outputPort == nullptr)
          return false;
        return true;
    }
  }

  /**
   * @brief Get function
   * @param whichPort Which of the input ports
   * @return ID of port
   */
  ID CBlock::getPortID(Ports whichPort) const
  {
    switch (whichPort)
    {
      case Ports::P_INPUT1:
        return m_inputPort1->getPortID();
      case Ports::P_INPUT2:
        return m_inputPort2->getPortID();
      case Ports::P_OUTPUT:
        return m_outputPort->getPortID();
    }
  }
}