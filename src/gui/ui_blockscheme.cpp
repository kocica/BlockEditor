/**
 * @file    ui_blockscheme.cpp
 * @date    14/04/2018
 * @author  Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 * @author  Filip Kocica <xkocic01@fit.vutbr.cz>
 * @brief   Impelementation of connecting Gui and the application logic.
 */

#include "ui_blockscheme.hpp"
#include <QPoint>
#include <QMessageBox>

#include "ui_connection.hpp"
#include "cassert"

#include "../Port.hpp"
#include "../BlockAction.hpp"
#include "../BlockEditorException.hpp"
#include "../TypeName.hpp"

using namespace gui;

/**
 * @brief   Insert a block into the logic block scheme.
 * @param t Type of the block to insert.
 * @param vt Value type of the block.
 * @return  Block id assigned by the aplication logic. Negative on failure.
 */
int BlockScheme::add_block(BlockType t, ValueType vt)
{
    try
    {
      return block_scheme.addBlock(bltype_gui2logic(t), valtype_gui2logic(vt));
    }
    catch(BlockEditorLogic::CBlockEditorException& e)
    {
      QMessageBox err;
      err.critical(0, "ERROR", e.what());
      err.setFixedSize(500,200);
    }

    return -1;
}

/**
 * @brief Insert value into a port in the logic block scheme.
 * @param s Port to set the value to.
 */
void BlockScheme::set_port_value(Port *s)
{
    BlockEditorLogic::Ports in_type;
    if (s->get_type() == TOP_IN)
        in_type = BlockEditorLogic::Ports::P_INPUT1;
    else if (s->get_type() == BOT_IN)
        in_type = BlockEditorLogic::Ports::P_INPUT2;
    else
        assert(0 && "Invalid invalid port type."); // interface connection control

    try
    {
      block_scheme.addInputValue(s->get_block()->get_id(), s->get_value(), in_type);
    }
    catch(BlockEditorLogic::CBlockEditorException& e)
    {
      QMessageBox err;
      err.critical(0, "ERROR", e.what());
      err.setFixedSize(500,200);
    }
}

/**
 * @brief Remove value from a port in the logic block scheme.
 * @param s Port to remove the value from.
 */
void BlockScheme::remove_port_value(Port *s)
{
    try
    {
        block_scheme.removeInputValue(s->get_block()->get_id(),
        (s->get_type() == TOP_IN ? BlockEditorLogic::Ports::P_INPUT1 : BlockEditorLogic::Ports::P_INPUT2));
    }
    catch(BlockEditorLogic::CBlockEditorException& e)
    {
          QMessageBox err;
          err.critical(0, "ERROR", e.what());
          err.setFixedSize(500,200);
    }
}

/**
 * @brief Remove a block from the logic block scheme.
 * @param id ID of the block to delete.
 */
void BlockScheme::remove_block(int id)
{
    try
    {
      block_scheme.removeBlock(id);
    }
    catch(BlockEditorLogic::CBlockEditorException& e)
    {
      QMessageBox err;
      err.critical(0, "ERROR", e.what());
      err.setFixedSize(500,200);
    }
}

/**
 * @brief Insert a connection into the logic block scheme.
 * @param from  Output port.
 * @param to    Input port.
 */
void BlockScheme::add_connection(Port *from, Port *to)
{
    int id_from, id_to;
    BlockEditorLogic::Ports in_type;

    id_from = from->get_block()->get_id();
    id_to = to->get_block()->get_id();
    if (to->get_type() == TOP_IN)
        in_type = BlockEditorLogic::Ports::P_INPUT1;
    else if (to->get_type() == BOT_IN)
        in_type = BlockEditorLogic::Ports::P_INPUT2;
    else
        assert(0 && "Invalid port type"); // interface connection control

    block_scheme.addPort(id_from, id_to, in_type);
}

/**
 * @brief Remove a connection from the logic block scheme.
 * @param from  Output port.
 * @param to    Input port.
 */
void BlockScheme::remove_connection(Port *from, Port *to)
{
    int id_from, id_to;
    BlockEditorLogic::Ports in_type;

    id_from = from->get_block()->get_id();
    id_to = to->get_block()->get_id();
    if (to->get_type() == TOP_IN)
        in_type = BlockEditorLogic::Ports::P_INPUT1;
    else if (to->get_type() == BOT_IN)
        in_type = BlockEditorLogic::Ports::P_INPUT2;
    else
        assert(0 && "Invalid port type"); // interface connection control

    try
    {
      block_scheme.removePort(id_from, id_to, in_type);
    }
    catch(BlockEditorLogic::CBlockEditorException& e)
    {
      QMessageBox err;
      err.critical(0, "ERROR", e.what());
      err.setFixedSize(500,200);
    }
}

/**
 * @brief Trigger block scheme saving.
 * @param file File name to save into (with path).
 */
void BlockScheme::save_scheme(std::string file)
{
    BlockEditorLogic::CBlockScheme::Coords coords;

    // put all block locations into a container
    for (auto i : ui->block_list->block_list)
    {
        QPoint p = i->pos();

        int id = i->get_id();
        BlockEditorLogic::CBlockScheme::BlockCoord c {id, {p.x(), p.y()}};
        coords.push_back(c);
    }

    try
    {
      block_scheme.saveScheme(coords, file);
    }
    catch(BlockEditorLogic::CBlockEditorException& e)
    {
      QMessageBox err;
      err.critical(0, "ERROR", e.what());
      err.setFixedSize(500,200);
    }
}

/// Converts Logic block type to gui block type
BlockType BlockScheme::bltype_logic2gui(BlockEditorLogic::EBlockType t)
{
    using namespace BlockEditorLogic;
    switch (t)
    {
    case BT_ADD:
        return BLOCK_ADD;
    case BT_SUB:
        return BLOCK_SUB;
    case BT_MUL:
        return BLOCK_MUL;
    case BT_DIV:
        return BLOCK_DIV;
    case BT_POW:
        return BLOCK_POW;
    default:
        std::cerr << "Warning: bltype_logic2gui: Unknown block type. Returning default type." << std::endl;
        return BLOCK_ADD;
    }
}

/// Converts Gui block type to logic block type
BlockEditorLogic::EBlockType BlockScheme::bltype_gui2logic(BlockType t)
{
    using namespace BlockEditorLogic;
    switch (t)
    {
    case BLOCK_ADD:
        return BT_ADD;
    case BLOCK_SUB:
        return BT_SUB;
    case BLOCK_MUL:
        return BT_MUL;
    case BLOCK_DIV:
        return BT_DIV;
    case BLOCK_POW:
        return BT_POW;
    default:
        std::cerr << "Warning: bltype_gui2logic: Unknown block type. Returning default type." << std::endl;
        return BT_ADD;
    }
}

/**
 * @brief Trigger block scheme loading.
 * @param file File name to load from (with path).
 */
void BlockScheme::load_scheme(QWidget *window, std::string file)
{
    // clear the window by deleting all blocks
    ui->block_list->delete_all();

      using namespace BlockEditorLogic;

      CBlockScheme::PartBuffer pb;

      try
      {
        pb = block_scheme.loadScheme(file);
      }
      catch(BlockEditorLogic::CBlockEditorException& e)
      {
        QMessageBox err;
        err.critical(0, "ERROR", e.what());
        err.setFixedSize(500,200);

        // wipe the incomplete load
        block_scheme.clearScheme();
        return;
      }

      BlockType t;

      /// First create all blocks
      for (auto& it : pb)
      {
        /// Logic block type to gui block type
        t = bltype_logic2gui(it.m_bt);

        /// Save loaded position
        QPoint p(it.m_coords.first, it.m_coords.second);

        /// Set ID counter correctly
        block_scheme.setID(it.m_blockID);

        ValueType vt;
        if (it.m_tn == "FLT") vt = FLOAT;
        else if (it.m_tn == "INT") vt = INT;
        else vt = HEX;

        /// Add new block
        new Block(window, ui, t, vt, p);
      }

      /// In second iteration connect blocks together
      for (auto& it : pb)
      {
          Block *bl = ui->block_list->find(it.m_blockID);

        if (it.m_inPort1 == VALUE)
        {
            bl->port_one->set_value(it.m_inVal1);
        }
        else if (it.m_inPort1 == CONNECTION)
        {
            Block *in_bl = ui->block_list->find(it.m_inBlock1);
            new Connection(window, ui, in_bl->port_out, bl->port_one);
        }

        if (it.m_inPort2 == VALUE)
        {
          bl->port_two->set_value(it.m_inVal2);
        }
        else if (it.m_inPort2 == CONNECTION)
        {
          Block *in_bl = ui->block_list->find(it.m_inBlock2);
          new Connection(window, ui, in_bl->port_out, bl->port_two);
        }
      }
}

/// Converts Gui value type to logic value type
std::string BlockScheme::valtype_gui2logic(ValueType vt)
{
    switch (vt)
    {
    case INT:
        return BlockEditorLogic::TN_INTEGER;
    case FLOAT:
        return BlockEditorLogic::TN_FLOAT;
    case HEX:
        return BlockEditorLogic::TN_HEXA;
    default:
        assert(0 && "Invalid value type"); // interface connection control
    }
}
