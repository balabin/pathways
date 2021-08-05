# Introduction
Pathways is a VMD plugin for identifying and visualizing electron tunneling pathways in redox proteins (see https://onlinelibrary.wiley.com/doi/full/10.1002/jcc.22927 for the article in J. Comput. Chem. and https://people.chem.duke.edu/~ilya/Software/Pathways/docs/pathways.html for more on theoretical background).

## Installation

Repository structure:

pathways: the Pathways plugin itself

pathcore/bin: pathcore binaries compiled for Linux (64- and 32-bit), Windows, and MacOS Sierra

pathcore/src: pathcore source code for Linux/MacOS and Windows, respectively, including compile scripts

## Contacts
ilya.balabin@duke.edu

## Citation
Balabin, I. A., Hu, X., & Beratan, D. N. (2012). Exploring biological electron transfer pathway dynamics with the Pathways Plugin for VMD. Journal of Computational Chemistry, 33(8), 906–910.

## DOI
https://doi.org/10.1002/jcc.22927

# Windows installation instructions
Since a number of users have recently had issues with installing the plugin under Windows, here are step-by-step installation instructions:

* Download a zip archive (pathways-master.zip) of the plugin code from https://github.com/balabin/pathways
* Unzip the archive anywhere, e.g., in your home folder. That will create a folder named pathways-master
* Make a permanent folder for the Pathways plugin, e.g., C:\Users\nopphon\pathways (hereafter Pathways home folder)
* Copy Tcl scripts pathways-master/pathways/pathways.tcl, pathways-master/pathways/pkgIndex.tcl, and pathways-master/pathways/collective.tcl to the Pathways home folder
* Copy the pathcore utility binary for Windows (pathways-master/pathways/pathcore/bin/pathcore_windows.zip) to the Pathways home folder and unzip the file. That should extract a file called pathcore.exe
* Install VMD, following the directions from http://www.ks.uiuc.edu/research/vmd
* Add to the VMD startup file vmd.rc (probably C:\Users\nopphon\vmd.rc ; if not, search for the file) the following lines  
>     _global env_<br>
>    _lappend auto_path $env(HOME)/pathways_<br>
These lines tell VMD to look for the Pathways plugin in that directory
* Add the Pathways home folder to your environmental variable PATH (e.g., following https://www.c-sharpcorner.com/article/add-a-directory-to-path-environment-variable-in-windows-10 or any other manual on the topic)
* Reboot, start VMD, open Tkconsole, and try running the following command there:  
>    _pathcore_<br>
If you see a pathcore greeting, you are all set and can start using the Pathways plugin right away. Otherwise, try some troubleshooting, starting with this:  
    Does VMD see the plugin? Open Tkconsole and run the following command there:  
>        _package require pathways_<br>  
    Does Windows see the pathcore executable? Open a terminal and run the following command there:  
>        _pathcore_<br> 
* Delete the pathways-master folder - you no longer need it.
