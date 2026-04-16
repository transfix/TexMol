# CVC Ecosystem Modernization — Implementation Progress

Tracking implementation of the [CVC Modernization Plan](CVC-modernization-plan.md).

**Branch:** `modernization`
**Started:** 2026-04-15

---

## Phase 0: Repo Cleanup ✅

**Commit:** `e56a381b` — Phase 0: Repo cleanup — add .gitignore, remove build artifacts

### Completed
- [x] Created comprehensive `.gitignore` (CMake artifacts, objects, libraries, executables, IDE files, coverage)
- [x] Removed root-level CMake generated files: `CMakeCache.txt`, `cmake_install.cmake`, `Makefile`, `CPackConfig.cmake`, `CPackSourceConfig.cmake`
- [x] Removed `CMakeFiles/` directory from repo root
- [x] Removed pre-built binary wrapper (`bin/runTexMol`)
- [x] Removed pre-built vendored static libraries (`lib/libgmp.a`, `lib/libmpfr.a`, etc.)
- [x] Removed vendored third-party headers (`include/gmp.h`, `include/mpfr.h`, `include/mpf2mpfr.h`)
- [x] Removed entire `release/` build tree (~687MB of committed build artifacts, headers, libraries)
- [x] Removed all 35 `.Qt3.ui` files — Qt3 is 20+ years EOL
- [x] Removed committed binary data (`v2.rawiv`) and generated config files (`host.ini`, `includes.dot`)

### Stats
- ~19,920 files removed from tracking
- ~2.9M lines of generated/binary content removed

---

## Phase 1: Module Audit — libcvc Replacement Mapping ✅

**Commit:** `359cc89d` — Phase 1: Remove 17 modules fully replaced by libcvc

### Modules Removed (17 total, headers + sources)

| Module | libcvc Replacement |
|--------|-------------------|
| `SimpleVolumeData` | `cvc::voxels`, `cvc::volume` |
| `VolMagick` | `cvc::volume_file_io` |
| `VolumeFileTypes` | `cvc::volume_file_io` |
| `VolumeLibrary` | `cvc::algorithm` + `cvc::voxels` filters |
| `SignDistanceFunction` | `cvc::sdf(SDF_V1)` |
| `SignDistanceFunction_v2` | `cvc::sdf(SDF_V2)` |
| `Contouring` | `cvc::iso()` |
| `Contour` | `cvc::iso()` |
| `LBIE_lib` | `cvc::iso(LBIE)` via cvc-mesher |
| `Geometry` | `cvc::geometry` |
| `ComputeNormals` | `cvc::geometry::calculate_surf_normals()` |
| `ByteOrder` | `cvc::endians.h` |
| `CVC` (App/State) | `cvc::app`, `cvc::state` |
| `Utility` | `cvc::utility.h` |
| `XmlRPC` | Deprecated; replaced by `cvc serve` (Phase 14) |
| `GeometryFileTypes` | `cvc::geometry_file_io` |
| `GraphicsPrimitives` | `cvc::utility` (affine, color) |

### Stats
- 411 files removed
- ~422K lines of redundant code eliminated
- `src/CMakeLists.txt` updated with removal documentation

### Remaining Modules (~46)
The following modules remain in TexMol after Phase 1:

**Kept (GUI/visualization):** `ColorManager`, `ColorTable2`, `DataManager`, `GeometryRenderer`, `Histogram`, `ImposterRenderer`, `ObjectRenderer`, `OpenGL_Viewer`, `QGLViewer`, `RayTracer`, `SplattingRenderer`, `TexMol` (main), `glew`, `vis-lib`

**Kept (compute — candidates for libcvc in Phase 10c):** `ASpline`, `AssemblyUtils`, `Blurmaps`, `C2C`, `ColorSurfaceByFunction`, `contourtree`, `Curvatures`, `Decimation`, `DPG`, `DynamicAdaptiveGrid`, `fast-clash`, `fast-hydro`, `fast-LJ`, `fast-resCont`, `GOAFileTypes`, `HLevelSet`, `Interface`, `levmar-2.5`, `libCG`, `MolecularCharacteristics`, `MolEnergyUtil`, `MolSurfAPI`, `MultiresolutionSurface`, `Nurbs`, `PatchComplex`, `PDBParser`, `Pocket`, `PocketTunnel`, `SecondaryStructures`, `SmoothingFunction`, `SphericalPatchIntersections`, `Summation`, `UsefulMath`, `VirusAssembly`, `InterfaceStats`

**Kept (conditional):** `FastSummation`, `GBEnergy` (NFFT), `KIFMM3D`, `TriElement`, `PB2` (PETSc), `F2DockClient`, `log4cplus`

---

## Phase 2: Integrate libcvc as Core Dependency 🔄

**Status:** In progress

---

## Phases 3–9: Pending

See [CVC-modernization-plan.md](CVC-modernization-plan.md) for details.
