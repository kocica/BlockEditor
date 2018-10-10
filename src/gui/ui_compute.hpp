/**
 * @file    ui_compute.hpp
 * @date    14/04/2018
 * @author  Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 * @author  Filip Kocica <xkocic01@fit.vutbr.cz>
 */

#pragma once

#include "mainwindow.hpp"
#include "ui_blockscheme.hpp"

namespace gui {

    /// Takes care of computation displaying.
    class Compute
    {
    public:
        Compute(MainWindow *ui) : ui(ui) {}

        bool run_computation();      
        void stop_computation();
        bool next_step();

    private:
        MainWindow *ui;

        /// computation in progress flag
        bool computation_running = false;

        /// Queue of remaining results for stepping
        BlockEditorLogic::CBlockScheme::ActionBuffer steps_to_go;

    };

}
