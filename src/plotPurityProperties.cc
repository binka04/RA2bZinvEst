#include "TString.h"
#include "TChain.h"
#include "TH1F.h"
#include "TROOT.h"
#include "THStack.h"
#include "TPad.h"

#include <vector>
#include <map>
#include <iostream>

#include "plotterUtils.cc"
#include "skimSamples.cc"
#include "definitions.h"
#include "RA2bTree.cc"

using namespace std;

int main(int argc, char** argv){

  gROOT->ProcessLine(".L ~/tdrstyle.C");
  gROOT->ProcessLine("setTDRStyle()");

  if(argc != 2 ){
    cout << "please provide a region index" << endl;
    return 1;
  }

  int regInt = atoi(argv[1]);
  skimSamples::region reg = static_cast<skimSamples::region>(regInt);

  if( reg != skimSamples::kPhotonLoose && reg != skimSamples::kPhotonLDPLoose ){
    cout << "only photon regions are valid" << endl;
    return 1;
  }

  skimSamples skims(reg);
  typedef plot<RA2bTree> plot;
  
  if( skims.regionNames[regInt] == "signal") 
    lumi = 12900;
  else
    lumi = 24500;
  cout << skims.skimType << endl;
  for( int i = 0 ; i < skims.ntuples.size() ; i++){
    cout << "sample name: " << skims.sampleName[i] << endl;
  }

  double MHTbinning[7]={200.,225.,250.,300.,350.,500.,2000.};
  plot MHTplot(*fillMHT<RA2bTree>,"MHT_"+skims.regionNames[regInt],"MHT [GeV]",6,MHTbinning);
  double HTbinning[7]={200.,250.,300.,500.,800.,1500.,3000.};
  plot HTplot(*fillHT<RA2bTree>,"HT_"+skims.regionNames[regInt],"H_{T} [GeV]",6,HTbinning);
  double NJetsbinning[7]={0.5,1.5,2.5,3.5,5.5,7.5,15.5};
  plot NJetsplot(*fillNJets<RA2bTree>,"NJets_"+skims.regionNames[regInt],"n_{j}",6,NJetsbinning);
  double BTagsbinning[6]={-0.5,0.5,1.5,2.5,3.5,6.5};
  plot BTagsplot(*fillBTags<RA2bTree>,"BTags_"+skims.regionNames[regInt],"n_{b}",5,BTagsbinning);

  vector<plot> projections;
  projections.push_back(MHTplot);
  projections.push_back(HTplot);
  projections.push_back(NJetsplot);
  projections.push_back(BTagsplot);
  vector<TString> projLabel;
  projLabel.push_back("MHT");
  projLabel.push_back("HT");
  projLabel.push_back("NJets");
  projLabel.push_back("BTags");

  vector<vector<plot> > chargeIsoEEHighSieieVersus;
  vector<vector<plot> > chargeIsoEELowSieieVersus;
  vector<vector<plot> > chargeIsoEBHighSieieVersus;
  vector<vector<plot> > chargeIsoEBLowSieieVersus;
  for( int iProj = 0 ; iProj<projections.size(); iProj++ ){
    vector<plot> chargeIsoEEHighSieieTemp;
    vector<plot> chargeIsoEELowSieieTemp;
    vector<plot> chargeIsoEBHighSieieTemp;
    vector<plot> chargeIsoEBLowSieieTemp;
    for( int iBin = 0 ; iBin <= projections[iProj].nbins ; iBin++ ){
      TString binEdge="_";
      binEdge+=projections[iProj].binEdges[iBin];
      chargeIsoEEHighSieieTemp.push_back(plot(*photonIsoChrg<RA2bTree>,"photonIsoChrgHighSieie_EE_"+projLabel[iProj]+binEdge+"_"+skims.regionNames[regInt],"Iso_{chrg}",25,0.,10.));
      chargeIsoEELowSieieTemp.push_back(plot(*photonIsoChrg<RA2bTree>,"photonIsoChrgLowSieie_EE_"+projLabel[iProj]+binEdge+"_"+skims.regionNames[regInt],"Iso_{chrg}",25,0.,10.));

      chargeIsoEBHighSieieTemp.push_back(plot(*photonIsoChrg<RA2bTree>,"photonIsoChrgHighSieie_EB_"+projLabel[iProj]+binEdge+"_"+skims.regionNames[regInt],"Iso_{chrg}",25,0.,10.));
      chargeIsoEBLowSieieTemp.push_back(plot(*photonIsoChrg<RA2bTree>,"photonIsoChrgLowSieie_EB_"+projLabel[iProj]+binEdge+"_"+skims.regionNames[regInt],"Iso_{chrg}",25,0.,10.));
    }
    chargeIsoEEHighSieieVersus.push_back(chargeIsoEEHighSieieTemp);
    chargeIsoEELowSieieVersus.push_back(chargeIsoEELowSieieTemp);
    chargeIsoEBHighSieieVersus.push_back(chargeIsoEBHighSieieTemp);
    chargeIsoEBLowSieieVersus.push_back(chargeIsoEBLowSieieTemp);
  }
  
  plot SieieEBplot(*photonSieie<RA2bTree>,"photonSieie_EB_"+skims.regionNames[regInt],"#sigma_{i#etai#eta}",100,0.,.03);
  plot IsoChrgEBplot(*photonIsoChrg<RA2bTree>,"photonIsoChrg_EB_"+skims.regionNames[regInt],"Iso_{chrg}",100,0.,10.);
  plot IsoGamEBplot(*photonIsoGam<RA2bTree>,"photonIsoGam_EB_"+skims.regionNames[regInt],"Iso_{gam}",100,0.,10.);
  plot IsoNeuEBplot(*photonIsoNeu<RA2bTree>,"photonIsoNeu_EB_"+skims.regionNames[regInt],"Iso_{neu}",100,0.,10.);
  plot HoverEEBplot(*photonHoverE<RA2bTree>,"photonHoverE_EB_"+skims.regionNames[regInt],"H/E",100,0.,.1);
  plot IsoChrgHighSieiEBplot(*photonIsoChrg<RA2bTree>,"photonIsoChrgHighSieie_EB_"+skims.regionNames[regInt],"Iso_{chrg}",100,0.,10.);
  plot IsoChrgLowSieieEBplot(*photonIsoChrg<RA2bTree>,"photonIsoChrgLowSieie_EB_"+skims.regionNames[regInt],"Iso_{chrg}",100,0.,10.);
  
  plot SieieEEplot(*photonSieie<RA2bTree>,"photonSieie_EE_"+skims.regionNames[regInt],"#sigma_{i#etai#eta}",100,0.,.07);
  plot IsoChrgEEplot(*photonIsoChrg<RA2bTree>,"photonIsoChrg_EE_"+skims.regionNames[regInt],"Iso_{chrg}",100,0.,10.);
  plot IsoChrgHighSieiEEplot(*photonIsoChrg<RA2bTree>,"photonIsoChrgHighSieie_EE_"+skims.regionNames[regInt],"Iso_{chrg}",100,0.,10.);
  plot IsoChrgLowSieieEEplot(*photonIsoChrg<RA2bTree>,"photonIsoChrgLowSieie_EE_"+skims.regionNames[regInt],"Iso_{chrg}",100,0.,10.);
  plot HoverEEEplot(*photonHoverE<RA2bTree>,"photonHoverE_EE_"+skims.regionNames[regInt],"H/E",100,0.,.1);
  plot IsoGamEEplot(*photonIsoGam<RA2bTree>,"photonIsoGam_EE_"+skims.regionNames[regInt],"Iso_{gam}",100,0.,10.);
  plot IsoNeuEEplot(*photonIsoNeu<RA2bTree>,"photonIsoNeu_EE_"+skims.regionNames[regInt],"Iso_{neu}",100,0.,10.);

  // need this plotted for QCD, GJets, Data above sieie, and Data below sieie
  // then, we ultimately need these histograms in different regions (i.e. MHT regions)

  /*
  plots.push_back(MHTplot);
  plots.push_back(HTplot);
  plots.push_back(NJetsplot);
  plots.push_back(BTagsplot);
  plots.push_back(Binsplot);
  */

  vector<plot> plotsEB;
  plotsEB.push_back(SieieEBplot);
  plotsEB.push_back(IsoChrgEBplot);
  plotsEB.push_back(IsoGamEBplot);
  plotsEB.push_back(IsoNeuEBplot);
  plotsEB.push_back(HoverEEBplot);
  plotsEB.push_back(IsoChrgHighSieiEBplot);
  plotsEB.push_back(IsoChrgLowSieieEBplot);
  
  vector<plot> plotsEE;
  plotsEE.push_back(SieieEEplot);
  plotsEE.push_back(IsoChrgEEplot);
  plotsEE.push_back(IsoGamEEplot);
  plotsEE.push_back(IsoNeuEEplot);
  plotsEE.push_back(HoverEEEplot);
  plotsEE.push_back(IsoChrgHighSieiEEplot);
  plotsEE.push_back(IsoChrgLowSieieEEplot);

  // background MC samples
  for( int iSample = 0 ; iSample < skims.ntuples.size() ; iSample++){

    RA2bTree* ntuple = skims.ntuples[iSample];

    for( int iPlot = 0 ; iPlot < plotsEB.size() ; iPlot++){
      plotsEB[iPlot].addNtuple(ntuple,skims.sampleName[iSample]);
      plotsEB[iPlot].setFillColor(ntuple,skims.fillColor[iSample]);
      plotsEE[iPlot].addNtuple(ntuple,skims.sampleName[iSample]);
      plotsEE[iPlot].setFillColor(ntuple,skims.fillColor[iSample]);
    }
    
    for( int iProj = 0 ; iProj < projections.size() ; iProj++){
	projections[iProj].addNtuple(ntuple,skims.sampleName[iSample]);
	projections[iProj].setFillColor(ntuple,skims.fillColor[iSample]);
      for( int iBin = 0 ; iBin < chargeIsoEBHighSieieVersus[iProj].size() ; iBin++){
	chargeIsoEBHighSieieVersus[iProj][iBin].addNtuple(ntuple,skims.sampleName[iSample]);
	chargeIsoEBHighSieieVersus[iProj][iBin].setFillColor(ntuple,skims.fillColor[iSample]);
	chargeIsoEEHighSieieVersus[iProj][iBin].addNtuple(ntuple,skims.sampleName[iSample]);
	chargeIsoEEHighSieieVersus[iProj][iBin].setFillColor(ntuple,skims.fillColor[iSample]);
	chargeIsoEBLowSieieVersus[iProj][iBin].addNtuple(ntuple,skims.sampleName[iSample]);
	chargeIsoEBLowSieieVersus[iProj][iBin].setFillColor(ntuple,skims.fillColor[iSample]);
	chargeIsoEELowSieieVersus[iProj][iBin].addNtuple(ntuple,skims.sampleName[iSample]);
	chargeIsoEELowSieieVersus[iProj][iBin].setFillColor(ntuple,skims.fillColor[iSample]);
      }
    }

    int numEvents = ntuple->fChain->GetEntries();
    ntupleBranchStatus<RA2bTree>(ntuple);
    for( int iEvt = 0 ; iEvt < numEvents ; iEvt++ ){
      ntuple->GetEntry(iEvt);
      if( iEvt % 1000000 == 0 ) cout << skims.sampleName[iSample] << ": " << iEvt << "/" << numEvents << endl;

      if( skims.regionNames[regInt] == "photonLDP" || skims.regionNames[regInt] == "photon" ){
	if( skims.sampleName[iSample] == "QCD" && isPromptPhoton(ntuple) ) continue;
	if( skims.sampleName[iSample] == "GJets" && !isPromptPhoton(ntuple) ) continue;
      }
      
      if( ntuple->Photons->at(0).Pt()<200. ) continue;
      if( !RA2bBaselineCut(ntuple) ) continue;

      for( int iProj = 0 ; iProj<projections.size() ; iProj++ ){

	int iBin = projections[iProj].fill(ntuple);
	if( iBin <= 0 ) continue;
	if( ntuple->Photons_isEB->at(0) ){
	  if(ntuple->Photons_sigmaIetaIeta->at(0)>.0107)
	    chargeIsoEBHighSieieVersus[iProj][iBin-1].fill(ntuple);
	  else
	    chargeIsoEBLowSieieVersus[iProj][iBin-1].fill(ntuple);
	}
	else{
	  if(ntuple->Photons_sigmaIetaIeta->at(0)>.0272)
	    chargeIsoEEHighSieieVersus[iProj][iBin-1].fill(ntuple);
	  else
	    chargeIsoEELowSieieVersus[iProj][iBin-1].fill(ntuple);
	}
      }
      
      for( int iPlot = 0 ; iPlot < plotsEB.size()-2 ; iPlot++ ){
	if( ntuple->Photons_isEB->at(0) ) 
	  plotsEB[iPlot].fill(ntuple);
	else 
	  plotsEE[iPlot].fill(ntuple);
      }
      if( ntuple->Photons_isEB->at(0) ){
	if(ntuple->Photons_sigmaIetaIeta->at(0)>.0107)
	  plotsEB[plotsEB.size()-2].fill(ntuple);
	else
	  plotsEB[plotsEB.size()-1].fill(ntuple);
      }else{
	if(ntuple->Photons_sigmaIetaIeta->at(0)>.0272)
	  plotsEE[plotsEB.size()-2].fill(ntuple);
	else
	  plotsEE[plotsEB.size()-1].fill(ntuple);
      }
    }
  }

  // Data samples
  RA2bTree* ntuple = skims.dataNtuple;
  for( int iPlot = 0 ; iPlot < plotsEB.size() ; iPlot++){
    plotsEB[iPlot].addDataNtuple(ntuple,"data");
    plotsEE[iPlot].addDataNtuple(ntuple,"data");
  }

  for( int iProj = 0 ; iProj < projections.size() ; iProj++){
    projections[iProj].addDataNtuple(ntuple,"data");
    for( int iBin = 0 ; iBin < chargeIsoEBHighSieieVersus[iProj].size() ; iBin++){
      chargeIsoEBHighSieieVersus[iProj][iBin].addDataNtuple(ntuple,"data");
      chargeIsoEEHighSieieVersus[iProj][iBin].addDataNtuple(ntuple,"data");
      chargeIsoEBLowSieieVersus[iProj][iBin].addDataNtuple(ntuple,"data");
      chargeIsoEELowSieieVersus[iProj][iBin].addDataNtuple(ntuple,"data");
    }
  }
  
  int numEvents = ntuple->fChain->GetEntries();
  ntupleBranchStatus<RA2bTree>(ntuple);
  for( int iEvt = 0 ; iEvt < numEvents ; iEvt++ ){
    ntuple->GetEntry(iEvt);
    if( iEvt % 1000000 == 0 ) cout << "data: " << iEvt << "/" << numEvents << endl;
    
    if( ntuple->TriggerPass->size() < 44 || !ntuple->TriggerPass->at(50) ) continue;
    if( ntuple->Photons->at(0).Pt()<200. ) continue;
    if( !RA2bBaselineCut(ntuple) ) continue;

    for( int iProj = 0 ; iProj<projections.size() ; iProj++ ){
      int iBin = projections[iProj].fillData(ntuple);
      if( iBin <= 0 ) continue;
      if( ntuple->Photons_isEB->at(0) ){
	if(ntuple->Photons_sigmaIetaIeta->at(0)>.0107)
	  chargeIsoEBHighSieieVersus[iProj][iBin-1].fillData(ntuple);
	else
	  chargeIsoEBLowSieieVersus[iProj][iBin-1].fillData(ntuple);
      }
      else{
	if(ntuple->Photons_sigmaIetaIeta->at(0)>.0272)
	  chargeIsoEEHighSieieVersus[iProj][iBin-1].fillData(ntuple);
	else
	  chargeIsoEELowSieieVersus[iProj][iBin-1].fillData(ntuple);
      }
    }
    
      
    for( int iPlot = 0 ; iPlot < plotsEB.size()-2 ; iPlot++ ){
      if( ntuple->Photons_isEB->at(0) ) 
	plotsEB[iPlot].fillData(ntuple);
      else 
	plotsEE[iPlot].fillData(ntuple);
    }
    if( ntuple->Photons_isEB->at(0) ){
      if(ntuple->Photons_sigmaIetaIeta->at(0)>.0107)
	plotsEB[plotsEB.size()-2].fillData(ntuple);
      else
	plotsEB[plotsEB.size()-1].fillData(ntuple);
    }else{
      if(ntuple->Photons_sigmaIetaIeta->at(0)>.0272)
	plotsEE[plotsEB.size()-2].fillData(ntuple);
      else
	plotsEE[plotsEB.size()-1].fillData(ntuple);
    }

  }
  
  TFile* outputFile = new TFile("plotPurityProperties_"+skims.regionNames[regInt]+".root","RECREATE");

  for( int iPlot = 0 ; iPlot < plotsEB.size() ; iPlot++){
    TCanvas* can = new TCanvas("can","can",500,500);
    plotsEB[iPlot].Draw(can,skims.ntuples,skims.signalNtuples,"plotPurityProperties_plots");
    outputFile->cd();
    plotsEB[iPlot].Write();
  }

  for( int iPlot = 0 ; iPlot < plotsEE.size() ; iPlot++){
    TCanvas* can = new TCanvas("can","can",500,500);
    plotsEE[iPlot].Draw(can,skims.ntuples,skims.signalNtuples,"plotPurityProperties_plots");
    outputFile->cd();
    plotsEE[iPlot].Write();
  }

  for( int iProj = 0 ; iProj < projections.size() ; iProj++ ){
    TCanvas* can = new TCanvas("can","can",500,500);
    projections[iProj].Draw(can,skims.ntuples,skims.signalNtuples,"plotPurityProperties_plots");
    outputFile->cd();
    projections[iProj].Write();
  }

  for( int iProj = 0 ; iProj < projections.size() ; iProj++ ){
    for( int iBin = 0 ; iBin < chargeIsoEBHighSieieVersus[iProj].size() ; iBin++ ){ 
      TCanvas* can = new TCanvas("can","can",500,500);
      chargeIsoEBHighSieieVersus[iProj][iBin].Draw(can,skims.ntuples,skims.signalNtuples,"plotPurityProperties_plots");
      outputFile->cd();
      chargeIsoEBHighSieieVersus[iProj][iBin].Write();
    }

    for( int iBin = 0 ; iBin < chargeIsoEBHighSieieVersus[iProj].size() ; iBin++ ){ 
      TCanvas* can = new TCanvas("can","can",500,500);
      chargeIsoEBLowSieieVersus[iProj][iBin].Draw(can,skims.ntuples,skims.signalNtuples,"plotPurityProperties_plots");
      outputFile->cd();
      chargeIsoEBLowSieieVersus[iProj][iBin].Write();
    }

    for( int iBin = 0 ; iBin < chargeIsoEEHighSieieVersus[iProj].size() ; iBin++ ){ 
      TCanvas* can = new TCanvas("can","can",500,500);
      chargeIsoEEHighSieieVersus[iProj][iBin].Draw(can,skims.ntuples,skims.signalNtuples,"plotPurityProperties_plots");
      outputFile->cd();
      chargeIsoEEHighSieieVersus[iProj][iBin].Write();
    }
    for( int iBin = 0 ; iBin < chargeIsoEEHighSieieVersus[iProj].size() ; iBin++ ){ 
      TCanvas* can = new TCanvas("can","can",500,500);
      chargeIsoEELowSieieVersus[iProj][iBin].Draw(can,skims.ntuples,skims.signalNtuples,"plotPurityProperties_plots");
      outputFile->cd();
      chargeIsoEELowSieieVersus[iProj][iBin].Write();
    }
  }
  outputFile->Close();
}
