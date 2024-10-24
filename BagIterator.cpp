#include <exception>
#include "BagIterator.h"
#include "Bag.h"

using namespace std;


BagIterator::BagIterator(const Bag& c): bag(c)
{
	this->current = 0;

	while (this->current < this->bag.m && this->bag.elems[this->current] == -111111)
		this->current++;

	if (this->current == this->bag.m)
		this->current = -1;
}
// BC = Theta(1) - if the current can be initialized from 0
// WC = Theta(m) - if there are no elements in the bag or the first element is the very last one
// TC = O(m)


void BagIterator::first() {
	this->current = 0;

	while (this->current < this->bag.m && this->bag.elems[this->current] == -111111)
		this->current++;
	
	if (this->current == this->bag.m)
		this->current = -1;
}
// BC = Theta(1) - if the first can be initialized from 0
// WC = Theta(m) - if there are no elements in the bag or the first element is the very last one
// TC = O(m)


void BagIterator::next() {
	if (!this->valid())
		throw exception();

	this->current++;

	while (this->current < this->bag.m && this->bag.elems[this->current] == -111111)
		this->current++;

	if (this->current == this->bag.m)
		this->current = -1;
}
// BC = Theta(1) - if the next element is on the following position or the iterator is invalid
// WC = Theta(m) - if from the position 0 we need to go up to the last index (m-1) to search for the next element
// TC = O(m)


bool BagIterator::valid() const {
	if (this->current == -1 or this->bag.elems[this->current] == -111111)
		return false;

	return true;
}
// BC = WC = TC = Theta(1)


TElem BagIterator::getCurrent() const
{
	if (!this->valid())
		throw exception();

	return this->bag.elems[this->current];
}
// BC = WC = TC = Theta(1)
