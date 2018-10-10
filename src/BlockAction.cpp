/**
 *		@file 		BlockAction.cpp
 *		@date 		15/04/2018
 *		@author 	Filip Kocica <xkocic01@fit.vutbr.cz>
 *    @author   Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 *		@brief 		Implementation of action to show in UI in block scheme
 */

#include "BlockAction.hpp"

///
/// Namespace with implementation of logic of an application
///
namespace BlockEditorLogic
{
  CBlockAction::CBlockAction(ID id, PortValue pv)
    : m_id{id}
    , m_pv{pv}
  {

  }

  ID CBlockAction::getID() const
  {
    return this->m_id;
  }

  PortValue CBlockAction::getValue() const
  {
    return this->m_pv;
  }
}