/**
 * @file   mainwindow.hpp
 * @date   14/04/2018
 * @author Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 * @author Filip Kocica <xkocic01@fit.vutbr.cz>
 */

#pragma once

#include <QMainWindow>
#include <QPainter>
#include <QComboBox>
#include <QMessageBox>

#include "ui_forward_declare.hpp"

#include "ui_cursor.hpp"
#include "ui_compute.hpp"
#include "ui_blocklist.hpp"
#include "ui_blockscheme.hpp"

namespace Ui {
class MainWindow;
}

/// namespace for Gui related classes, types, functions,...
namespace gui {

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

        Compute *computation;       ///< The only Compute instance in the program.
        BlockList *block_list;      ///< The only BlockList instance in the program.
        BlockScheme *block_scheme;  ///< The only BlockSheme instance in the program.
        Cursor *cursor;             ///< The only Cursor instance in the program.

        bool can_edit();

    private slots:

        void mousePressEvent(QMouseEvent *);
        void on_actionSelect_none_toggled(bool);
        void on_actionSelect_delete_toggled(bool);
        void on_actionSelect_add_toggled(bool);
        void on_actionSelect_sub_toggled(bool);
        void on_actionSelect_mul_toggled(bool);
        void on_actionSelect_div_toggled(bool);
        void on_actionClear_triggered();
        void on_actionSave_triggered();
        void on_actionLoad_triggered();
        void on_actionRun_triggered();
        void on_actionStep_triggered();
        void on_actionStop_triggered();
        void on_actionInt_selected_toggled(bool);
        void on_actionFloat_seleted_toggled(bool);
        void on_actionHex_selected_toggled(bool);
        void on_actionSelect_pow_toggled(bool);

    private:
        Ui::MainWindow *ui; ///< pointer to the main window

        void disable_editing();
        void enable_editing();

        bool allow_editing = true; ///< used in enable/disable editing
    };
}
