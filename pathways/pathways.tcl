#!/usr/local/bin/vmd -dispdev text

#########################################################################

###
# pathways.tcl  (C) 2012-2021
# Authors:      Ilya Balabin (project design and management, VMD integration,
#                             user interfaces, graphics, animation)
#                             <ilya.balabin@duke.edu>
#               Xiangqian Hu (graph search wrapper)
#               David Beratan (general management)
# Released under the terms of GNU public license version 3.0 or later
###

### 
# compute and visualize electron transfer pathways (D. N. Beratan,
# J. N. Betts, and J. N. Onuchic, Science 252, 1285-1288 (1991))
###

###
# new in version 1.1: collective electronic states (by default, 
# nucleic acid bases and protein aromatic side chains)
###

###
# new in version 1.2: a) worked around the old VMD issue with measuring
# hydrogen bonds (results dependent on whether the atom selection has
# been moved or rotated); b) fixed the molecule ID from 'top' to '$mol'
###

###
# new in version 1.3: a) the 'guessbonds' function for guessing covalent 
# bonds and hydrogen bonds for biological molecules; b) the 'autocbonds'
# function for automatically adding covalent bonds based on interatomic
# distance (useful for non-standard cofactors). Both functions are handy
# if no structure file is used.
###

#########################################################################

### Tell Tcl that we're a package and any dependencies we may have
set path_version "1.4"
package provide pathways 1.4

### package help
proc pathways0_usage {} {
    puts "Pathways functions:"
    puts "  pathways (type \"pathways0\" for help)"
    puts "  pathtraj (type \"pathtraj\" for help)"
    puts "  addbond (type \"addbond\" for help)"
    puts "  delbond (type \"delbond\" for help)"
    puts "  setseg (type \"setseg\" for help)"
    puts "  guessbonds (type \"guessbonds\" for help)"
    puts "  autocbonds (type \"autocbonds\" for help)"
    return 1
}

### pathways syntax
proc pathways0 {} {
    puts "Pathways usage:"
    puts "  pathways -d \"name FE\" -a \"name RU\""
    puts "Mandatory parameters:"
    puts "  -d <value> - donor atom selection string"
    puts "  -a <value> - acceptor atom selection string"
    puts "Optional rate parameters:"
    puts "  -lambda <value> - ET reorganization energy, eV"
    puts "  -deltag <value> - ET driving force, eV"
    puts "Optional molecule parameters:"
    puts "  -mol <value> - molecule id (default top)"
    puts "  -frame <value> - trajectory frame number (default 0)"
    puts "  -b <value> - bridge atom selection string (default \"all\")"
    puts "  -p <value> - number of pathways, must be positive (default 1)"
    puts "  -q <value> - identify importance of bridge atoms - SLOW (default 0 = no)"
    puts "  -renv <value> - pathway environment size (default 4.0 A)"
    puts "  -cda - treat multiatom donor/acceptor as single states (default 1 = yes)"
    puts "Optional Pathways parameters:"
    puts "  -withh <value> - include hydrogens (default 0). USE WITH CARE!"
    puts "  -hcut <value> - H-bond cutoff distance (default 3A)"
    puts "  -hang <value> - H-bond cutoff angle (default 30 deg)"
    puts "  -tscut <value> - TS cutoff distance (default 6A)"
    puts "  -procut <value> - protein pruning margin (default 7A)"
    puts "  -epsc <value> - decay per covalent bond (default 0.6)"
    puts "  -epsh <value> - decay prefactor per covalent bond (default 0.36)"
    puts "  -exph <value> - exponential factor per hydrogen bond (default 1.7A^-1)"
    puts "  -r0h <value> - distance offset per hydrogen bond (default 2.8A)"
    puts "  -epsts <value> - decay prefactor TS jump (default 0.6)"
    puts "  -expts <value> - exponential factor per TS jump (default 1.7A^-1)"
    puts "  -r0ts <value> - distance offset per TS jump (default 1.4A)"
    puts "Optional output parameters:"
    puts "  -o <value> - output file prefix (default \"pathways\")"
    puts "  -debugug <value> - keep temp files (default 1 = yes)"
    puts "Optional display parameters:"
    puts "  -rmax <value> - max pathway radius (default 0.5)"
    puts "  -res <value> - pathway resolution (default 20.0)"
    puts "  -col <value> - pathway color (default rainbow color by number)"
    puts "  -mat <value> - pathway material (default Opaque)"
    puts "  -rnd <value> - renderer, snapshot or TachyonInternal (default none)"
    puts "  -snap <value> - snapshot prefix (default \"snapshot\", ignored if no rendering)"
    puts "Optional experimental parameters (use with care!):"
    puts "  -cs <value> - use collective electronic states (default 0)"
    puts "  -cf <value> - file describing collective states (default \"collective.sts\")"
    return 1
}

### pathtraj syntax
proc pathtraj_usage {} {
    puts "Pathtraj is simply a wrapper that runs Pathways for each frame of"
    puts "a molecular dynamics trajectory. It accepts all Pathways arguments"
    puts "as well as the following optional arguments:"
    puts "  -first <value> - first trajectory frame (default 0)"
    puts "  -last <value> - last trajectory frame (default end)"
    puts "  -stride <value> - frame number increment (default 1)"
    puts "Default renderer in pathtraj is TachyonInternal"
    return 1
}

### add bond syntax
proc addbond_usage {} {
    puts "Add a covalent bond:"
    puts "  addbond \"name FE\" \"protein and resid 18 and name NE2\" top"
    puts "Mandatory parameters:"
    puts "  <value> - first atom selection string"
    puts "  <value> - second atom selection string"
    puts "Optional parameters:"
    puts "  -mol <value> - molecule id (default top)"
    return 1
}

### delete bond syntax
proc delbond_usage {} {
    puts "Delete a covalent bond:"
    puts "  delbond \"name FE\" \"protein and resid 18 and name NE2\" top"
    puts "Mandatory parameters:"
    puts "  <value> - first atom selection string"
    puts "  <value> - second atom selection string"
    puts "Optional parameters:"
    puts "  -mol <value> - molecule id (default top)"
    return 1
}

### set missing segment IDs
proc setseg_usage {} {
    puts "Set missing segment IDs:"
    puts "  setseg top"
    puts "Mandatory parameters:"
    puts "  none"
    puts "Optional parameters:"
    puts "  -mol <value> - molecule id (default top)"
    return 1
}

### guess covalent and hydrogen bond structure from coordinates of biological molecules
proc guessbonds_usage {} {
    puts "Add covalent bonds betweenany atoms based on distance (useful for cofactors):"
    puts "  guessbonds \"protein and chain A\""
    puts "Mandatory parameters:"
    puts "  <value> - atom selection string for adding covalent/hydrogen bonds (default empty)"
    return 1
}

### automatically add covalent bonds between atoms based on distance
proc autocbonds_usage {} {
    puts "Add covalent bonds betweenany atoms based on distance (useful for cofactors):"
    puts "  autocbonds 1.8 \"resname REQ\""
    puts "Mandatory parameters:"
    puts "  <value> - distance threshold (default 1.7A)"
    puts "  <value> - atom selection string for adding covalent bonds (default empty)"
    return 1
}

### put command line options in global array cmdline
proc cmdoptions { args } {
    global cmdline
    set args [eval "concat $args"]
    set n [llength $args]
    array unset cmdline
    for { set i 0 } { $i < $n } { incr i 2 } {
        set key [lindex $args $i]
        set val [lindex $args [expr $i + 1]]
        set cmdline($key) $val
    }
    return 0
}

### check command line for mandatory parameters
proc check_params { flags } {
    global cmdline
    foreach f $flags {
        if { ![info exists cmdline($f)] } { return 1 }
    }
    return 0
}

### get covalent bond list
proc get_cbonds { sel } {

    set cbonds {}
    set ilist [$sel get index]
    set blist [$sel getbonds]
    foreach i $ilist bb $blist {
	foreach b $bb {lappend cbonds [lsort -integer [list $i $b]]}
    }
    set cbonds [lsort -unique $cbonds]   ;# remove duplicates
    return $cbonds

}

### get hydrogen bond list
proc get_hbonds { sel hcut hang } {

    set hbonds {}
    foreach {dlist alist hlist} [measure hbonds $hcut $hang $sel] { break }
    foreach d $dlist a $alist {
	lappend hbonds [lsort -integer [list $d $a]]
    }
    return $hbonds
    
}

# convert a list of atom ID pairs to a list of atom description pairs
proc aip2dp { mol aiplist } {

    set dplist {}
    foreach aip $aiplist {
	set dp {}
	foreach i $aip {
	    set desc {}
	    set sel [atomselect $mol "index $i"]
	    lappend desc [$sel get segid]
	    lappend desc [$sel get resid]
	    lappend desc [$sel get name]
	    $sel delete
	    lappend dp $desc
	}
	lappend dplist $dp
    }
    return $dplist
    
}

# convert a list of atom description pairs to a list of atom ID pairs
proc dp2aip { mol dplist } {

    set aiplist {}
    foreach dp $dplist {
	set aip {}
	foreach desc $dp {
	    foreach { seg res n } $desc { break }
	    set sel [atomselect $mol "segid $seg and resid $res and name $n"]
	    lappend aip [$sel get index]
	    $sel delete
	}
	lappend aiplist $aip
    }
    return $aiplist

}

# convert index to segid, resname, resid, and name
proc i2d {mol i } {

    set sel [atomselect $mol "index [expr $i-1]"]
    set segid [$sel get segid]
    set resid [$sel get resid]
    set resname [$sel get resname]
    set name [$sel get name]
    set r [measure center $sel]
    $sel delete

    return [list $segid $resname $resid $name $r]

}

# check if the list includes the element
proc is_in_list { l a } {

    foreach v $l {
	if { "${v}" == "${a}" } {
	    return 1
	}
    }
    return 0

}

# convert pathcore output line into a path object
proc line2path { line d1list a1list } {

    # sanity check
    if { [llength $line]<3 } {
	puts "INTERNAL ERROR: incorrect pathcore output"
	quit
    }

    # get coupling
    set coupl [lvarpop line]

    # remove all D atoms but the last
    set d10 [lvarpop line]
    while { [llength $line] } {
	set d1 [lvarpop line]
	if { [is_in_list $d1list $d1] } {
	    set d10 $d1
	    continue
	} else {
	    set line [concat $d10 $d1 $line]
	    break
	}
    }

    # remove all A atoms but the first
    set line [lreverse $line]
    set a10 [lvarpop line]
    while { [llength $line] } {
	set a1 [lvarpop line]
	if { ![is_in_list $a1list $a1] } {
	    set line [concat $a10 $a1 $line]
	    break
	} else {
	    set a10 $a1
	    continue
	}
    }
    set line [lreverse $line]

    return [list $coupl $line $d10 $a10]

}

# draw a pathway step
proc draw_step { mol r1 r2 rad res col mat type } {

    # substep size for dashed lines
    set sssize 0.5

    # graphics attributes
    graphics $mol color $col
    graphics $mol material $mat

    # list of primitives
    set sslist {}
    if { $type == "CB" } {
	lappend sslist [list $r1 $r2]
    } else {
	set stepvec [vecsub $r2 $r1]
	set steplen [veclength $stepvec]
	set dv [vecscale [vecnorm $stepvec] $sssize]
	set ds [veclength $dv]
	set s 0
	set q1 $r1

	# calculate primitives
	while { $s < $steplen } {
	    set q2 [vecadd $q1 $dv]
	    set s [expr $s + $ds]
	    if { $s >= $steplen } {set q2 $r2}
	    lappend sslist [list $q1 $q2]
	    set q1 [vecadd $q2 $dv]
	    set s [expr $s + $ds]
	}
    }

    # draw the step
    foreach step $sslist {
	foreach {q1 q2} $step { break }
	graphics $mol cylinder $q1 $q2 radius $rad resolution $res filled 1
    }

    # cap the step
    graphics $mol sphere $r1 radius $rad resolution $res
    graphics $mol sphere $r2 radius $rad resolution $res
    return 0
}

### add a covalent bond
proc addbond { args } {
    
    # sanity check
    if { [llength $args]<2 || [llength $args]>3 } {
	addbond_usage
	return 1
    }

    # parameters
    set s1 [lvarpop args]
    set s2 [lvarpop args]
    if { [llength $args] } {
	set mol [lvarpop args]
    } else {
	set mol "top"
    }

    # index and bond lists
    set sel1 [atomselect $mol "$s1"]
    set i1 [eval "concat [$sel1 get index]"]
    set bonds1 [eval "concat [$sel1 getbonds]"]
    set sel2 [atomselect $mol "$s2"]
    set i2 [eval "concat [$sel2 get index]"]
    set bonds2 [eval "concat [$sel2 getbonds]"]

    # sanity check
    if { [llength [$sel1 get name]]!=1 || [llength [$sel2 get name]]!=1 } {
	puts "ERROR: zero or multiple atoms in selection(s)!"
	addbond_usage
	return 9
    }

    # add the bond
    lappend bonds1 $i2
    $sel1 setbonds [list $bonds1]
    lappend bonds2 $i1
    $sel2 setbonds [list $bonds2]

    return 0
}

### delete a covalent bond
proc delbond { args } {
    
    # sanity check
    if { [llength $args]<2 || [llength $args]>3 } {
	delbond_usage
	return 1
    }

    # parameters
    set s1 [lvarpop args]
    set s2 [lvarpop args]
    if { [llength $args] } {
	set mol [lvarpop args]
    } else {
	set mol "top"
    }

    # index and bond lists
    set sel1 [atomselect $mol "$s1"]
    set i1 [eval "concat [$sel1 get index]"]
    set bonds1 [eval "concat [$sel1 getbonds]"]
    set sel2 [atomselect $mol "$s2"]
    set i2 [eval "concat [$sel2 get index]"]
    set bonds2 [eval "concat [$sel2 getbonds]"]

    # sanity check
    if { [llength [$sel1 get name]]!=1 || [llength [$sel2 get name]]!=1 } {
	puts "ERROR: zero or multiple atoms in selection(s)!"
	addbond_usage
	return 9
    }

    # delete the bond
    set k [lsearch -integer $bonds1 $i2]
    if { $k<0 } {
	puts "WARNING: Bond from \"$s1\" to \"$s2\" not found!"
    } else {
	set bonds1 [lreplace $bonds1 $k $k]
	$sel1 setbonds [list $bonds1]
    }
    set k [lsearch -integer $bonds2 $i1]
    if { $k<0 } {
	puts "WARNING: Bond from \"$s1\" to \"$s2\" not found!"
    } else {
	set bonds2 [lreplace $bonds2 $k $k]
	$sel2 setbonds [list $bonds2]
    }

    return 0
}

### set missing segment IDs
proc setseg { args } {

    # sanity check
    if { [llength $args]<0 || [llength $args]>1 } {
	setseg_usage
	return 1
    }

    # parameters
    if { [llength $args] } {
	set mol [lvarpop args]
    } else {
	set mol "top"
    }

    # set segments
    set sel [atomselect $mol all]
    set seglist [lsort -unique [$sel get segid]]
    set chainlist [lsort -unique [$sel get chain]]
    $sel delete
    
    foreach c $chainlist {
	set sel [atomselect $mol "chain $c"]
	set segments [lsort -unique [$sel get segid]]
	set seg [lvarpop segments]
	if { $seg == "" || [llength $segments] } {
	    $sel set segid $c
	}
	$sel delete
    }

    return 0
}


### guess covalent and hydrogen bond structure from coordinates of biological molecules
proc guesscbonds { args } {

    # sanity check
    if { [llength $args]!=1 } {
	guessbonds_usage
	return 1
    } else {
	set selstr [lvarpop args]
    }
    
    
    
    
    return 0
}




### automatically add covalent bonds between atoms based on distance
proc autocbonds { args } {

    # sanity check
    if { [llength $args]!=2 } {
	autocbonds_usage
	return 1
    } else {
	set thr [lvarpop args]
	set selstr [lvarpop args]
    }
    
    set sel [atomselect top $selstr]
    set ilist [$sel get index]
    
    foreach i $ilist {
	set seli [atomselect top "index $i"]
	set rri [$seli get {x y z}]
	$seli delete

	foreach j $ilist {
	    set selj [atomselect top "index $j"]
	    set rrj [$selj get {x y z}]
	    $selj delete

	    set s "vecsub $rri $rrj"

	    set x [eval $s]

	    puts $x

	    set dist [veclength $x]

	    puts $dist

	    if { $dist<$thr } {
	    puts "before"
		addbond "index $i" "index j"
	    puts "after"
	    }
	}
    }

    return 0
}




### main pathways procedure
proc pathways { args } {

    # internal
    set ph "PATHWAYS)"
    global path_version
    
    # find pathcore binary
    set pathcore "pathcore"
    if { ![llength $pathcore] } {
	puts "$ph ERROR: can not find a pathcore binary!"
	puts "$ph        Make sure pathcore is installed and in your path!"
    } else {
	puts "$ph Found $pathcore"
    }

    # detect display
    global env
    if { [catch {eval "set display $env(DISPLAY)"}] } {
	puts "$ph No display found"
	set display 0
    } else {
	puts "$ph Display $display found"
	set display 1
    }

    # get command line options
    global cmdline
    cmdoptions $args

    # sanity check
    if { [check_params {"-d" "-a"}] } {
	pathways0_usage
        return 1
    }

    # set mandatory parameters
    set dstr $cmdline(-d)
    set astr $cmdline(-a)
    
    # set optional rate parameters
    set lambda 999999; if {[info exists cmdline(-lambda)]} {set lambda $cmdline(-lambda)}
    set deltag 999999; if {[info exists cmdline(-deltag)]} {set deltag $cmdline(-deltag)}
    if { [expr abs($lambda*$deltag)] < 1000000 } {set printrate 1} else { set printrate 0}

    # set optional molecule parameters
    set mol [molinfo top]; if {[info exists cmdline(-mol)]} {set mol $cmdline(-mol)}
    set frame 0; if {[info exists cmdline(-frame)]} {set frame $cmdline(-frame)}
    set bstr "all"; if {[info exists cmdline(-b)]} {set bstr $cmdline(-b)}
    set p 1; if {[info exists cmdline(-p)]} {set p $cmdline(-p)}
    set q 0; if {[info exists cmdline(-q)]} {set q $cmdline(-q)}
    set renv 4.0; if {[info exists cmdline(-renv)]} {set renv $cmdline(-renv)}
    set cda 1; if {[info exists cmdline(-cda)]} {set cda $cmdline(-cda)}

    # set optional Pathways parameters
    set withh 0; if {[info exists cmdline(-withh)]} {set withh $cmdline(-withh)}
    set hcut 3.0; if {[info exists cmdline(-hcut)]} {set hcut $cmdline(-hcut)}
    set hang 30; if {[info exists cmdline(-hang)]} {set hang $cmdline(-hang)}
    set tscut 6.0; if {[info exists cmdline(-tscut)]} {set tscut $cmdline(-tscut)}
    set procut 7.0; if {[info exists cmdline(-procut)]} {set procut $cmdline(-procut)}
    set epsc 0.6; if {[info exists cmdline(-epsc)]} {set epsc $cmdline(-epsc)}
    set epsh 0.36; if {[info exists cmdline(-epsh)]} {set epsh $cmdline(-epsh)}
    set exph 1.7; if {[info exists cmdline(-exph)]} {set exph $cmdline(-exph)}
    set r0h 2.8; if {[info exists cmdline(-r0h)]} {set r0h $cmdline(-r0h)}
    set epsts 0.6; if {[info exists cmdline(-epsts)]} {set epsts $cmdline(-epsts)}
    set expts 1.7; if {[info exists cmdline(-expts)]} {set expts $cmdline(-expts)}
    set r0ts 1.4; if {[info exists cmdline(-r0ts)]} {set r0ts $cmdline(-r0ts)}

    # set optional output parameters
    set out "pathways"; if {[info exists cmdline(-o)]} {set out $cmdline(-o)}
    set debug 1; if {[info exists cmdline(-debug)]} {set debug $cmdline(-debug)}

    # set optional display parameters
    set rmax 0.5; if {[info exists cmdline(-rmax)]} {set rmax $cmdline(-rmax)}
    set res 20; if {[info exists cmdline(-res)]} {set res $cmdline(-res)}
    set mat "Opaque"; if {[info exists cmdline(-mat)]} {set mat $cmdline(-mat)}
    set rnd "none"; if {[info exists cmdline(-rnd)]} {set rnd $cmdline(-rnd)}
    set snap "snapshot"; if {[info exists cmdline(-snap)]} {set snap $cmdline(-snap)}

    # set optional experimental parameters
    set cs 0; if {[info exists cmdline(-cs)]} {set cs $cmdline(-cs)}
    set cf ""; if {[info exists cmdline(-cf)]} {set cf $cmdline(-cf)}
    set cfname "collective.tcl"   ;# default is included in Pathways
    
    # which collective state file?
    if { $cf == "" } {
	# default file included in Pathways
	set cfname $env(PATHWAYSDIR)/$cfname
    } else {
	# custom user-supplied file
	set cfname $cf
    }
    
    # get collective states, if any
    set cstates []
    if { $cs != 0 } {
	# does the collective state file exist?
	if { [file exists $cfname] } {
	    set FH [open "${cfname}" "r"]
	    foreach line [split [read "$FH"] "\n"] {
		if { [regexp {^(\s*)?(\#.*)?$} $line] } {
		    continue
		} else {
		    lappend cstates [split $line]
		}
	    }
	    close $FH
	    puts "$ph Read [llength $cstates] collective states from $cfname"
	} else {
	    puts "$ph WARNING: can not read collective states from $cfname"
	    puts "Collective electronic states ignored"
	    set cs 0
	}
    }
    
    # sanity check
    if { $p <= 0 } {
	puts "$ph ERROR: number of pathways should be positive"
	quit
    }
    
    # print header
    puts "$ph Pathways $path_version (C) 2008-2010 I. Balabin, X. Hu, and D. Beratan"
    puts "$ph Starting pathway calculations:"
    puts ""
    puts "          MOLECULE:"
    puts "          Molecule:             [molinfo $mol get name]"
    puts "          Trajectory frame:     $frame"
    puts "          Donor selection:      $dstr"
    puts "          Acceptor selection:   $astr"
    puts "          Bridge selection:     $bstr"
    puts "          Number of pathways:   $p"
    puts "          Atom importance:      $q"
    puts "          Environment size:     $renv"
    puts "          Collective D/A:       $cda"
    puts ""
    if { $printrate } {
	puts "          RATE:"
	puts "          Reorg. energy        :$lambda eV"
	puts "          Driving force        :$deltag eV"
	puts ""
    }
    puts "          PATHWAYS:"
    puts "          Include hydrogens:    $withh"
    puts "          H-bond cutoff:        $hcut A"
    puts "          H-bond angle:         $hang deg."
    puts "          TS jump cutoff:       $tscut A"
    puts "          Protein pruning:      $procut A"
    puts "          Epsilon_C:            $epsc"
    puts "          Epsilon_H:            $epsh"
    puts "          Exp. H-bond:          $exph"
    puts "          Offset H-bond:        $r0h A"
    puts "          Epsilon_TS:           $epsts"
    puts "          Exp. TS jump:         $expts"
    puts "          Offset TS jump:       $r0ts A"
    puts ""
    puts "          OUTPUT:"
    puts "          Output prefix:        $out"
    puts "          Debugging mode on:    $debug"
    puts ""
    puts "          GRAPHICS:"
    puts "          Max pathway radius:   $rmax"
    puts "          Pathway resolution:   $res"
    puts "          Pathway material:     $mat"
    puts "          Renderer:             $rnd"
    puts "          Snapshot (TGA):       $snap"
    puts ""
    puts "          EXPERIMENTAL:"
    puts "          Collective states     $cs"
    puts "          CS config file        $cfname"

    # sanity check
    set sel [atomselect $mol "($dstr) and ($astr)"]
    if { [llength [$sel get name]] } {
	puts "$ph ERROR: donor and acceptor selections overlap!"
	return 1
    }
    $sel delete

    # renderer check
    if { $display } {
	set rndlist { "none" "snapshot" "TachyonInternal" }
	if { [lsearch $rndlist $rnd]<0 } {
	    puts "$ph ERROR: unknown renderer!"
	    return 1
	}
    }
    
    # save all viewpoints...
    array unset viewpoint
    foreach m [molinfo list] {
	set viewpoint($m) [molinfo $m get {center_matrix rotate_matrix scale_matrix global_matrix}]
    }
    # save the viewpoint for the ET molecule 
    set vp [molinfo $mol get {center_matrix rotate_matrix scale_matrix global_matrix}]

    # tag bridge, donor, acceptor, and collective states
    set sel [atomselect $mol "all"]
    $sel set occupancy 0.00
    $sel set beta 0.00
    $sel delete

    set occ 2.00
    set occincr 0.01
    foreach state $cstates {

	set segid [lvarpop state]
	set resid [lvarpop state]
	set resname [lvarpop state]
	set names [join $state " "]

	set selstr "resname \"$resname\" and name $names"
	if { $resid != 0 } { set selstr "resid \"$resid\" and $selstr" }
	if { $segid != "ANY" } { set selstr "segid \"$segid\" and $selstr" }

	set sel [atomselect $mol "$selstr"]
	set groups [lsort -unique [$sel get {segid resid}]]
	$sel delete
	#puts "DEBUG: groups are $groups"
  	foreach group $groups {
  	    foreach {s r} $group {continue}
 	    #puts "DEBUG: $group ==> $s $r $resname $names"
 	    set ss "segid $s and resid $r and resname $resname and name $names"
 	    #puts "DEBUG: ss = $ss"
 	    set sel [atomselect $mol "$ss"]
 	    $sel set occupancy $occ
 	    #set zzz [$sel get {segid resid resname name}]
 	    #puts "occ $occ set to $zzz"
 	    $sel delete
 	    set occ [expr $occ + $occincr]
  	}
    }

    set sel [atomselect $mol "$dstr"]
    $sel set occupancy -1.00
    set nd [llength [$sel get name]]
    $sel delete

    set sel [atomselect $mol "$astr"]
    $sel set occupancy 1.00
    set na [llength [$sel get name]]
    $sel delete

    if { ![expr $nd*$na] } {
	puts "$ph ERROR: donor and/or acceptor has no atoms!"
	return 1
    }
    puts "$ph Tagged donor and acceptor atoms and collective states (if any)"


    # copy and reload donor + bridge + acceptor selection
    set sel [atomselect $mol "(${dstr}) or (${astr}) or (${bstr})" frame $frame]
    $sel writepsf "${out}.psf"
    $sel writepdb "${out}.pdb"
    $sel delete
    mol load psf "${out}.psf" pdb "${out}.pdb"
    puts "$ph Reloaded the donor + bridge + acceptor structure"

    # turn off the original molecule, show the snapshot instead
    mol off $mol
    set molnew [molinfo top]
    if { $display } { CloneRep::clone_reps $mol $molnew }

    # get hydrogen bond list and convert it to a list of atom description pairs
    if { !$withh } {
	set sel [atomselect top all]
	set hblist [get_hbonds $sel $hcut $hang]
	$sel delete
	set hblist [aip2dp top $hblist]
	puts "$ph Found [llength $hblist] hydrogen bonds"
    
	# remove hydrogens, generate and reload new structure
	set sel [atomselect top "noh"]
	$sel writepsf "${out}.psf"
	$sel writepdb "${out}.pdb"
	$sel delete
	mol delete top
	mol load psf "${out}.psf" pdb "${out}.pdb"
	set molnew [molinfo top]
	if { $display } { CloneRep::clone_reps $mol $molnew }

	set molstr [molinfo top]
	set sel [atomselect $molstr all]
	puts "$ph Removed hydrogens"
    } else {
	set molstr [molinfo top]
	set sel [atomselect $molstr all]
    }	
    
    # generate covalent bond list
    set FH [open "${out}.cb" "w"]
    set cblist {}
    foreach cbond [get_cbonds $sel] {
	foreach {i1 i2} $cbond { break }
	set cb [list [expr $i1+1] [expr $i2+1]]
	puts $FH $cb
	lappend cblist $cb
    }
    $sel delete
    close $FH
    puts "$ph Found [llength $cblist] covalent bonds"

    # regenerate hydrogen bond list
    if { !$withh } {
	set FH [open "${out}.hb" "w"]
	set hblist [dp2aip $molstr $hblist]
	set oldhblist $hblist
	set hblist {}
	foreach hbond $oldhblist {
	    foreach {i1 i2} $hbond { break }
	    set hb [list [expr $i1+1] [expr $i2+1]]
	    puts $FH $hb
	    lappend hblist $hb
	}
	close $FH
	set oldhblist {}
	puts "$ph Regenerated the hydrogen bond list"
    } else {
	set hblist {}
	set FH [open "${out}.hb" "w"]
	close $FH
	puts "$ph Hydrogen atoms included explicitly - no hydrogen bonds needed"
    }

    # generate donor index list
    set sel [atomselect top "${dstr}"]
    set d0list [lsort -integer -increasing [$sel get index]]
    $sel delete
    set d1list {}
    foreach i $d0list {
	lappend d1list [expr $i + 1]
    }

    # generate acceptor index list
    set sel [atomselect top "${astr}"]
    set a0list [lsort -integer -increasing [$sel get index]]
    $sel delete
    set a1list {}
    foreach i $a0list {
	lappend a1list [expr $i + 1]
    }

    # run pathcore
    puts "$ph Searching for pathways, this may take a while..."
    set comlist {}
    lappend comlist "$pathcore"
    lappend comlist "-i" "${out}.pdb"
    lappend comlist "-c" "${out}.cb"
    lappend comlist "-h" "${out}.hb"
    lappend comlist "-p" "$p"
    lappend comlist "-ab" "$cda"
    lappend comlist "-tscut"  "$tscut"
    lappend comlist "-procut" "$procut"
    lappend comlist "-epsc"   "$epsc"
    lappend comlist "-epsh"   "$epsh"
    lappend comlist "-exph"   "$exph"
    lappend comlist "-r0h"    "$r0h"
    lappend comlist "-epsts"   "$epsts"
    lappend comlist "-expts"  "$expts"
    lappend comlist "-r0ts"   "$r0ts"
    lappend comlist ">" "${out}1.out"
    puts "$ph Running \"$comlist\""
    if { [catch {eval "exec $comlist"} pout ] } {
        puts "$ph WARNING: pathcore generated warnings/errors:\n$pout"
    }
    puts "$ph Pathcore run completed"

    # parse pathways
    set pathways {}
    set FH [ open "${out}1.out" "r" ]
    foreach line [split [read $FH] "\n"] {
	if { [regexp {^\s?$} $line] } { continue }
	lappend pathways [ line2path $line $d1list $a1list ]
    }
    close $FH
    puts "$ph Found [llength $pathways] pathway(s)"

    # sort pathways
    set pathways [lsort -real -index 0 -decreasing $pathways]

    # get and store couplings
    set maxcoupl [lindex [lindex $pathways 0] 0]
    puts "$ph Sorted [llength $pathways] pathway(s): max coupling $maxcoupl"
	
    # print rate if needed
    if { $printrate } {
	# direct vdW contact, activationless, lambda=1eV rate
	set k0 1e13
	# room temperature
	set temp 310
	# Boltzmann constant in eV
	set kb [expr 1.380658 / 1.6021772 * 1e-4]
	# hack to avoid divergence
	if { $lambda<0.001 } {set lambda 0.001}
	# FC factor
	set sum [expr $deltag+$lambda]
	set fc [expr 1.0/sqrt($lambda)]
	set fc [expr $fc * exp(-$sum * $sum / 4 / $lambda / $kb / $temp)]
	# rate estimate
	set k [expr $k0 * $fc * $maxcoupl * $maxcoupl / $nd / $na]
	set k [format "%7.1g s^-1" $k]
	puts "\n$ph MARCUS ESTIMATE OF ET RATE: $k\n"
    }
    
    # prepare to display pathways
    if { $display } {
	# display the molecule
	mol representation Lines 1.0
	mol selection all
	mol color Name
	mol material Opaque

	# display pathways
	mol load pdb "${out}.pdb"
	set molpath [molinfo top]
	mol delrep all $molpath
	puts "$ph Displaying pathways:"
    }

    # set rainbow pathway colors: red, orange, yellow, green, cyan, blue, violet
    set colorlist {1 3 4 7 10 0 25}

    # print and if needed display pathways
    puts "$ph ELECTRON TRANSFER PATHWAYS:\n"
    set i 0
    foreach path $pathways {

	# pathway properties
	incr i
	if { $i > $p } { break }
	foreach {coupl atoms d a} $path { break }
	puts "PATH $i:\tnet decay $coupl"
	set plength 0
	set a1 [lvarpop atoms]

	# pathway radius
	set rad [expr $rmax * $coupl / $maxcoupl]

	# pathway color (loop through color list)
	set col [lvarpop colorlist]
	set colorlist [concat $colorlist $col]

	while { [llength $atoms]>0 } {

	    # parse atoms
	    foreach {segid1 resname1 resid1 name1 r1} [i2d $molstr $a1] { break }
	    set a2 [lvarpop atoms]
	    foreach {segid2 resname2 resid2 name2 r2} [i2d $molstr $a2] { break }
	    set thisstep [lsort -integer [list $a1 $a2]]
	    set dr [veclength [vecsub $r2 $r1]]
	    set plength [expr $plength + $dr]
	    if { [is_in_list $hblist $thisstep] } {
		set type "HB"
	    } elseif { [is_in_list $cblist $thisstep] } {
		set type "CB"
	    } else {
		set type "TS"
	    }
	    
	    # print step
	    set s [format "%-6d %-4s %-4s %-4d %-4s --> %-6d %-4s %-4s %-4d %-4s : %2s %5.2f %5.2f" $a1 $segid1 $resname1 $resid1 $name1 $a2 $segid2 $resname2 $resid2 $name2 $type $dr $plength]
	    puts "$s"
	    set a1 $a2

	    # display step
	    if { $display } {
		draw_step $molpath $r1 $r2 $rad $res $col $mat $type

	    }

	}
	puts ""
    }

    # evaluate atom importance
    if { $q } {
	
	# tag non-environment atoms
	set envlist {}
	foreach path $pathways {
	    foreach {coupl atomlist d a} $path { break }
	    foreach i $atomlist {
		lappend envlist [expr $i-1]
	    }
	}
	set envlist [lsort -unique [concat $envlist]]
	set sel [atomselect $molstr "(occupancy 0) and not (within $renv of (index $envlist))"]
	$sel set occupancy 9
	$sel delete
	set sel [atomselect $molstr all]
	$sel writepdb "${out}.pdb"
	$sel delete
	
	# run pathcore
	puts "$ph Evaluating atom importance, this may take a while..."
	set comlist {}
	lappend comlist "$pathcore"
	lappend comlist "-i" "${out}.pdb"
	lappend comlist "-c" "${out}.cb"
	lappend comlist "-h" "${out}.hb"
	lappend comlist "-p" 1
	lappend comlist "-ab" 1
	lappend comlist "-tscut"  "$tscut"
	lappend comlist "-procut" "$procut"
	lappend comlist "-epsc"   "$epsc"
	lappend comlist "-exph"   "$exph"
	lappend comlist "-exph"   "$exph"
	lappend comlist "-r0h"    "$r0h"
	lappend comlist "-expts"   "$expts"
	lappend comlist "-expts"  "$expts"
	lappend comlist "-r0ts"   "$r0ts"
	lappend comlist "-q"
	lappend comlist ">" "${out}2.out"
	puts "$ph Running \"$comlist\""
	if { [catch {eval "exec $comlist"} pout ] } {
	    puts "$ph WARNING: pathcore generated warnings/errors:\n$pout"
	}
	puts "$ph Pathcore run completed"
	
	# read pathcore output
	set implist {}
	set fh [ open "${out}2.out" "r" ]
	foreach line [split [read $fh] "\n"] {
	    if { [regexp {^\s?$} $line] } { continue }
	    lappend lines $line
	}
	close $fh
	puts "$ph Found [llength $lines] pathway(s)"

	# parse atom importance
	while { [llength $lines]>1 } {
	    
	    # get coupling with the donor
	    set line [lvarpop lines]
	    set coupl1 [lvarpop line]
	    set d1 [lvarpop line]

	    # get coupling with the acceptor
	    set line [lvarpop lines]
	    set coupl2 [lvarpop line]
	    set d2 [lvarpop line]

	    # sanity check
	    if { $d1 != $d2 } {
		puts "$ph INTERNAL ERROR: incorrect indices!"
		return 9
	    }
	    set coupl [expr $coupl1 * $coupl2]
	    lappend implist [list $coupl $d1]
	}
	set implist [lsort -decreasing $implist]

	# print atom importance
	puts "$ph BRIDGE ATOM IMPORTANCE:"
	foreach imp $implist {
	    foreach { coupl atom } $imp { break }
	    puts "  [i2d top $atom] ==> $coupl"
	}
	puts ""
	
	# display atom importance
	if { $display } {

	    # color by beta
	    color scale method BGR
	    set atomlist {}
	    foreach imp $implist {
		foreach { coupl atom } $imp { break }
		set atom [expr $atom-1]
		set sel [atomselect $molstr "index $atom"]
		$sel set beta $coupl
		$sel delete
		lappend atomlist $atom
	    }

	    # display
	    mol addrep $molstr
	    mol modselect 0 $molstr "index $atomlist"
	    mol modstyle 0 $molstr Beads 1.0 $res
	    mol modmaterial 0 $molstr Transparent
	    mol modcolor 0 $molstr beta
	}

    }

    # restore viewpoints
    if { $display } {
	foreach m [molinfo list] {
	    if [info exists viewpoint($m)] {
		molinfo $m set {center_matrix rotate_matrix scale_matrix global_matrix} $viewpoint($m)
	    } else {
		molinfo $m set {center_matrix rotate_matrix scale_matrix global_matrix} $vp
	    }
	}
    }
	
    # create a snapshot
    if { $display && ($rnd != "none") } {
	display update
	render $rnd "${snap}.tga"
    }

    # clean up
    if { !$debug } {
	puts "$ph Cleaning up temporary files..."
	file delete "${out}.cb" "${out}.hb" "${out}1.out" "${out}2.out"
    }
    puts "$ph ALL DONE!"
    
    return $maxcoupl
    
}

# explore pathway dynamics over an MD trajectory
proc pathtraj { args } {

    # internal
    set ph "PATHTRAJ)"
    global path_version
    
        # detect display
    global env
    if { [catch {eval "set display $env(DISPLAY)"}] } {
	puts "$ph No display found"
	set display 0
    } else {
	puts "$ph Display $display found"
	set display 1
    }

    # find pathcore binary
    set pathcore [which "pathcore"]
    if { ![llength $pathcore] } {
	puts "$ph ERROR: can not find a pathcore binary!"
	puts "$ph        Make sure pathcore is installed and in your path!"
    } else {
	puts "$ph Found $pathcore"
    }

    # find convert binary
    set convert [which "convert"]
    if { ![llength $convert] } {
	puts "$ph ERROR: can not find a convert binary!"
	puts "$ph        Make sure Image Magick is installed and in your path!"
    } else {
	puts "$ph Found $convert"
    }

    # find ffmpeg binary
    set ffmpeg [which "ffmpeg"]
    if { ![llength $ffmpeg] } {
	puts "$ph ERROR: can not find a ffmpeg binary!"
	puts "$ph        Make sure FFMPEG is installed and in your path!"
    } else {
	puts "$ph Found $ffmpeg"
    }

    # detect display
    global env
    if { [catch {eval "set display $env(DISPLAY)"}] } {
	puts "$ph No display found"
	set display 0
    } else {
	puts "$ph Display $display found"
	set display 1
    }

    # get command line options
    global cmdline
    cmdoptions $args

    # sanity check
    if { [check_params {"-d" "-a"}] } {
	pathtraj_usage
        return 1
    }

    # set mandatory parameters
    set dstr $cmdline(-d)
    set astr $cmdline(-a)
    
    # Pathtraj-specific parameters
    set first 0; if {[info exists cmdline(-first)]} {set first $cmdline(-first)}
    set last 0; if {[info exists cmdline(-last)]} {set last $cmdline(-last)}
    set stride 1; if {[info exists cmdline(-stride)]} {set stride $cmdline(-stride)}

    # set optional rate parameters
    set lambda 999999; if {[info exists cmdline(-lambda)]} {set lambda $cmdline(-lambda)}
    set deltag 999999; if {[info exists cmdline(-deltag)]} {set deltag $cmdline(-deltag)}
    if { [expr abs($lambda*$deltag)] < 1000000 } {set printrate 1} else { set printrate 0}

    # set optional molecule parameters
    set mol [molinfo top]; if {[info exists cmdline(-mol)]} {set mol $cmdline(-mol)}
    set frame 0; if {[info exists cmdline(-frame)]} {set frame $cmdline(-frame)}
    set bstr "all"; if {[info exists cmdline(-b)]} {set bstr $cmdline(-b)}
    set p 1; if {[info exists cmdline(-p)]} {set p $cmdline(-p)}
    set q 0; if {[info exists cmdline(-q)]} {set q $cmdline(-q)}
    set renv 4.0; if {[info exists cmdline(-renv)]} {set renv $cmdline(-renv)}

    # set optional Pathways parameters
    set withh 0; if {[info exists cmdline(-withh)]} {set withh $cmdline(-withh)}
    set hcut 3.0; if {[info exists cmdline(-hcut)]} {set hcut $cmdline(-hcut)}
    set hang 30; if {[info exists cmdline(-hang)]} {set hang $cmdline(-hang)}
    set tscut 6.0; if {[info exists cmdline(-tscut)]} {set tscut $cmdline(-tscut)}
    set procut 7.0; if {[info exists cmdline(-procut)]} {set procut $cmdline(-procut)}
    set epsc 0.6; if {[info exists cmdline(-epsc)]} {set epsc $cmdline(-epsc)}
    set epsh 0.36; if {[info exists cmdline(-epsh)]} {set epsh $cmdline(-epsh)}
    set exph 1.7; if {[info exists cmdline(-exph)]} {set exph $cmdline(-exph)}
    set r0h 2.8; if {[info exists cmdline(-r0h)]} {set r0h $cmdline(-r0h)}
    set epsts 0.6; if {[info exists cmdline(-epsts)]} {set epsts $cmdline(-epsts)}
    set expts 1.7; if {[info exists cmdline(-expts)]} {set expts $cmdline(-expts)}
    set r0ts 1.4; if {[info exists cmdline(-r0ts)]} {set r0ts $cmdline(-r0ts)}

    # set optional output parameters
    set out "pathways"; if {[info exists cmdline(-o)]} {set out $cmdline(-o)}
    set debug 1; if {[info exists cmdline(-debug)]} {set debug $cmdline(-debug)}

    # set optional display parameters
    set sdba 0; if {[info exists cmdline(-sdba)]} {set sdba $cmdline(-sdba)}
    set rmax 0.5; if {[info exists cmdline(-rmax)]} {set rmax $cmdline(-rmax)}
    set res 20; if {[info exists cmdline(-res)]} {set res $cmdline(-res)}
    set mat "Opaque"; if {[info exists cmdline(-mat)]} {set mat $cmdline(-mat)}
    set rnd "TachyonInternal"; if {[info exists cmdline(-rnd)]} {set rnd $cmdline(-rnd)}
    set snap "snapshot"; if {[info exists cmdline(-snap)]} {set snap $cmdline(-snap)}

    # set optional experimental parameters
    set cs 0; if {[info exists cmdline(-cs)]} {set cs $cmdline(-cs)}
    set cf "collective.tcl"; if {[info exists cmdline(-cf)]} {set cf $cmdline(-cf)}

    # print header
    puts "$ph Pathways $path_version (C) 2008-2010 I. Balabin, X. Hu, and D. Beratan"

    # print parameters
    puts "$ph Starting pathway calculations:"
    puts ""
    puts "          MOLECULE:"
    puts "          Molecule:             [molinfo $mol get name]"
    puts "          Trajectory frame:     $frame"
    puts "          Donor selection:      $dstr"
    puts "          Acceptor selection:   $astr"
    puts "          Bridge selection:     $bstr"
    puts "          Number of pathways:   $p"
    puts "          Atom importance:      $q"
    puts "          Environment size:     $renv"
    puts ""
    if { $printrate } {
	puts "          RATE:"
	puts "          Reorg. energy        :$lambda eV"
	puts "          Driving force        :$deltag eV"
	puts ""
    }
    puts "          PATHWAYS:"
    puts "          Include hydrogens:    $withh"
    puts "          H-bond cutoff:        $hcut A"
    puts "          H-bond angle:         $hang deg."
    puts "          TS jump cutoff:       $tscut A"
    puts "          Protein pruning:      $procut A"
    puts "          Epsilon_c:            $epsc"
    puts "          Epsilon_H:            $epsh"
    puts "          Exp. H-bond:          $exph"
    puts "          Offset H-bond:        $r0h A"
    puts "          Epsilon_TS:           $epsts"
    puts "          Exp. TS jump:         $expts"
    puts "          Offset TS jump:       $r0ts A"
    puts ""
    puts "          OUTPUT:"
    puts "          Output prefix:        $out"
    puts "          Debugging mode on:    $debug"
    puts ""
    puts "          GRAPHICS:"
    puts "          Show D-B-A structure: $sdba"
    puts "          Max pathway radius:   $rmax"
    puts "          Pathway resolution:   $res"
    puts "          Pathway material:     $mat"
    puts "          Renderer:             $rnd"
    puts "          Snapshot (TGA):       $snap"
    puts ""
    puts "          EXPERIMENTAL:"
    puts "          Collective states     $cs"
    puts "          CS config file        $cf"


    # sanity check
    set sel [atomselect $mol "($dstr) and ($astr)"]
    if { [llength [$sel get name]] } {
	puts "$ph ERROR: donor and acceptor selections overlap!"
	return 1
    }
    $sel delete

    # renderer check
    if { $display } {
	set rndlist { "none" "snapshot" "TachyonInternal" }
	if { [lsearch $rndlist $rnd]<0 } {
	    puts "$ph ERROR: unknown renderer!"
	    return 1
	}
    }
    
    # set up movie directory
    set outdir "${out}.dir"
    if { [file exists $outdir] } { file delete -force $outdir }
    file mkdir $outdir

    # run Pathways for each MD trajectory frame
    set couplings {}
    set n [ molinfo $mol get numframes ]
    if { $first < 0 } {set first 0}
    if { $last == 0 || $last >= $n } {set last [expr $n-1]}
    for {set k $first} {$k<=$last} {incr k } {

	puts "$ph Processing frame $k"
	set thissnap [format "${outdir}/${snap}_%06d" $k]
	lappend couplings [pathways \
			       "-d" "$dstr" \
			       "-a" "$astr" \
			       "-lambda" "$lambda" \
			       "-deltag" "$deltag" \
			       "-mol" "$mol" \
			       "-frame" "$k" \
			       "-b" "$bstr" \
			       "-p" "$p" \
			       "-q" "$q" \
			       "-renv" "$renv" \
			       "-withh" "$withh" \
			       "-hcut" "$hcut" \
			       "-hang" "$hang" \
			       "-tscut" "$tscut" \
			       "-procut" "$procut" \
			       "-epsc" "$epsc" \
			       "-epsh" "$epsh" \
			       "-exph" "$exph" \
			       "-r0h" "$r0h" \
			       "-epsts" "$epsts" \
			       "-expts" "$expts" \
			       "-r0ts" "$r0ts" \
			       "-o" "$out" \
			       "-debug" "$debug" \
			       "-sdba" "$sdba" \
			       "-rmax" "$rmax" \
			       "-res" "$res" \
			       "-mat" "$mat" \
			       "-rnd" "$rnd" \
			       "-snap" "$thissnap" \
			       "-cs" "$cs" \
			       "-cf" "$cf"]

	# remove molecule for drawing pathways created by pathways
	if { $display } {
	    mol delete top
	}
	# remove clone molecule created by pathways
	mol delete top
    }
    # restore original view
    mol on $mol

    # convert Targa images into PNG format for FFMPEG
    puts "$ph Converting Targe images into PNG format"
    foreach file [glob "${outdir}/${snap}*.tga"] {

	regsub {\.tga} "$file" {.png} pfile
	set comlist [list $convert $file $pfile]
	puts "$ph Running \"$comlist\""
	if { [catch {eval "exec $comlist"} pout ] } {
	    puts "$ph WARNING: convert generated warnings/errors:\n$pout"
	} else {
	    puts "$ph  $file ==> $pfile"
	}

    }

    # create a movie
    puts "$ph Creating the movie from snapshots"
    set comlist [list "$ffmpeg" "-y" "-i" "${outdir}/${snap}_%06d.png" "${out}.mpg"]
    puts "$ph Running \"$comlist\""
    if { [catch {eval "exec $comlist"} pout ] } {
        puts "$ph WARNING: ffmpeg generated warnings/errors:\n$pout"
    }
    puts "$ph Created the movie"

    # analyze pathway coupling statistics
    set cplmean [vecmean $couplings]
    set cplstdd [vecstddev $couplings]
    set cplmean2 [expr $cplmean * $cplmean + $cplstdd * $cplstdd]
    set cohpar [expr $cplmean * $cplmean / $cplmean2]
    puts "$ph PATHWAY COUPLING STATISTICS:"
    puts "$ph   <T_DA>:       $cplmean"
    puts "$ph   \sigma(T_DA): $cplstdd"
    puts "$ph   <T_DA^2>      $cplmean2"
    puts "$ph   \"Coherence\"   $cohpar"
}
