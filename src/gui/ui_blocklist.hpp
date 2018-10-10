/**
 * @file    ui_blocklist.hpp
 * @date    14/04/2018
 * @author  Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 * @author  Filip Kocica <xkocic01@fit.vutbr.cz>
 */

#pragma once

#include "ui_block.hpp"

namespace gui {

    /// List of all currently existing blocks.
    class BlockList
    {
    public:
        void add(Block* b){block_list.push_back(b);}
        Block* find(int id);
        void remove(int id);
        void delete_all();
        bool empty() {return block_list.empty();}

        std::vector<Block*> block_list;
    };
}
