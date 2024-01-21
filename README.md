# Pathways plugin in a nutshell
This repository includes the current version of code, documentation, and usage examples of the Pathways plugin for VMD described in the [2012 *J. Comp. Chem.* article](https://onlinelibrary.wiley.com/doi/full/10.1002/jcc.22927). My [old Pathways page](https://people.chem.duke.edu/~ilya/Software/Pathways/docs/pathways.html) at Duke University that the article links to will no longer be updated or maintained, nor will I respond to emails to my former Duke University address. (Actually, the old page has not been updated for a while, largely because of Duke requiring to use a non-standard closed-source client for VPN access). Please contact me on Github instead, if needed.

### Repository structure:
<pre>
/---
   |--- 1bex_wt_movie   (example for an MD trajectory with generating a pathway animation)
   |--- images   (images included in README.html)
   |--- install_vmdstore.txt   (info for VMD store (https://biosim.pt/how-to-install-vmd-store))
   |--- install_windows.md   (installation instructions for Windows)
   |--- LICENSE   (GPL v.3 or later)
   |--- pathways ---   (code for Pathways plugin and pathcore executable)
   |                |--- collective.tcl
   |                |--- pathways.tcl
   |                |--- pkgIndex.tcl
   |                |--- pathcore ---
   |                                 |--- bin ---   (binary executables for different platforms)
   |                                 |           |--- pathcore_x86-64.zip
   |                                 |           |--- pathcore_i386.zip
   |                                 |           |--- pathcore_windows.zip
   |                                 |           |--- pathcore_darwin.zip
   |                                 |--- src ---   (source code for different platforms)
   |                                             |--- Linux_MacOS
   |                                             |--- Windows
   |--- README.html   (theory|documentation page)
   |--- README.md   (this file)
</pre>

### Documentation
The old theory|documentation page, including installation instructions, is in [README.html](README.html). To browse it, please clone the repository and open the file locally on your computer.

### Installation
Installation instructions for Linux and MacOS are included into the documentation page. Installation instructions for Windows are in [install_windows.md](install_windows.md).

### Usage examples
Directory [1bex_wt_movie](1bex_wt_movie) includes a complete example of a Pathways plugin run on an MD trajectory with generating a video animation of pathway dynamics. Example output files are included.

### Credit
If you find Pathways helpful, please cite the article:<br><br>
Balabin, I. A., Hu, X., & Beratan, D. N. (2012). Exploring biological electron transfer pathway dynamics with the Pathways Plugin for VMD. *J. Comp. Chem.* **33**(8), 906–910.<br><br>
(DOI https://doi.org/10.1002/jcc.22927)
