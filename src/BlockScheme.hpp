/**
 *		@file 		BlockScheme.hpp
 *		@date 		02/03/2018
 *		@author 	Filip Kocica <xkocic01@fit.vutbr.cz>
 *    @author   Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 *		@brief    Implementation of whole scheme, storing all
 *              blocks, actions in scheme, save/load operations
 */

#pragma once

#include <iostream>
#include <deque>
#include <memory>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "BlockAction.hpp"
#include "Block.hpp"
#include "Error.hpp"
#include "TypeName.hpp"
#include "BlockEditorException.hpp"
#include "SchemePart.hpp"

///
/// Namespace with implementation of logic of an application
///
namespace BlockEditorLogic
{
  /// Forward declaration
  struct SchemePart;

  ///
  /// Class which holds data about whole scheme and implements all actions
  /// with scheme such as save, open saved, etc.
  ///
  class CBlockScheme
  {
  public:
    CBlockScheme();
    virtual ~CBlockScheme();

    /// Alliases
    using BlockBuffer  = std::deque<CBlock>;
    using ActionBuffer = std::deque<CBlockAction>;
    using PartBuffer   = std::deque<SchemePart>;
    using BlockCoord   = std::pair<ID, std::pair<int, int>>;
    using Coords       = std::deque<BlockCoord>;

    /// Functions called from GUI
    void          saveScheme(Coords, std::string&);
    PartBuffer    loadScheme(std::string&);

    ActionBuffer  run();

    /* ! Used from GUI when loading saved scheme ! */
    void          setID(ID);

    ID            addBlock(EBlockType, TypeName);
    void          removeBlock(ID);

    void          addInputValue(ID, PortValue, Ports);
    void          removeInputValue(ID, Ports);

    void          addPort(ID, ID, Ports);
    void          removePort(ID, ID, Ports);

    /// Debug
    void          debug_printBlocks() const;

    void          clearScheme();
  protected:
    PartBuffer    getParts() const;
    std::pair<ID, Ports> findBlockByPortID(ID, ID) const;
    bool          isInput(ID) const;
    PortValue     getInputValue(ID) const;

  private:
    unsigned long             m_blockCounter;     /**< Counter of block ID's in scheme */
    unsigned long             m_portCounter;      /**< Counter of port ID's in scheme */
    unsigned long             m_blocksInScheme;   /**< Counter of blocks in scheme */
    BlockBuffer               m_blocks;           /**< Buffer of blocks used in scheme */
    ActionBuffer              m_actions;          /**< Buffer of actions in scheme */
  };
}