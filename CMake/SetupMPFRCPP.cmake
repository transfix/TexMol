#
# This macro is for setting up a sub-project to use FFTW (float or double version)
#

macro(SetupMPFRCPP)

  set(MPFRCPP_FOUND NOTFOUND)

  find_package(MPFRCPP)

  MESSAGE(MPFRCPP_LIB: ${MPFRCPP_LIB})

endmacro(SetupMPFRCPP)
