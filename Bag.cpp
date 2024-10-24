#include "Bag.h"
#include "BagIterator.h"
#include <exception>
#include <iostream>

using namespace std;


Bag::Bag() {
	this->m = 100;
	this->elems = new TElem[this->m];
	this->next = new int[this->m];

	for (int i = 0; i < this->m; i++)
	{
		this->next[i] = -1;
		this->elems[i] = NULL_TELEM;
	}
	
	this->firstEmpty = 0;
	this->nrElems = 0;
} 
// BC = WC = TC = Theta(m)


int Bag::hash(TElem e, int m) const 
{
	if (e < 0)
		return -e % m;
	else
		return e % m;
}
// BC = WC = TC = Theta(1)


void Bag::changeFirstEmpty()
{
	this->firstEmpty++;

	while (this->firstEmpty < this->m && this->elems[this->firstEmpty] != -111111)
	{
		this->firstEmpty++;
	}
}
// BC = Theta(1) - in the case where we just need to go to the position right by it
// WC = Theta(m) - in the case where we need to go from 0 to position m-1
// TC = O(m)


void Bag::resize_rehash() {
	int oldcap = this->m;
	this->m *= 2;

	TElem* newElems = new TElem[this->m];
	int* newNext = new int[this->m];

	std::fill(newNext, newNext + this->m, -1);
	std::fill(newElems, newElems + this->m, -111111);

	this->firstEmpty = 0;

	for (int i = 0; i < oldcap; i++) {
		if (this->elems[i] != -111111) {
			int pos = this->hash(this->elems[i], this->m);

			if (newElems[pos] == -111111) {
				newElems[pos] = this->elems[i];

				if (this->firstEmpty == pos)
					this->firstEmpty++;

			}
			else {
				int current = pos;
				while (newNext[current] != -1) {
					current = newNext[current];
				}

				newElems[this->firstEmpty] = this->elems[i];
				newNext[this->firstEmpty] = -1;
				newNext[current] = this->firstEmpty;

				// Update firstEmpty after inserting the new element
				this->firstEmpty++;
				while (this->firstEmpty < this->m && newElems[this->firstEmpty] != -111111) {
					this->firstEmpty++;
				}
			}
		}
	}

	delete[] this->elems;
	delete[] this->next;
	this->elems = newElems;
	this->next = newNext;

} 
// BC = WC = TC = Theta(m) - where m is the old capacity * 2 


void Bag::add(TElem elem) {
	if (this->firstEmpty == this->m) {
		this->resize_rehash();
	}

	int pos = this->hash(elem, this->m);

	if (this->elems[pos] == -111111) {
		this->elems[pos] = elem;
		this->nrElems++;

		if (pos == this->firstEmpty) {
			changeFirstEmpty();
		}
	}
	else {
		int current = pos;
		while (this->next[current] != -1) {
			current = this->next[current];
		}

		this->nrElems++;
		this->elems[this->firstEmpty] = elem;
		this->next[this->firstEmpty] = -1;
		this->next[current] = this->firstEmpty;
		changeFirstEmpty();
	}
}
// BC = Theta(1) - when the element can be placed on the position given by the hash function without any links
//				   needed, no resize is needed and the changeFirstEmpty() operation is also Theta(1)
// WC = Theta(m) - when the added element is going to be added at the end of a link of length m. For example,
//				   if m elements with the same hash value are added, the last one will give a complexity of
//                 Theta(m), or when a resize is needed.
// TC = O(m)


bool Bag::remove(TElem elem) {
	int pos;
	int prev = -1;
	pos = this->hash(elem, this->m);

	while (pos != -1 and this->elems[pos] != elem) {
		prev = pos;
		pos = this->next[pos];
	}
		
	if (pos == -1)
		return false; // we didn't find it
	else {
		bool over = false;
		do {
			int p = this->next[pos]; //going through the next of the links
			int pp = pos;
			while (p != -1 and this->hash(this->elems[p], this->m) != pos) { //finding another element to hash to p
				pp = p;
				p = this->next[p];
			}
			if (p == -1) {
				over = true;
			}
			else {
				this->elems[pos] = this->elems[p];
				prev = pp;
				pos = p;
			}

		} while (!over);

		if (prev == -1) { //if the searched element is the first, we need to relink the other elements if they need it
			int idx = 0;

			while (idx < this->m and prev == -1) {
				if (this->next[idx] == pos) {
					prev = idx;
				}
				else
					idx++;
			}
		}

		if (prev != -1) { //if not we just set the links as usual
			this->next[prev] = this->next[pos];
		}
		this->elems[pos] = -111111; //removing it:
		this->next[pos] = -1;

		if (this->firstEmpty > pos) { //if the list was full, resetting firstEmpty
			this->firstEmpty = pos;
		}

		this->nrElems--;
		return true;
	}
}
// BC = Theta(1) - when the element is not in the hashtable, and from its hashed position there is no link started
//				   or when the element to be removed is not part of a link
// WC = Theta(m) - when we need to search for it until the end of a link of length m, or when removing the element
//                 in a link of length m (no matter the position of it in the link) forces the moving of other elements
// TC = O(m) 


bool Bag::search(TElem elem) const {
	int pos = this->hash(elem, this->m);
	
	while (pos != -1) {
		if (this->elems[pos] == elem) {
			return true;
		}

		pos = this->next[pos];
	}

	return false;
}
// BC = Theta(1) - when the element searched is on the position given by the hash function
// WC = Theta(m) - when the element searched for is at the end of a link of length m
// TC = O(m)


int Bag::nrOccurrences(TElem elem) const {
	int pos = this->hash(elem, this->m);

	int count = 0;

	while (pos != -1) {
		if (this->elems[pos] == elem) {
			count++;
		}

		pos = this->next[pos];
	}

	return count;
}
// BC = Theta(1) - when the element searched is on the position given on the hash function, and it doesn't link to others
//				   or when the element is not in the hashtable
// WC = Theta(m) - when the element searched for is either at the end of a link of length m, or it has more occurances
//				   throughout the link
// TC = O(m)


int Bag::size() const {
	return this->nrElems;
}
// BC = WC = TC = Theta(1)


bool Bag::isEmpty() const {
	return this->nrElems == 0;
}
// BC = WC = TC = Theta(1)


BagIterator Bag::iterator() const {
	return BagIterator(*this);
}
// BC = WC = TC = Theta(1)


int Bag::elementsWithThisFrequency(int frequency) const
{
	if (frequency <= 0)  
		throw exception();

	int count = 0; //nr of elements with this frequency
	TElem current; //the current TElem
	int current_frequency; //the frequency of the current TElem

	int* unique_elems = new int[this->m]; //array where we will store the unique elements
	int nr_unique = 0; //index for the array
	bool ok;
	int i;
	BagIterator it(*this);

	while (it.valid()) { //going through each element
		current = it.getCurrent();
		ok = true;
	
		for (i = 0; i < nr_unique; i++) { //checking whether the value was already accounted for, so we don't count twice
			if (unique_elems[i] == current)
				ok = false; //set to false to signal that it was already accounted for
		}

		if (ok == true) { //if not, then we found a new unique element
			current_frequency = this->nrOccurrences(current);

			if (current_frequency == frequency)
				count++;

			//adding it to the unique list
			unique_elems[nr_unique] = current;
			nr_unique++;
		}

		it.next();
	}

	delete[] unique_elems;
	return count;
}
// BC = Theta(m) - if there is only one element in the hash table
//                 The nrOccurences will take Theta(1)
//				   The iterator will still take Theta(m)
//				   we still have to go through each element, but looking in the unique_elems will take Theta(1)
// WC = Theta(m(m+1)/2) = Theta(m^2) - if the hash table is full and every element is unique, we need to search for each 
//						  element (m elements) in the unique_elems each time, so 1 + 2 + 3 + ... + (m-1). The
//						  nrOccurences function is Theta(1) in this case.
// TC = O(m^2)  



Bag::~Bag() {
	delete[] this->elems;
	delete[] this->next;
}
// BC = WC = TC = Theta(m)

