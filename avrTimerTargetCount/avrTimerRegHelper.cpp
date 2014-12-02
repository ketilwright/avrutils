// avrTimerTargetCount.cpp : Defines the entry point for the console application.
//

#include <cstdint>
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;
uint16_t preScalers[5] = 
//          CS12    CS11    CS10
{
    1,    //   0       0       1
    8,    //   0       1       0  
    64,   //   0       1       1
    256,  //   1       0       0
    1024  //   1       0       1
};

std::string getCS(uint16_t prescaler)
{
    switch (prescaler)
    {
        case 1:     { return "CS10";  }
        case 8:     { return "CS11";  }
        case 64:    { return "CS10|CS11";  }
        case 256:   { return "CS12";  }
        case 1024:  { return "CS12|CS10";  }
        default:    { return "not supported"; }
    }
}


double getTargetTimerCount(double timerResolution, double seconds, uint16_t prescale)
{
    return seconds / (timerResolution * prescale) - 1.0;
}
int main(int argc, char* argv[])
{
    cout << "Atmel TCCR1B:CS scaler to timer count generator." << endl;
    cout << "Please enter cpu frequency in herz: ";
    double cpuFreq;
    cin >> cpuFreq;

    cout << "Please enter timer in seconds: ";
    double seconds;
    cin >> seconds;
    cout << "Prescaler    targetTimerCount CSBITS" << endl;
    cout << "------------------------------------" << endl;
    for (size_t i = 0; i < sizeof(preScalers) / sizeof(preScalers[0]); i++)
    {
        cout << setw(9) << preScalers[i] << setw(20) << fixed << getTargetTimerCount(1.0 / cpuFreq, seconds, preScalers[i]) << " " << getCS(preScalers[i]) << endl;
    }
	return 0;
}

