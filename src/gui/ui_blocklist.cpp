/**
 * @file    ui_blocklist.cpp
 * @date    14/04/2018
 * @author  Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 * @author  Filip Kocica <xkocic01@fit.vutbr.cz>
 */

#include "ui_blocklist.hpp"
#include <algorithm>
#include <iostream>

using namespace gui;

/**
 * @brief   Remove a block from the list.
 * @param id ID of the block to remove.
 */
void BlockList::remove(int id)
{
    for (auto i = block_list.begin(); i != block_list.end(); i++)
    {
      if ((*i)->get_id() == id)
      {
          block_list.erase(i);
          return ;
      }
    }

    std::cerr << "Failed to remove block with ID: " << id << "." << std::endl;
}

/**
 * @brief Find a block in the list.
 * @param id ID of the block to find.
 * @return Pointer to the block.
 */
Block* BlockList::find(int id)
{
    for (auto i : block_list)
    {
        if (i->get_id() == id)
            return i;
    }
    return NULL;
}

/**
 * @brief Deletes all blocks inside of the list. "Delete" meaning physicaly destroy
 */
void BlockList::delete_all()
{
    while (!block_list.empty())
    {
        delete (block_list.back());
    }
}
