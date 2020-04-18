#include <string>
#include "stdlib.h"

#include "../includes/waiter.h"
#include "../includes/externs.h"

using namespace std;
//producer
//order grabs lock, puts order into vector, unlocks
//gcount++ >> add to vector
//exclusive access to adding order. not as difficult
//cv,notify vs cv.notifyall

//cv.notify normally,
//when everything is done, cv.notifyall to make everything shut down

//when woken up
//if in vector do job

//if woken up and nothing in queue, saying done.

Waiter::Waiter(int id,std::string filename):id(id),myIO(filename) {
}

Waiter::~Waiter() {
}




//gets next Order(s) from file_IO
int Waiter::getNext(ORDER &anOrder){
	return myIO.getNext(anOrder);
}

void Waiter::beWaiter() {

	while (true) {
		ORDER ord;

		int areThereMoreOrders = this -> getNext(ord);
		if (areThereMoreOrders != SUCCESS) {
			b_WaiterIsFinished = true;
			break;
		}

		{
			lock_guard<mutex> lck(mutex_order_inQ);
			order_in_Q.push(ord);
		}

		cv_order_inQ.notify_one();
	}
	cv_order_inQ.notify_all();
}

