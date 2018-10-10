/**
 * @file    mainwindow.cpp
 * @date    14/04/2018
 * @author  Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 * @author  Filip Kocica <xkocic01@fit.vutbr.cz>
 * @note    QtCreator was used to make the window layout (toolbars and buttons in them,
 *          scrollable frame for block placement). The rest was coded by hand.
 */

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QFileDialog>

#include "ui_block.hpp"
#include "ui_blocktype.hpp"


using namespace gui;

/// Instatiates block_list, coputation, block_scheme and cursor.
/// @note Initialy generated using QtCreator. Modified by hand.

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set white background
    ui->scrollAreaWidgetContents->setStyleSheet("background-color: rgb(230,230,230)");

    block_list = new BlockList;
    computation = new Compute(this);
    block_scheme = new BlockScheme(this);
    cursor = new Cursor;

    setWindowTitle("Block Editor");

    // prevent toolbars from being hidden using right click
    setContextMenuPolicy(Qt::NoContextMenu);

    ui->toolBar->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
    ui->toolBar_2->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);

    ui->actionRun->setIcon(QIcon(":/img/start.png"));
    ui->actionStop->setIcon(QIcon(":/img/stop.png"));
    ui->actionSave->setIcon(QIcon(":/img/save.png"));
    ui->actionLoad->setIcon(QIcon(":/img/load.png"));
    ui->actionStep->setIcon(QIcon(":/img/step.png"));
    ui->actionClear->setIcon(QIcon(":/img/clear.png"));
    ui->actionSelect_none->setIcon(QIcon(":/img/select.png"));
    ui->actionSelect_delete->setIcon(QIcon(":/img/delete.png"));

    // Set default cursor
    ui->actionSelect_none->toggle();
    ui->actionFloat_seleted->toggle();
}

/// Delets all block and destroys block_list, coputation, block_scheme and cursor.
/// @note Initialy generated using QtCreator. Modified by hand.
MainWindow::~MainWindow()
{
    // delete all blocks
    block_list->delete_all();

    delete block_list;
    delete computation;
    delete block_scheme;
    delete cursor;

    delete ui;
}


/**
 * @brief Takes care of block insert and block delete using the mouse.
 * @param event Event type.
 */
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        // place new block
        if (cursor->get_ctype() == CURSOR_PLACE && can_edit())
        {
            QPoint loc = QCursor::pos();
            loc = ui->frame->mapFromGlobal(loc);

            // prevent clicking outside the window
            if (loc.y() < ui->frame->height())
                new Block(ui->frame, this, cursor->get_bltype(), cursor->get_block_value(), loc);
        }
    }
    // reset cursor to select
    else if (event->button() == Qt::RightButton)
    {
        if (!ui->actionSelect_none->isChecked())
            ui->actionSelect_none->toggle();
    }
}

/// Add block selected.
/// @note Declaration generated using QtCreator.
void MainWindow::on_actionSelect_add_toggled(bool enabled)
{
    if (enabled)
    {
        cursor->set_cursor(CURSOR_PLACE);
        cursor->select_block(BLOCK_ADD);
    }
}

/// Sub block selected.
/// @note Declaration generated using QtCreator.
void MainWindow::on_actionSelect_sub_toggled(bool enabled)
{
    if (enabled)
    {
        cursor->set_cursor(CURSOR_PLACE);
        cursor->select_block(BLOCK_SUB);
    }
}

/// Delete cursor selected.
/// @note Declaration generated using QtCreator.
void MainWindow::on_actionSelect_delete_toggled(bool enabled)
{
    if (enabled)
    {
        cursor->set_cursor(CURSOR_DELETE);
        cursor->select_block(BLOCK_NONE);
    }
}

/// Mul block selected.
/// @note Declaration generated using QtCreator.
void MainWindow::on_actionSelect_mul_toggled(bool enabled)
{
    if (enabled)
    {
        cursor->set_cursor(CURSOR_PLACE);
        cursor->select_block(BLOCK_MUL);
    }
}

/// Div block selected.
/// @note Declaration generated using QtCreator.
void MainWindow::on_actionSelect_div_toggled(bool enabled)
{
    if (enabled)
    {
        cursor->set_cursor(CURSOR_PLACE);
        cursor->select_block(BLOCK_DIV);
    }
}

/// Select cursor selected.
/// @note Declaration generated using QtCreator.
void MainWindow::on_actionSelect_none_toggled(bool enabled)
{
    if (enabled)
    {
        cursor->set_cursor(CURSOR_SELECT);
        cursor->select_block(BLOCK_NONE);
    }
}

/// Clear button clicked.
/// @note Declaration generated using QtCreator.
void MainWindow::on_actionClear_triggered()
{
    // stop computation in case one is in progress
    computation->stop_computation();

    // wipe all blocks
    block_list->delete_all();
}

/// Save button clicked.
/// @note Declaration generated using QtCreator.
void gui::MainWindow::on_actionSave_triggered()
{
    // get save file name
    QString file = QFileDialog::getSaveFileName(this, tr("Save file"), "./saved_scheme.txt", tr("Block schemes (*)"));
    if (file.cbegin() == file.cend())
        return;

    // pass the file name to application logic
    block_scheme->save_scheme(file.toStdString());
}

/// Load button clicked.
/// @note Declaration generated using QtCreator.
void gui::MainWindow::on_actionLoad_triggered()
{
    // stop computation in case one is in progress
    computation->stop_computation();

    // get load file name
    QString file = QFileDialog::getOpenFileName(this, tr("Load file"), "./saved_scheme.txt", tr("Block schemes (*)"));
    if (file.cbegin() == file.cend())
        return;

    // pass the file name to application logic
    block_scheme->load_scheme(ui->frame, file.toStdString());
}

/// Disables add/delete for computation.
void gui::MainWindow::disable_editing()
{
    // reset cursor to select
    if (!ui->actionSelect_none->isChecked())
        ui->actionSelect_none->toggle();

    allow_editing = false;
}

/// Enables add/delete.
void gui::MainWindow::enable_editing()
{
    allow_editing = true;
}

/// Run button clicked.
/// @note Declaration generated using QtCreator.
void gui::MainWindow::on_actionRun_triggered()
{
    // if there are any blocks
    if (!block_list->empty())
        if (computation->run_computation())
            disable_editing();
}

/// Step button clicked.
/// @note Declaration generated using QtCreator.
void gui::MainWindow::on_actionStep_triggered()
{
    // if there are any blocks
    if (!block_list->empty())
        if (computation->next_step())
            if (allow_editing)  // Used instead of can_edit, because of popup window
                disable_editing();
}

/// Stop button clicked.
/// @note Declaration generated using QtCreator.
void gui::MainWindow::on_actionStop_triggered()
{
    computation->stop_computation();
    enable_editing();
}

/// Int value type selected.
/// @note Declaration generated using QtCreator.
void gui::MainWindow::on_actionInt_selected_toggled(bool enabled)
{
    if (enabled)
    {
        cursor->slect_block_value(INT);
    }
}

/// Float value type selected.
/// @note Declaration generated using QtCreator.
void gui::MainWindow::on_actionFloat_seleted_toggled(bool enabled)
{
    if (enabled)
    {
        cursor->slect_block_value(FLOAT);
    }
}

/// Hex value type selected.
/// @note Declaration generated using QtCreator.
void gui::MainWindow::on_actionHex_selected_toggled(bool enabled)
{
    if (enabled)
    {
        cursor->slect_block_value(HEX);
    }
}

/// Pow block selected.
/// @note Declaration generated using QtCreator.
void gui::MainWindow::on_actionSelect_pow_toggled(bool enabled)
{
    if (enabled)
    {
        cursor->set_cursor(CURSOR_PLACE);
        cursor->select_block(BLOCK_POW);
    }
}

/// Checks if we can edit scheme (scheme is not running)
/// @return True if scheme editing is allowed.
bool gui::MainWindow::can_edit()
{
  if (!allow_editing)
  {
    QMessageBox err;
    err.critical(0, "Cannot edit", "Modification is not allowed during computation. Press 'Stop' before editing.");
    err.setFixedSize(500,200);
  }

  return allow_editing;
}
