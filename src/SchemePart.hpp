/**
 *		@file 		SchemePart.hpp
 *		@date 		22/04/2018
 *		@author 	Filip Kocica <xkocic01@fit.vutbr.cz>
 *    @author   Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 *		@brief    SchemePart is structure used for easy load of data from
 *              file & pass'em to the UI
 */

#pragma once

///
/// Namespace with implementation of logic of an application
///
namespace BlockEditorLogic
{
    ///
    /// Used for loadScheme, detects if input port
    ///  - has value
    ///  - is connected to another block
    ///  - or none of them (is empty)
    ///
    enum Port
    {
      EMPTY = 0,
      VALUE,
      CONNECTION
    };

    ///
    /// Struct which holds all data about one scheme block
    /// needed to completely restore scheme
    /// eg. ID, type, position, which ports are connected
    ///     where input & output ports are connected
    ///     values on ports
    ///
    struct SchemePart
    {
      SchemePart(EBlockType bt, ID blockID, std::pair<int, int> coords,
        Port inPort1, Port inPort2, Port outPort, PortValue inVal1,
        PortValue inVal2, ID inBlock1, ID inBlock2, ID outBlock, Ports which,
        TypeName tn)
        :
        m_bt{bt}, m_blockID{blockID}, m_coords{coords}, m_inPort1{inPort1},
        m_inPort2{inPort2}, m_outPort{outPort}, m_inVal1{inVal1}, m_inVal2{inVal2},
        m_inBlock1{inBlock1}, m_inBlock2{inBlock2}, m_outBlock{outBlock}, m_which{which},
        m_tn{tn}
       {

       }

      EBlockType          m_bt;
      ID                  m_blockID;
      std::pair<int, int> m_coords;

      Port                m_inPort1;
      Port                m_inPort2;
      Port                m_outPort;

      PortValue           m_inVal1;
      PortValue           m_inVal2;

      ID                  m_inBlock1;
      ID                  m_inBlock2;
      ID                  m_outBlock;
      Ports               m_which;

      TypeName            m_tn;
    };
}