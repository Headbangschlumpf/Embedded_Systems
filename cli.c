//================================================================================================================================
// Includes
//================================================================================================================================
#include <avr/pgmspace.h>
#include <stdio.h>
#include "string.h"

#include "Taster.h"
#include "Led.h"

#include "cli.h"

//================================================================================================================================
// defines
//================================================================================================================================
#define COUNT_COMMANDS ( sizeof(command) / sizeof(command[0]) )



#define CHAR_DEL         		0x7F
#define CHAR_BACKSPACE   		0x08
#define CHAR_CR          		0x0D
#define CHAR_LF          		0x0A
#define CHAR_ESC         		0x1B

#define NEXT_LINE   (char*)"\r\n"
#define CURSOR      (char*)">>"

//================================================================================================================================
// Lokale Variablen
//================================================================================================================================
static char data[200];		// Zwischenspeicher für die Befehlsdecodierung
static char rs232_buffer[50];
static uint8_t pointer=0;	// Zeiger zu obiger Variable
static void (*callback_function)(char _data) = NULL; // Funktionszeiger auf eine Funktion, welche die Daten entgegen nimmt,
// und auf der Ausgabeschnittstelle ausgibt.
//================================================================================================================================
// Lokale Functionen
//================================================================================================================================
static void Ausfuehren(char *_data);
static char* get_next(char *_data);
static char Grossbuchstaben (char _data);
static void print_char (char _data);
static void print_string (char* _data);

// Funktionen für Befehlsdecoder
static void cmd_help(const char* _data);
static void cmd_test(const char* _data);
static void cmd_taster(const char* _data);
static void cmd_ledX_on(const char* _data);
static void cmd_ledX_off(const char* _data);


//================================================================================================================================
// command-Tabelle
//================================================================================================================================
const struct {
	const char cmd[20];
	void (*funktion)(const char* _data);
	const char info[40];
	} command[] PROGMEM = {
	{"HELP", &cmd_help, "Hilfe"},
	{"TEST", &cmd_test, "Test"},
	{"TASTER", &cmd_taster, "Gibt die Tasterzustaende aus"},
	{"LED_ON", &cmd_ledX_on, "Schaltet LEDX an : LED_ON X"},
	{"LED_OFF", &cmd_ledX_off, "Schaltet LEDX aus : LED_OFF X"},
};



//================================================================================================================================
// Funktion:		line_interpreter_init
//					Initialisierung des Line-Interpreters.
// Parameter:		_ptr: Zeiger auf die Funktion, welche die einzelne Zeichen auf der Konsole ausgeben kann.
// Rückgabewert:	keine
//================================================================================================================================
void line_interpreter_init(void (*_ptr)(char _data)){
	callback_function = _ptr;
}


//================================================================================================================================
// Funktion:		line_interpreter_get_data
//					Dieser Funktion werden die Daten von der RS232 Schnittstelle übergeben. In dieser Funktion werden die Empfangen
//					Zeichen zu einer Zeichenkette zusammengefügt, und wenn die ein Carryage-Return empfangen wurde, wird die
//					Zeichenkette an den Decoder übergeben.
// Parameter:		_char: Empfangenes Zeichen
// Rückgabewert:	keine
//================================================================================================================================
void line_interpreter_get_data(char _char){
	
	switch (_char){
		case CHAR_BACKSPACE:
		case CHAR_DEL:
		if (pointer > 0){
			pointer--;				// Zeichen aus Zeichenkette entfernen
			print_char (CHAR_BACKSPACE);
			print_char (' ');
			print_char (CHAR_BACKSPACE);
		}
		break;
		case CHAR_CR:					// Enter wurde gedrückt,
		data[pointer] = 0;			// Terminate String;
		print_string(NEXT_LINE);
		if (pointer>0){				// Wenn mindestens ein Zeichen empfangen wurde dann
			Ausfuehren (data);			// Empfangene Zeichenkette Ausweten
			pointer = 0;
		}
		print_string (CURSOR);
		break;
		default:						// Normales Zeichen empfangen.
		_char = Grossbuchstaben(_char);	// Kleinbuchstaben zu Großbuchstaben wandeln
		print_char(_char);			// Empfangenes Zeichen Ausgeben
		data[pointer++] = _char;	// Empfangenes Zeichen in Zwischenspeicher speichern.
		break;
	}//end switch
}

//================================================================================================================================
// Funktion:		Ausfuehren
//					Diese Funktion überprüft das erste Wort in der Zeichenkette mit der Command-Tabelle und wenn es eine Über-
//					einstimmung gibt, wird die entsprechende Funktion aufgerufen.
// Parameter:		_string: Auszuwertende Zeichenkette
// Rückgabewert:	keine
//================================================================================================================================
static void Ausfuehren(char *_string){
	char *temp = 0;
	uint8_t i;
	void (*temp_func)(char* const _string);
	
	temp = get_next(data);
	
	for (i=0; i<COUNT_COMMANDS; i++){
		if (strcmp_P(_string, command[i].cmd)==0){
			memcpy_P(&temp_func, &command[i].funktion, sizeof (temp_func));
			temp_func(temp);
			break;
		}
	}
	
	if (i==COUNT_COMMANDS){
		
		sprintf_P (rs232_buffer, PSTR("COMMAND ERROR\r\n\r\n"));
		print_string(rs232_buffer);
	}
}

//================================================================================================================================
// Funktion:		get_next
//					Diese Funktion überprüft das erste Wort in der Zeichenkette mit der Command-Tabelle und wenn es eine Über-
//					einstimmung gibt, wird die entsprechende Funktion aufgerufen.
// Parameter:		_string: Auszuwertende Zeichenkette
// Rückgabewert:	keine
//================================================================================================================================
static char* get_next(char *_data){
	char *temp = _data;

	while (*temp != 0){
		if (*temp==' '){
			*temp = 0;
			temp++;
			break;
			} else{
			temp++;
		}
	}
	return temp;
}


//================================================================================================================================
// Funktion:		Grossbuchstaben
//					Diese Funktion wandelt kleinbuchstaben in Großbuchstaben.
// Parameter:		_char: Auszuwertendes Zeichen
// Rückgabewert:	keine
//================================================================================================================================
static char Grossbuchstaben (char _char){
	if ( (_char>='a') && (_char<='x') ){
		_char -= 'a' - 'A';
	}
	return _char;
}


//================================================================================================================================
// Funktion:		print_char
//					Ausgabe eines Zeichens auf der Konsole.
//	Parameter:		_data: Zeichen, welches an die Callbackfunktion übergeben werden soll.
// Rückgabewert:	keine
//================================================================================================================================
static void print_char (char _data){
	if (callback_function != NULL){
		callback_function(_data);
	}
}

//================================================================================================================================
// Funktion:		print_string
//					Ausgabe eines Strings auf der Konsole.
//	Parameter:		_data: Zeichenkette
// Rückgabewert:	keine
//================================================================================================================================
static void print_string (char* _data){
	for(uint8_t i=0; i<200; i++) {
		if(_data[i] == 0){
			break;
		}
		print_char(_data[i]);
	}
	
}


//================================================================================================================================
// Funktion:		cmd_help
//					Hilfe Funktion. Gibt die Hilfe Tabelle aus.
// Parameter:		_data: Zeichenkette mit Parametern des Befehls
// Rückgabewert:	keine
//================================================================================================================================
static void cmd_help(const char* _data){
	for (uint8_t i=0; i<COUNT_COMMANDS; i++){
		strcpy_P(rs232_buffer, command[i].cmd);
		print_string(rs232_buffer);
		print_string((char*)"  ");
		strcpy_P(rs232_buffer, command[i].info);
		print_string(rs232_buffer);
		print_string(NEXT_LINE);
	}
}

//================================================================================================================================
// Funktion:		cmd_test
//					Test Funktion. Gibt die Test Tabelle aus.
// Parameter:		_data: Zeichenkette mit Parametern des Befehls
// Rückgabewert:	keine
//================================================================================================================================
static void cmd_test(const char* _data){
	print_string("Test");
	print_string(NEXT_LINE);
}

//================================================================================================================================
// Funktion:		cmd_taster
//					taster Funktion. Gibt die Tasterzustaende aus
// Parameter:		_data: Zeichenkette mit Parametern des Befehls
// Rückgabewert:	keine
//================================================================================================================================
static void cmd_taster(const char* _data){
	if(Taster1_get()){
		print_string("Taster 1 = 1");
	}else{
		print_string("Taster 1 = 0");
	}
	print_string(NEXT_LINE);
	if(Taster2_get()){
		print_string("Taster 2 = 1");
		}else{
		print_string("Taster 2 = 0");
	}
	print_string(NEXT_LINE);
	if(Taster3_get()){
		print_string("Taster 3 = 1");
		}else{
		print_string("Taster 3 = 0");
	}
	print_string(NEXT_LINE);
	if(Taster4_get()){
		print_string("Taster 4 = 1");
		}else{
		print_string("Taster 4 = 0");
	}
	print_string(NEXT_LINE);
}

//================================================================================================================================
// Funktion:		cmd_ledxON
//					SChaltet LEDX an
// Parameter:		_data: Zeichenkette mit Parametern des Befehls
// Rückgabewert:	keine
//================================================================================================================================
static void cmd_ledX_on(const char* _data){
	char data = *_data;
	int i_data = data - '0';
	switch(i_data){
		case 1:
			Led1_On();
			break;
		case 2:
			Led2_On();
			break;
		case 3:
			Led3_On();
			break;
		case 4:
			Led4_On();
			break;
		case 5:
			Led5_On();
			break;
		case 6:
			Led6_On();
			break;
		case 7:
			Led7_On();
			break;
		case 8:
			Led8_On();
		default:
			print_string("Falsche Eingabe");
			print_string(NEXT_LINE);
			break;
	}
}

//================================================================================================================================
// Funktion:		cmd_ledxOFF
//					SChaltet LEDX aus
// Parameter:		_data: Zeichenkette mit Parametern des Befehls
// Rückgabewert:	keine
//================================================================================================================================
static void cmd_ledX_off(const char* _data){
	char data = *_data;
	int i_data = data - '0';
	switch(i_data){
		case 1:
			Led1_Off();
			break;
		case 2:
			Led2_Off();
			break;
		case 3:
			Led3_Off();
			break;
		case 4:
			Led4_Off();
			break;
		case 5:
			Led5_Off();
			break;
		case 6:
			Led6_Off();
			break;
		case 7:
			Led7_Off();
			break;
		case 8:
			Led8_Off();
		default:
			print_string("Falsche Eingabe");
			print_string(NEXT_LINE);
			break;
	}
}