_**pSketcher**_ is a parametric 2D sketcher application with constraints. Currently much of the primitive and constraint rendering is completed and the geometric constraint solver is working. Missing features in progress include geometry import and export and user feedback for under and over constrained sketches. This page will be updated as these features are completed.

Current development is on the Linux platform using Qt for the widget toolkit. However, it is my intent to keep the program portable so that it can eventually be built on multiple platforms.

| ![http://lh4.ggpht.com/michael.greminger/SJ9Wsth-ctI/AAAAAAAAAEQ/yhMViFnxeT0/s400/constrained_sketch_pre_solve.jpg](http://lh4.ggpht.com/michael.greminger/SJ9Wsth-ctI/AAAAAAAAAEQ/yhMViFnxeT0/s400/constrained_sketch_pre_solve.jpg) |  ![http://lh5.ggpht.com/michael.greminger/SJ9WsxUSN5I/AAAAAAAAAEY/7wL3H0II84g/s400/constrained_sketch_post_solve.jpg](http://lh5.ggpht.com/michael.greminger/SJ9WsxUSN5I/AAAAAAAAAEY/7wL3H0II84g/s400/constrained_sketch_post_solve.jpg) |
|:--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|:-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| **Constrained sketch**                                                                                                                                                                                                                | **Same sketch after the execution of the constraint solver**                                                                                                                                                                             |

**The currently implemented primitives are:**
  * point
  * line
  * arc
  * circle

**The currently implemented constraints are:**
  * Distance between two points
  * Distance between point and line
  * Horizontal and vertical lines
  * Angle between two lines
  * Edge tangency
  * Parallel lines
  * Arc and circle radius (explicit)
  * Coincident point (explicit)
  * Concentric circles (explicit)

**Implemented features:**
  * File save and open using an SQLite database as the file format
  * Unlimited undo/redo (the undo/redo history is stored in the database file so it is persistent across sessions)
  * DXF file export using the Dime library

**Features that need to be implemented (in no particular order):**
  * Improved method to interactively create arcs
  * DXF file import using the Dime library
  * SVG export
  * General improvements to the constraint solver
  * Background grid
  * User feedback for over and under constrained sketches
  * Merging two pSketcher model database files to create one model