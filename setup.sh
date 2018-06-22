#!/bin/bash

dir_list="bin plots/RzGamma_DR0p05_plots plots/RzGamma_plots plots/fragmentationFits plots/plotPurity plots/Properties_plots plots/purityResults plots/plotObs_baseline_plots plots/plotObs_weightGJets_plots plots/plotPurityProperties_plots plots/purityFits plots/triggerEff_plots"

if [ ! -d $d ]; then
    mkdir $d
fi

export PATH=$PWD/bin/:$PATH