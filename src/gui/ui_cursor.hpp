/**
 * @file    ui_cursor.hpp
 * @date    14/04/2018
 * @author  Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 * @author  Filip Kocica <xkocic01@fit.vutbr.cz>
 */

#pragma once

#include "ui_blocktype.hpp"
#include <qstring.h>

namespace gui {

    /// Enum of possible cursors.
    enum CursorType
    {
        CURSOR_SELECT,
        CURSOR_DELETE,
        CURSOR_PLACE
    };

    /// Manages the state of the application cursor.
    class Cursor {

    private:
        CursorType cursor_selected; ///< What type of cursor is currently selected
        BlockType block_selected;   ///< What type of block is currently selected
        ValueType block_value_type;   ///< What type of block value to use

    public:
        /// Change cursor type.
        void set_cursor(CursorType t) {cursor_selected = t;}

        /// @return Current cursor type.
        CursorType get_ctype() {return cursor_selected;}

        /// Change what block is selected.
        void select_block(BlockType t) {block_selected = t;}

        /// @return Currently selected block type.
        BlockType get_bltype() {return block_selected;}

        /// Change block value type selected
        void slect_block_value(ValueType s) {block_value_type = s;}

        /// @return Currently selected block value type.
        ValueType get_block_value() {return block_value_type;}
    };
}
