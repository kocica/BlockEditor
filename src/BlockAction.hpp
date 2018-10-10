/**
 *		@file 		BlockAction.hpp
 *		@date 		15/04/2018
 *		@author 	Filip Kocica <xkocic01@fit.vutbr.cz>
 *    @author   Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 *		@brief 		Implementation of action to show in UI in block scheme
 */

#pragma once

#include "Port.hpp"

///
/// Namespace with implementation of logic of an application
///
namespace BlockEditorLogic
{
  /**
   * @brief Class with info about one action in running scheme
   */
  class CBlockAction
  {
  public:
    CBlockAction() = delete;
    CBlockAction(ID, PortValue pv = 0.0);
    ~CBlockAction() = default;

    ID              getID() const;
    PortValue       getValue() const;

  private:
    ID            m_id; /**< ID of block */
    PortValue     m_pv; /**< In some cases we need value */
  };
}