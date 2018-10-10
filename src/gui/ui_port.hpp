/**
 * @file    ui_port.hpp
 * @date    14/04/2018
 * @author  Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 * @author  Filip Kocica <xkocic01@fit.vutbr.cz>
 */

#pragma once

#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>

#include "mainwindow.hpp"
#include "ui_connection.hpp"
#include "ui_block.hpp"

#define PORT_SIZE 20

namespace gui {

    /// Enum of port types.
    enum PortType {
        TOP_IN = 1,
        BOT_IN,
        OUT
    };

    /// Represents a port. Ports are a part of Blocks providing input output using connections or explicit values
    class Port : public QPushButton
    {
        Q_OBJECT

    public:
        Port(QWidget *parent, MainWindow *ui, Block *part_of, PortType type, int offx, int offy);
        virtual ~Port();

        PortType get_type() {return type;}	///< @return Port type
        int get_size() {return size;}		///< @return Pixel size
        Block *get_block() {return part_of;}	///< @return Parent block pointer

        void select();

	/// @return True if the port has a connection.
        bool connected() {return connection_exists;}
        void port_connect(Port *to, Connection *by);
        void port_disconnect();

	/// @return True if the port has a value assigned.
        bool has_value() {return in_port;}
        double get_value() {return value;}	///< @return Assigned value

	/// @return Pointer to a port at the other side of connection.
        Port *get_connect_port() {return connected_to;}
	/// @return Pointer to connection instance
        Connection *get_connection() {return connected_by;}

        void set_value(double val);



    private:
        MainWindow *ui;

        /// Currently selected port. Shared between all ports.
        static Port *port_selected;

        PortType type;
        int size;
        Block *part_of; ///< Block in which the port is located.

        double value;   ///< Set value. For input port without connection.
        bool in_port = false;   ///< If the port has a value set.


        Port *connected_to = NULL;      ///< The other side of a connection.
        Connection *connected_by = NULL;    ///< Connection from this port.
        bool connection_exists = false; ///< connection flag

        const QString color_default = "background-color: rgb(74,107,150);";
        const QString color_selected = "background-color: rgb(71,173,255);";
        const QString color_has_value = "background-color: rgb(30,190,30);";


        QLabel *label_value_info = NULL; ///< Used to display value info

    public slots:
        void mousePressEvent(QMouseEvent *);
        void enterEvent(QEvent *);
        void leaveEvent(QEvent *);

        void set_value();
        void remove_value();
        void edit_value();
        void disconnect_triggered();
        void unselect();
    };
}
