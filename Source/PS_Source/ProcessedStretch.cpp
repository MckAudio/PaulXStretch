/*
  Copyright (C) 2009 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License 
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "ProcessedStretch.h"

ProcessedStretch::ProcessedStretch(REALTYPE rap_,int in_bufsize_,FFTWindow w,bool bypass_,REALTYPE samplerate_,int stereo_mode_)
	: Stretch(rap_,in_bufsize_,w,bypass_,samplerate_,stereo_mode_)
{
	
    
};

ProcessedStretch::~ProcessedStretch()
{
	
//	delete [] fbfreq;
};

void ProcessedStretch::set_parameters(ProcessParameters *ppar)
{
	pars=*ppar;
	//update_free_filter();
}

void ProcessedStretch::setFreeFilterEnvelope(shared_envelope env)
{
	m_free_filter_envelope = env;
}

void ProcessedStretch::setBufferSize(int sz)
{
	jassert(sz > 0);
	Stretch::setBufferSize(sz);
	//if (nfreq != sz)
	{
		nfreq = bufsize;
		infreq = floatvector(nfreq);
		sumfreq = floatvector(nfreq);
		tmpfreq1 = floatvector(nfreq);
		tmpfreq2 = floatvector(nfreq);
		//fbfreq=new REALTYPE[nfreq];
		free_filter_freqs = floatvector(nfreq);
		for (int i = 0; i < nfreq; i++) {
			free_filter_freqs[i] = 1.0;
			//	fbfreq[i]=0.0;
		};
	}
}


/*
void ProcessedStretch::copy(const realvector& freq1,realvector& freq2)
{
	for (int i=0;i<nfreq;i++) freq2[i]=freq1[i];
};
*/

void ProcessedStretch::copy(REALTYPE* freq1, REALTYPE* freq2)
{
	for (int i = 0; i<nfreq; i++) freq2[i] = freq1[i];
};

void ProcessedStretch::add(REALTYPE *freq2,REALTYPE *freq1,REALTYPE a){
	for (int i=0;i<nfreq;i++) freq2[i]+=freq1[i]*a;
};

void ProcessedStretch::mul(REALTYPE *freq1,REALTYPE a){
	for (int i=0;i<nfreq;i++) freq1[i]*=a;
};

void ProcessedStretch::zero(REALTYPE *freq1){
	for (int i=0;i<nfreq;i++) freq1[i]=0.0;
};

REALTYPE ProcessedStretch::get_stretch_multiplier(REALTYPE pos_percents){
	REALTYPE result=1.0;
	/*
	if (pars.stretch_multiplier.get_enabled()){
		result*=pars.stretch_multiplier.get_value(pos_percents);
	};
	*/
	///REALTYPE transient=pars.get_transient(pos_percents);
	///printf("\n%g\n",transient);
	///REALTYPE threshold=0.05;
	///REALTYPE power=1000.0;
	///transient-=threshold;
	///if (transient>0){
	///	transient*=power*(1.0+power);
	///	result/=(1.0+transient);
	///};
	///printf("tr=%g\n",result);
	return result;
};

void ProcessedStretch::process_spectrum(REALTYPE *freq)
{
	for (auto& e : m_spectrum_processes)
    {
		spectrum_copy(nfreq, freq, infreq.data());
		if (e.m_index == 0 && e.m_enabled == true)
			spectrum_do_harmonics(pars, tmpfreq1, nfreq, samplerate, infreq.data(), freq);
		if (e.m_index == 1 && e.m_enabled == true)
			spectrum_do_tonal_vs_noise(pars,nfreq,samplerate,tmpfreq1, infreq.data(), freq);
		if (e.m_index == 2 && e.m_enabled == true)
			spectrum_do_freq_shift(pars,nfreq,samplerate,infreq.data(), freq);
		if (e.m_index == 3 && e.m_enabled == true)
			spectrum_do_pitch_shift(pars,nfreq,infreq.data(), freq, pow(2.0f, pars.pitch_shift.cents / 1200.0f));
		if (e.m_index == 4 && e.m_enabled == true)
			spectrum_do_octave(pars,nfreq,samplerate, sumfreq, tmpfreq1, infreq.data(), freq);
		if (e.m_index == 5 && e.m_enabled == true)
			spectrum_spread(nfreq,samplerate,tmpfreq1,infreq.data(), freq, pars.spread.bandwidth);
		if (e.m_index == 6 && e.m_enabled == true)
			spectrum_do_filter(pars,nfreq,samplerate,infreq.data(), freq);
		if (e.m_index == 7 && e.m_enabled == true)
			spectrum_do_compressor(pars,nfreq, infreq.data(), freq);
		if (e.m_index == 8 && e.m_enabled == true)
			spectrum_do_free_filter(m_free_filter_envelope, nfreq, samplerate, infreq.data(), freq);
	}

#ifdef USE_OLD_SPEC_PROC
    if (pars.harmonics.enabled) {
		copy(freq,infreq.data());
		do_harmonics(infreq.data(),freq);
	};

	if (pars.tonal_vs_noise.enabled){
		copy(freq,infreq.data());
		do_tonal_vs_noise(infreq.data(),freq);
	};

	if (pars.freq_shift.enabled) {
		copy(freq,infreq.data());
		do_freq_shift(infreq.data(),freq);
	};
	if (pars.pitch_shift.enabled) {
		copy(freq,infreq.data());
		do_pitch_shift(infreq.data(),freq,pow(2.0,pars.pitch_shift.cents/1200.0));
	};
	if (pars.octave.enabled){
		copy(freq,infreq.data());
		do_octave(infreq.data(),freq);
	};


	if (pars.spread.enabled){
		copy(freq,infreq.data());
		do_spread(infreq.data(),freq);
	};


	if (pars.filter.enabled){
		copy(freq,infreq.data());
		do_filter(infreq.data(),freq);
	};
	
	if (pars.free_filter.get_enabled()){
		copy(freq,infreq.data());
		do_free_filter(infreq.data(),freq);
	};

	if (pars.compressor.enabled){
		copy(freq,infreq.data());
		do_compressor(infreq.data(),freq);
	};
#endif
};

//void ProcessedStretch::process_output(REALTYPE *smps,int nsmps){
//};



void ProcessedStretch::update_free_filter()
{
	/*
	pars.free_filter.update_curve();
	if (pars.free_filter.get_enabled()) {
		for (int i=0;i<nfreq;i++){
			REALTYPE freq=(REALTYPE)i/(REALTYPE) nfreq*samplerate*0.5f;
			free_filter_freqs[i]=pars.free_filter.get_value(freq);
		};
	}else{
		for (int i=0;i<nfreq;i++){
			free_filter_freqs[i]=1.0f;
		};
	};
	*/
};

