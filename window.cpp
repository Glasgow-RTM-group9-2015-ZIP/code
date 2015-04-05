#include "window.h"
#include "ADCutilities.h"
#include <Iir.h>

Window::Window() : plot( QString("ECG PEAK") ), gain(5), count(0) // <-- 'c++ initialisation list' - google it!
{
  // set up the gain knob
	knob.setValue(gain);

	// use the Qt signals/slots framework to update the gain -
	// every time the knob is moved, the setGain function will be called
	connect( &knob, SIGNAL(valueChanged(double)), SLOT(setGain(double)) );

	// set up the thermometer
	thermo.setFillBrush( QBrush(Qt::red) );
	thermo.setRange(0, 20);
	thermo.show();

	setADC();

	// set up the initial plot data
	for( int index=0; index<plotDataSize; ++index )
	{
		xData[index] = index;
		yData[index] = 0;
	}

	// make a plot curve from the data and attach it to the plot
	curve.setSamples(xData, yData, plotDataSize);
	curve.attach(&plot);

	plot.replot();
	plot.show();


	// set up the layout - knob above thermometer
	vLayout.addWidget(&knob);
	vLayout.addWidget(&thermo);

	// plot to the left of knob and thermometer
	hLayout.addLayout(&vLayout);
	hLayout.addWidget(&plot);

	setLayout(&hLayout);
        startTimer(10);
        f = new Iir::Butterworth::LowPass<3>;
        const int order = 3;
        const float samplingrate = 100; // Hz
        const float cutoff_frequency = 5; // LP
        f->setup (order, samplingrate, cutoff_frequency);
        f->reset ();


}


void Window::timerEvent( QTimerEvent *)
{
	double inVal = gain * read_data();
	++count;
        double signal_out = f->filter(inval);

	// add the new input to the plot
	memmove( yData, yData+1, (plotDataSize-1) * sizeof(double) );
	yData[plotDataSize-1] = signal_out;
	curve.setSamples(xData, yData, plotDataSize);
	plot.replot();

	// set the thermometer value
	thermo.setValue( inVal + 10 );
}


// this function can be used to change the gain of the A/D internal amplifier
void Window::setGain(double gain)
{
	// for example purposes just change the amplitude of the generated input
	this->gain = gain;
}
