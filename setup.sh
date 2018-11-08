#!/bin/bash

set dir_list="bin plots/RzGamma_DR0p05_plots plots/RzGamma_plots plots/fragmentationFits plots/plotPurity plots/Properties_plots plots/purityResults plots/plotObs_baseline_plots plots/plotObs_weightGJets_plots plots/plotPurityProperties_plots plots/purityFits plots/triggerEff_plots"

foreach d ($dir_list)
    if ( ! -d $d ) then
        mkdir -p $d
    endif
end

setenv PATH $PWD/bin/:$PATH
