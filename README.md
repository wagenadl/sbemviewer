# SBEMViewer
Viewer for serial blockface electron microscopy data

SBEMViewer is a tool for viewing three-dimensional 
neuroanatomical data as may be obtained from serial blockface 
electron microscopy or related techniques.

SBEMViewer is primarily written to view the data recently collected by the Wagenaar
lab in collaboration with Mark Ellisman's group at UCSD. To use it for other data
sets, a small webserver intermediary would have to be written, but this should be 
straightforward as the web access protocol is extremely simple.

SBEMViewer can be used to navigate through 3D volumes and also to manually trace 
neurites and to label structures of interest such as somata and synapses. Tracings 
and labelings are stored in a SQLite database.

SBEMViewer contains a nascent movie maker to visualize tracing work in 3D.
