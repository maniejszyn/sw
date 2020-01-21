
#include <systemc.h>
#include <conio.h>

using namespace std;


SC_MODULE(cpu1) {
	sc_inout<sc_uint<8>> SW;
	sc_inout<sc_uint<8>> HEX[6];
	sc_fifo_out<int> output_fifo;
	sc_out<sc_uint<8>> output_module;
	sc_inout<sc_uint<8>> LED;
	
	void task1() {
		int choosen = 0;
		while (true) {
			wait(500, SC_NS);

			if (_kbhit()) {
				system("CLS");                  //czyszczenie ekranu
				showHEX();
				showLED();
				showSW();

				wait(25, SC_NS);
				cin >> choosen;
				choosen = INT_BIN(choosen);

				if (choosen >= 0 && choosen <= 32) {
					wait(500, SC_NS);
					sc_uint<8> sw_state = 0;
					sw_state = SW.read() ^ choosen;             //sumowanie bitowe czytanego z zastanym 
					SW.write(sw_state);                         // sw_state -> stan przelacznikow

					wait(500, SC_NS);
				}
				else {
					cout << "Poza zakresem funkcji do wyboru" << endl;
					choosen = 0;
				}
			}

			wait(500, SC_NS);

			sc_uint<8> sw_state = SW.read();                    //sw_state -> stan przelacznikow

			wait(500, SC_NS);

			
			if (sw_state != 0 and sw_state != 1 and sw_state != 2 and sw_state != 4 and sw_state != 8 and sw_state != 16 and sw_state != 32)          //if more than 1 functions are on -> error
			{
				
				HEX[2].write('E');
				HEX[1].write('r');
				HEX[0].write('r');

				LED.write(64);
			}

			//brak funkcji
			if (sw_state == 0) {                      
				
				for (int i = 0; i < 6; i++) {
					HEX[i].write(' ');
				}

				LED.write(0);
			}

			//funkcja 1
			if (sw_state == 1 && LED.read() != 1) {                   
				LED.write(1);
				HEX[0].write(1);
				HEX[2].write(' ');
				HEX[1].write(' ');
			}

			//funkcja 5 -> czyszczenie HEX
			if (sw_state == 16 && LED.read() != 16) {
				HEX[0].write(' ');
				HEX[2].write(' ');
				HEX[1].write(' ');
			}

			
			wait(500, SC_NS);
			SW.write(sw_state);
			wait(100, SC_MS);

			//state -> fifo
			output_fifo.write(sw_state);

			//state -> module
			output_module = sw_state;

			
			
		}
	}


	//funkcja 2
	void task2() {
		while (true) {
			sc_uint<8> fc = SW.read();
			if (fc == 2 && LED.read() != 2) {
				LED.write(2);
				HEX[0].write(2);
				HEX[2].write(' ');
				HEX[1].write(' ');
			}
			wait(10, SC_MS);

			
		}
	}

	//funkcja 3
	void task3() {
		while (true) {
			sc_uint<8> fc = SW.read();
			if (fc == 4 && LED.read() != 4) {
				LED.write(4);
				HEX[0].write(3);
				HEX[2].write(' ');
				HEX[1].write(' ');
			}
			wait(10, SC_MS);
			
		}
	}

	//funkcja 4
	void task4() {
		while (true) {
			sc_uint<8> fc = SW.read();
			if (fc == 8 && LED.read() != 8) {
				LED.write(8);
				HEX[0].write(4);
				HEX[2].write(' ');
				HEX[1].write(' ');
			}
			wait(10, SC_MS);
			
		}
	}


	//HEX
	void showHEX() {

		sc_uint<8> hex_state[6];
		for (int i = 0; i < 6; i++)
			hex_state[i] = HEX[i].read();
		cout << "HEX: |     ";
		for (int i = 5; i >= 0; i--) {
			if (hex_state[i] >= 0 && hex_state[i] <= 6)
				cout << hex_state[i] << " ";
			else if (hex_state[i] == (sc_uint<8>) 'E')
				cout << "E ";
			else if (hex_state[i] == (sc_uint<8>) 'r')
				cout << "r ";
			else if (hex_state[i] == (sc_uint<8>) ' ')
				cout << "  ";
		}
		cout << endl;
	}

	//LED
	void showLED() {
		sc_uint<8> led_state = LED.read();
		sc_uint<8> mask = 0x80;
		cout << "LED: |";
		for (int i = 7; i >= 0; i--) {
			if (led_state & mask) {
				cout << " *";
			}
			else {
				cout << "  ";
			}
			mask = mask >> 1;
		}
		cout << endl;
	}

	//SW
	void showSW() {
		sc_uint<8> sw_state = SW.read();
		sc_uint<8> mask = 0x80;
		cout << "SW : |";

		for (int i = 7; i >= 0; i--) {
			if (sw_state & mask) {
				cout << " ^";
			}
			else {
				cout << "  ";
			}
			mask = mask >> 1;
		}
		cout << endl;
		cout << "     | 8 7 6 5 4 3 2 1" << endl;
	}



	// int -> bit
	int INT_BIN(int value) {
		switch (value) {
		case 0:
			return 0;
		case 1:
			return 1;
		case 2:
			return 2;
		case 3:
			return 4;
		case 4:
			return 8;
		case 5:
			return 16;
		case 6:
			return 32;
		default:
			return -1;
		}

	}


	//bin -> int
	int BIN_INT(int value) {
		switch (value) {
		case 0:
			return 0;
		case 1:
			return 1;
		case 2:
			return 2;
		case 4:
			return 3;
		case 8:
			return 4;
		case 16:
			return 5;
		case 32:
			return 6;
		default:
			return 0;
		}

	}


	SC_CTOR(cpu1) {
		SC_THREAD(task1);
		SC_THREAD(task2);
		SC_THREAD(task3);
		SC_THREAD(task4);
	}
};