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

  if(argc != 2 ) return 1;
  
  int regInt = atoi(argv[1]);
  skimSamples::region reg = static_cast<skimSamples::region>(regInt);
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
  
  plot METplot(*fillMET<RA2bTree>,"MET_"+skims.regionNames[regInt]+"_baseline","MET [GeV]",15,300.,1800.);
  plot HTplot(*fillHT<RA2bTree>,"HT_"+skims.regionNames[regInt]+"_baseline","H_{T} [GeV]",15,300,2800.);
  plot NJetsplot(*fillNJets<RA2bTree>,"NJets_"+skims.regionNames[regInt]+"_baseline","n_{j}",14,1.5,15.5);
  plot BTagsplot(*fillBTags<RA2bTree>,"BTags_"+skims.regionNames[regInt]+"_baseline","n_{b}",6,-0.5,5.5);
  plot Binsplot(*fillRA2b160Bins<RA2bTree>,"AnalysisBins_"+skims.regionNames[regInt]+"_baseline","i^th Bin",160,0.5,160.5);

  // need a function that fills the leading photon's charged isolation
  //  - photon should have pt>200. 
  //  - photon sieie should be below OR above the photon definition cut
  
  // need this plotted for QCD, GJets, Data above sieie, and Data below sieie
  // then, we ultimately need these histograms in different regions (i.e. MHT regions)

  vector<plot> plots;
  plots.push_back(METplot);
  plots.push_back(HTplot);
  plots.push_back(NJetsplot);
  plots.push_back(BTagsplot);
  plots.push_back(Binsplot);


  // background MC samples
  for( int iSample = 0 ; iSample < skims.ntuples.size() ; iSample++){

    RA2bTree* ntuple = skims.ntuples[iSample];

    for( int iPlot = 0 ; iPlot < plots.size() ; iPlot++){
      plots[iPlot].addNtuple(ntuple,skims.sampleName[iSample]);
      plots[iPlot].setFillColor(ntuple,skims.fillColor[iSample]);
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

      for( int iPlot = 0 ; iPlot < plots.size() ; iPlot++ ){
	plots[iPlot].fill(ntuple);
      }

    }
  }

  // Data samples
  RA2bTree* ntuple = skims.dataNtuple;
  for( int iPlot = 0 ; iPlot < plots.size() ; iPlot++){
    plots[iPlot].addDataNtuple(ntuple,"data");
  }
  
  int numEvents = ntuple->fChain->GetEntries();
  ntupleBranchStatus<RA2bTree>(ntuple);
  for( int iEvt = 0 ; iEvt < numEvents ; iEvt++ ){
    ntuple->GetEntry(iEvt);
    if( iEvt % 1000000 == 0 ) cout << "data: " << iEvt << "/" << numEvents << endl;
    //if( ntuple->TriggerPass->size() < 44 || ( !ntuple->TriggerPass->at(41) && !ntuple->TriggerPass->at(42) && !ntuple->TriggerPass->at(43) && !ntuple->TriggerPass->at(44)) ) continue;
    for( int iPlot = 0 ; iPlot < plots.size() ; iPlot++){
      plots[iPlot].fillData(ntuple);
    }
  }

  TCanvas* can = new TCanvas("can","can",500,500);
  for( int iPlot = 0 ; iPlot < plots.size() ; iPlot++){
    plots[iPlot].Draw(can,skims.ntuples,skims.signalNtuples,"plotObs_baseline_plots");
  }
}
