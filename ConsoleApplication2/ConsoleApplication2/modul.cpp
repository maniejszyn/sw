#include <systemc.h>

SC_MODULE(modul) {
	sc_in<sc_uint<8>> input;             //input
	sc_out<sc_uint<8>> HEX[3];           //HEX
	sc_out<sc_uint<8>> LED;              //LED

	void modul_start() {
		
		while (true) {
			
			//funkcja 5
			 if (input.read() == 16) {        
				wait(50, SC_MS);
				LED.write(16);                
				HEX[0].write(5);              
				HEX[2].write(' ');
				
			 }
			 //funkcja 5 i jakas inna
			 else if(input.read() & 16)      
			 {
				 int x = input.read() & 16;
				 LED.write(128);
				 HEX[0].write(' ');
				 HEX[2].write('E');

			 }
			 //jakas inna funkcja lub funkcje ale nie 5
			 else{                          
				 HEX[2].write(' ');
				 HEX[0].write(' ');
			 }
			wait(50, SC_MS);
		}
	}

	SC_CTOR(modul) {
		SC_THREAD(modul_start);
	}
};