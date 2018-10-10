/**
 * @file    ui_compute.cpp
 * @date    14/04/2018
 * @author  Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 * @author  Filip Kocica <xkocic01@fit.vutbr.cz>
 * @brief   Implementation of computation displaying
 */

#include "ui_compute.hpp"
#include "ui_blocklist.hpp"
#include <QObject>
#include <QMessageBox>
#include <QTime>

using namespace gui;


void delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

/**
 *  Run the computation and display all results at once (or all remaining results)
 *  @return True on success, False on exception
 */
bool Compute::run_computation()
{
    // start computation and display all at once
    if (!computation_running)
    {
        computation_running = true;

        BlockEditorLogic::CBlockScheme::ActionBuffer buff;
        try
        {
          buff = ui->block_scheme->block_scheme.run();
        }
        catch(BlockEditorLogic::CBlockEditorException& e)
        {
          QMessageBox err;
          err.critical(0, "Computation error", e.what());
          err.setFixedSize(500,200);
          computation_running = false;
          return false;
        }

        for (auto i : buff)
        {
            Block* bl = ui->block_list->find(i.getID());
            bl->display_result(i.getValue());
        }
    }
    // display remaining (finish stepping)
    else
    {
        for (auto i : steps_to_go)
        {
            Block* bl = ui->block_list->find(i.getID());
            bl->display_result(i.getValue());
        }
        steps_to_go.clear();
    }
    return true;
}

/**
 *  Run computation and display results one by one
 *  @return True on success, False on exception
 */
bool Compute::next_step()
{
    // first step setup - run computation
    if (!computation_running)
    {
        computation_running = true;

        try
        {
          steps_to_go = ui->block_scheme->block_scheme.run();
        }
        catch(BlockEditorLogic::CBlockEditorException& e)
        {
          QMessageBox err;
          err.critical(0, "Computation error", e.what());
          err.setFixedSize(500,200);
          computation_running = false;
          return false;
        }
    }

    // no more steps remaining
    if (steps_to_go.empty())
        return true;

    // display next step
    auto step = steps_to_go.front();

    Block* bl = ui->block_list->find(step.getID());
    bl->display_result(step.getValue());

    steps_to_go.pop_front();

    return true;
}

/// Stop the computation and hide all results
void Compute::stop_computation()
{
    if (computation_running)
    {
        computation_running = false;

        // hide results
        for (auto i : ui->block_list->block_list)
        {
            i->hide_result();
        }

        // reset stepping
        steps_to_go.clear();
    }
}
