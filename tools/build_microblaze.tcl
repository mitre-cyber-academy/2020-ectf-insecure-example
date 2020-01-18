#******************************************************************
# buildMicroblaze.tcl: Tcl script for automating microblaze building
#
# This file contains tcl commands for building the microblaze project
# after successful generation of the bitstream
#
# Example expected dev_path -- repo root of 2020-ectf:
#   /media/sf_Vagrant/2020-ectf/
#
# ** Note ** this is mainly to automate the first time build.
#    Subsequent builds mainly need to set the workspace and build
#******************************************************************

if { $argc != 2 } {
    puts "buildMicroblaze requires the user to provide the following:
    project_name and dev_path."
    exit 1
}

set project_name [lindex $argv 0]
set dev_path [lindex $argv 1]

set worksp "$dev_path/mb"

set pl_sdk "$dev_path/pl/proj/$project_name/$project_name.sdk"
set pl_sysdef "$dev_path/pl/proj/$project_name/$project_name.runs/impl_1/system_wrapper.sysdef"
set mb_hdf "$worksp/system_wrapper.hdf"
set mipod "$dev_path/miPod"

file mkdir $pl_sdk
file copy -force $pl_sysdef $mb_hdf

setws $worksp

importprojects $worksp
importprojects $mipod

# only need if doesn't already exist
if {[catch {createhw -name $project_name -hwspec "$worksp/system_wrapper.hdf"} errmsg]} {
    puts "Already created the project hw"
}

projects -clean
projects -build
