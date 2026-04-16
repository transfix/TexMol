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

## Phase 2+3: CMake Modernization + libcvc Integration ✅

**Commit:** `74b8691c` — Phase 2+3: CMake modernization + libcvc integration

### Top-Level CMakeLists.txt (complete rewrite)
- [x] `cmake_minimum_required(VERSION 3.16)`, C++17
- [x] libcvc via `add_subdirectory(LIBCVC_SOURCE_DIR)` or `FetchContent` (SHA `573a89bf...`)
- [x] `AUTOMOC`, `AUTOUIC`, `AUTORCC` enabled globally
- [x] Qt6 `find_package` (Core, Gui, Widgets, Xml, OpenGL, OpenGLWidgets)
- [x] Modern `find_package` for OpenGL, GLEW, GSL, FFTW, CGAL, LAPACK
- [x] Removed Cg shader support (NVIDIA discontinued)
- [x] Removed 19 obsolete CMake Find/Setup modules
- [x] Created `CMake/LegacyCompat.cmake` — bridge macros for old `SetupBoost()` / `SetupQt()` / etc. calls
- [x] Feature flag options: `HAVE_CGAL`, `HAVE_LAPACK`, `HAVE_NFFT`, `HAVE_PETSC`, etc.
- [x] CPack packaging preserved

### Per-Module Qt6 Migration (6 modules)
- [x] `Histogram` — removed Qt3/Qt4 wrapping, added Qt6 targets
- [x] `ColorTable2` — disabled contour tree/spectrum (depended on deleted modules), Qt6 targets
- [x] `QGLViewer` — AUTOMOC/AUTOUIC, Qt6 targets, VRender optional
- [x] `DataManager` — removed Qt3/Qt4 conditional blocks, AUTOMOC/AUTOUIC, Qt6 targets
- [x] `F2DockClient` — removed Qt3/Qt4 blocks, removed SubmitDockingJob/CheckStatus (XmlRPC deleted)
- [x] `TexMol` (main) — link list updated: deleted modules → `cvc`, Qt6 targets

### .ui File Renames (22 files)
- [x] Renamed all `.Qt4.ui` → `.ui` across QGLViewer, DataManager, F2DockClient, TexMol/Dialogs

### libcvc Integration Fixes
- [x] Fixed `CMAKE_SOURCE_DIR` → `PROJECT_SOURCE_DIR` in libcvc (3 files) — enables `add_subdirectory` use
- [x] Disabled `volrover3` build when used as subdirectory
- [x] Added libcvc include paths globally for compatibility shims

### Compatibility Shim Headers (Phase 5 bridge, `inc/`)
| Shim | Redirects To |
|------|-------------|
| `Geometry/Geometry.h` | `cvc/geometry.h` |
| `SimpleVolumeData/SimpleVolumeData.h` | `cvc/voxels.h` + `cvc/volume.h` |
| `LBIE_lib/Geoframe.h` | `cvc/geometry.h` |
| `GeometryFileTypes/GeometryLoader.h` | `cvc/geometry_file_io.h` |
| `XmlRPC/XmlRpc.h` | `xmlrpc/XmlRpc.h` (case fix) |
| `Utility/utility.h` | `cvc/utility.h` |
| `qgl.h` | `QOpenGLWidget` / `QOpenGLContext` (Qt6 compat) |

Restored from git (still needed by remaining modules):
- `Utility/utils.h`, `Geometry/MyExtensions.h`, `Geometry/GeometryScene.h`
- `LBIE_lib/normalspline.h`, `SimpleVolumeData/SimpleVolumeDataIsocontourer.h`

### Status
- cmake configures successfully ✅
- Compilation blocked by Phase 5 (#include rewiring) and Phase 6 (Qt6 source migration)

---

## QGLViewer Upgrade: v2.3.6 → v3.0.0 ✅

**Commit:** `9e84e7aa` — Replace QGLViewer v2.3.6 (2008) with v3.0.0 from volrover

- [x] Replaced TexMol's ancient QGLViewer v2.3.6 (2008, GPL) with volrover's v3.0.0 (2025, LGPL)
- [x] Native `QOpenGLWidget` base class (no `QGLWidget` compatibility shim needed in QGLViewer itself)
- [x] Qt 5.4+ / Qt 6 support built-in
- [x] No bundled GLEW dependency in headers
- [x] Modern CMake with explicit file lists and proper target exports
- [x] Builds cleanly ✅

---

## Phase 5: Qt6 Migration ✅

**No single commit yet — staged on `modernization` branch**

### Compile Fixes (Qt3/Qt4 → Qt6 API migration)

#### MainWindow.cpp (4024 lines — 66 compile errors → 0)
- [x] Added `#include <QFileDialog>`, `#include <QColorDialog>`
- [x] `setCaption()` → `setWindowTitle()`
- [x] `addWidget(label, 0, true)` → `addPermanentWidget(label)` (5 status bar widgets)
- [x] `Q3ListBoxItem` rightButtonClicked signal → commented out (needs QListWidget port)
- [x] `highlighted(int)` → `currentRowChanged(int)` for QListWidget
- [x] Q3Http constructor and connects → commented out (stubbed `downloadPDBSlot`)
- [x] QFileDialog old API (6-arg) → new API (parent, caption, dir, filter)
- [x] `currentItem()` → `currentRow()` for QListWidget
- [x] `insertItem(text, -1)` → `addItem(text)` for QListWidget
- [x] `.ascii()` → `.toLatin1().constData()`
- [x] `QGridLayout::remove()` → `removeWidget()`
- [x] `createLightsMenu()`: `insertItem`/`connectItem`/`setItemParameter` → `addAction()` + lambda connects + `m_LightActions` QList
- [x] `enableFirstLight()`/`editLight()`: `changeItem()` → `m_LightActions[i]->setText()`
- [x] `showPopup()`: `insertItem` → `addAction()`, `exec()` returns `QAction*`
- [x] `ViewRowEventMain`: `QCustomEvent` → `QEvent`, `static_cast<QEvent::Type>(QEvent::User+105)`
- [x] QFileDialog::getSaveFileName old 5-arg → new 4-arg order
- [x] `atoi(dataTypes[c])` → `dataTypes[c].toInt()`
- [x] `surfaceDialogSlot` fully stubbed (SurfaceDialog member references removed)
- [x] `finishedop` stubbed (Q3Http code removed)
- [x] `dataSetSelectedRightMouse` stubbed (Q3ListBoxItem param type removed)

#### Server.cpp
- [x] VolMagick include → commented out (VolMagick now restored as compat lib)
- [x] pqr.h include → commented out
- [x] `shrinkPQRintoSurface()` fully stubbed (was VolMagick-dependent)
- [x] `SDFLibrary::getSDF()` calls → `nullptr` (sdfLib.h removed)

#### Other Header/Source Fixes
- [x] `inc/SignDistanceFunction_v2/geom.h`: `EPS` → `SDF_GEOM_EPS` (macro conflict)
- [x] `inc/TexMol/Dialogs/SetViewingParametersDialog.h`: `setViewingParametersDialogBase` → `SetViewingParametersDialogBase` (case fix)
- [x] `inc/TexMol/Dialogs/MainWindow.h`: Added `QList<QAction*> m_LightActions` member
- [x] `src/TexMol/MainWindowBase.ui`: `Q3ListBox` → `QListWidget`, `Q3Frame` → `QFrame`

### Modules Restored as Internal Compatibility Libraries
Modules deleted in Phase 1 that remaining code still depends on were restored from git (`359cc89d~1`) as internal static libraries:

| Module | Reason Restored |
|--------|----------------|
| `VolumeFileTypes` | DataManager, VolumeLibrary file loading |
| `VolumeLibrary` | Volume rendering pipeline |
| `Contouring` | User requested contour re-enablement |
| `Contour` | User requested contour re-enablement |
| `contourtree` | ColorTable2 computeCT dependency |
| `XmlRPC` | CVC::State networking (optional) |
| `libCG` (ARM, OptimizerLib, CoarseGrain) | MOLECULE namespace, GEOMETRY::Surface |
| `CVC` (App, State) | VolMagick, log4cplus integration |
| `VolMagick` | ColorTable2 volume type system |
| `ByteOrder` | VolumeFileTypes endian handling |
| `Utility` | Various module utility functions |
| `SimpleVolumeData` | VolumeFileTypes, DataManager |
| `Geometry` | GeometryFileTypes, ComputeNormals |
| `ComputeNormals` | GEOMETRY::Surface mesh normals |
| `GeometryFileTypes` | Geometry file I/O |
| `LBIE_lib` | Mesh generation |
| `SignDistanceFunction_v2` | SDF computation |

### C++17 Compatibility Fixes (in restored modules)
- [x] `inc/CVC/BoundingBox.h`: Removed dynamic exception specifications (`throw(...)`)
- [x] `inc/CVC/Exception.h`: `throw()` → `noexcept`
- [x] `inc/VolMagick/VolumeCache.h`: `dimcmp::operator()` → `const`
- [x] `src/CVC/State.cpp`: `_1` → `boost::placeholders::_1`
- [x] `src/CVC/CMakeLists.txt`: `CVC_USING_HDF5` default → `OFF`
- [x] `src/libCG/CoarseGrain/groupOfAtoms.cpp`: FastSummation guarded with `#ifdef HAVE_NFFT`
- [x] `src/libCG/CoarseGrain/CMakeLists.txt`: `SetupNFFT()` guarded with `if(HAVE_NFFT)`
- [x] `src/VolumeFileTypes/RawVFile.cpp`: `return false` → `return nullptr` (pointer-returning functions)
- [x] `src/levmar-2.5/lm.h`: Fixed for C++17 compatibility

### Dialog Stub System
Created `src/TexMol/Dialogs/dialog_stubs.cpp` — provides link-time symbols for 14 Qt3 dialog classes excluded from compilation (Phase 6 TODO: full Qt6 port):

**Stubbed dialogs:** AboutDialog, ConstructCurvaturesDialog, ConstructDepthColoredVolumesDialog, ConstructVolumes, FileSaveDialog, LightsDialog, MouseHandlerDialog, MovieImageFileSaveDialog, ScriptsDialog, SetViewingParametersDialog, SliceDialog, SurfaceAreaAndVolumeDialog, SurfaceDialog, TransformationsDialog

**Also stubbed:** DownloadPDB (constructor/destructor/blockedDownload/finishedop), MouseHandler (constructor/destructor/updateUserPreferences/getUserSelectedTransformation)

**Moc integration:** Explicitly includes moc output for Q_OBJECT dialog classes (AboutDialog, ConstructCurvaturesDialog, ConstructVolumesDialog, DownloadPDB)

### Link Resolution
- [x] TexMol executable links 28+ internal static libraries
- [x] Explicit `-lGL` for `glBindFramebufferEXT`
- [x] All dialog slot stubs for AUTOMOC vtable resolution

### Build Status
- **Compile errors:** 0 ✅
- **Linker errors:** 0 ✅
- **Executable:** `build/bin/TexMol` (19.6 MB) ✅
- **Configure:** `cmake -B build -DLIBCVC_SOURCE_DIR=... -DHAVE_PETSC=OFF -DHAVE_NFFT=OFF -DCVC_ENABLE_CUDA=OFF -DCVC_USING_HDF5=OFF`

---

## Phase 6: OpenGL Modernization (Partial) ✅

**Commit:** `72b20f23` — Phase 6: Replace bundled GLEW 1.7.0 with system GLEW 2.2.0

### Completed
- [x] Replaced bundled GLEW 1.7.0 (`src/glew/`, `inc/glew/`) with system GLEW 2.2.0 via `find_package(GLEW REQUIRED)`
- [x] Changed `#include <glew/glew.h>` → `#include <GL/glew.h>` in 13 ImposterRenderer/ObjectRenderer files
- [x] Added `glewInit()` with `glewExperimental = GL_TRUE` in `RenderAreaWidgetBase::initializeGL()`
- [x] Ported 7 files from `MyExtensions*` wrapper to direct GLEW calls:
  - `OpenGLShadingLanguageRenderer.cpp/.h` — shader create/compile/link/delete
  - `BumpMapRenderer.cpp/.h` — extension availability check
  - `RenderAreaWidget.cpp` — FBO creation/deletion (removed `#ifdef USE_MY_EXTENSION` blocks)
  - `ImposterSphereRenderer.cpp/.h` — VBO buffer operations
  - `OpenGLSphereRenderer.cpp/.h` — VBO operations, uses `GLEW_VERSION_1_5` for VBO availability
  - `PlainImposterSphereRenderer.cpp/.h` — shader program bind/unbind
  - `FunctionImposterSphereRenderer.cpp/.h` — constructor simplified
- [x] Removed `m_Extensions` members and `MyExtensions*` constructor parameters from ported classes
- [x] Fixed `GL/glew.h` include ordering (must precede `GL/gl.h`) in headers and .cpp files
- [x] Fixed `src/ImposterRenderer/CMakeLists.txt`: `set(LIBS "glew")` → `set(LIBS GLEW::GLEW)`
- [x] Removed `ADD_SUBDIRECTORY(glew)` from `src/CMakeLists.txt`

### Deferred
- [ ] Port remaining MyExtensions consumers: Geometry, VolumeLibrary modules (~10 files)
- [ ] Remove `StaticExtensionPointers.h` files (2 copies, ~1400 function pointers each)
- [ ] Remove Cg shader remnants (`GlobalCGContext.h`)
- [ ] Set GL compatibility profile via `QSurfaceFormat`
- [ ] Convert fixed-function GL (`glBegin`/`glEnd`) to modern GL (44 call sites across 17 files)

### Build Status
- **Errors:** 0
- **Net change:** 31 files, +120 −337 lines

---

## Phases 7–9: Pending

See [CVC-modernization-plan.md](CVC-modernization-plan.md) for details.
