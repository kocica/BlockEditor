/**
 * @file    ui_port.cpp
 * @date    14/04/2018
 * @author  Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 * @author  Filip Kocica <xkocic01@fit.vutbr.cz>
 * @brief   Implementation of ports and their editing.
 */

#include "mainwindow.hpp"
#include "ui_port.hpp"
#include "ui_connection.hpp"
#include <QMenu>
#include <QInputDialog>
#include <QAction>
#include <QMessageBox>
#include <QSpinBox>
#include "ui_blockscheme.hpp"

using namespace gui;

Port * Port::port_selected = NULL;

/**
 * @brief Port ctor
 * @param parent    Parent widget.
 * @param ui        MainWindow pointer.
 * @param part_of   Block in which this port is located.
 * @param n         Port number.
 * @param offx
 * @param offy
 */
Port::Port(QWidget *parent, MainWindow *ui, Block* part_of, PortType type, int offx, int offy) : QPushButton(parent), ui(ui), type(type), size(PORT_SIZE), part_of(part_of)
{
    this->resize(size, size);
    QPoint loc (offx,offy);
    this->move(loc);
    this->setStyleSheet(color_default + "outline: none;");
    this->setText(">>");
    this->show();

    label_value_info =  new QLabel(ui);
}

/**
 * @brief Port dtor
 * @details Deletes outgoing connection.
 */
Port::~Port()
{
    // unselect if selected
    if (port_selected == this)
        unselect();


    if (connected())
        delete connected_by;

    label_value_info->hide();
    delete label_value_info;
}

/**
 * @brief Allows: Block deletion. Connection creation and deletion. Port value assignment and removal.
 * @param event Event type.
 */
void Port::mousePressEvent(QMouseEvent *event)
{
    // editing allowed
    if (ui->can_edit())
    {
        if (event->button() == Qt::LeftButton)
        {
            // delete block
            if (ui->cursor->get_ctype() == CURSOR_DELETE)
            {
                delete parentWidget();
            }
            // connection creation
            else
            {
                // unselect if clicked a second time
                if (port_selected == this)
                {
                    unselect();
                }
                // select if nothing selected and not connected already
                else if (port_selected == NULL && !connected())
                {
                    select();
                }
                // select the second port (one is selected already)
                else if (port_selected != NULL)
                {
                    if (get_block()->getTypeName() != port_selected->get_block()->getTypeName()) // different value types
                    {
                        QMessageBox err;
                        err.critical(0, "Invalid connection", "Can't connect ports with different value types.");
                        err.setFixedSize(500,200);

                    }
                    else if (get_block() == port_selected->get_block()) // connecting with the same block
                    {
                        QMessageBox err;
                        err.critical(0, "Invalid connection", "Can't connect ports inside of the same block.");
                        err.setFixedSize(500,200);
                    }
                    else if (get_type() == OUT && port_selected->get_type() == OUT) // connecting OUT-OUT
                    {
                        QMessageBox err;
                        err.critical(0, "Invalid connection", "Can't connect two output ports.");
                        err.setFixedSize(500,200);
                    }
                    else if (get_type() != OUT && port_selected->get_type() != OUT) // connecting IN-IN
                    {
                        QMessageBox err;
                        err.critical(0, "Invalid connection", "Can't connect two input ports.");
                        err.setFixedSize(500,200);
                    }
                    else if (connected() || port_selected->connected()) // already connected
                    {
                        QMessageBox err;
                        err.critical(0, "Invalid connection", "Port is already connected.");
                        err.setFixedSize(500,200);
                    }

                    // valid connection selected
                    else
                    {
                        new Connection(this->parentWidget()->parentWidget(), ui, port_selected, this);
                        port_selected->unselect();
                    }
                }
            }

        }
        // right click popup menu
        else if (event->button() == Qt::RightButton)
        {
            QMenu contextMenu(tr("Context menu"), this);
            contextMenu.setStyleSheet("background-color: rgb(150,150,150);");

            QAction a_unselect("Unselect port", this);
            QAction a_setval("Set value", this);
            QAction a_editval("Edit value", this);
            QAction a_remval("Remove value", this);
            QAction a_discon("Disconnect", this);

            if (port_selected != NULL)
            {
                connect(&a_unselect, SIGNAL(triggered()), port_selected, SLOT(unselect()));
                contextMenu.addAction(&a_unselect);
            }

            if (!connected())
            {
                if (this->type != OUT)
                {
                    // value set and remove if not connected and not an output port

                    if (!has_value())
                    {
                        connect(&a_setval, SIGNAL(triggered()), this, SLOT(set_value()));
                        contextMenu.addAction(&a_setval);
                    }
                    else
                    {
                        connect(&a_editval, SIGNAL(triggered()), this, SLOT(edit_value()));
                        contextMenu.addAction(&a_editval);

                        connect(&a_remval, SIGNAL(triggered()), this, SLOT(remove_value()));
                        contextMenu.addAction(&a_remval);
                    }
                }
            }
            // connection removal through ports
            else
            {
                connect(&a_discon, SIGNAL(triggered()), this, SLOT(disconnect_triggered()));
                contextMenu.addAction(&a_discon);
            }
            contextMenu.exec(mapToGlobal(event->pos()));
        }
    }
}

/// Show an info label on mouse hover.
void Port::enterEvent(QEvent *)
{
    if (has_value() && label_value_info->isHidden())
    {
        QPoint p = this->mapTo(ui, this->pos());

        QString val = part_of->value_to_string(get_value());

        p.ry() -= label_value_info->height();
        p.rx() += size;

        label_value_info->setStyleSheet("background-color: white; border: 1px solid black; font-size:12px;");
        label_value_info->setText("Value: " + val);
        label_value_info->setMargin(5);
        label_value_info->adjustSize();
        label_value_info->setAlignment(Qt::AlignCenter);
        label_value_info->move(p);
        label_value_info->show();
    }
}

/// Hide the info label on mouse leave.
void Port::leaveEvent(QEvent *)
{
    label_value_info->hide();
}

/// Set this port as selected. Being one of the two ports needed to create a connection.
void Port::select()
{
    port_selected = this;
    this->setStyleSheet(color_selected);
}

/// Unselect the current port.
void Port::unselect()
{
    port_selected = NULL;

    if (has_value())
        this->setStyleSheet(color_has_value);
    else
        this->setStyleSheet(color_default);
}

/// Set this port as an input port.
void Port::set_value()
{
    if (port_selected == this)
        unselect();

    double in;
    bool ok;

    if (part_of->getTypeName() == FLOAT)
    {
      // get float value from the user
      in = QInputDialog::getDouble(this->parentWidget()->parentWidget(), "Set port value", "Enter double value:",
                                   0,-2147483647, 2147483647, 10, &ok);
    }
    else if (part_of->getTypeName() == INT)
    {
      // get  int value from the user
      in = QInputDialog::getInt(this->parentWidget()->parentWidget(), "Set port value", "Enter integer value:",
                                0, -2147483647, 2147483647, 1, &ok);
    }
    else
    {
      // get hex value from the user
      QInputDialog dialog;
      bool bStatus;
      dialog.setWindowTitle("Set port value");
      dialog.setLabelText("Enter hexadecimal value:");
      dialog.setIntValue(0);
      dialog.setIntRange(-2147483647, 2147483647);
      QSpinBox *spinbox = dialog.findChild<QSpinBox*>();
      spinbox->setDisplayIntegerBase(16);

      if (dialog.exec() == QDialog::Accepted)
        in = spinbox->text().toUInt(&bStatus,16);
      else
        return;
    }

    if (!ok)
        return;

    this->value = in;
    this->in_port = true;
    this->setStyleSheet(color_has_value);

    // set the value in the logic block scheme aswell
    ui->block_scheme->set_port_value(this);
}

/// Set this port as an input port. Used when loading from a save file.
void Port::set_value(double val)
{
    this->value = val;
    this->in_port = true;
    this->setStyleSheet(color_has_value);

    // set port value in the logic block scheme
    ui->block_scheme->set_port_value(this);
}

/// Remove input value from this port.
void Port::remove_value()
{
    this->value = 0;
    this->in_port = false;
    this->setStyleSheet(color_default);

    // remove port value from the logic block scheme aswell
    ui->block_scheme->remove_port_value(this);
}

/// Edit input value of this port.
void Port::edit_value()
{
    remove_value();
    set_value();
}

/// Slot for connection deletion
void Port::disconnect_triggered()
{
    delete connected_by;
}


/// Set connection information
void Port::port_connect(Port *to, Connection *by)
{
    connection_exists = true;
    connected_by = by;
    connected_to = to;
}

/// Remove connection information
void Port::port_disconnect()
{
    connection_exists = false;
    connected_by = NULL;
    connected_to = NULL;
}
