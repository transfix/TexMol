cmake_minimum_required(VERSION 3.16)

# ===========================================================================
# Internal compatibility libraries (restored from Phase 1 deletion)
# These modules are kept as internal code until consumers are adapted
# to use libcvc APIs directly.  Will be removed in future phases.
# ===========================================================================
# ByteOrder: now header-only, delegates to cvc/endians.h (Phase 11)
ADD_SUBDIRECTORY(Utility)
ADD_SUBDIRECTORY(SimpleVolumeData)
ADD_SUBDIRECTORY(Geometry)
ADD_SUBDIRECTORY(ComputeNormals)
ADD_SUBDIRECTORY(GeometryFileTypes)
ADD_SUBDIRECTORY(LBIE_lib)
ADD_SUBDIRECTORY(SignDistanceFunction_v2)
ADD_SUBDIRECTORY(VolumeFileTypes)
ADD_SUBDIRECTORY(VolumeLibrary)
ADD_SUBDIRECTORY(Contouring)
ADD_SUBDIRECTORY(Contour)
ADD_SUBDIRECTORY(XmlRPC)
ADD_SUBDIRECTORY(libCG)
ADD_SUBDIRECTORY(CVC)
ADD_SUBDIRECTORY(VolMagick)

# ===========================================================================
# Phase 1: Modules FULLY REMOVED — replaced by libcvc
# ===========================================================================
# VolMagick           → cvc::volume_file_io (shim header in inc/)
# VolumeFileTypes     → cvc::volume_file_io
# VolumeLibrary       → cvc::algorithm + cvc::voxels filters
# SignDistanceFunction (v1) → cvc::sdf(SDF_V1)
# Contouring          → cvc::iso()
# Contour             → cvc::iso()
# CVC (App/State)     → cvc::app, cvc::state
# XmlRPC              → deprecated; replaced by cvc serve (Phase 14)
# GraphicsPrimitives  → cvc::utility (affine, color)
# ===========================================================================

#ADD_SUBDIRECTORY(AdaptiveIsocontouring)
ADD_SUBDIRECTORY(ASpline)
ADD_SUBDIRECTORY(AssemblyUtils)
ADD_SUBDIRECTORY(VirusAssembly)
ADD_SUBDIRECTORY(Blurmaps)
ADD_SUBDIRECTORY(C2C)
# Bundled log4cplus 1.0.4 removed — no longer needed (Phase 9)
# libcvc provides log4cplus if CVC_LOG4CPLUS_DEFAULT=ON
ADD_SUBDIRECTORY(ColorManager)
ADD_SUBDIRECTORY(ColorTable2)
ADD_SUBDIRECTORY(ColorSurfaceByFunction)
ADD_SUBDIRECTORY(Curvatures)
ADD_SUBDIRECTORY(Decimation)
#ADD_SUBDIRECTORY(Docking)
ADD_SUBDIRECTORY(DPG)
ADD_SUBDIRECTORY(DynamicAdaptiveGrid)
#ADD_SUBDIRECTORY(F2DockAnimator)
ADD_SUBDIRECTORY(fast-clash)
ADD_SUBDIRECTORY(fast-hydro)
ADD_SUBDIRECTORY(fast-LJ)
ADD_SUBDIRECTORY(fast-resCont)
ADD_SUBDIRECTORY(F2DockClient)
ADD_SUBDIRECTORY(GeometryRenderer)
ADD_SUBDIRECTORY(GOAFileTypes)
ADD_SUBDIRECTORY(Histogram)
ADD_SUBDIRECTORY(HLevelSet)
ADD_SUBDIRECTORY(ImposterRenderer)
ADD_SUBDIRECTORY(Interface)
ADD_SUBDIRECTORY(levmar-2.5)
ADD_SUBDIRECTORY(MolecularCharacteristics)
ADD_SUBDIRECTORY(Nurbs)
ADD_SUBDIRECTORY(ObjectRenderer)
ADD_SUBDIRECTORY(OpenGL_Viewer)
ADD_SUBDIRECTORY(PatchComplex)
ADD_SUBDIRECTORY(PDBParser)
ADD_SUBDIRECTORY(Pocket)
ADD_SUBDIRECTORY(RayTracer)
ADD_SUBDIRECTORY(SmoothingFunction)
ADD_SUBDIRECTORY(SphericalPatchIntersections)
ADD_SUBDIRECTORY(SplattingRenderer)
ADD_SUBDIRECTORY(Summation)
ADD_SUBDIRECTORY(UsefulMath)
ADD_SUBDIRECTORY(vis-lib)

IF (ENABLE_POCKET_TUNNEL)
ADD_SUBDIRECTORY(PocketTunnel)
ENDIF (ENABLE_POCKET_TUNNEL)
IF (ENABLE_SECONDARY_STRUCTURE)
ADD_SUBDIRECTORY(SecondaryStructures)
ENDIF (ENABLE_SECONDARY_STRUCTURE)

IF (BUILD_MOLSURF_API)
ADD_SUBDIRECTORY(MolSurfAPI)
ENDIF (BUILD_MOLSURF_API)

ADD_SUBDIRECTORY (MolEnergyUtil)

IF(HAVE_NFFT)
ADD_SUBDIRECTORY (FastSummation)
ADD_SUBDIRECTORY (GBEnergy)
ENDIF(HAVE_NFFT)

if (HAVE_PETSC)
ADD_SUBDIRECTORY (KIFMM3D)
ADD_SUBDIRECTORY (TriElement)
ADD_SUBDIRECTORY (PB2)
endif(HAVE_PETSC)

if(ENABLE_MULTIRESOL_SURFACE)
ADD_SUBDIRECTORY(MultiresolutionSurface)
endif(ENABLE_MULTIRESOL_SURFACE)

ADD_SUBDIRECTORY(QGLViewer)
# Bundled glew removed — using system GLEW (Phase 6)
ADD_SUBDIRECTORY(contourtree)

ADD_SUBDIRECTORY(DataManager)

ADD_SUBDIRECTORY(TexMol)

#OPTION(DISABLE_BUILD_COARSEGRAIN "Doesn't build outside of CVC machines" OFF)

if(BUILD_INTERFACESTATS)
  ADD_SUBDIRECTORY(InterfaceStats)
endif(BUILD_INTERFACESTATS)
