/**
 * @file    ui_connection.cpp
 * @date    14/04/2018
 * @author  Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 * @author  Filip Kocica <xkocic01@fit.vutbr.cz>
 * @brief   Implementation of connections and their editing.
 */

#include "mainwindow.hpp"
#include "ui_connection.hpp"
#include "ui_port.hpp"
#include <QPainter>
#include <QStyleOption>
#include <QMenu>
#include <QAction>
#include <cassert>

using namespace gui;

Line::Line(QWidget *parent, MainWindow *ui, Connection *c) : QWidget(parent), ui(ui), part_of(c)
{
    // enable mouse hover over
    this->setAttribute(Qt::WA_Hover);
    setMouseTracking(true);
}

/// Allows the widget to be show.
void Line::paintEvent(QPaintEvent* event)
{
    (void)event;
    QPainter painter(this);
    QStyleOption StyleOpt;

    StyleOpt.initFrom(this);
    style()->drawPrimitive(QStyle::PE_Widget, &StyleOpt, &painter, this);
}

/**
 * @brief Allows connectinos to be deleted using the mouse.
 * @param event Event type.
 */
void Line::mousePressEvent(QMouseEvent *event)
{
    // editing allowed
    if (ui->can_edit())
    {
        if (event->button() == Qt::LeftButton)
        {
            if (ui->cursor->get_ctype() == CURSOR_DELETE)
            {
                // delete the whole connection
                delete part_of;
            }
        }
        else if (event->button() == Qt::RightButton)
        {
            // popup menu
            QMenu contextMenu(tr("Context menu"), this);
            contextMenu.setStyleSheet("background-color: rgb(150,150,150);");
            QAction action1("Delete", this);
            connect(&action1, SIGNAL(triggered()), part_of, SLOT(delete_triggered()));
            contextMenu.addAction(&action1);
            contextMenu.exec(mapToGlobal(event->pos()));
        }
    }
}

/// Catches mouse hovering over a line to show a info label.
void Line::mouseMoveEvent(QMouseEvent * /* unused */)
{
    part_of->showLineInfo();
}

/** @details    Removes the connetion from the logic block sheme.
 *              Also removes connection information from ports.
 */
Connection::~Connection()
{
    // remove the connection from the logic block scheme
    ui->block_scheme->remove_connection(from, to);

    // set ports as unconnected
    from->port_disconnect();
    to->port_disconnect();

    delete l1;
    delete l2;
    delete l3;
    delete l4;
    delete l5;

    // hide the info label
    labelShowConnectionInfo->hide();
    delete labelShowConnectionInfo;
}

/**
 * @brief Connection ctor
 * @details Adds the connection into the logic block scheme.
 * @param parent
 * @param ui
 * @param pfrom
 * @param pto
 */
Connection::Connection(QWidget *parent, MainWindow *ui, Port* pfrom, Port* pto) : QWidget(parent), ui(ui), from(pfrom), to(pto)
{
    bool createConnection = true;

    assert((from->get_type() == OUT || to->get_type() == OUT) && "Invalid connection ports");

    // normalise to: OUT to IN connection
    if (from->get_type() != OUT)
    {
        this->from = pto;
        this->to = pfrom;
    }

    // remove port values
    if (from->has_value())
        from->remove_value();
    if (to->has_value())
        to->remove_value();

    // add the connection into the logic block shceme
    try
    {
        ui->block_scheme->add_connection(from, to);
    }
    catch(BlockEditorLogic::CBlockEditorException& e)
    {
        QMessageBox err;
        err.critical(0, "ERROR", e.what());
        err.setFixedSize(500,200);
        createConnection = false;
    }

    // Because cannot return from constructor
    if (createConnection)
    {
        // set ports as connected
        from->port_connect(to,this);
        to->port_connect(from,this);

        // create lines that make up the connection
        l1 = new Line(parent, ui, this);
        l2 = new Line(parent, ui, this);
        l3 = new Line(parent, ui, this);
        l4 = new Line(parent, ui, this);
        l5 = new Line(parent, ui, this);
        place_lines();

        // create value info label
        labelShowConnectionInfo = new QLabel( parent );
        connect(&intervalShowConnectionInfo, SIGNAL(timeout()), labelShowConnectionInfo, SLOT(hide()));
    }
}

/**
 * @brief Show a info label on mouse hover.
 */
void Connection::showLineInfo()
{
    QPoint p = l3->pos();
    QString value = "None";
    QString type = "--None"; // Dont remove --

    if (from && from->get_block()->has_result())
    {
      value = from->get_block()->getValueString();
      type = from->get_block()->getTypeString();
    }

    // Remove <type> and <whitespace>
    //     eg. + TYPE
    //     to  TYPE
    type = type.mid(2, type.length());

    labelShowConnectionInfo->setMargin(5);
    labelShowConnectionInfo->setStyleSheet("background-color: white; border: 1px solid black; font-size:12px;");
    labelShowConnectionInfo->setText("Value: " + value + "\n" + "Type: " + type);
    labelShowConnectionInfo->setAlignment(Qt::AlignCenter);
    labelShowConnectionInfo->adjustSize();
    p.rx() -= labelShowConnectionInfo->width();
    p.ry() -= labelShowConnectionInfo->height() / 2;
    labelShowConnectionInfo->move(p);
    labelShowConnectionInfo->show();
    intervalShowConnectionInfo.start(1000);
}

/// Initiates line drawing
void Connection::place_lines()
{
    l1->hide();
    l2->hide();
    l3->hide();
    l4->hide();
    l5->hide();

    QPoint pfrom = from->parentWidget()->mapToParent(from->pos());
    QPoint pto = to->parentWidget()->mapToParent(to->pos());

    pfrom.rx() += PORT_SIZE;
    pfrom.ry() += PORT_SIZE/2 - LINE_THICKNESS/2;
    pto.ry() += PORT_SIZE/2 - LINE_THICKNESS/2;

    // simple left to right connection
    if (pfrom.x() < pto.x())
        draw_basic_line(pfrom, pto);
    // connection looping backwards
    else
    {
        // extend the board the new line would go outside
        if (parentWidget()->minimumWidth() < pfrom.x() + OFFSET_LINE_LEN + LINE_THICKNESS)
            parentWidget()->setMinimumWidth(parentWidget()->minimumWidth() + OFFSET_LINE_LEN + LINE_THICKNESS);
        draw_complex_line(pfrom, pto);
    }
}

/// Draw a line connecting blocks from left to right using only 3 lines.
void Connection::draw_basic_line(QPoint pfrom, QPoint pto)
{
    int width = abs(pfrom.x() - pto.x());
    int height = abs(pfrom.y() - pto.y());

    // not used
    l4->hide();
    l5->hide();

    // draw horizontal lines
    QPoint l1_start;
    l1_start.rx() = pfrom.x();
    l1_start.ry() = pfrom.y();
    l1->setFixedHeight(LINE_THICKNESS);
    l1->setFixedWidth(width/2);
    l1->setStyleSheet(QString("background-color: rgb(0,0,0);"));
    l1->move(l1_start);
    l1->show();

    QPoint l3_start;
    l3_start.rx() = pto.x() - width/2;
    l3_start.ry() = pto.y();
    l3->setFixedHeight(LINE_THICKNESS);
    l3->setFixedWidth(width/2);
    l3->setStyleSheet(QString("background-color: rgb(0,0,0);"));
    l3->move(l3_start);
    l3->show();


    // draw vertical line
    if (pfrom.y() < pto.y())
    {
        QPoint l2_start;
        l2_start.rx() = pfrom.x() + width/2;
        l2_start.ry() = pfrom.y();
        l2->setFixedHeight(height);
        l2->setFixedWidth(LINE_THICKNESS);
        l2->setStyleSheet(QString("background-color: rgb(0,0,0);"));
        l2->move(l2_start);
        l2->show();
    }
    else
    {
        QPoint l2_start;
        l2_start.rx() = pfrom.x() + width/2;
        l2_start.ry() = pfrom.y() - height + LINE_THICKNESS;
        l2->setFixedHeight(height);
        l2->setFixedWidth(LINE_THICKNESS);
        l2->setStyleSheet(QString("background-color: rgb(0,0,0);"));
        l2->move(l2_start);
        l2->show();
    }
}

/// Draw a line connecting blocks from right to left using all 5 lines.
void Connection::draw_complex_line(QPoint pfrom, QPoint pto)
{
    int width = abs(pfrom.x() - pto.x());
    int height = abs(pfrom.y() - pto.y());

    // input ports have a different ofset to avoid colision
    int port_offset = 0;
    if (this->to->get_type() == TOP_IN)
        port_offset = OFFSET_LINE_LEN/2;
    else if (this->to->get_type() == BOT_IN)
        port_offset = -OFFSET_LINE_LEN/2;

    // loop down
    if (pfrom.y() < pto.y())
    {
        // draw horizontal lines
        QPoint l1_start;
        l1_start.rx() = pfrom.x();
        l1_start.ry() = pfrom.y();
        l1->setFixedHeight(LINE_THICKNESS);
        l1->setFixedWidth(OFFSET_LINE_LEN);
        l1->setStyleSheet(QString("background-color: rgb(0,0,0);"));
        l1->move(l1_start);
        l1->show();

        QPoint l3_start;
        l3_start.rx() = pfrom.x() - OFFSET_LINE_LEN - width + port_offset;
        l3_start.ry() = pfrom.y() + height/2;
        l3->setFixedHeight(LINE_THICKNESS);
        l3->setFixedWidth(width + OFFSET_LINE_LEN * 2 - port_offset);
        l3->setStyleSheet(QString("background-color: rgb(0,0,0);"));
        l3->move(l3_start);
        l3->show();

        QPoint l5_start;
        l5_start.rx() = pto.x() - OFFSET_LINE_LEN + port_offset;
        l5_start.ry() = pto.y();
        l5->setFixedHeight(LINE_THICKNESS);
        l5->setFixedWidth(OFFSET_LINE_LEN - port_offset);
        l5->setStyleSheet(QString("background-color: rgb(0,0,0);"));
        l5->move(l5_start);
        l5->show();

        // draw vertical lines
        QPoint l2_start;
        l2_start.rx() = pfrom.x() + OFFSET_LINE_LEN - LINE_THICKNESS;
        l2_start.ry() = pfrom.y();
        l2->setFixedHeight(height/2);
        l2->setFixedWidth(LINE_THICKNESS);
        l2->setStyleSheet(QString("background-color: rgb(0,0,0);"));
        l2->move(l2_start);
        l2->show();

        QPoint l4_start;
        l4_start.rx() = pto.x() - OFFSET_LINE_LEN + port_offset;
        l4_start.ry() = pto.y() - height/2;
        l4->setFixedHeight(height/2);
        l4->setFixedWidth(LINE_THICKNESS);
        l4->setStyleSheet(QString("background-color: rgb(0,0,0);"));
        l4->move(l4_start);
        l4->show();
    }
    // loop up
    else
    {
        // draw horizontal lines
        QPoint l1_start;
        l1_start.rx() = pfrom.x();
        l1_start.ry() = pfrom.y();
        l1->setFixedHeight(LINE_THICKNESS);
        l1->setFixedWidth(OFFSET_LINE_LEN);
        l1->setStyleSheet(QString("background-color: rgb(0,0,0);"));
        l1->move(l1_start);
        l1->show();

        QPoint l3_start;
        l3_start.rx() = pfrom.x() - OFFSET_LINE_LEN - width - port_offset;
        l3_start.ry() = pfrom.y() - height/2;
        l3->setFixedHeight(LINE_THICKNESS);
        l3->setFixedWidth(width + OFFSET_LINE_LEN * 2 + port_offset);
        l3->setStyleSheet(QString("background-color: rgb(0,0,0);"));
        l3->move(l3_start);
        l3->show();

        QPoint l5_start;
        l5_start.rx() = pto.x() - OFFSET_LINE_LEN - port_offset;
        l5_start.ry() = pto.y();
        l5->setFixedHeight(LINE_THICKNESS);
        l5->setFixedWidth(OFFSET_LINE_LEN + port_offset);
        l5->setStyleSheet(QString("background-color: rgb(0,0,0);"));
        l5->move(l5_start);
        l5->show();

        // draw vertical lines
        QPoint l2_start;
        l2_start.rx() = pfrom.x() + OFFSET_LINE_LEN - LINE_THICKNESS;
        l2_start.ry() = pfrom.y() - height/2;
        l2->setFixedHeight(height/2);
        l2->setFixedWidth(LINE_THICKNESS);
        l2->setStyleSheet(QString("background-color: rgb(0,0,0);"));
        l2->move(l2_start);
        l2->show();

        QPoint l4_start;
        l4_start.rx() = pto.x() - OFFSET_LINE_LEN - port_offset;
        l4_start.ry() = pto.y() + LINE_THICKNESS;
        l4->setFixedHeight(height/2);
        l4->setFixedWidth(LINE_THICKNESS);
        l4->setStyleSheet(QString("background-color: rgb(0,0,0);"));
        l4->move(l4_start);
        l4->show();
    }
}

/// Redraws all lines according to the current port positions.
void Connection::reposition()
{
    place_lines();
}

/// Slot for connection deletion
void Connection::delete_triggered()
{
    this->deleteLater();
}
