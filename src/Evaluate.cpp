#include "Evaluate.h"

Evaluate::Evaluate() {}

Evaluate::~Evaluate() {}

// ------------ Validate -----------------

bool Evaluate::validate( Point2DImpl  &spatialObj_F, Point2DImpl   &spatialObj_G, std::vector<bool> &featureVectorF, std::vector<bool> &featureVectorG, PREDICATE predicate)
{
	if( Evaluate::determine(spatialObj_F, spatialObj_G, featureVectorF, featureVectorG) == predicate ) { return true; } else { return false; }
}

bool Evaluate::validate( Point2DImpl  &spatialObj_F, Line2DImpl    &spatialObj_G, std::vector<bool> &featureVectorF, std::vector<bool> &featureVectorG, PREDICATE predicate)
{
	if( Evaluate::determine(spatialObj_F, spatialObj_G, featureVectorF, featureVectorG) == predicate ) { return true; } else { return false; }
}

bool Evaluate::validate( Point2DImpl  &spatialObj_F, Region2DImpl    &spatialObj_G, std::vector<bool> &featureVectorF, std::vector<bool> &featureVectorG, PREDICATE predicate)
{
	if( Evaluate::determine(spatialObj_F, spatialObj_G, featureVectorF, featureVectorG) == predicate ) { return true; } else { return false; }
}

bool Evaluate::validate( Line2DImpl   &spatialObj_F, Line2DImpl    &spatialObj_G, std::vector<bool> &featureVectorF, std::vector<bool> &featureVectorG, PREDICATE predicate)
{
	if( Evaluate::determine(spatialObj_F, spatialObj_G, featureVectorF, featureVectorG) == predicate ) { return true; } else { return false; }
}

bool Evaluate::validate( Line2DImpl   &spatialObj_F, Region2DImpl  &spatialObj_G, std::vector<bool> &featureVectorF, std::vector<bool> &featureVectorG, PREDICATE predicate)
{
	if( Evaluate::determine(spatialObj_F, spatialObj_G, featureVectorF, featureVectorG) == predicate ) { return true; } else { return false; }
}

bool Evaluate::validate( Region2DImpl &spatialObj_F, Region2DImpl  &spatialObj_G, std::vector<bool> &featureVectorF, std::vector<bool> &featureVectorG, PREDICATE predicate)
{
	if( Evaluate::determine(spatialObj_F, spatialObj_G, featureVectorF, featureVectorG) == predicate ) { return true; } else { return false; }
}

// ------------ Determine -----------------

PREDICATE Evaluate::determine( Point2DImpl  &spatialObj_F, Point2DImpl   &spatialObj_G, std::vector<bool> &featureVectorF, std::vector<bool> &featureVectorG )
{
	enum VectorFlag {poi_shared, poi_disjoint};

	if ( featureVectorF[poi_disjoint] ) { // A0B-
		if ( featureVectorF[poi_shared] ) { // A0B0, moved right
			if ( featureVectorG[poi_disjoint] ) { // A-B0
				return overlap; // 5
			} else {
				return contains; // 4
			}
		} else {
			return disjoint; // 1
		}
	} else { // A-B0, moved left
		if ( featureVectorG[poi_disjoint] ) { // vg[poi_disjoint]
			return inside; // 3
		} else {
			return equal; // 2
		}

	}
}

PREDICATE Evaluate::determine( Point2DImpl  &spatialObj_F, Line2DImpl &spatialObj_G, std::vector<bool> &featureVectorF, std::vector<bool> &featureVectorG )
{
	enum VectorFlag {poi_disjoint, poi_on_interior, poi_on_bound, bound_poi_disjoint};

	if( featureVectorF[poi_on_interior] ) { // A0B0
		if( featureVectorF[poi_on_bound] ) { // A0&B (boundary B) 
			if( featureVectorF[poi_disjoint] ) { // A0B-
				if ( featureVectorG[bound_poi_disjoint] ) { // A-&B
					return overlap; // 14
				} else {
					return overlap; //13
				}
			} else {
				if ( featureVectorG[bound_poi_disjoint] ) { // A-&B
					return inside; // 12
				} else {
					return inside; // 11
				}
			}
		} else {
			if (  featureVectorF[poi_disjoint]) { // A0B-
				if ( featureVectorG[bound_poi_disjoint] ) { // A-&B
					return overlap; // 9
				} else {
					return overlap; // 10
				}
			} else {
				if ( featureVectorG[bound_poi_disjoint] ) { // A-&B
					return inside; // 7
				} else {
					return inside; // 8
				}
			}
		}
	} else {
		if( featureVectorF[poi_on_bound] ) { // A0&B
			if ( featureVectorF[poi_disjoint] ) { // A0B-
				if ( featureVectorG[bound_poi_disjoint] ) { // A-&B
					return meet; // 6
				} else {
					return meet; // 5
				}
			} else {
				if ( featureVectorG[bound_poi_disjoint] ) { // A-&B
					return meet; // 4
				} else {
					return meet; // 3
				}
			}
		} else { // A-&B
			if ( featureVectorG[bound_poi_disjoint] ) {
				return disjoint; // 2
			} else {
				return disjoint; // 1
			}
		}
	}
}

PREDICATE Evaluate::determine( Point2DImpl  &spatialObj_F, Region2DImpl  &spatialObj_G, std::vector<bool> &featureVectorF, std::vector<bool> &featureVectorG)
{
	enum VectorFlag {poi_inside, poi_on_bound, poi_outside};
	
	if ( featureVectorF[poi_inside] ) { // A0B0
		if ( featureVectorF[poi_on_bound] ) { // A0&B
			if ( featureVectorF[poi_outside] ) { // A0B-
				return overlap; // 7
			} else {
				return inside; // 6
			}
		} else {
			if ( featureVectorF[poi_outside] ) { // A0B-
				return inside; // 4
			} else {
				return overlap; // 5
			}
		}
	} else {
		if ( featureVectorF[poi_on_bound] ) { // A0&B
			if ( featureVectorF[poi_outside] ) { //A0B-
				return meet; // 3
			} else {
				return meet; // 2
			}
		} else {
			return disjoint; // 1
		}
	}
}

PREDICATE Evaluate::determine( Line2DImpl   &spatialObj_F, Line2DImpl &spatialObj_G, std::vector<bool> &featureVectorF, std::vector<bool> &featureVectorG )
{
	enum VectorFlag {seg_unshared, bound_on_interior, bound_disjoint, seg_shared, interior_poi_shared, bound_shared};

	if (featureVectorF[bound_shared])
	{
		if (featureVectorG[bound_on_interior])
		{
			if (featureVectorF[bound_on_interior])
			{
				if (featureVectorF[bound_disjoint])
				{
					if (featureVectorF[seg_unshared])
					{
						if (featureVectorG[seg_unshared])
						{
							return equal;
						}
						else
						{
							if (featureVectorG[bound_disjoint])
							{
								return inside;
							}
							else
							{
								return inside;
							}
						}
					}
					else
					{
						if (featureVectorF[seg_shared] || featureVectorF[interior_poi_shared])
						{
							if (featureVectorG[bound_disjoint])
							{
								return disjoint;
							}
							else
							{
								return disjoint;
							}
						}
						else
						{
							if (featureVectorG[seg_unshared])
							{
								return contains;
							}
							else
							{
								if (featureVectorG[bound_disjoint])
								{
									return overlap;
								}
								else
								{
									return overlap;
								}
							}
						}
					}
				}
				else
				{
					if (featureVectorF[seg_shared] || featureVectorF[interior_poi_shared])
					{
						if (featureVectorG[bound_disjoint])
						{
							return disjoint;
						}
						else
						{
							return disjoint;
						}
					}
					else
					{
						if (featureVectorG[seg_unshared])
						{
							return contains;
						}
						else
						{
							if (featureVectorG[bound_disjoint])
							{
								return overlap;
							}
							else
							{
								return overlap;
							}
						}
					}
				}
			}
			else
			{
				if (featureVectorG[bound_disjoint])
				{
					if (featureVectorF[seg_shared] || featureVectorF[interior_poi_shared])
					{
						if (featureVectorF[bound_disjoint])
						{
							return meet;
						}
						else
						{
							return meet;
						}
					}
					else
					{
						if (featureVectorF[seg_unshared])
						{
							return inside;
						}
						else
						{
							if (featureVectorF[bound_disjoint])
							{
								return overlap;
							}
							else
							{
								return overlap;
							}
						}
					}
				}
				else
				{
					if (featureVectorF[seg_shared] || featureVectorF[interior_poi_shared])
					{
						if (featureVectorF[bound_disjoint])
						{
							return meet;
						}
						else
						{
							return meet;
						}
					}
					else
					{
						if (featureVectorF[seg_unshared])
						{
							return inside;
						}
						else
						{
							if (featureVectorF[bound_disjoint])
							{
								return overlap;
							}
							else
							{
								return overlap;
							}
						}
					}
				}
			}
		}
		else
		{
			if (featureVectorF[bound_disjoint])
			{
				if (featureVectorF[seg_shared] || featureVectorF[interior_poi_shared])
				{
					if (featureVectorF[bound_on_interior])
					{
						if (featureVectorG[bound_disjoint])
						{
							return meet;
						}
						else
						{
							return meet;
						}
					}
					else
					{
						if (featureVectorG[bound_disjoint])
						{
							return meet;
						}
						else
						{
							return meet;
						}
					}
				}
				else
				{
					if (featureVectorF[bound_on_interior])
					{
						if (featureVectorG[seg_unshared])
						{
							return contains;
						}
						else
						{
							if (featureVectorG[bound_disjoint])
							{
								return overlap;
							}
							else
							{
								return overlap;
							}
						}
					}
					else
					{
						if (featureVectorG[bound_disjoint])
						{
							return overlap;
						}
						else
						{
							return overlap;
						}
					}
				}
			}
			else
			{
				if (featureVectorF[seg_shared] || featureVectorF[interior_poi_shared])
				{
					if (featureVectorF[bound_on_interior])
					{
						if (featureVectorG[bound_disjoint])
						{
							return meet;
						}
						else
						{
							return meet;
						}
					}
					else
					{
						if (featureVectorG[bound_disjoint])
						{
							return meet;
						}
						else
						{
							return meet;
						}
					}
				}
				else
				{
					if (featureVectorF[bound_on_interior])
					{
						if (featureVectorG[seg_unshared])
						{
							return contains;
						}
						else
						{
							if (featureVectorG[bound_disjoint])
							{
								return overlap;
							}
							else
							{
								return overlap;
							}
						}
					}
					else
					{
						if (featureVectorG[bound_disjoint])
						{
							return overlap;
						}
						else
						{
							return overlap;
						}
					}
				}
			}
		}
	}
	else
	{
		if (featureVectorG[bound_on_interior])
		{
			if (featureVectorF[bound_on_interior])
			{
				if (featureVectorF[bound_disjoint])
				{
					if (featureVectorF[seg_unshared])
					{
						if (featureVectorG[seg_unshared])
						{
							return equal;
						}
						else
						{
							if (featureVectorG[bound_disjoint])
							{
								return coveredBy;
							}
							else
							{
								return coveredBy;
							}
						}
					}
					else
					{
						if (featureVectorF[seg_shared] || featureVectorF[interior_poi_shared])
						{
							if (featureVectorG[bound_disjoint])
							{
								return meet;
							}
							else
							{
								return meet;
							}
						}
						else
						{
							if (featureVectorG[seg_unshared])
							{
								return covers;
							}
							else
							{
								if (featureVectorG[bound_disjoint])
								{
									return overlap;
								}
								else
								{
									return overlap;
								}
							}
						}
					}
				}
				else
				{
					if (featureVectorF[seg_shared] || featureVectorF[interior_poi_shared])
					{
						if (featureVectorG[bound_disjoint])
						{
							return meet;
						}
						else
						{
							return meet;
						}
					}
					else
					{
						if (featureVectorG[seg_unshared])
						{
							return covers;
						}
						else
						{
							if (featureVectorG[bound_disjoint])
							{
								return overlap;
							}
							else
							{
								return overlap;
							}
						}
					}
				}
			}
			else
			{
				if (featureVectorG[bound_disjoint])
				{
					if (featureVectorF[seg_shared] || featureVectorF[interior_poi_shared])
					{
						if (featureVectorF[bound_disjoint])
						{
							return meet;
						}
						else
						{
							return meet;
						}
					}
					else
					{
						if (featureVectorF[seg_unshared])
						{
							return coveredBy;
						}
						else
						{
							if (featureVectorF[bound_disjoint])
							{
								return overlap;
							}
							else
							{
								return overlap;
							}
						}
					}
				}
				else
				{
					if (featureVectorF[seg_shared] || featureVectorF[interior_poi_shared])
					{
						if (featureVectorF[bound_disjoint])
						{
							return meet;
						}
						else
						{
							return meet;
						}
					}
					else
					{
						if (featureVectorF[seg_unshared])
						{
							return coveredBy;
						}
						else
						{
							if (featureVectorF[bound_disjoint])
							{
								return overlap;
							}
							else
							{
								return overlap;
							}
						}
					}
				}
			}
		}
		else
		{
			if (featureVectorF[bound_disjoint])
			{
				if (featureVectorF[seg_shared] || featureVectorF[interior_poi_shared])
				{
					if (featureVectorF[bound_on_interior])
					{
						if (featureVectorG[bound_disjoint])
						{
							return meet;
						}
						else
						{
							return meet;
						}
					}
					else
					{
						if (featureVectorG[bound_disjoint])
						{
							return meet;
						}
						else
						{
							return meet;
						}
					}
				}
				else
				{
					if (featureVectorF[bound_on_interior])
					{
						if (featureVectorG[seg_unshared])
						{
							return covers;
						}
						else
						{
							if (featureVectorG[bound_disjoint])
							{
								return overlap;
							}
							else
							{
								return overlap;
							}
						}
					}
					else
					{
						if (featureVectorG[bound_disjoint])
						{
							return overlap;
						}
						else
						{
							return overlap;
						}
					}
				}
			}
			else
			{
				if (featureVectorF[seg_shared] || featureVectorF[interior_poi_shared])
				{
					if (featureVectorF[bound_on_interior])
					{
						if (featureVectorG[bound_disjoint])
						{
							return meet;
						}
						else
						{
							return meet;
						}
					}
					else
					{
						if (featureVectorG[bound_disjoint])
						{
							return meet;
						}
						else
						{
							return meet;
						}
					}
				}
				else
				{
					if (featureVectorF[bound_on_interior])
					{
						if (featureVectorG[seg_unshared])
						{
							return covers;
						}
						else
						{
							if (featureVectorG[bound_disjoint])
							{
								return overlap;
							}
							else
							{
								return overlap;
							}
						}
					}
					else
					{
						if (featureVectorG[bound_disjoint])
						{
							return overlap;
						}
						else
						{
							return overlap;
						}
					}
				}
			}
		}
	}
}

PREDICATE Evaluate::determine( Line2DImpl   &spatialObj_F, Region2DImpl  &spatialObj_G, std::vector<bool> &featureVectorF, std::vector<bool> &featureVectorG )
{
	enum VectorFlag {seg_unshared, seg_inside, seg_shared, seg_outside, poi_shared, bound_inside, bound_shared, bound_disjoint};

	if (featureVectorF[bound_shared])
	{
		if (featureVectorF[bound_inside])
		{
			if (featureVectorF[seg_inside])
			{
				if (featureVectorF[bound_disjoint])
				{
					if (featureVectorF[seg_outside])
					{
						if (featureVectorG[seg_unshared])
						{
							return meet;
						}
						else
						{
							return meet;
						}
					}
					else
					{
						if (featureVectorF[seg_shared] || featureVectorF[poi_shared])
						{
							return disjoint;
						}
						else
						{
							if (featureVectorG[seg_unshared])
							{
								return meet;
							}
							else
							{
								return meet;
							}
						}
					}
				}
				else
				{
					if (featureVectorF[seg_shared] || featureVectorF[poi_shared])
					{
						return disjoint;
					}
					else
					{
						if (featureVectorG[seg_unshared])
						{
							return meet;
						}
						else
						{
							return meet;
						}
					}
				}
			}
			else
			{
				if (featureVectorF[seg_shared] || featureVectorF[poi_shared])
				{
					if (featureVectorF[seg_outside])
					{
						return inside;
					}
					else
					{
						if (featureVectorF[bound_disjoint])
						{
							return overlap;
						}
						else
						{
							return overlap;
						}
					}
				}
				else
				{
					if (featureVectorG[seg_unshared])
					{
						if (featureVectorF[seg_outside])
						{
							return inside;
						}
						else
						{
							if (featureVectorF[bound_disjoint])
							{
								return overlap;
							}
							else
							{
								return overlap;
							}
						}
					}
					else
					{
						if (featureVectorF[seg_outside])
						{
							return inside;
						}
						else
						{
							if (featureVectorF[bound_disjoint])
							{
								return overlap;
							}
							else
							{
								return overlap;
							}
						}
					}
				}
			}
		}
		else
		{
			if (featureVectorF[seg_shared] || featureVectorF[poi_shared])
			{
				if (featureVectorF[seg_outside])
				{
					return inside;
				}
				else
				{
					if (featureVectorF[bound_disjoint])
					{
						return overlap;
					}
					else
					{
						return overlap;
					}
				}
			}
			else
			{
				if (featureVectorG[seg_unshared])
				{
					if (featureVectorF[seg_outside])
					{
						return inside;
					}
					else
					{
						if (featureVectorF[bound_disjoint])
						{
							return overlap;
						}
						else
						{
							return overlap;
						}
					}
				}
				else
				{
					if (featureVectorF[seg_outside])
					{
						return inside;
					}
					else
					{
						if (featureVectorF[bound_disjoint])
						{
							return overlap;
						}
						else
						{
							return overlap;
						}
					}
				}
			}
		}
	}
	else
	{
		if (featureVectorF[seg_shared] || featureVectorF[poi_shared])
		{
			if (featureVectorF[bound_inside])
			{
				if (featureVectorF[seg_inside])
				{
					if (featureVectorF[bound_disjoint])
					{
						return meet;
					}
					else
					{
						return meet;
					}
				}
				else
				{
					if (featureVectorF[seg_outside])
					{
						return coveredBy;
					}
					else
					{
						if (featureVectorF[bound_disjoint])
						{
							return overlap;
						}
						else
						{
							return overlap;
						}
					}
				}
			}
			else
			{
				if (featureVectorF[seg_outside])
				{
					return coveredBy;
				}
				else
				{
					if (featureVectorF[bound_disjoint])
					{
						return overlap;
					}
					else
					{
						return overlap;
					}
				}
			}
		}
		else
		{
			if (featureVectorF[seg_inside])
			{
				if (featureVectorF[seg_outside])
				{
					return meet;
				}
				else
				{
					if (featureVectorF[bound_disjoint])
					{
						return meet;
					}
					else
					{
						return meet;
					}
				}
			}
			else
			{
				if (featureVectorF[bound_inside])
				{
					if (featureVectorF[seg_outside])
					{
						return coveredBy;
					}
					else
					{
						if (featureVectorF[bound_disjoint])
						{
							return overlap;
						}
						else
						{
							return overlap;
						}
					}
				}
				else
				{
					if (featureVectorF[seg_outside])
					{
						return coveredBy;
					}
					else
					{
						if (featureVectorF[bound_disjoint])
						{
							return overlap;
						}
						else
						{
							return overlap;
						}
					}
				}
			}
		}
	}
}

PREDICATE Evaluate::determine( Region2DImpl &spatialObj_F, Region2DImpl  &spatialObj_G, std::vector<bool> &featureVectorF, std::vector<bool> &featureVectorG )
{
	enum VectorFlag {zero_one, one_zero, one_two, two_one, zero_two, two_zero, one_one, bound_poi_shared};

	if (featureVectorG[one_two] || featureVectorG[two_one])
	{
		if (featureVectorF[zero_one] || featureVectorF[one_zero])
		{
			if (featureVectorF[zero_one] || featureVectorF[one_zero] || featureVectorF[one_one] || featureVectorG[one_two] || featureVectorG[two_one])
			{
				if (featureVectorF[one_two] || featureVectorF[two_one])
				{
					if (featureVectorF[one_two] || featureVectorF[two_one] || featureVectorF[one_one] || featureVectorG[zero_one] || featureVectorG[one_zero])
					{
						return equal;
					}
					else
					{
						return coveredBy;
					}
				}
				else
				{
					if (featureVectorF[zero_two] || featureVectorF[two_zero] || featureVectorF[one_one] || featureVectorF[bound_poi_shared])
					{
						return inside;
					}
					else
					{
						if (featureVectorG[zero_one] || featureVectorG[one_zero])
						{
							return coveredBy;
						}
						else
						{
							return coveredBy;
						}
					}
				}
			}
			else
			{
				if (featureVectorF[one_two] || featureVectorF[two_one])
				{
					if (featureVectorF[zero_two] || featureVectorF[two_zero] || featureVectorF[one_two] || featureVectorF[two_one] || featureVectorG[one_two] || featureVectorG[two_one])
					{
						return meet;
					}
					else
					{
						return overlap;
					}
				}
				else
				{
					if (featureVectorG[zero_one] || featureVectorG[one_zero])
					{
						return overlap;
					}
					else
					{
						return overlap;
					}
				}
			}
		}
		else
		{
			if (featureVectorG[zero_one] || featureVectorG[one_zero])
			{
				if (featureVectorF[zero_two] || featureVectorF[two_zero] || featureVectorF[one_two] || featureVectorF[two_one] || featureVectorG[one_two] || featureVectorG[two_one])
				{
					return meet;
				}
				else
				{
					if (featureVectorF[one_two] || featureVectorF[two_one])
					{
						if (featureVectorF[one_two] || featureVectorF[two_one] || featureVectorF[one_one] || featureVectorG[zero_one] || featureVectorG[one_zero])
						{
							return covers;
						}
						else
						{
							return overlap;
						}
					}
					else
					{
						return overlap;
					}
				}
			}
			else
			{
				if (featureVectorF[zero_two] || featureVectorF[two_zero] || featureVectorF[one_two] || featureVectorF[two_one] || featureVectorG[one_two] || featureVectorG[two_one])
				{
					if (featureVectorF[zero_two] || featureVectorF[two_zero] || featureVectorF[one_one] || featureVectorF[bound_poi_shared])
					{
						return disjoint;
					}
					else
					{
						return meet;
					}
				}
				else
				{
					if (featureVectorF[one_two] || featureVectorF[two_one])
					{
						return overlap;
					}
					else
					{
						if (featureVectorF[zero_two] || featureVectorF[two_zero] || featureVectorF[one_one] || featureVectorF[bound_poi_shared])
						{
							return overlap;
						}
						else
						{
							return overlap;
						}
					}
				}
			}
		}
	}
	else
	{
		if (featureVectorF[one_two] || featureVectorF[two_one])
		{
			if (featureVectorF[zero_one] || featureVectorF[one_zero])
			{
				if (featureVectorF[one_two] || featureVectorF[two_one] || featureVectorF[one_one] || featureVectorG[zero_one] || featureVectorG[one_zero])
				{
					return covers;
				}
				else
				{
					if (featureVectorG[zero_one] || featureVectorG[one_zero])
					{
						return overlap;
					}
					else
					{
						return overlap;
					}
				}
			}
			else
			{
				if (featureVectorF[zero_two] || featureVectorF[two_zero] || featureVectorF[one_one] || featureVectorF[bound_poi_shared])
				{
					if (featureVectorF[one_two] || featureVectorF[two_one] || featureVectorF[one_one] || featureVectorG[zero_one] || featureVectorG[one_zero])
					{
						return contains;
					}
					else
					{
						return overlap;
					}
				}
				else
				{
					if (featureVectorF[one_two] || featureVectorF[two_one] || featureVectorF[one_one] || featureVectorG[zero_one] || featureVectorG[one_zero])
					{
						return covers;
					}
					else
					{
						if (featureVectorG[zero_one] || featureVectorG[one_zero])
						{
							return overlap;
						}
						else
						{
							return overlap;
						}
					}
				}
			}
		}
		else
		{
			if (featureVectorF[zero_two] || featureVectorF[two_zero] || featureVectorF[one_one] || featureVectorF[bound_poi_shared])
			{
				if (featureVectorF[zero_one] || featureVectorF[one_zero])
				{
					return overlap;
				}
				else
				{
					if (featureVectorG[zero_one] || featureVectorG[one_zero])
					{
						return overlap;
					}
					else
					{
						return overlap;
					}
				}
			}
			else
			{
				if (featureVectorF[zero_one] || featureVectorF[one_zero])
				{
					if (featureVectorG[zero_one] || featureVectorG[one_zero])
					{
						return overlap;
					}
					else
					{
						return overlap;
					}
				}
				else
				{
					if (featureVectorG[zero_one] || featureVectorG[one_zero])
					{
						return overlap;
					}
					else
					{
						return overlap;
					}
				}
			}
		}
	}
}