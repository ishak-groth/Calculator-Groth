//including the libraries needed for the program
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;
//defining the SerialPort class to handle serial communication
class SerialPort
{
private:
    HANDLE hSerial;

public:
	//constructor to initialize the serial port with the specified port name
    SerialPort(const char* portName)
    {
        hSerial = CreateFileA(
            portName,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL);

        if (hSerial == INVALID_HANDLE_VALUE)
        {
            cout << "Fehler beim Oeffnen des Ports!" << endl;
            return;
        }
		//setting the serial port parameters
        DCB dcbSerialParams = { 0 };
        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

        GetCommState(hSerial, &dcbSerialParams);

        dcbSerialParams.BaudRate = CBR_9600;
        dcbSerialParams.ByteSize = 8;
        dcbSerialParams.StopBits = ONESTOPBIT;
        dcbSerialParams.Parity = NOPARITY;
        
        SetCommState(hSerial, &dcbSerialParams);

        COMMTIMEOUTS timeouts = { 0 };
        timeouts.ReadIntervalTimeout = 50;
        timeouts.ReadTotalTimeoutConstant = 50;
        timeouts.ReadTotalTimeoutMultiplier = 10;

        SetCommTimeouts(hSerial, &timeouts);
    }
	//function to send data through the serial port
    bool sendData(string data)
    {
        DWORD bytesWritten;

        return WriteFile(
            hSerial,
            data.c_str(),
            data.length(),
            &bytesWritten,
            NULL);
    }
	//function to read data from the serial port
    string readData()
    {
        char buffer[256];
        DWORD bytesRead;

        if (ReadFile(hSerial,
            buffer,
            sizeof(buffer) - 1,
            &bytesRead,
            NULL))
        {
            buffer[bytesRead] = '\0';
            return string(buffer);
        }

        return "";
    }
	//destructor to close the serial port handle
    ~SerialPort()
    {
        CloseHandle(hSerial);
    }
};
//main function to handle user input and serial communication
int main()
{
    const char* port = "\\\\.\\COM6";

    SerialPort serial(port);

    ofstream logfile("kommunikation.txt", ios::app);

    cout << "Calculator gestartet" << endl;
    cout << "Beenden mit 'Ende'" << endl;

	//loop to continuously read user input and send it through the serial port
    while (true)
    {
        string expression;

        cout << "\nTerm eingeben: ";
        getline(cin, expression);

        if (expression == "Ende")
            break;

        expression += "\n";

        serial.sendData(expression);

        Sleep(200);

        string result = serial.readData();

        cout << "Ergebnis: " << result << endl;

        logfile << "Gesendet: " << expression;
        logfile << "Empfangen: " << result << endl;
    }
	//closing the logfile
    logfile.close();

    return 0;
}