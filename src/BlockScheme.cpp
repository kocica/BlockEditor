/**
 *		@file 		BlockScheme.cpp
 *		@date 		02/03/2018
 *		@author 	Filip Kocica <xkocic01@fit.vutbr.cz>
 *    @author   Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 *		@brief    Implementation of whole scheme, storing all
 *              blocks, actions in scheme, save/load operations
 */

#include "BlockScheme.hpp"


///
/// Namespace with implementation of logic of an application
///
namespace BlockEditorLogic
{
  /**
   * @brief Constructor of scheme, sets counter of block
   *            id's to 0
   */
  CBlockScheme::CBlockScheme() : m_blockCounter{0}, m_portCounter{0},
      m_blocksInScheme{0}
  {

  }

  /**
   * @brief Destructor of scheme, removes all blocks from
   *             scheme
   */
  CBlockScheme::~CBlockScheme()
  {

  }

  /**
   * @brief Function adds new block to the deque.
   *        Assigns him unique ID and specified type.
   * @param type Type of new block
   * @return ID of new block
   */
  ID CBlockScheme::addBlock(EBlockType type, TypeName tn)
  {
    CBlock newBlock(m_blockCounter, type, tn);
    m_blockCounter++;

    // Append new block to deque
    m_blocks.push_back(std::move(newBlock));

    // Increment counter of blocks in scheme
    m_blocksInScheme++;

    // Return ID of new block
    return m_blockCounter-1;
  }

  /**
   * @brief Function sets block counter to blockID
   * @param type ID of next block
   */
  void CBlockScheme::setID(ID blockID)
  {
    this->m_blockCounter = blockID;
  }

  /**
   * @brief Function adds value to port of the block
   * @param ID id of block with port to assign
   * @param value the value to assign
   * @param whichPort input/output port
   */
  void CBlockScheme::addInputValue(ID blockID, PortValue value, Ports whichPort)
  {
    /*if (whichPort == Ports::P_OUTPUT)
    {
      throw CBlockEditorException("Cannot assign value to non-input port");
    }*/

    for (auto& it : m_blocks)
    {
      if (it.getID() == blockID)
      {
        ID id = addBlock(BT_INPUT, TN_INPUT);
        for (auto& it2 : m_blocks)
        {
          if (it2.getID() == id)
          {
            it2.setInputValue(value);
            break;
          }
        }
        addPort(id, blockID, whichPort);
      }
    }
  }

  /**
   * @brief Removes value of input port on specified block
   * @param blockID ID of block whose value we want to remove
   * @param whichPort which of input ports value should be removed
   */
  void CBlockScheme::removeInputValue(ID blockID, Ports whichPort)
  {
    CPort* p;

    for (auto it = m_blocks.begin(); it != m_blocks.end(); it++)
    {
      if (it->getID() == blockID)
      {
        if (!it->hasPort(whichPort))
        {
          throw CBlockEditorException("Selected block hasnt selected input port", EErrorCode::E_INTERN);
        }
        p = it->getPort(whichPort);
        it->removePort(whichPort);
        for (auto it2 = m_blocks.begin(); it2 != m_blocks.end(); it2++)
        {
          if (it2->getType() == BT_INPUT)
          {
            if (p->getPortID() == it2->getPort(Ports::P_OUTPUT)->getPortID())
            {
              m_blocks.erase(it2);
              m_blocksInScheme--;
              delete p;
              return ;
            }
          }
        }
      }
    }

    throw CBlockEditorException("Block with specified ID doesn't exist", EErrorCode::E_INTERN);
  }

  /**
   * @brief Function removes block specified by unique ID
   * @param blockID ID of block to remove
   */
  void CBlockScheme::removeBlock(ID blockID)
  {
    // Decrement counter of blocks in scheme
    m_blocksInScheme--;

    for (auto it = m_blocks.begin(); it != m_blocks.end(); it++)
    {
      if (it->getID() == blockID)
      {
        // Get portID of output port of this block
        if (it->hasPort(Ports::P_OUTPUT))
        {
          CPort *op = it->getPort(Ports::P_OUTPUT);
          // Find Block with input port with portID
          // Set pointer to port of that Block to nullptr
          for (auto& it2 : m_blocks)
          {
            CPort *ip1 = it2.getPort(Ports::P_INPUT1);
            CPort *ip2 = it2.getPort(Ports::P_INPUT2);
            if (ip1 && (ip1->getPortID() == op->getPortID()))
            {
              it2.setPort(Ports::P_INPUT1);
            }
            else if (ip2 && (ip2->getPortID() == op->getPortID()))
            {
              it2.setPort(Ports::P_INPUT2);
            }
          }
        }
        m_blocks.erase(it);
        return;
      }
    }
    throw CBlockEditorException(std::string("Block with ID ") + std::to_string(blockID) + " doesnt exist", EErrorCode::E_INTERN);
  }

  /**
   * @brief Debugging function - prints all blocks with ports
   */
  void CBlockScheme::debug_printBlocks() const
  {
    for (auto& it : m_blocks)
    {
      std::cout << "Block ID: " << it.getID() << ", type: " << it.getType() << std::endl;
      if (it.getType() == BT_INPUT)
        std::cout << "    Has input value: " << it.getValue() << std::endl;
      if (it.hasPort(Ports::P_INPUT1))
        std::cout << "    Has input 1 ID:" << it.getPortID(Ports::P_INPUT1) <<  std::endl;
      if (it.hasPort(Ports::P_INPUT2))
        std::cout << "    Has input 2 ID:" << it.getPortID(Ports::P_INPUT2) <<  std::endl;
      if (it.hasPort(Ports::P_OUTPUT))
        std::cout << "    Has output ID:" << it.getPortID(Ports::P_OUTPUT) <<  std::endl;
    }
  }

  /**
   * @brief Function creates new port between blockID_out & blockID_in (first/second)
   * @param blockID_out Which block output port is
   * @param blockID_int Which block input port is
   * @param wchiPort Which port it is
   */
  void CBlockScheme::addPort(ID blockID_out, ID blockID_in, Ports whichPort)
  {
    CPort* port;
    TypeName tn;
    auto pit = m_blocks.begin();

    for ( ; pit != m_blocks.end(); pit++)
    {
      if (pit->getID() == blockID_out)
      {
        if (pit->hasPort(Ports::P_OUTPUT))
        {
          throw CBlockEditorException("Block already has output port", EErrorCode::E_INTERN);
        }
        tn = pit->getTypeName();
        break;
      }
    }

    for (auto& it : m_blocks)
    {
      if (it.getID() == blockID_in)
      {
        if (it.getType() == BT_INPUT)
        {
          throw CBlockEditorException("Input block hasnt any input ports", EErrorCode::E_INTERN);
        }
        if (tn != it.getTypeName()
              && tn != TN_INPUT)
        {
          throw CBlockEditorException("Types of blocks differ -> cannot connect them", EErrorCode::E_UI_BAD_TYPES);
        }
        port = pit->addOutputPort(this->m_portCounter);
        port->setPortName(tn);
        it.addInputPort(whichPort, port);
        break;
      }
    }
    this->m_portCounter++;
  }

  /**
   * @brief Function removes port between blockID_out & blockID_in (first/second)
   * @param blockID_out Which block output port is
   * @param blockID_int Which block input port is
   * @param firstSecond Which port it is
   */
  void CBlockScheme::removePort(ID blockID_out, ID blockID_in, Ports whichPort)
  {
    for (auto& it : m_blocks)
    {
      if (it.getID() == blockID_out)
      {
        if (!it.hasPort(Ports::P_OUTPUT))
        {
          throw CBlockEditorException(std::string("Removing port -- Block " + std::to_string(blockID_out) + " hasnt output port"), EErrorCode::E_INTERN);
        }
        it.removePort(Ports::P_OUTPUT);
        break;
      }
    }

    for (auto& it : m_blocks)
    {
      if (it.getID() == blockID_in)
      {
        if (!it.hasPort(whichPort))
        {
          throw CBlockEditorException(std::string("Removing port -- Block " + std::to_string(blockID_in) + " hasnt input port"), EErrorCode::E_INTERN);
        }
        it.removePort(whichPort);
        break;
      }
    }
  }

  /**
   * @brief Saves scheme in TODO xml/json format to the file
   * @param fileName Name of file where scheme will be saved
   */
  void CBlockScheme::saveScheme(CBlockScheme::Coords c, std::string& fileName)
  {
    std::ofstream fd;
    fd.open(fileName);

    for (auto& it : c)
    {
      for (auto& it2 : m_blocks)
      {
        if (it2.getID() == it.first)
        {
          it2.setPosition(it.second);
          break;
        }
      }
    }

    for (auto& it : m_blocks)
    {
      fd << it;
    }

    fd.close();
  }

  /**
   * @brief Clear buffer of blocks/ports and sets coutners to zero
   */
  void CBlockScheme::clearScheme()
  {
    m_blocks.clear();
    m_actions.clear();
    m_blocksInScheme = 0;
    m_portCounter = 0;
    m_blockCounter = 0;
  }

  /**
   * @brief Load scheme saved in xml/json format in the file
   * @param fileName Name of file where scheme is saved
   * @return Buffer of parts in scheme
   */
  CBlockScheme::PartBuffer CBlockScheme::loadScheme(std::string& fileName)
  {
    std::ifstream fd;
    std::unordered_map<ID, CPort*> ports;
    std::string delimiter = ":";
    std::string line, token1, token2;
    EBlockType bt; ID bID, maxID = 0, maxIDport = 0; int x, y; PortValue pv = .0;
    TypeName tn;
    CPort *p1 = nullptr, *p2 = nullptr, *p3 = nullptr;

    fd.open(fileName);

    // Clear ports
    clearScheme();

    while (std::getline(fd, line))
    {
      token1 = line.substr(0, line.find(delimiter));
      token2 = line.substr(line.find(delimiter) + 1);

      try
      {
        if (token1 == "Type")
        {
          bt = static_cast<EBlockType>(std::stoi(token2));
        }
        else if (token1 == "ID")
        {
          bID = std::stoul(token2, nullptr, 0);
          if (bID > maxID) maxID = bID;
        }
        else if (token1 == "Position X")
        {
          x = std::stoi(token2);
        }
        else if (token1 == "Position Y")
        {
          y = std::stoi(token2);
        }
        else if (token1 == "Input value")
        {
          if (token2 == "None") continue;
          pv = stod(token2);
        }
        else if (token1 == "Type name")
        {
          tn = std::string(token2);
        }
        else if (token1 == "Input 1 ID")
        {
          if (token2 == "None") continue;
          ID id = std::stoi(token2);
          if (ports.count(id) > 0)
          {
            p1 = ports[id];
          }
          else
          {
            p1 = new CPort(id);
            ports[id] = p1;
          }
        }
        else if (token1 == "Input 2 ID")
        {
          if (token2 == "None") continue;
          ID id = std::stoi(token2);
          if (ports.count(id) > 0)
          {
            p2 = ports[id];
          }
          else
          {
            p2 = new CPort(id);
            ports[id] = p2;
          }
        }
        else if (token1 == "Output ID")
        {
          if (token2 != "None")
          {
            ID id = std::stoi(token2);
            if (ports.count(id) > 0)
            {
              p3 = ports[id];
            }
            else
            {
              p3 = new CPort(id);
              ports[id] = p3;
            }
          }

          // Save block
          CBlock b(bID, bt, x, y, pv, p1, p2, p3, tn);
          this->m_blocks.push_back(b);

          b.setPort(Ports::P_OUTPUT);

          // Reset values
          pv = .0; p1 = nullptr; p2 = nullptr; p3 = nullptr;
        }
        else
        {
          throw CBlockEditorException("", EErrorCode::E_UI_BAD_FILE);
        }
      } catch (std::exception& e)
      {
        throw CBlockEditorException(std::string("Something went bad when"
                  " laoding scheme (Probably wrong file format) -- " + std::string(e.what())), EErrorCode::E_UI_BAD_FILE);
      }
    }

    // Find out max IDs (block; port) used in scheme
    for (const auto &pair : ports)
    {
      if (pair.first > maxIDport)
        maxIDport = pair.first;
    }
    m_blockCounter = maxID+1;
    m_portCounter = maxIDport+1;
    fd.close();

    // Get parts
    CBlockScheme::PartBuffer pb = getParts();

    //Clear current scheme
    clearScheme();

    // Return parts
    return std::move(pb);
  }

  /**
   * @brief Stores info about all blocks
   *        and passes them into GUI
   * @return Buffer of infos about single part in scheme
   */
  CBlockScheme::PartBuffer CBlockScheme::getParts() const
  {
    CBlockScheme::PartBuffer pb;
    EBlockType bt;
    ID id, idPort;
    std::pair<int, int> coords;
    PortValue vin1, vin2;
    ID in1, in2, out;
    Port p1, p2, p3;
    Ports which;
    CPort *p;
    TypeName tn;

    for (auto& it : m_blocks)
    {
      bt = it.getType();
      if (bt != BT_INPUT)
      {
        id = it.getID();
        coords = it.getPosition();
        tn = it.getTypeName();

        // Input port 1
        if (it.hasPort(Ports::P_INPUT1))
        {
          p = it.getPort(Ports::P_INPUT1);
          idPort = p->getPortID();
          std::pair<ID, Ports> p = findBlockByPortID(idPort, id);
          if (p.second != Ports::P_OUTPUT)
          {
            throw CBlockEditorException(std::string("Input port connected to input port " + std::to_string(id) + " " + std::to_string(p.first)), EErrorCode::E_INTERN);
          }
          // Its input block
          if (isInput(p.first))
          {
            p1 = VALUE;
            vin1 = getInputValue(p.first);
          }
          // Its normal block
          else
          {
            p1 = CONNECTION;
            in1 = p.first;
          }
        }
        else
        {
          p1 = EMPTY;
        }

        // Input port 2
        if (it.hasPort(Ports::P_INPUT2))
        {
          p = it.getPort(Ports::P_INPUT2);
          idPort = p->getPortID();
          std::pair<ID, Ports> p = findBlockByPortID(idPort, id);
          if (p.second != Ports::P_OUTPUT)
          {
            throw CBlockEditorException("Input port connected to input port", EErrorCode::E_INTERN);
          }
          // Its input block
          if (isInput(p.first))
          {
            p2 = VALUE;
            vin2 = getInputValue(p.first);
          }
          // Its normal block
          else
          {
            p2 = CONNECTION;
            in2 = p.first;
          }
        }
        else
        {
          p2 = EMPTY;
        }

        // Output port
        if (it.hasPort(Ports::P_OUTPUT))
        {
          p = it.getPort(Ports::P_OUTPUT);
          idPort = p->getPortID();
          std::pair<ID, Ports> p = findBlockByPortID(idPort, id);
          if (p.second == Ports::P_OUTPUT)
          {
            throw CBlockEditorException("Output port connected to output port", EErrorCode::E_INTERN);
          }
          p3 = CONNECTION;
          which = p.second;
          out = p.first;
        }
        else
        {
          p3 = EMPTY;
        }

        pb.push_back(SchemePart{bt, id, coords, p1, p2, p3, vin1, vin2,
                                in1, in2, out, which, tn});
      }
    }

    return std::move(pb);
  }

  /**
   * @brief Finds block and port (1/2) by port ID
   * @param portID ID of port
   * @param blockID ID of block which we doesnt look for
   * @return pair of ID of block and which block is connected to that port
   */
  std::pair<ID, Ports> CBlockScheme::findBlockByPortID(ID portID, ID blockID) const
  {
    CPort *p;

    for (auto it = m_blocks.begin(); it != m_blocks.end(); it++)
    {
      if (it->getID() == blockID)
      {
        continue;
      }

      if (it->hasPort(Ports::P_INPUT1))
      {
        p = it->getPort(Ports::P_INPUT1);
        if (p->getPortID() == portID)
        {
          return {it->getID(), Ports::P_INPUT1};
        }
      }
      if (it->hasPort(Ports::P_INPUT2))
      {
        p = it->getPort(Ports::P_INPUT2);
        if (p->getPortID() == portID)
        {
          return {it->getID(), Ports::P_INPUT2};
        }
      }
      if (it->hasPort(Ports::P_OUTPUT))
      {
        p = it->getPort(Ports::P_OUTPUT);
        if (p->getPortID() == portID)
        {
          return {it->getID(), Ports::P_OUTPUT};
        }
      }
    }

    throw CBlockEditorException(std::string("Could not find block with port with specified ID " + std::to_string(portID) + " " + std::to_string(blockID)), EErrorCode::E_INTERN);
  }

  /**
   * @brief Checks if block with id ID is input block
   * @param blockID id of block we wanna check
   * @return true if block is input
   */
  bool CBlockScheme::isInput(ID blockID) const
  {
    for (auto& it : m_blocks)
    {
      if (it.getID() == blockID)
      {
        if (it.getType() == BT_INPUT)
        {
          return true;
        }
        else
        {
          return false;
        }
      }
    }

    throw CBlockEditorException("Couldnt find block", EErrorCode::E_INTERN);
  }

  /**
   * @brief Returns value of specified intput port
   * @param blockID id of block we wanna check
   * @return value of input block
   */
  PortValue CBlockScheme::getInputValue(ID blockID) const
  {
    for (auto& it : m_blocks)
    {
      if (it.getID() == blockID)
      {
        if (it.getType() == BT_INPUT)
        {
          return it.getValue();
        }
        else
        {
          throw CBlockEditorException("Asking for value of non-input block", EErrorCode::E_INTERN);
        }
      }
    }
  }

  /**
   * @brief Runs the scheme
   * @return Buffer of actions to do in GUI in right order
   */
  CBlockScheme::ActionBuffer CBlockScheme::run()
  {
    CPort* p1;
    CPort* p2;
    bool changed;
    std::vector<ID> v;

    /// Clear actions buffer
    this->m_actions.clear();

    /// First iterate over input blocks and set their ports
    for (auto& it : m_blocks)
    {
      if (it.getType() == BT_INPUT)
      {
        v.emplace_back(it.getID());
        p1 = it.getPort(Ports::P_OUTPUT);
        if (p1 == nullptr)
          throw CBlockEditorException("Block hasnt output port", EErrorCode::E_UI_NOT_CON);
        //p1->setPortName(TN_NUMBER1);
        p1->setPortValue(it.getValue());
      }
    }

    /// Then keep iteration over other blocks while any of port values changes
    while (true)
    {
      changed = false;
      for (auto& it : m_blocks)
      {
        // Weve already been in this block, skip it
        if (std::find(v.begin(), v.end(), it.getID()) != v.end())
        {
          continue;
        }

        if (it.getType() != BT_INPUT)
        {

          // Ma oba vstupni porty ? Pokud ne chyba

          if (!it.hasPort(Ports::P_INPUT1) || !it.hasPort(Ports::P_INPUT2))
            throw CBlockEditorException(
              "Input value missing for some blocks. Make sure all input ports are either connected or have a value assigned.",
               EErrorCode::E_UI_NOT_CON);

          // Jsou oba vstupni porty s hodnotou ?
          //  - Nastav changed = true;
          //  - Proved operaci s hodnotami
          //  - Nastav hodnotu vystupnimu portu
          //  - ! Zkontrolovat jestli uz se v tomto bloku hodnota nepocitala (priznak)

          p1 = it.getPort(Ports::P_INPUT1);
          p2 = it.getPort(Ports::P_INPUT2);

          if (p1->hasValue() && p2->hasValue())
          {
            // Unset values for next runs
            p1->unsetValue();
            p2->unsetValue();

            // Add ID of block to buffer
            v.emplace_back(it.getID());
            // Weve changed any port, keep iterating
            changed = true;
            PortValue pv = it.performOperation(p1->getPortValue(), p2->getPortValue());

            if (it.getTypeName() == "INT" || it.getTypeName() == "HEX")
            {
              pv = static_cast<int>(pv);
            }

            m_actions.push_back(std::move(CBlockAction{it.getID(), pv}));

            p1 = it.getPort(Ports::P_OUTPUT);
            if (p1 == nullptr)
            {
              continue;
            }
            //p1->setPortName(TN_NUMBER1);
            p1->setPortValue(pv);
          }
          else
          {
            continue;
          }
        }
      }
      if (!changed) break;
    }

    // Did we check all blocks in sheme ? If not there is cycle
    if (m_blocksInScheme != v.size())
    {
      throw CBlockEditorException("Detected cycle in the scheme", EErrorCode::E_UI_CYCLE);
    }

    return this->m_actions;
  }

}