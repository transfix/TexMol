/*
  Copyright 2011 The University of Texas at Austin

	Advisor: Chandrajit Bajaj <bajaj@cs.utexas.edu>

  This file is part of MolSurf.

  MolSurf is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.


  MolSurf is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef HFN_UTIL_H
#define HFN_UTIL_H

#include <PocketTunnel/datastruct_pt.h>
#include <PocketTunnel/util.h>
#include <PocketTunnel/intersect.h>

bool is_maxima(const Cell_handle& c);
bool is_outflow(const Facet& f);
bool is_transversal_flow(const Facet& f);
bool find_acceptor(const Cell_handle& c, const int& id, int& uid, int& vid, int& wid);
bool is_acceptor_for_any_VE(const Triangulation& triang, const Edge& e);
bool is_i2_saddle(const Facet& f);
bool is_i1_saddle(const Edge& e, const Triangulation& triang);
void grow_maxima(Triangulation& triang, Cell_handle c_max);
void find_flow_direction(Triangulation& triang);

#endif
