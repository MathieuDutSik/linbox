/* linbox/randiter/abstract.h
 * Copyright (C) 1999-2001 William J Turner,
 *               2002 Bradford Hovinen
 *
 * Written by William J Turner <wjturner@math.ncsu.edu>,
 *            Bradford Hovinen <hovinen@cis.udel.edu>
 *
 * ========LICENCE========
 * This file is part of the library LinBox.
 *
  * LinBox is free software: you can redistribute it and/or modify
 * it under the terms of the  GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * ========LICENCE========
 */

#ifndef __LINBOX_randiter_abstract_H
#define __LINBOX_randiter_abstract_H

#include <iostream>
#include "linbox/integer.h"

namespace LinBox
{
	// forward declarations
	class FieldAbstract;
	class ElementAbstract;

	/** Random field element generator.
	 * This encapsulated class is a generator of random field elements for
	 * the encapsulating field.
	 * It is required to contain constructors from a field object and
	 * two integers.  The first integer being a cardinality of a set to
	 * draw the random elements from, and the second being a seed for the
	 * random number generator.
	 * It is also required to contain a copy constructor, a destructor, and
	 * random() which acts on a reference to a field element.
	 * The random value is written to the argument
	 * and also returned as a reference.
	 * @see \subpage randomFEGW Randiter Field Element Generator wrapper
	 */
	class RandIterAbstract {
	public:

		typedef ElementAbstract Element;

		/** Virtual constructor from field, sampling size, and seed.
		 * Required because constructors cannot be virtual.
		 * Passes construction on to derived classes.
		 * The random field element iterator works in the field F, is seeded
		 * by seed, and it returns any one element with probability no more
		 * than 1/min (size, F.cardinality (c)).
		 * A sampling size of zero means to sample from the entire field.
		 * A seed of zero means to use some arbitrary seed for the generator.
		 * Purely virtual.
		 * @param F LinBox field archetype object in which to do arithmetic
		 * @param size constant integer reference of sample size from which to
		 *             sample (default = 0)
		 * @param seed constant integer reference from which to seed random number
		 *             generator (default = 0)
		 */
		virtual RandIterAbstract *construct (const FieldAbstract &F,
						     const integer &size = 0,
						     const uint64_t seed = 0) const = 0;

		/** Virtual copy constructor.
		 * Required because constructors cannot be virtual.
		 * Passes construction on to derived classes.
		 * Purely virtual.
		 * @return pointer to new RandIterAbstract object in dynamic memory.
		 */
		virtual RandIterAbstract *clone (void) const = 0;

		/** Assignment operator.
		 * Purely virtual.
		 * @param  x constant reference to RandIterAbstract object
		 * @return reference to self
		 */
		virtual RandIterAbstract &operator= (const RandIterAbstract &x) = 0;

		/** Destructor.
		*/
		virtual ~RandIterAbstract (void) {}

		/** Random field element creator.
		 * Purely virtual.
		 * @return reference to ElementAbstract object
		 */
		virtual Element &random (Element &a) = 0;

	protected:

		/** Default constructor
		 * Required by derived classes, but protected because this class should
		 * never be constructed by itself.
		 */
		RandIterAbstract (void) {}

	}; // class RandIterAbstract

} // namespace LinBox

#endif // __LINBOX_randiter_abstract_H

// Local Variables:
// mode: C++
// tab-width: 4
// indent-tabs-mode: nil
// c-basic-offset: 4
// End:
// vim:sts=4:sw=4:ts=4:et:sr:cino=>s,f0,{0,g0,(0,\:0,t0,+0,=s
