/**
 * @file    ui_block.cpp
 * @date    14/04/2018
 * @author  Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 * @author  Filip Kocica <xkocic01@fit.vutbr.cz>
 * @brief   Implementation of blocks and their editing.
 */

#include "ui_block.hpp"
#include "ui_blocktype.hpp"
#include <QMenu>
#include <QAction>

using namespace gui;

/**
 * @brief   Initializes a new block and creates its ports.
 * @details Adds the block into the block_list and into the logic block_scheme.
 * @param parent Parent widget.
 * @param ui    MainWindow pointer.
 * @param t     Type of the block.
 * @param vt    Type of the block value.
 * @param loc   Screen location.
 */
Block::Block(QWidget *parent, MainWindow* ui, BlockType t, ValueType vt, QPoint loc)
    : QPushButton(parent), ui(ui), type(t), value_type(vt)
{
    // add into the logic block scheme
    this->id = ui->block_scheme->add_block(type, value_type);

    if (id == -1)
    {
        std::cerr << "Block creation failed" << std::endl;
        return;
    }

    // add into the block list
    ui->block_list->add(this);

    // register tap and hold
    grabGesture(Qt::TapAndHoldGesture);
    QTapAndHoldGesture::setTimeout(100);

    // snap to left border
    if (loc.x() < size/2)
        loc.rx() = size/2;

    // snap to top border
    if (loc.y() < size/2)
        loc.ry() = size/2;

    // extend the frame if needed
    if (loc.x() > parent->minimumWidth() - size/2)
        parent->setMinimumWidth(loc.x() + size/2);
    if (loc.y() > parent->minimumHeight() - size/2)
        parent->setMinimumHeight(loc.y() + size/2);


    this->resize(size, size);
    loc.rx() -= size/2;
    loc.ry() -= size/2;
    this->move(loc);
    this->setStyleSheet(color_default + "font-size: 11px; border: 1px solid black; outline: none;");
    set_text(type, value_type);
    this->show();

    // create ports
    port_one = new Port(this, ui, this, TOP_IN, 0, 0);
    port_two = new Port(this, ui, this, BOT_IN, 0, size - PORT_SIZE);
    port_out = new Port(this, ui, this, OUT, size - PORT_SIZE, size/2 - PORT_SIZE/2);
}

/**
 * @details Removes the block from the block_list and from the logic block_scheme.
 *          Deletes block ports and result labels.
 */
Block::~Block()
{
    // destroy ports
    delete port_one;
    delete port_two;
    delete port_out;

    // delete result labels
    if (has_result())
        hide_result();

    // remove from the block list
    ui->block_list->remove(id);

    // remove from the logic block scheme
    ui->block_scheme->remove_block(id);
}

/**
 * @brief Set the block display text based on block type and value type.
 * @param t Type of the block.
 */
void Block::set_text(BlockType t, ValueType vt)
{
    switch(t) {
        case BLOCK_ADD:
            this->setText("+\n" +  valtype_to_str(vt));
            break;
        case BLOCK_SUB:
            this->setText("-\n" + valtype_to_str(vt));
            break;
        case BLOCK_MUL:
            this->setText("*\n" + valtype_to_str(vt));
            break;
        case BLOCK_DIV:
            this->setText("/\n" + valtype_to_str(vt));
            break;
        case BLOCK_POW:
            this->setText("^\n" + valtype_to_str(vt));
            break;
        default:
            std::cerr << "Warning: Block::set_text: Unknown block type." << std::endl;
            this->setText("?\n" + valtype_to_str(vt));
            break;
    }
}

/**
 * @brief Allows block repositioning using mouse dragging.
 * @param event Not used.
 */
void Block::mouseMoveEvent(QMouseEvent *event)
{
    (void)event;

    // click and hold registered
    if (hold_triggered)
    {
            QPoint mouse_loc = QCursor::pos();
            mouse_loc = parentWidget()->mapFromGlobal(mouse_loc);

            // snap to left border
            if (mouse_loc.x() < size/2)
                mouse_loc.rx() = size/2;

            // snap to top border
            if (mouse_loc.y() < size/2)
                mouse_loc.ry() = size/2;

            // extend frame if needed
            if (mouse_loc.x() > parentWidget()->minimumWidth() - size/2)
                parentWidget()->setMinimumWidth(mouse_loc.x() + size/2);
            if (mouse_loc.y() > parentWidget()->minimumHeight() - size/2)
                parentWidget()->setMinimumHeight(mouse_loc.y() + size/2);

            mouse_loc.rx() -= size/2;
            mouse_loc.ry() -= size/2;
            move(mouse_loc);

            // reposition connection lines
            if (port_one->connected())
                port_one->get_connection()->reposition();
            if (port_two->connected())
                port_two->get_connection()->reposition();
            if (port_out->connected())
                port_out->get_connection()->reposition();
        }
}

/**
 * @brief   Allows the block to be deleted using the mouse.
 * @param event Event type.
 */
void Block::mousePressEvent(QMouseEvent *event)
{
    // left click
    if (event->button() == Qt::LeftButton)
    {
        // reset tap and hold
        hold_triggered = false;

        // delete
        if (ui->cursor->get_ctype() == CURSOR_DELETE && ui->can_edit())
        {
            delete this;
        }
    }
    // right click
    else if (event->button() == Qt::RightButton && ui->can_edit())
    {
        // popup window
        QMenu contextMenu(tr("Context menu"), this);
        contextMenu.setStyleSheet("background-color: rgb(150,150,150);");

        QAction action2("Disconnect all", this);
        if (port_one->connected() || port_two->connected() || port_out->connected())
        {
            connect(&action2, SIGNAL(triggered()), this, SLOT(disconnect_all()));
            contextMenu.addAction(&action2);
        }

        QAction action1("Delete", this);
        connect(&action1, SIGNAL(triggered()), this, SLOT(deleteLater()));
        contextMenu.addAction(&action1);

        contextMenu.exec(mapToGlobal(event->pos()));
    }
}

/**
 * @brief Catches events. Used to get the tap and hold event.
 * @param event Event type.
 * @return True or QWidget::event().
 */
bool Block::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(event));
    return QWidget::event(event);
}

/**
 * @brief Registers mouse click and hold event.
 * @param event Event type.
 * @return True.
 */
bool Block::gestureEvent(QGestureEvent *event)
{
    // tap and hold registered
    if (event->gesture(Qt::TapAndHoldGesture))
    {
        hold_triggered = true;
    }

    return true;
}

/// Convert value (double) to a string
QString Block::value_to_string(double val)
{
    // convert value to string
    std::string s = std::to_string(val);

    // strip trailing zeroes for floats
    for (auto i = s.cend() - 1; i != s.cbegin(); i--)
    {
        if (s.size() == 1)
            break;

        if (*i == ',' || *i == '.')
        {
            s.pop_back();
            break;
        }

        if (*i != '0')
            break;

        if (*i == '0')
            s.pop_back();
    }

    // draw labels
    QString str;

    str = str.fromStdString(s);
    // If its hex value, convert it to 16 base
    if (value_type == HEX)
    {
      str = QString::number(str.toInt(), 16).toUpper();
    }

    return str;
}

/**
 * @brief   Display computation results for this block.
 * @details Creates two label covering the whole block. One shows the blok type and the other shows the result.
 * @param val   Result to be displayed.
 */
void Block::display_result(double val)
{
    QString str = value_to_string(val);

    value_label = new QLabel(this);
    value_label->setText(str);
    value_label->move(0,size/2);
    value_label->resize(size, size/2);
    value_label->setAlignment(Qt::AlignCenter);
    value_label->setStyleSheet("background-color: rgb(255,255,255);border-radius: 2px; border: 2px solid black;");
    value_label->setStyleSheet(
      "border-style: outset;"
      "border-width: 2px;"
      "border-color: red;"
    );
    value_label->show();

    type_label = new QLabel(this);
    type_label->setText("+");
    type_label->resize(size, size/2);
    type_label->setStyleSheet("background-color: rgb(255,255,255);border-radius: 2px; border: 2px solid black;");
    type_label->setStyleSheet(
      "border-style: outset;"
      "border-width: 2px;"
      "border-color: red;"
    );
    type_label->setAlignment(Qt::AlignCenter);

    switch(type) {
        case BLOCK_ADD:
            type_label->setText("+\n" + valtype_to_str(value_type));
            break;
        case BLOCK_SUB:
            type_label->setText("-\n" + valtype_to_str(value_type));
            break;
        case BLOCK_MUL:
            type_label->setText("*\n" + valtype_to_str(value_type));
            break;
        case BLOCK_DIV:
            type_label->setText("/\n" + valtype_to_str(value_type));
            break;
        case BLOCK_POW:
            type_label->setText("^\n" + valtype_to_str(value_type));
            break;
        default:
            std::cerr << "Warning: Block::display_result: Unknown block type." << std::endl;
            this->setText("?\n" + valtype_to_str(value_type));
            break;
    }
    type_label->show();

    connect(&remove_highlight, SIGNAL(timeout()), this, SLOT(removeHighlight()));
    remove_highlight.start(200);

    result_allocated = true;
}

void Block::removeHighlight()
{
  if (result_allocated)
  {
    type_label->setStyleSheet(
      "border-radius: 2px; border: 2px solid black;"
    );
    value_label->setStyleSheet(
      "border-radius: 2px; border: 2px solid black;"
    );
  }
}

/**
 * @brief   Hide the result.
 * @detail  Deletes the two labels created by display_results(), if they exist.
 */
void Block::hide_result()
{
    if (has_result())
    {
        delete type_label;
        delete value_label;
    }
    result_allocated = false;
}

/// Translates value type to a string
QString Block::valtype_to_str(ValueType vt)
{
    switch (vt)
    {
    case INT:
        return "INT";
    case FLOAT:
        return "FLT";
    case HEX:
        return "HEX";
    default:
        std::cerr << "valtype_to_str: unknown value type" << std::endl;
        return "HEX";
    }
}

/// Reimplemented to correct placement
QPoint Block::pos() const
{
    QPoint p = QWidget::pos();
    p.rx() += size/2;
    p.ry() += size/2;
    return p;
}

/// Disconnect all connected ports
void Block::disconnect_all()
{
    if (port_one->connected())
        port_one->disconnect_triggered();

    if (port_two->connected())
        port_two->disconnect_triggered();

    if (port_out->connected())
        port_out->disconnect_triggered();
}
