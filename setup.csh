#!/bin/tcsh

if ( ! -d bin ) then 
    mkdir bin
endif
if ( ! -d plots ) then 
    mkdir plots
endif
if ( ! -d plots/RzGamma_DR0p05_plots ) then 
    mkdir plots/RzGamma_DR0p05_plots
endif
if ( ! -d plots/fragmentationFits ) then 
    mkdir plots/fragmentationFits
endif
if ( ! -d plots/plotPurity ) then 
    mkdir plots/plotPurity
endif
if ( ! -d plots/Properties_plots ) then 
    mkdir plots/Properties_plots
endif
if ( ! -d plots/purityResults ) then 
    mkdir plots/purityResults
endif

setenv PATH $PWD/bin/:$PATH
