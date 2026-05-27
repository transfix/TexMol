cmake_minimum_required(VERSION 3.16)

ADD_SUBDIRECTORY (ARM)
ADD_SUBDIRECTORY (OptimizerLib)

# this is redundant in MolSurf but
# TexMol needs this to work...
ADD_SUBDIRECTORY (CoarseGrain)

