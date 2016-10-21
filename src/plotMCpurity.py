from ROOT import *

var="NJets"

inputFile = TFile("plotObs_baseline.root")
totalEB = TH1F(inputFile.Get("{0}_EB_photon_baseline_GJets".format(var)))
totalEB.Add(inputFile.Get("{0}_EB_photon_baseline_QCD".format(var)))
#totalEB.Rebin(4)
purityEB = TH1F(inputFile.Get("{0}_EB_photon_baseline_GJets".format(var)))
#purityEB.Rebin(4)
gEB = TGraphAsymmErrors(purityEB,totalEB)
gEB.SetMarkerStyle(8)

totalEE = TH1F(inputFile.Get("{0}_EE_photon_baseline_GJets".format(var)))
totalEE.Add(inputFile.Get("{0}_EE_photon_baseline_QCD".format(var)))
#totalEE.Rebin(4)
purityEE = TH1F(inputFile.Get("{0}_EE_photon_baseline_GJets".format(var)))
#purityEE.Rebin(4)
gEE = TGraphAsymmErrors(purityEE,totalEE)
gEE.SetMarkerStyle(4)
gEB.Draw("Ap")
gEE.Draw("p")
