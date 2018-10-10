/**
 * @file    ui_blocktype.hpp
 * @date    14/04/2018
 * @author  Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 * @author  Filip Kocica <xkocic01@fit.vutbr.cz>
 */

#pragma once

namespace gui {

    /// Enum of block types.
    enum BlockType {
        BLOCK_NONE = -1,
        BLOCK_ADD,
        BLOCK_SUB,
        BLOCK_DIV,
        BLOCK_MUL,
        BLOCK_POW
    };

    /// Enum of block value types
    enum ValueType {
        INT,
        FLOAT,
        HEX
    };
}
