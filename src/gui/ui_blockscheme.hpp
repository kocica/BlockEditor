/**
 * @file    ui_blockscheme.hpp
 * @date    14/04/2018
 * @author  Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 * @author  Filip Kocica <xkocic01@fit.vutbr.cz>
 */

#pragma once

#include "../BlockScheme.hpp"
#include "ui_port.hpp"

namespace gui {

    /// Interface that translates Gui actions to application logic.
    class BlockScheme
    {
    private:
        MainWindow *ui;

        BlockType bltype_logic2gui(BlockEditorLogic::EBlockType t);
        BlockEditorLogic::EBlockType bltype_gui2logic(BlockType t);
        std::string valtype_gui2logic(ValueType vt);

    public:
        BlockScheme(MainWindow *ui) : ui(ui) {}

        /// Application logic handeling object
        BlockEditorLogic::CBlockScheme block_scheme;

        int add_block(BlockType t, ValueType vt);
        void set_port_value(Port *s);
        void remove_port_value(Port *s);
        void remove_block(int id);
        void add_connection(Port *from, Port *to);
        void remove_connection(Port *from, Port *to);

        void save_scheme(std::string file);
        void load_scheme(QWidget *window, std::string file);
    };
}
