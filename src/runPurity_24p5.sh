#!/bin/bash

""" Code to looper over different regions of phase space and compute the photon purity """
""" Currently this code will grab a prompt and non-prompt shape from MC, using the cuts 
    provided by the -c option.  It will also use the sieie sideband, after the cuts provided
    with the -n option, to compute and alternative non-prompt shape.  The purity calculated 
    with both non-prompt shapes are printed.
"""

#python photonPurityChargedIso_24p5fb.py -s "MHT200_225" -b "MHT_200"
#python photonPurityChargedIso_24p5fb.py -s "MHT225_250" -b "MHT_225"
python photonPurityChargedIso_24p5fb.py -s "MHT250_300" -b "MHT_250"
python photonPurityChargedIso_24p5fb.py -s "MHT300_350" -b "MHT_300"
python photonPurityChargedIso_24p5fb.py -s "MHT350_500" -b "MHT_350"
python photonPurityChargedIso_24p5fb.py -s "MHT500" -b "MHT_500"

#python photonPurityChargedIso_24p5fb.py -e -s "MHT200_225" -b "MHT_200"
#python photonPurityChargedIso_24p5fb.py -e -s "MHT225_250" -b "MHT_225"
python photonPurityChargedIso_24p5fb.py -e -s "MHT250_300" -b "MHT_250"
python photonPurityChargedIso_24p5fb.py -e -s "MHT300_350" -b "MHT_300"
python photonPurityChargedIso_24p5fb.py -e -s "MHT350_500" -b "MHT_350"
python photonPurityChargedIso_24p5fb.py -e -s "MHT500" -b "MHT_500"

python photonPurityChargedIso_24p5fb.py -s "HT250_300" -b "HT_250"
python photonPurityChargedIso_24p5fb.py -s "HT300_500" -b "HT_300"
python photonPurityChargedIso_24p5fb.py -s "HT500_800" -b "HT_500"
python photonPurityChargedIso_24p5fb.py -s "HT800_1500" -b "HT_800"
python photonPurityChargedIso_24p5fb.py -s "HT1500_3000" -b "HT_1500"

python photonPurityChargedIso_24p5fb.py -e -s "HT250_300" -b "HT_250"
python photonPurityChargedIso_24p5fb.py -e -s "HT300_500" -b "HT_300"
python photonPurityChargedIso_24p5fb.py -e -s "HT500_800" -b "HT_500"
python photonPurityChargedIso_24p5fb.py -e -s "HT800_1500" -b "HT_800"
python photonPurityChargedIso_24p5fb.py -e -s "HT1500_3000" -b "HT_1500"

python photonPurityChargedIso_24p5fb.py -e -s "NJets0p5_1p5" -b "NJets_0.5"
python photonPurityChargedIso_24p5fb.py -e -s "NJets1p5_2.5" -b "NJets_1.5"
python photonPurityChargedIso_24p5fb.py -e -s "NJets2p5_3p5" -b "NJets_2.5"
python photonPurityChargedIso_24p5fb.py -e -s "NJets3p5_5p5" -b "NJets_3.5"
python photonPurityChargedIso_24p5fb.py -e -s "NJets5p5_7p5" -b "NJets_5.5"
python photonPurityChargedIso_24p5fb.py -e -s "NJets7p5_15p5" -b "NJets_7.5"

python photonPurityChargedIso_24p5fb.py -s "NJets0p5_1p5" -b "NJets_0.5"
python photonPurityChargedIso_24p5fb.py -s "NJets1p5_2.5" -b "NJets_1.5"
python photonPurityChargedIso_24p5fb.py -s "NJets2p5_3p5" -b "NJets_2.5"
python photonPurityChargedIso_24p5fb.py -s "NJets3p5_5p5" -b "NJets_3.5"
python photonPurityChargedIso_24p5fb.py -s "NJets5p5_7p5" -b "NJets_5.5"
python photonPurityChargedIso_24p5fb.py -s "NJets7p5_15p5" -b "NJets_7.5"
