#!/usr/bin/env python3

"""
buildMicroblaze.py
Automate building of microblaze

This calls on the build_microblaze.tcl script in the /tools,
and will add in the /mb and /miPod directories to the workspace to be built.

The output for miPod will show up under /miPod/debug
"""
import os


def output_helper(dev_path):
    """ A helper that handles mb build output, including saving old mipod copies """

    mi_path = dev_path + "/mb//miPod/Debug/"
    mipod_elf = mi_path + "miPod.elf"
    mipod = mi_path + "miPod"
    mipod_old = mi_path + "miPod_old"

    # may want to make a copy of the old miPod
    try:
        if os.path.exists(mipod):
            print("Saving %s as %s\n" % (mipod, mipod_old))
            os.rename(mipod, mipod_old)

            print("Please delete or save your old miPod copy\n")

        os.rename(mipod_elf, mipod)
        print("Output produced: %s\n" % mipod)

    except Exception as err:
        print("Error renaming miPod:\n {%s}\n" % err)

def build_microblaze(xsct, proj_name, dev_path_tools, dev_path):
    """ Automate calling a tcl script that builds microblaze"""

    build_tcl = dev_path_tools + "/build_microblaze.tcl"
    # xsct script.tcl arg1 arg2
    xsct_script = xsct + " " + build_tcl + " " + proj_name + " " + dev_path
    os.system(xsct_script)
    
    output_helper(dev_path)
