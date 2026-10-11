# Licensing and third-party notices

TexMol is licensed under the **GNU Lesser General Public License, version 2.1** (see [LICENSE](LICENSE)),
the same licence as libcvc. This matches the per-file headers: 891 of TexMol's source files carry a
"GNU Lesser General Public License version 2.1" header, © The University of Texas at Austin, and
[`doc/LICENSE`](doc/LICENSE) records TexMol's original distribution under the LGPL.

(Between 2026-05-18 and this change the root `LICENSE` file held the GPL-2.0 text. That was a mistake; the
project licence was always intended to match libcvc.)

## Files under the GNU General Public License

The files below carry GPL headers (version 2, or any later version) and are **not** covered by TexMol's LGPL
grant; each remains under the licence its header states. The table records whether the default build
compiles them into TexMol, so that anyone distributing binaries knows which obligations apply.

| Component | Files | Copyright holder (per header) | Licence | In the default build? |
|---|---|---|---|---|
| `C2C` arithmetic coder (`inc/C2C/arithlib/`, `src/C2C/arithlib/libdjvu++/`) | 19 | The University of Texas at Austin (2000–2003); LizardTech, Inc. (libdjvu++, 1999–2000) | GPL-2.0-or-later; libdjvu++ GPL-2.0 | **Yes** — `src/C2C` is always built and linked by the TexMol app |
| `levmar-2.5` (`src/levmar-2.5/`) | 21 | Manolis Lourakis (2004) | GPL-2.0-or-later | **Yes** — linked by the TexMol app |
| `ColorTable2` (`inc/ColorTable2/`, `src/ColorTable2/`) | 3 | The University of Texas at Austin (2002–2003) | GPL-2.0-or-later | **Yes** — linked by the TexMol app |
| `fast-hydro` element table (`inc/fast-hydro/ElementInformation.h`) | 1 | The University of Texas at Austin (2003) | GPL-2.0-or-later | **Yes** — included by `pseudoGsol.h` |
| `KIFMM3D` (`inc/KIFMM3D/`, `src/KIFMM3D/`) | 47 | Lexing Ying, New York University (2004) | GPL-2.0-or-later | No — only when PETSc is found (`HAVE_PETSC`) |
| `FastSummation` (`src/FastSummation/fastsum.c`) | 1 | Jens Keiner, Stefan Kunis, Daniel Potts (2002, 2009) | GPL-2.0-or-later | No — only when NFFT is found (`HAVE_NFFT`) |

**Consequence for binaries.** Because the default build links `C2C`, `levmar-2.5`, `ColorTable2` and the GPL
element table, a TexMol binary built with the default configuration is a combined work that must be
distributed under the GPL's terms. The TexMol sources themselves remain LGPL-2.1. Removing these components
is planned: `levmar` is replaced, the UT-owned `C2C` arithlib, `ColorTable2` and element table are
relicensed by UT or replaced (F2Dock already carries an LGPL-2.1 copy of the same element table), and the
optional `KIFMM3D` and `FastSummation` paths are dropped.

## Other notices

- [`doc/LICENSE`](doc/LICENSE) — the original UT TexMol notice, including the citation request for published
  work.
- `share/doc/mpfr/` — documentation of the bundled MPFR library, under its own licence (see the files there).
