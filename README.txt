===========================
  ICP 2018 -- blockeditor
===========================


 Team members
==============
Filip Kocica <xkocic01>
 - Application logic
Ondrej Vasicek <xvasic25>
 - Graphical user interface


 Assignment
============
The task was to create a application with a graphical user interface, that
allows to create and edit block diagrams.


 Functionality
===============

 Blocks
--------
First you need to select what type of block to place and what type of values
should it use. That can be achieved using one of the toolbars (the one located
on the left by default) or using the keys 1-5 for block types. There are 5
types of blocks (sum, subtract, multiplication, division and power of N) and 3
types of values (floating point, integer and hexadecimal). Upon selecting a
block, place it by clicking into the scrolling frame, it can then be repositioned
by mouse dragging. To stop placing blocks change back to the empty cursor using
the toolbar, by pressing escape or by right clicking on an empty space in the
frame. Blocks can be removed using the delete cursor (selected from the toolbar)
or using a popup menu invoked by right clicking the desired block.

 Connections
-------------
To connect blocks together select the ports you wish to connect by left clicking
them. Connections can be created only between an input port and an output port
of two different blocks with matching value types. Connections can be removed by
the delete cursor, a right click popup menu of the connection, using a right
click popup menu of one of the connected ports or using a right click popup menu 
of a connected block. Ports with no connection can be assingned a value as an 
input via their right click popup menu. The value can then be removed or edited 
using the same method. Review assingned values by hovering over the desired port.

 Computation
-------------
Once all blocks and connections are in place you can launch the computation
using the second toolbar (located at the top by default). But doing so is only
possible after all input ports of all block are either connected or have a
value assigned and when there are no connection cycles. The computation can
be run all at once using the 'Run' button or it can be done one block at a
time using the 'Step' button. Values transmited by connections can be rewied
by hovering over the connection. New blocks or connections can not be added
during computation, nor can the existing ones be deleted. To end the
computation press the 'Stop' button.

 Save and Load
---------------
Diagrams can be saved or loaded using the 'Save' and 'Load' buttons. The 
current diagram can be cleaned using the 'clear' button.

 Etc
-----
Both of the toolbars can be repositioned and the frame for block placement 
allows scrolling in the case of a block being out of view.
