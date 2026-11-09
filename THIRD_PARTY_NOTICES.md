# Licensing and third-party notices

TexMol is licensed under the **GNU Lesser General Public License, version 2.1** (see [LICENSE](LICENSE)),
the same licence as libcvc. This matches the per-file headers: 901 of TexMol's source files carry a
"GNU Lesser General Public License version 2.1" header, © The University of Texas at Austin, and
[`doc/LICENSE`](doc/LICENSE) records TexMol's original distribution under the LGPL.

(Between 2026-05-18 and 2026-10-10 the root `LICENSE` file held the GPL-2.0 text. That was a mistake; the
project licence was always intended to match libcvc. Until 2026-11-08, 10 UT-held files, the `C2C` arithlib
headers, `ColorTable2` and the `fast-hydro` element table, had GPL-2.0-or-later headers, and 520 more UT files
had an LGPL-2.1 grant followed by GPL boilerplate. Those were mistakes too; all of them now carry the LGPL-2.1
header.)

## Files under the GNU General Public License

The files below carry GPL headers (version 2, or any later version) and are **not** covered by TexMol's LGPL
grant; each remains under the licence its header states. The table records whether the default build
compiles them into TexMol, so that anyone distributing binaries knows which obligations apply.

| Component | Files | Copyright holder (per header) | Licence | In the default build? |
|---|---|---|---|---|
| `libdjvu++` ZP coder in `C2C` (`inc/C2C/arithlib/libdjvu++/`, `src/C2C/arithlib/libdjvu++/`) | 13 | LizardTech, Inc. (1999–2000) | GPL-2.0 | **Yes** — `src/C2C` is always built and linked by the TexMol app |
| `levmar-2.5` (`src/levmar-2.5/`) | 21 | Manolis Lourakis (2004) | GPL-2.0-or-later | **Yes** — linked by the TexMol app |
| VRender (`inc/QGLViewer/VRender/`, `src/QGLViewer/VRender/`) | 28 | Cyril Soler (2005); each file also carries the libQGLViewer header (Gilles Debunne, LGPL-3.0-or-later) | GPL-2.0-or-later | **Yes** — compiled into the vendored QGLViewer library, which `ColorTable2` links, unless `-DNO_VECTORIAL_RENDER=ON` |
| `KIFMM3D` (`inc/KIFMM3D/`, `src/KIFMM3D/`) | 47 | Lexing Ying, New York University (2004) | GPL-2.0-or-later | No — only when PETSc is found (`HAVE_PETSC`) |
| `FastSummation` (`inc/FastSummation/fastsum.h`, `src/FastSummation/fastsum.c`) | 2 | Jens Keiner, Stefan Kunis, Daniel Potts (2002, 2009); `fastsum.h` also carries a UT LGPL-2.1 header | GPL-2.0-or-later | No — only when NFFT is found (`HAVE_NFFT`) |

**Consequence for binaries.** Because the default build links `libdjvu++`, `levmar-2.5` and VRender, a TexMol
binary built with the default configuration is a combined work that must be distributed under the GPL's terms,
and VRender's GPC clipper forbids commercial use (see below). The TexMol sources themselves remain LGPL-2.1.
Removing these components is planned: `levmar` and the `libdjvu++` coder are replaced, and the optional
`KIFMM3D` and `FastSummation` paths are dropped. Configuring with `-DNO_VECTORIAL_RENDER=ON` leaves VRender
and GPC out, at the cost of EPS/PS/XFIG snapshot export.

## Other third-party code

| Component | Path | Copyright holder | Licence | In the default build? |
|---|---|---|---|---|
| libQGLViewer 3.0.0 | `inc/QGLViewer/`, `src/QGLViewer/` | Gilles Debunne (2002–2025) | LGPL-3.0-or-later (https://github.com/GillesDebunne/libQGLViewer) | **Yes** — linked through `ColorTable2` |
| Generic Polygon Clipper (GPC) 2.32 | `inc/QGLViewer/VRender/gpc.h`, `src/QGLViewer/VRender/gpc.cpp` | Alan Murta, Advanced Interfaces Group, University of Manchester (1997–2004) | Free for non-commercial use only; commercial use needs the author's consent. Not an open-source licence, and not GPL-compatible. These two files also carry the VRender and libQGLViewer headers. | **Yes** — part of VRender |

## Other notices

- [`doc/LICENSE`](doc/LICENSE) — the original UT TexMol notice, including the citation request for published
  work.
- `share/doc/mpfr/` — documentation of the bundled MPFR library, under its own licence (see the files there).
