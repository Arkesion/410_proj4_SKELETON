#include <mutex>

#include "../includes/baker.h"
#include "../includes/externs.h"

using namespace std;

//consumer
//mutex the queue, waiter cannot access same time as baker, vise versa
//when adding and removing from queue, use unique lock
//more meaty and difficult order

//if queue empty, stop >> Need to use condition variable (cv.wait()) >> cv.wait(lck) releases mutex lock
//else, get from queue

//while queue is empty AND waiter not done

Baker::Baker(int id):id(id)
{
}

Baker::~Baker()
{
}

//bake, box and append to anOrder.boxes vector
//if order has 13 donuts there should be 2 boxes
//1 with 12 donuts, 1 with 1 donut
void Baker::bake_and_box(ORDER &anOrder) {

	int needed_boxes = anOrder.number_donuts / 12;
	int extra_donuts = anOrder.number_donuts % 12;

	if (extra_donuts != 0) {
		needed_boxes++;
	}

	int leftover_donuts = anOrder.number_donuts;

	for(int i = 0; i < needed_boxes; i++) {
		if (leftover_donuts > 11) {
			Box aBox;

			for (int j = 0; j < 12; j++) {
				leftover_donuts--;
				DONUT aDonut;
				aBox.addDonut(aDonut);
			}
			anOrder.boxes.push_back(aBox);
		}
	}
}

void Baker::beBaker() {
	
	while (!b_WaiterIsFinished) {

		{
			unique_lock<mutex> lck(mutex_order_inQ);
			cv_order_inQ.wait(lck);
		}

		ORDER ord;

		{
			lock_guard<mutex> lck(mutex_order_inQ);

			ord = order_in_Q.front();
			order_in_Q.pop();
		}

		this -> bake_and_box(ord);

		{
			lock_guard<mutex> lck(mutex_order_outQ);
			order_out_Vector.push_back(ord);
		}

		while (!order_in_Q.empty()) {

			ORDER ord;

			{
				lock_guard<mutex> lck(mutex_order_inQ);

				ord = order_in_Q.front();
				order_in_Q.pop();
			}

			this -> bake_and_box(ord);

			{
				lock_guard<mutex> lck(mutex_order_outQ);
				order_out_Vector.push_back(ord);
			}
		}

	}

}
