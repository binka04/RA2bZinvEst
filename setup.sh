#!/bin/bash

if [ ! -d "bin" ]; then
    mkdir bin 
fi
if [ ! -d "plots" ]; then
    mkdir plots
fi
if [ ! -d "plots/RzGamma_DR0p05_plots" ]; then
    mkdir plots/RzGamma_plots  
fi
if [ ! -d "plots/fragmentationFits" ]; then
    mkdir plots/plotObs_baseline_plots
fi
if [ ! -d "plots/plotPurity" ]; then
    mkdir plots/purityFits  
fi
if [ ! -d "plots/Properties_plots" ]; then
    mkdir plots/Properties_plots
fi
if [ ! -d "plots/purityResults" ]; then 
    mkdir plots/triggerEff_plots
fi

export PATH=$PWD/bin/:$PATH