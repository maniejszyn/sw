// ConsoleApplication2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "systemc.h"

#include "CPU_1.cpp"
#include "CPU_2.cpp"
#include "modul.cpp"

//---------------------------FIFO------------------------------------

SC_MODULE(fifo) {
	sc_fifo<int> fifoo;

	SC_CTOR(fifo) {
		sc_fifo<int> fifoo(6);
	}
};


//--------------------------MAIN---------------------------------------

int main(int argc, char* argv[]) {
	sc_signal<sc_uint<8>, SC_MANY_WRITERS> LED;
	sc_signal<sc_uint<8>, SC_MANY_WRITERS> SW;
	sc_signal<sc_uint<8>, SC_MANY_WRITERS> HEX[6];
	sc_signal<sc_uint<8>, SC_MANY_WRITERS> cpu_1_out_module;


	//- - - - - - - - - Modul sprzetowy- - - - - - - - - - -
	modul mod("modul");
	mod.LED(LED);                       
	mod.HEX[2](HEX[5]);                 
	mod.HEX[1](HEX[4]);
	mod.HEX[0](HEX[3]);
	mod.input(cpu_1_out_module);          //from cpu 1

	//- - - - - - - - - -fifo - - - - - - - - -
	fifo f("fifoo");

	// - - - - - - - - - procesor 1- - - - - - - - - -
	cpu1 cpu_1("cpu1");
	cpu_1.output_fifo(f.fifoo);                          //to fifo
	cpu_1.output_module(cpu_1_out_module);               //to module
	cpu_1.LED(LED);                     
	cpu_1.SW(SW);                       

	for (int i = 0; i < 6; i++) {       
		cpu_1.HEX[i](HEX[i]);
	}

	//- - - - - - - - - procesor 2- - - - - - - - - -
	cpu2 cpu_2("cpu2");
	cpu_2.input_fifo(f.fifoo);          // from fifo
	cpu_2.LED(LED);

	for (int i = 0; i < 6; i++) {       
		cpu_2.HEX[i](HEX[i]);
	}

	//-------------------start-----------------------
	sc_start();

	return(0);
}