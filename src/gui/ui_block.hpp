/**
 * @file    ui_block.hpp
 * @date    14/04/2018
 * @author  Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 * @author  Filip Kocica <xkocic01@fit.vutbr.cz>
 */

#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <QTapAndHoldGesture>
#include <QLabel>
#include <QTimer>

#include "mainwindow.hpp"
#include "ui_port.hpp"
#include "ui_blocktype.hpp"

namespace gui {

    /// Represents a block. Blocks are inserted into a scheme and connected with connections.
    class Block : public QPushButton
    {
        Q_OBJECT

    public:
        Block(QWidget *parent, MainWindow* ui, BlockType t, ValueType vt, QPoint mouse_loc);
        virtual ~Block();

        /// @return Block type
        BlockType get_type() {return type;}

        Port *port_one = NULL;  ///< First input port
        Port *port_two = NULL;  ///< Second input port
        Port *port_out = NULL;  ///< Output port

        /// @return Block id
        int get_id() const {return id;}
        /// @return Result value as string
        QString getValueString() {return value_label->text();}
        /// @return Result type as string
	QString getTypeString() {return type_label->text();}
        /// @return Value type
	ValueType getTypeName() {return value_type;}
        QString value_to_string(double val);

        void display_result(double val);
        void hide_result();
        /// @return True if there are results shown on the block
        bool has_result() {return result_allocated;}

        /// Default background color
        const QString color_default = "background-color: white;";

        QPoint pos() const;

    public slots:

        void mouseMoveEvent(QMouseEvent *);
        void mousePressEvent(QMouseEvent *);
        void disconnect_all();

    private slots:
        void removeHighlight();

    private:
        MainWindow *ui;

        BlockType type;
        int id;

        QTimer remove_highlight;

        bool result_allocated = false;
        QLabel *value_label = NULL; ///< to show compute results
        QLabel *type_label = NULL; ///< to show compute results

        ValueType value_type; ///< Type of the block value

        void set_text(BlockType t, ValueType vt);

        bool gestureEvent(QGestureEvent *event);
        bool hold_triggered = false; ///< tap and hold event flag

        const int size = 70;    /// predefined block size

        QString valtype_to_str(ValueType vt);

    protected:
        virtual bool event(QEvent *event);
    };
}
