/* Loop flattening for Graphite.
   Copyright (C) 2010, 2011 Free Software Foundation, Inc.
   Contributed by Sebastian Pop <sebastian.pop@amd.com>.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"

#ifdef HAVE_cloog
#include <isl/set.h>
#include <isl/map.h>
#include <isl/union_map.h>
#include <cloog/cloog.h>
#include <cloog/isl/domain.h>
#endif

#include "system.h"
#include "coretypes.h"
#include "tree-flow.h"
#include "tree-dump.h"
#include "cfgloop.h"
#include "tree-chrec.h"
#include "tree-data-ref.h"
#include "tree-scalar-evolution.h"
#include "sese.h"

#ifdef HAVE_cloog
#include "ppl_c.h"
#include "graphite-ppl.h"
#include "graphite-poly.h"

/* The loop flattening pass transforms loop nests into a single loop,
   removing the loop nesting structure.  The auto-vectorization can
   then apply on the full loop body, without needing the outer-loop
   vectorization.

   The loop flattening pass that has been described in a very Fortran
   specific way in the 1992 paper by Reinhard von Hanxleden and Ken
   Kennedy: "Relaxing SIMD Control Flow Constraints using Loop
   Transformations" available from
   http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.54.5033

   The canonical example is as follows: suppose that we have a loop
   nest with known iteration counts

   | for (i = 1; i <= 6; i++)
   |   for (j = 1; j <= 6; j++)
   |     S1(i,j);

   The loop flattening is performed by linearizing the iteration space
   using the function "f (x) = 6 * i + j".  In this case, CLooG would
   produce this code:

   | for (c1=7;c1<=42;c1++) {
   |   i = floord(c1-1,6);
   |   S1(i,c1-6*i);
   | }

   There are several limitations for loop flattening that are linked
   to the expressivity of the polyhedral model.  One has to take an
   upper bound approximation to deal with the parametric case of loop
   flattening.  For example, in the loop nest:

   | for (i = 1; i <= N; i++)
   |   for (j = 1; j <= M; j++)
   |     S1(i,j);

   One would like to flatten this loop using a linearization function
   like this "f (x) = M * i + j".  However CLooG's schedules are not
   expressive enough to deal with this case, and so the parameter M
   has to be replaced by an integer upper bound approximation.  If we
   further know in the context of the scop that "M <= 6", then it is
   possible to linearize the loop with "f (x) = 6 * i + j".  In this
   case, CLooG would produce this code:

   | for (c1=7;c1<=6*M+N;c1++) {
   |   i = ceild(c1-N,6);
   |   if (i <= floord(c1-1,6)) {
   |     S1(i,c1-6*i);
   |   }
   | }

   For an arbitrarily complex loop nest the algorithm proceeds in two
   steps.  First, the LST is flattened by removing the loops structure
   and by inserting the statements in the order they appear in
   depth-first order.  Then, the scattering of each statement is
   transformed accordingly.

   Supposing that the original program is represented by the following
   LST:

   | (loop_1
   |  stmt_1
   |  (loop_2 stmt_3
   |   (loop_3 stmt_4)
   |   (loop_4 stmt_5 stmt_6)
   |   stmt_7
   |  )
   |  stmt_2
   | )

   Loop flattening traverses the LST in depth-first order, and
   flattens pairs of loops successively by projecting the inner loops
   in the iteration domain of the outer loops:

   lst_project_loop (loop_2, loop_3, stride)

   | (loop_1
   |  stmt_1
   |  (loop_2 stmt_3 stmt_4
   |   (loop_4 stmt_5 stmt_6)
   |   stmt_7
   |  )
   |  stmt_2
   | )

   lst_project_loop (loop_2, loop_4, stride)

   | (loop_1
   |  stmt_1
   |  (loop_2 stmt_3 stmt_4 stmt_5 stmt_6 stmt_7)
   |  stmt_2
   | )

   lst_project_loop (loop_1, loop_2, stride)

   | (loop_1
   |  stmt_1 stmt_3 stmt_4 stmt_5 stmt_6 stmt_7 stmt_2
   | )

   At each step, the iteration domain of the outer loop is enlarged to
   contain enough points to iterate over the inner loop domain.  */


/* Flatten all the loop nests in SCOP.  Returns true when something
   changed.  */

bool
flatten_all_loops (scop_p scop ATTRIBUTE_UNUSED)
{
  /* FIXME: This pass is disabled until it is translated to use
     isl_map_flatten_range.  */
  return false;
}

#endif
