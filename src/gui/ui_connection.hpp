/**
 * @file    ui_connection.hpp
 * @date    14/04/2018
 * @author  Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 * @author  Filip Kocica <xkocic01@fit.vutbr.cz>
 */

#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QTimer>
#include <QMessageBox>

#include "ui_mainwindow.h"
#include "ui_port.hpp"

#define LINE_THICKNESS 3
#define OFFSET_LINE_LEN 20

namespace gui {
    class Connection;

    /// Lines used to make a connection between blocks.
    class Line : public QWidget
    {
        Q_OBJECT

    public:
        explicit Line(QWidget *parent, MainWindow *ui, Connection * part_of);

    private:
        MainWindow *ui;
        void paintEvent(QPaintEvent* event);

        ///< Pointer to the object representing the whole connection.
        Connection * part_of = NULL;

    public slots:
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *);

    };

    /// Represents a connection by holding all the individual lines.
    class Connection : public QWidget
    {
        Q_OBJECT
    public:
        explicit Connection(QWidget *parent, MainWindow *ui, Port* from, Port* to);
        virtual ~Connection();

        void reposition();
        void showLineInfo();

    public slots:
        void delete_triggered();

    private:
        MainWindow *ui;

        void place_lines();
        void draw_basic_line(QPoint, QPoint);
        void draw_complex_line(QPoint, QPoint);

        Line *l1 = NULL;
        Line *l2 = NULL;
        Line *l3 = NULL;
        Line *l4 = NULL;
        Line *l5 = NULL;

        Port* from = NULL; ///< Output port
        Port* to = NULL; ///< Input port

        QLabel *labelShowConnectionInfo; ///< Hover over label
        QTimer intervalShowConnectionInfo; ///< Timer to hide the info label
    };
}
