
#include <systemc.h>

using namespace std;

SC_MODULE(cpu2) {
	sc_fifo_in<int> input_fifo;
	sc_inout<sc_uint<8>> LED;
	sc_inout<sc_uint<8>> HEX[6];
	
	//funkcja 6
	void task6() {
		while (true) {
	
			if (input_fifo.read() == 32 && LED.read() != 32) {
				LED.write(32);
				HEX[0].write(6);
				HEX[2].write(' ');
				HEX[1].write(' ');
				
			}
			wait(10, SC_MS);

		}
	}
	
	SC_CTOR(cpu2) {
	
		SC_THREAD(task6);
	}

};